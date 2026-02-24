/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:58:33 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:58:35 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_next_time(t_coder *coder)
{
	return (get_time_in_ms() + coder->data->dongle_cooldown);
}

int	wait_cooldown(t_coder *coder, t_dongle *dongle)
{
	while (!check_flag(coder))
	{
		pthread_mutex_lock(&dongle->cooldown_lock);
		if (dongle->next_available_time <= get_time_in_ms())
		{
			pthread_mutex_unlock(&dongle->cooldown_lock);
			return (0);
		}
		pthread_mutex_unlock(&dongle->cooldown_lock);
		usleep(500);
	}
	return (1);
}

void	exit_queue(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_lock);
	if (dongle->first_in_queue == coder)
		dongle->first_in_queue = NULL;
	else
		dongle->second_in_queue = NULL;
	pthread_mutex_unlock(&dongle->queue_lock);
}

int	unlock_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->rd->take_lock);
	pthread_mutex_unlock(&coder->ld->take_lock);
	return (1);
}

void	update_values(t_coder *coder)
{
	pthread_mutex_lock(&coder->rd->cooldown_lock);
	coder->rd->next_available_time = get_next_time(coder);
	pthread_mutex_unlock(&coder->rd->cooldown_lock);
	pthread_mutex_lock(&coder->ld->cooldown_lock);
	coder->ld->next_available_time = get_next_time(coder);
	pthread_mutex_unlock(&coder->ld->cooldown_lock);
	pthread_mutex_lock(&coder->compile_lock);
	coder->times_compiled++;
	pthread_mutex_unlock(&coder->compile_lock);
	pthread_mutex_unlock(&coder->rd->take_lock);
	pthread_mutex_unlock(&coder->ld->take_lock);
}
