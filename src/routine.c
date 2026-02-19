/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:02:49 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/19 10:02:50 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void compile(t_coder *coder)
{
    coder->last_compile_start = current_time_ms();
    print_status(coder, "is compiling");
    usleep(coder->data->time_to_compile * 1000);
    coder->compiles_done++;
}

static void work_delay(t_coder *coder)
{
    print_status(coder, "is debugging");
    usleep(coder->data->time_to_debug * 1000);
    print_status(coder, "is refactoring");
    usleep(coder->data->time_to_refactor * 1000);
}

static int check_burnout(t_coder *coder)
{
    t_data *data = coder->data;

    if (!coder->burned_out && current_time_ms() > coder->deadline_time)
    {
        coder->burned_out = 1;
        print_status(coder, "has burned out");
        release_all(coder);
        pthread_mutex_lock(&data->status_mutex);
        data->end_simulation = 1;
        pthread_mutex_unlock(&data->status_mutex);
        return 1;
    }
    return 0;
}

void *coder_routine(void *arg)
{
    t_coder *coder;
    
    coder = (t_coder *)arg;
    while (1)
    {
        if (coder->data->scheduler == 2)
            coder->deadline_time = current_time_ms() + coder->data->time_to_burnout;

        take_all(coder);
        if (coder->data->end_simulation)
            break;
        coder->last_compile_start = current_time_ms();
        compile(coder);
        release_all(coder);
        work_delay(coder);
        if (check_burnout(coder))
            break;
        if (coder->data->end_simulation)
            break;
    }
    return NULL;
}
