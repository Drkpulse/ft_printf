/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:10:55 by joseferr          #+#    #+#             */
/*   Updated: 2023/11/07 17:19:27 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_put(char *frs, char c, t_info *data, int flag)
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
		if (!frs)
			return (ft_put("(null)", 'a', data, 1));
		while (frs[i])
			ft_put("a", frs[i++], data, 0);
	}
}

void	ft_nbrhex(unsigned long nb, t_info *data)
{
	data->nbase = 16;
	if (data->flag == 'x' || data->flag == 'p')
	{
		if (data->flag == 'p' && nb == (unsigned long)data->nbr)
			ft_put("0x", 'a', data, 1);
		data->base = "0123456789abcdef";
	}
	if (data->flag == 'X')
		data->base = "0123456789ABCDEF";
	if (nb >= data->nbase)
		ft_nbrhex(nb / data->nbase, data);
	ft_put("a", data->base[nb % data->nbase], data, 0);
}

void	ft_nbr(unsigned long nb, t_info *data)
{
	data->nbase = 10;
	data->base = "0123456789";
	if (data->nbr < 0)
	{
		ft_put("a", '-', data, 0);
		data->nbr *= -1;
		nb = data->nbr;
	}
	if (nb >= data->nbase)
		ft_nbr(nb / data->nbase, data);
	ft_put("a", data->base[nb % data->nbase], data, 0);
}

void	ft_flags(const char *frs, va_list arg, t_info *data)
{
	data->flag = *frs;
	if (*frs == 'c')
		ft_put("a", va_arg(arg, int), data, 0);
	else if (*frs == 's')
		ft_put(va_arg(arg, char *), 'a', data, 1);
	else if (*frs == 'd' || *frs == 'i')
	{
		data->nbr = va_arg(arg, int);
		ft_nbr(data->nbr, data);
	}
	else if (*frs == 'u')
		ft_nbr(va_arg(arg, unsigned int), data);
	else if (*frs == 'x' || *frs == 'X')
		ft_nbrhex(va_arg(arg, unsigned int), data);
	else if (*frs == 'p')
	{
		data->nbr = va_arg(arg, long);
		if (data->nbr == 0)
			return (ft_put("(nil)", 'a', data, 1));
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
			ft_put("a", *(++text), &data, 0);
		else if (*text == '%' && *(text + 1) != '%')
			ft_flags(++text, args, &data);
		else if (*text != '%')
			ft_put("a", *text, &data, 0);
		text++;
	}
	va_end(args);
	return (data.count);
}
