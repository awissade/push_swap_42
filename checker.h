/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abwissad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 14:09:37 by abwissad          #+#    #+#             */
/*   Updated: 2026/10/06 16:35:35 by abwissad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_H
# define CHECKER_H

# include "push_swap.h"
# define BUFFER_SIZE 1024
# define LINE_END '\n'

// these are for the get next line function
// reading from the file descriptor 1
char	*get_next_line(int fd);
char	*get_the_line(char **stash);
void	new_stash(char **stash, size_t i);
char	*ft_strjoin_ad(char **str, char *str2);
void	fast_copy(char *dst, char *src, size_t len);
size_t	ft_strlen(const char *str);
bool	ft_strchr_ad(char *str, char c);

#endif 
