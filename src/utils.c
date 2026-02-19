/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:41:17 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/12 09:00:45 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    ft_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

int     ft_isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

int     ft_ispositive(char *s)
{
    int i;
    
    i = 0;
    if (!s)
        return 0;
    while (s[i])
    {
        if (!ft_isdigit(s[i]))
            return 0;
        i++;
    }
    return 1;
}

int     ft_isspace(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

void    *safe_malloc(size_t bytes)
{
    void    *ret;
    ret = malloc(bytes);
    if (!ret)
    {
        ft_error("Error with malloc.\n");
        return NULL;
    }
    return (ret);
}