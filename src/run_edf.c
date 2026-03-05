/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_edf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:59:23 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:59:24 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_coder	*closest_deadline(t_dongle *dongle)
{
	t_coder	*c1;
	t_coder	*c2;
	long	d1;
	long	d2;

	pthread_mutex_lock(&dongle->queue_lock);
	c1 = dongle->first_in_queue;
	c2 = dongle->second_in_queue;
	pthread_mutex_unlock(&dongle->queue_lock);
	pthread_mutex_lock(&c1->compile_lock);
	d1 = c1->last_compile;
	pthread_mutex_unlock(&c1->compile_lock);
	pthread_mutex_lock(&c2->compile_lock);
	d2 = c2->last_compile;
	pthread_mutex_unlock(&c2->compile_lock);
	if (d1 < d2)
		return (c1);
	return (c2);
}

static t_coder	*highest_prio(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_lock);
	if (dongle->first_in_queue == NULL)
	{
		dongle->first_in_queue = dongle->second_in_queue;
		dongle->second_in_queue = NULL;
		pthread_mutex_unlock(&dongle->queue_lock);
		return (dongle->first_in_queue);
	}
	if (dongle->second_in_queue == NULL)
	{
		pthread_mutex_unlock(&dongle->queue_lock);
		return (dongle->first_in_queue);
	}
	pthread_mutex_unlock(&dongle->queue_lock);
	return (closest_deadline(dongle));
}

static int	get_right_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->rd->queue_lock);
	if (coder->rd->first_in_queue == NULL)
		coder->rd->first_in_queue = coder;
	else
		coder->rd->second_in_queue = coder;
	pthread_mutex_unlock(&coder->rd->queue_lock);
	if (wait_cooldown(coder, coder->rd))
		return (exit_queue(coder, coder->rd), -1);
	while (highest_prio(coder->rd) != coder)
	{
		if (check_flag(coder))
			return (exit_queue(coder, coder->rd), -1);
		usleep(500);
	}
	pthread_mutex_lock(&coder->rd->take_lock);
	exit_queue(coder, coder->rd);
	return (0);
}

static int	get_left_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->ld->queue_lock);
	if (coder->ld->first_in_queue == NULL)
		coder->ld->first_in_queue = coder;
	else
		coder->ld->second_in_queue = coder;
	pthread_mutex_unlock(&coder->ld->queue_lock);
	if (wait_cooldown(coder, coder->ld))
	{
		pthread_mutex_unlock(&coder->rd->take_lock);
		return (exit_queue(coder, coder->ld), -1);
	}
	while (highest_prio(coder->ld) != coder)
	{
		if (check_flag(coder))
		{
			pthread_mutex_unlock(&coder->rd->take_lock);
			return (exit_queue(coder, coder->ld), -1);
		}
		usleep(500);
	}
	pthread_mutex_lock(&coder->ld->take_lock);
	exit_queue(coder, coder->ld);
	return (0);
}

int	run_edf(t_coder *coder)
{
	if (get_right_dongle(coder))
		return (1);
	if (get_left_dongle(coder))
		return (1);
	return (0);
}
