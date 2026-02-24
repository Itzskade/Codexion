/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:59:54 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 14:59:56 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	solo_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->rd->take_lock);
	write_log(coder, "has taken a dongle");
	usleep((coder->data->time_to_burnout * 1000) + 1000);
	pthread_mutex_unlock(&coder->rd->take_lock);
}
