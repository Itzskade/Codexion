/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:02:37 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/19 10:02:38 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    take_dongle(t_coder *coder, t_dongle *dongle)
{
    (void)coder;
    pthread_mutex_lock(&dongle->mutex);

    while (dongle->is_taken)
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    dongle->is_taken = 1;
    pthread_mutex_unlock(&dongle->mutex);
}

int     is_earliest_deadline(t_coder *coder, t_dongle *dongle)
{
    t_data *data = coder->data;
    int i = 0;

    while (i < data->number_of_coders)
    {
        t_coder *other = &data->coders[i];
        if ((other->left_dongle == dongle || other->right_dongle == dongle)
            && other != coder && other->deadline < coder->deadline)
            return 0;
        i++;
    }
    return 1;
}

void    take_dongles_edf(t_coder *coder)
{
    t_dongle    *first;
    t_dongle    *second;

    if (coder->left_dongle < coder->right_dongle)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    pthread_mutex_lock(&first->mutex);
    while (first->is_taken || !is_earliest_deadline(coder, first))
        pthread_cond_wait(&first->cond, &first->mutex);
    first->is_taken = 1;
    pthread_mutex_unlock(&first->mutex);
    pthread_mutex_lock(&second->mutex);
    while (second->is_taken || !is_earliest_deadline(coder, second))
        pthread_cond_wait(&second->cond, &second->mutex);
    second->is_taken = 1;
    pthread_mutex_unlock(&second->mutex);
}

void release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->is_taken = 0;
    pthread_cond_broadcast(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
}

void    release_dongles(t_coder *coder)
{
    t_dongle *first;
    t_dongle *second;

    if (coder->left_dongle < coder->right_dongle)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    release_dongle(first);
    release_dongle(second);
}