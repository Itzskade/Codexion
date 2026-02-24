/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:00:07 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 15:00:08 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	write_log(t_coder *coder, char *action)
{
	long	ts;

	ts = get_time_in_ms() - coder->data->start_time;
	pthread_mutex_lock(&coder->data->flag_lock);
	if (coder->data->monitor_flag)
	{
		pthread_mutex_unlock(&coder->data->flag_lock);
		pthread_mutex_lock(&coder->data->print_lock);
		if (!strcmp("burned out", action))
			printf("%ld %d %s\n", ts, coder->id, action);
		pthread_mutex_unlock(&coder->data->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&coder->data->flag_lock);
	pthread_mutex_lock(&coder->data->print_lock);
	printf("%ld %d %s\n", ts, coder->id, action);
	pthread_mutex_unlock(&coder->data->print_lock);
	return (0);
}

void	threads_wait(t_data *data)
{
	pthread_mutex_lock(&data->threads_lock);
	while (!data->threads_ready)
		pthread_cond_wait(&data->threads_cond, &data->threads_lock);
	pthread_mutex_unlock(&data->threads_lock);
}

int	check_flag(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->flag_lock);
	if (coder->data->monitor_flag)
	{
		pthread_mutex_unlock(&coder->data->flag_lock);
		return (1);
	}
	pthread_mutex_unlock(&coder->data->flag_lock);
	return (0);
}
