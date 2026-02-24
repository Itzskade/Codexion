/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:59:12 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 15:11:45 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	sleep_and_check(t_coder *coder, long ms)
{
	long	end;

	end = get_time_in_ms() + ms;
	while (get_time_in_ms() < end)
	{
		if (check_flag(coder))
			return (1);
		usleep(1000);
	}
	return (0);
}

static int	compile_routine(t_coder *coder)
{
	int		err;

	if (coder->data->scheduler == SCHED_FIFO)
		err = run_fifo(coder);
	else
		err = run_edf(coder);
	if (err)
		return (1);
	write_log(coder, "is compiling");
	pthread_mutex_lock(&coder->compile_lock);
	coder->last_compile = get_time_in_ms();
	pthread_mutex_unlock(&coder->compile_lock);
	if (sleep_and_check(coder, coder->data->time_to_compile))
		return (unlock_dongles(coder));
	return (0);
}

static int	do_action(t_coder *coder, char *msg, long duration)
{
	write_log(coder, msg);
	return (sleep_and_check(coder, duration));
}

static void	run_routine(t_coder *coder)
{
	if (coder->data->number_of_coders == 1)
		return (solo_coder(coder));
	if (coder->id % 2 != 0)
		usleep(1000);
	while (1)
	{
		if (compile_routine(coder))
			break ;
		update_values(coder);
		if (do_action(coder, "is debugging", coder->data->time_to_debug))
			break ;
		if (do_action(coder, "is refactoring", coder->data->time_to_refactor))
			break ;
	}
}

void	*routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*tmp;

	coder = (t_coder *)arg;
	if (coder->id % 2)
	{
		tmp = coder->rd;
		coder->rd = coder->ld;
		coder->ld = tmp;
	}
	threads_wait(coder->data);
	if (check_flag(coder))
		return (NULL);
	run_routine(coder);
	return (NULL);
}
