/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:26:26 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/17 12:26:28 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    data_init(t_data *data)
{
    int i;

    data->end_simulation = 0;
    data->coders = safe_malloc(sizeof(t_coder) * data->number_of_coders);
    data->dongles = safe_malloc(sizeof(t_dongle) * data->number_of_coders);
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->status_mutex, NULL);
    i = -1;
    while (++i < data->number_of_coders)
    {
        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        pthread_cond_init(&data->dongles[i].cond, NULL);
        data->dongles[i].is_taken = 0;

        data->coders[i].id = i + 1;
        data->coders[i].compiles_done = 0;
        data->coders[i].last_compile_start = 0;
        data->coders[i].deadline = 0;
        data->coders[i].burned_out = 0;
        data->coders[i].deadline_time = 0;
        data->coders[i].left_dongle = &data->dongles[i];
        data->coders[i].right_dongle = &data->dongles[(i + 1) % data->number_of_coders];
        data->coders[i].data = data;
    }
}