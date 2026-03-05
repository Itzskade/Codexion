/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_fifo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:59:38 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:59:39 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	queue_full(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_lock);
	if (dongle->first_in_queue == NULL)
	{
		dongle->first_in_queue = coder;
		if (dongle->second_in_queue == coder)
			dongle->second_in_queue = NULL;
		pthread_mutex_unlock(&dongle->queue_lock);
		return (1);
	}
	if (dongle->second_in_queue == NULL)
		dongle->second_in_queue = coder;
	pthread_mutex_unlock(&dongle->queue_lock);
	return (0);
}

static int	get_right_dongle(t_coder *coder)
{
	while (queue_full(coder, coder->rd))
	{
		if (check_flag(coder))
			return (1);
		usleep(500);
	}
	if (wait_cooldown(coder, coder->rd))
	{
		pthread_mutex_lock(&coder->rd->queue_lock);
		coder->rd->first_in_queue = NULL;
		pthread_mutex_unlock(&coder->rd->queue_lock);
		return (1);
	}
	pthread_mutex_lock(&coder->rd->take_lock);
	pthread_mutex_lock(&coder->rd->queue_lock);
	coder->rd->first_in_queue = NULL;
	pthread_mutex_unlock(&coder->rd->queue_lock);
	return (0);
}

static int	get_left_dongle(t_coder *coder)
{
	while (queue_full(coder, coder->ld))
	{
		if (check_flag(coder))
		{
			pthread_mutex_unlock(&coder->rd->take_lock);
			return (1);
		}
		usleep(500);
	}
	if (wait_cooldown(coder, coder->ld))
	{
		pthread_mutex_unlock(&coder->rd->take_lock);
		pthread_mutex_lock(&coder->ld->queue_lock);
		coder->ld->first_in_queue = NULL;
		pthread_mutex_unlock(&coder->ld->queue_lock);
		return (1);
	}
	pthread_mutex_lock(&coder->ld->take_lock);
	pthread_mutex_lock(&coder->ld->queue_lock);
	coder->ld->first_in_queue = NULL;
	pthread_mutex_unlock(&coder->ld->queue_lock);
	return (0);
}

int	run_fifo(t_coder *coder)
{
	if (get_right_dongle(coder))
		return (1);
	if (get_left_dongle(coder))
		return (1);
	return (0);
}
