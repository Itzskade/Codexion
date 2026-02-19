/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarin-n <rmarin-n@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 11:32:18 by rmarin-n          #+#    #+#             */
/*   Updated: 2026/02/12 08:58:51 by rmarin-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define EXPECTED_ARGS 9

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <errno.h>
# include <limits.h>

typedef  pthread_mutex_t t_mtx;
typedef  pthread_cond_t t_cond;

typedef struct s_data t_data;

typedef enum   e_opcode
{
    LOCK,
    UNLOCK,
    INIT,
    DESTROY,
    CREATE,
    JOIN,
    WAIT,
    TIMEDWAIT,
    BROADCAST
}               t_opcode;


typedef struct s_dongle
{
    t_mtx       mutex;
    t_cond      cond;
    int         is_taken;
}               t_dongle;

typedef struct s_coder
{
    int         id;
    int         compiles_done;
    long        last_compile_start;
    long        deadline;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread_id;
    t_data      *data;
    long        deadline_time;
    int         burned_out; 
}               t_coder;

struct s_data
{
    long        number_of_coders;
    long        time_to_burnout;
    long        time_to_compile;
    long        time_to_debug;
    long        time_to_refactor;
    long        number_of_compiles_required;
    long        dongle_cooldown;
    int         scheduler;
    long        start_simulation;
    int         end_simulation;
    t_dongle    *dongles;
    t_coder     *coders;
    t_mtx       print_mutex;
    t_mtx       status_mutex;
};

/*utils.c*/
void    ft_error(const char *msg);
int     ft_isdigit(char c);
int     ft_ispositive(char *s);
int     ft_isspace(char c);
void    *safe_malloc(size_t bytes);

/*parse.c*/
void    parse_input(t_data *data, char **av);

/*init.c*/
void    data_init(t_data *data);

/*dongle.c*/
void    take_dongle(t_coder *coder, t_dongle *dongle);
int     is_earliest_deadline(t_coder *coder, t_dongle *dongle);
void    take_dongles_edf(t_coder *coder);
void    release_dongle(t_dongle *dongle);
void    release_dongles(t_coder *coder);

/*routine.c*/
void    *coder_routine(void *arg);

/*printer.c*/
long    current_time_ms(void);
void    print_status(t_coder *coder, const char *msg);
void    take_all(t_coder *coder);
void    release_all(t_coder *coder);

/*codexion.c*/
int     main(int ac, char **av);

#endif
