/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:58:53 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:58:56 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	max_compiles_reached(t_data *data, long finished)
{
	if (finished >= data->number_of_coders)
	{
		pthread_mutex_lock(&data->flag_lock);
		data->monitor_flag = true;
		pthread_mutex_unlock(&data->flag_lock);
		return (1);
	}
	return (0);
}

static int	raise_flag(t_data *data)
{
	long	i;
	long	finished;

	i = 0;
	finished = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].compile_lock);
		if (get_time_in_ms() - data->coders[i].last_compile
			>= data->time_to_burnout)
		{
			pthread_mutex_unlock(&data->coders[i].compile_lock);
			pthread_mutex_lock(&data->flag_lock);
			data->monitor_flag = true;
			pthread_mutex_unlock(&data->flag_lock);
			write_log(&data->coders[i], "burned out");
			return (1);
		}
		if (data->coders[i].times_compiled
			>= data->number_of_compiles_required)
			finished++;
		pthread_mutex_unlock(&data->coders[i].compile_lock);
		i++;
	}
	return (max_compiles_reached(data, finished));
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	threads_wait(data);
	while (1)
	{
		if (raise_flag(data))
			return (NULL);
		usleep(2000);
	}
	return (NULL);
}
