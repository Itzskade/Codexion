/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:02:49 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/19 10:02:50 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    current_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000L + tv.tv_usec / 1000L;
}

void    print_status(t_coder *coder, const char *msg)
{
    t_data *data = coder->data;
    pthread_mutex_lock(&data->print_mutex);
    printf("%ld %d %s\n", current_time_ms() - data->start_simulation,
           coder->id, msg);
    pthread_mutex_unlock(&data->print_mutex);
}

void    take_all(t_coder *coder)
{
    if (coder->data->scheduler == 1)
    {
        take_dongle(coder, coder->left_dongle);
        print_status(coder, "has taken a dongle");
        take_dongle(coder, coder->right_dongle);
        print_status(coder, "has taken a dongle");
    }
    else
    {
        take_dongles_edf(coder);
        print_status(coder, "has taken a dongle");
        print_status(coder, "has taken a dongle");        
    }
}

void    release_all(t_coder *coder)
{
    if (coder->data->scheduler == 1)
    {
        release_dongle(coder->left_dongle);
        release_dongle(coder->right_dongle);
    }
    else
        release_dongles(coder);
}