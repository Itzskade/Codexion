/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:01:44 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 16:09:17 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	clear_threads(t_data *data, long created)
{
	long	i;

	pthread_mutex_lock(&data->flag_lock);
	data->monitor_flag = 1;
	pthread_mutex_unlock(&data->flag_lock);
	pthread_mutex_lock(&data->threads_lock);
	data->threads_ready = 1;
	pthread_cond_broadcast(&data->threads_cond);
	pthread_mutex_unlock(&data->threads_lock);
	i = 0;
	while (i < created)
		pthread_join(data->coders[i++].thread_id, NULL);
	return (1);
}

static int	start_threads(t_data *data)
{
	long		i;
	pthread_t	monitor;

	data->start_time = get_time_in_ms();
	i = -1;
	while (++i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread_id,
				NULL, &routine, &data->coders[i]))
			return (clear_threads(data, i));
	}
	if (pthread_create(&monitor, NULL, &monitor_routine, data))
		return (clear_threads(data, i));
	pthread_mutex_lock(&data->threads_lock);
	data->threads_ready = 1;
	pthread_cond_broadcast(&data->threads_cond);
	pthread_mutex_unlock(&data->threads_lock);
	i = 0;
	while (i < data->number_of_coders)
		pthread_join(data->coders[i++].thread_id, NULL);
	pthread_join(monitor, NULL);
	return (0);
}

static int	check_args(int ac, char **av)
{
	int	i;

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
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (!check_args(ac, av))
		return (1);
	if (parse_input(&data, av))
		return (1);
	if (data_init(&data))
		return (1);
	if (start_threads(&data))
		return (1);
	data_destroy(&data);
	return (0);
}
