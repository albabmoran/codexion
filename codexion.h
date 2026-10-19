/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albben-a <albben-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 13:31:57 by albben-a          #+#    #+#             */
/*   Updated: 2026/10/19 13:38:20 by albben-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h> /*gettimeofday*/
# include <unistd.h> /*usleep, write*/
# include <stdlib.h> /*malloc, atoi*/
# include <stdio.h> /*printf*/
# include <string.h> /*strcmp, strlen, memset*/

typedef struct s_name
{
    int a;
}               t_name;

#endif