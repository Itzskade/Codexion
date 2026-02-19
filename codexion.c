/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:11:17 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/12 09:01:15 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int check_args(int ac, char **av)
{
    int i;

    if (ac != EXPECTED_ARGS)
        return (ft_error("Error: Wrong number of arguments"), 0);
    i = 1;
    while (i < EXPECTED_ARGS - 1)
    {
        if (!ft_ispositive(av[i]))
            return (ft_error("Error: Argument must be a positive number"), 0);
        i++;
    }
    if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
        return (ft_error("Error: Scheduler must be fifo or edf"), 0);

    return 1;
}

int main(int ac, char **av)
{
    t_data  data;
    int     i;

    if (!check_args(ac, av))
        return (1);
    parse_input(&data, av);
    data_init(&data);
    data.start_simulation = current_time_ms();
    data.end_simulation = 0;
    i = -1;
    while (++i < data.number_of_coders)
    {
        if (pthread_create(&data.coders[i].thread_id, NULL,
                           coder_routine, &data.coders[i]) != 0)
            return (ft_error("Error creating thread"), 1);
    }
    i = -1;
    while (++i < data.number_of_coders)
        pthread_join(data.coders[i].thread_id, NULL);
    free(data.coders);
    free(data.dongles);
    return 0;
}