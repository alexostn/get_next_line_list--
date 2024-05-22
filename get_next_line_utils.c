/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oostapen <oostapen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 21:52:41 by oostapen          #+#    #+#             */
/*   Updated: 2024/05/22 23:59:06 by oostapen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Looks for a newline character in the given linked list*/

int	found_newline(t_list *stash)
{
	int		i; /*index*/
	t_list	*current; /*pointer to structure*/
	
	if (stash == NULL)
		return (0);/*no we haven't found new line(the first time)*/
	current = ft_lst_get_last(stash);/*in case BUFFER is 1 and line 50 symbols Better to begin from end It returns ptr*/
	i = 0;
	while (current->content[i])/*we don't check while(current), because it already was checked for NULL above*/
	{
		if (current->content[i] == '\n')
			return (1);/*yes I found \n ew line*/
		i++;/*if not I increment*/
	}
		return (0);/*found any \n to to off the loop*/
}

/* Returns a pointer to the last node in our stash*/

t_list	*ft_lst_get_last(t_list *stash)
{
	t_list	*current;
	
	current = stash;
	while (current && current->next)
		current = current->next;
	return (current);
}

/*Calculates the number of chars in the current line,
including the trailing \n if there is one, and allocates memory accordingly */

void	generate_line(char **line, t_list *stash)
{
	int	i;
	int	len;
	
	len = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				len++;/*increment size of my line +1 and break*/
				break;
			}
			len++;/*if not increase number of characters*/
			i++;
		}
		stash = stash->next;/*to avoid infinite loop*/
	}
	*line = malloc(sizeof(char) * (len + 1));
}

/*Frees the entire stash. */

void	free_stash(t_list *stash)
{
	t_list	*current;
	t_list	*next;

	current = stash;
	while (current)
	{
		free(current->content);
		next = current->next;/*keep me a pointer to next element,
		and we will have a chance to free element on wich we are located*/
		free(current);
		current = next;
	}
}

int	ft_strlen(const char *str)
{
	int	len;
	
	len = 0;
	while (*(str++))
		len++;
	return (len);
}