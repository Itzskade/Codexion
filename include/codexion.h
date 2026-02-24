/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:00:28 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/24 16:07:44 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define SCHED_FIFO 0
# define SCHED_EDF  1

# define EXPECTED_ARGS 9

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

struct s_dongle
{
	long			next_available_time;
	pthread_mutex_t	take_lock;
	pthread_mutex_t	cooldown_lock;
	pthread_mutex_t	queue_lock;
	t_coder			*first_in_queue;
	t_coder			*second_in_queue;
};

struct s_coder
{
	pthread_t		thread_id;
	int				id;
	t_data			*data;
	t_dongle		*rd;
	t_dongle		*ld;
	long			last_compile;
	long			times_compiled;
	pthread_mutex_t	compile_lock;
};

struct s_data
{
	long			number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	long			start_time;
	t_dongle		*dongle;
	t_coder			*coders;
	pthread_mutex_t	print_lock;
	int				threads_ready;
	pthread_mutex_t	threads_lock;
	pthread_cond_t	threads_cond;
	int				monitor_flag;
	pthread_mutex_t	flag_lock;
};

/* utils.c */
long	get_time_in_ms(void);
int		write_log(t_coder *coder, char *action);
void	threads_wait(t_data *data);
int		check_flag(t_coder *coder);

/* utils2.c */
void	ft_error(const char *msg);
int		ft_isdigit(char c);
int		ft_ispositive(char *s);
int		ft_isspace(char c);
void	*safe_malloc(size_t bytes);

/* parse.c */
int		parse_input(t_data *data, char **av);

/* init.c */
int		data_init(t_data *data);
void	data_destroy(t_data *data);

/* dongle.c */
long	get_next_time(t_coder *coder);
int		wait_cooldown(t_coder *coder, t_dongle *dongle);
void	exit_queue(t_coder *coder, t_dongle *dongle);
int		unlock_dongles(t_coder *coder);
void	update_values(t_coder *coder);

/* run_fifo.c */
int		run_fifo(t_coder *coder);

/* run_edf.c */
int		run_edf(t_coder *coder);

/* routine.c */
void	*routine(void *arg);

/* monitor.c */
void	*monitor_routine(void *arg);

/* solo.c */
void	solo_coder(t_coder *coder);

#endif
