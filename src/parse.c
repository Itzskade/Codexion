/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:09:16 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/12 09:01:02 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static char *valid_input(char *s)
{
    char    *num;
    size_t  len;

    len = 0;
    while (ft_isspace(*s))
        s++;
    if (*s == '+')
        s++;
    else if (*s == '-')
        return (ft_error("Error: Must be a positive value"), NULL);
    if (!ft_ispositive(s))
        return (ft_error("Error: Input is not a correct digit"), NULL);
    while(*s == '0')
        s++;
    num = s;
    while(ft_isdigit(*s++))
        ++len;
    if (len > 10)
        return (ft_error("Error: Value too big, INT_MAX is the limit"), NULL);
    return (num);
}

static long ft_atol(char *s)
{
    long    res;
    char    *num;

    res = 0;
    num = valid_input(s);
    if (!num)
        return (0);
    while (ft_isdigit(*num))
        res = (res * 10) + (*num++ - '0');
    if (res > INT_MAX)
        return (ft_error("Error: Value too big, INT_MAX is the limit"), 0);
    return (res);
}

void        parse_input(t_data *data, char **av)
{
    data->number_of_coders = ft_atol(av[1]);
    if (data->number_of_coders <= 0)
    {
        ft_error("Error: Number of coders must be positive\n");
        return ;
    }
    data->time_to_burnout = ft_atol(av[2]);
    data->time_to_compile = ft_atol(av[3]);
    data->time_to_debug = ft_atol(av[4]);
    data->time_to_refactor = ft_atol(av[5]);
    data->number_of_compiles_required = ft_atol(av[6]);
    data->dongle_cooldown = ft_atol(av[7]);
    if (strcmp(av[8], "fifo") == 0)
        data->scheduler = 1;
    else if (strcmp(av[8], "edf") == 0)
        data->scheduler = 2;
    else
    {
        ft_error("Error: Scheduler must be fifo or edf\n");
        return ;
    }
}