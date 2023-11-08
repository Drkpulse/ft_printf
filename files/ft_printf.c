/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:10:55 by joseferr          #+#    #+#             */
/*   Updated: 2023/11/07 19:03:26 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_put(char c, char *str, t_info *data, int flag)
{
	int	i;

	i = 0;
	if (!flag)
	{
		write(1, &c, 1);
		data->count++;
	}
	else
	{
		if (!str)
			return (ft_put('a', "(null)", data, 1));
		while (str[i])
			ft_put(str[i++], "a", data, 0);
	}
}

void	ft_nbrhex(unsigned long nb, t_info *data)
{
	data->nbase = 16;
	if (data->flag == 'x' || data->flag == 'p')
	{
		if (data->flag == 'p' && nb == (unsigned long)data->nbr)
			ft_put('a', "0x", data, 1);
		data->base = "0123456789abcdef";
	}
	if (data->flag == 'X')
		data->base = "0123456789ABCDEF";
	if (nb >= data->nbase)
		ft_nbrhex(nb / data->nbase, data);
	ft_put(data->base[nb % data->nbase], "a", data, 0);
}

void	ft_nbr(unsigned long nb, t_info *data)
{
	data->nbase = 10;
	data->base = "0123456789";
	if (data->nbr < 0)
	{
		ft_put('-', "a", data, 0);
		data->nbr *= -1;
		nb = data->nbr;
	}
	if (nb >= data->nbase)
		ft_nbr(nb / data->nbase, data);
	ft_put(data->base[nb % data->nbase], "a", data, 0);
}

void	ft_conversions(va_list arg, t_info *data)
{
	if (data->flag == 'c')
		ft_put(va_arg(arg, int), "a", data, 0);
	else if (data->flag == 's')
		ft_put('a', va_arg(arg, char *), data, 1);
	else if (data->flag == 'd' || data->flag == 'i')
	{
		data->nbr = va_arg(arg, int);
		ft_nbr(data->nbr, data);
	}
	else if (data->flag == 'u')
		ft_nbr(va_arg(arg, unsigned int), data);
	else if (data->flag == 'x' || data->flag == 'X')
		ft_nbrhex(va_arg(arg, unsigned int), data);
	else if (data->flag == 'p')
	{
		data->nbr = va_arg(arg, long);
		if (data->nbr == 0)
			return (ft_put('a', "(nil)", data, 1));
		ft_nbrhex(data->nbr, data);
	}
}

int	ft_printf(const char *text, ...)
{
	va_list	args;
	t_info	data;

	va_start(args, text);
	data.nbase = 16;
	data.count = 0;
	data.nbr = 0;
	while (*text)
	{
		if (*text == '%' && *(text + 1) == '%')
			ft_put(*(++text), "a", &data, 0);
		else if (*text == '%' && *(text + 1) != '%')
		{
			data.flag = *++text;
			ft_conversions(args, &data);
		}
		else if (*text != '%')
			ft_put(*text, "a", &data, 0);
		text++;
	}
	va_end(args);
	return (data.count);
}

int main()
{
	char *frs = "abc123";
	char *ptr;
	int a;

	a = 42;

	printf("Decimal: %d \n", a);
	ft_printf("Decimal: %d \n", a);
	printf("String: %s \n", frs);
	ft_printf("String: %s \n", frs);
	printf("Inteiro: %i \n", a);
	ft_printf("Inteiro: %i \n", a);
	printf("hexdecimal: %x \n", a);
	ft_printf("hexdecimal: %x \n", a);
	printf("Hexdecimal: %X \n", a);
	ft_printf("Hexdecimal: %X \n", a);
}
