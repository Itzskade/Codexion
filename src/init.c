/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:58:46 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:58:47 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle(t_data *data, long i)
{
	data->dongle[i].next_available_time = 0;
	data->dongle[i].first_in_queue = NULL;
	data->dongle[i].second_in_queue = NULL;
	if (pthread_mutex_init(&data->dongle[i].take_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->dongle[i].cooldown_lock, NULL))
	{
		pthread_mutex_destroy(&data->dongle[i].take_lock);
		return (1);
	}
	if (pthread_mutex_init(&data->dongle[i].queue_lock, NULL))
	{
		pthread_mutex_destroy(&data->dongle[i].take_lock);
		pthread_mutex_destroy(&data->dongle[i].cooldown_lock);
		return (1);
	}
	return (0);
}

static int	init_dongles(t_data *data)
{
	long	i;

	data->dongle = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongle)
		return (1);
	i = 0;
	while (i < data->number_of_coders)
	{
		if (init_dongle(data, i))
			return (1);
		i++;
	}
	return (0);
}

static int	init_coders(t_data *data)
{
	long	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (-1);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].rd = &data->dongle[i];
		data->coders[i].ld = &data->dongle[(i + data->number_of_coders - 1)
			% data->number_of_coders];
		data->coders[i].last_compile = get_time_in_ms();
		data->coders[i].times_compiled = 0;
		if (pthread_mutex_init(&data->coders[i].compile_lock, NULL))
			return (-1);
		i++;
	}
	return (0);
}

int	data_init(t_data *data)
{
	data->monitor_flag = false;
	data->threads_ready = false;
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->flag_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->threads_lock, NULL))
		return (1);
	if (pthread_cond_init(&data->threads_cond, NULL))
		return (1);
	if (init_dongles(data))
		return (1);
	if (init_coders(data))
		return (1);
	return (0);
}

void	data_destroy(t_data *data)
{
	long	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongle[i].take_lock);
		pthread_mutex_destroy(&data->dongle[i].cooldown_lock);
		pthread_mutex_destroy(&data->dongle[i].queue_lock);
		pthread_mutex_destroy(&data->coders[i].compile_lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->flag_lock);
	pthread_mutex_destroy(&data->threads_lock);
	pthread_cond_destroy(&data->threads_cond);
	free(data->dongle);
	free(data->coders);
}
