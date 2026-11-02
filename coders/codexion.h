/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 13:31:57 by albben-a          #+#    #+#             */
/*   Updated: 2026/11/02 05:58:36 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h> /*gettimeofday*/
# include <unistd.h> /*usleep, write*/
# include <stdlib.h> /*malloc, atoi, free*/
# include <stdio.h> /*printf*/
# include <string.h> /*strcmp, strlen, memset*/
# include <stdint.h> /*uint64_t*/

typedef enum s_scheduler
{
	FIFO,
	EDF,
}	t_scheduler;

typedef struct s_dongle
{
	int					id;
	uint64_t			available_at;
	pthread_mutex_t		mutex;

}				t_dongle;

typedef struct s_log
{
	char			*message;
	struct s_log	*next;
}				t_log;

typedef struct s_log_queue
{
	t_log			*head;
	t_log			*tail;
	int				closed;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_log_queue;

typedef struct s_coder
{
	int					id;
	int					compiles_done;
	uint64_t			last_compile_start;
	pthread_t			thread;
	struct s_data		*data;

}				t_coder;

typedef struct s_request
{
	t_coder		*coder;
	uint64_t	request_time;
	uint64_t	deadline;
}				t_request;

typedef struct s_heap
{
	t_request		*requests;
	int				size;
	int				capacity;
	t_scheduler		scheduler;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_heap;

typedef struct s_data
{
	int					nb_coders;
	uint64_t			time_to_burnout;
	uint64_t			time_to_compile;
	uint64_t			time_to_debug;
	uint64_t			time_to_refactor;
	int					required_compiles;
	uint64_t			dongle_cooldown;
	t_scheduler			scheduler;

	uint64_t			start_time;
	int					stop;

	struct s_coder		*coders;
	struct s_dongle		*dongles;

	pthread_t			printer;
	pthread_t			monitor;
	pthread_t			req_manager;
	pthread_mutex_t		stop_mutex;
	t_log_queue			logs;
	t_heap				heap;

	int					dongles_mutex_init;
	int					stop_mutex_init;
	int					logs_mutex_init;
	int					logs_cond_init;
	int					heap_mutex_init;
	int					heap_cond_init;
	int					cod_thread_init;
	int					print_thread_init;
	int					mon_thread_init;
	int					rman_thread_init;
}				t_data;

int			parse_args(int argc, char **argv, t_data *data);

int			initializer(t_data *data);
int			init_data(t_data *data);
void		init_coders(t_data *data);
int			init_dongles(t_data *data);
int			init_heap(t_data *data);
int			init_threads(t_data *data);

uint64_t	get_time(void);

void		*coder_routine(void *coder);
void		*printer_routine(void *data);
void		*monitor_routine(void *data);
void		*req_manager_routine(void *data);

int			priority_request(t_heap *heap, t_request *a, t_request *b);
int			heap_push(t_heap *heap, t_request a);
int			heap_pop(t_heap *heap, t_request *req);

void		cleanup(t_data *data);
void		end_threads(t_data *data);
void		stop_simulation(t_data *data);

#endif