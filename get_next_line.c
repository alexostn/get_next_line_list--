/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oostapen <oostapen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 20:16:36 by oostapen          #+#    #+#             */
/*   Updated: 2024/05/22 23:59:23 by oostapen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>

char	*get_next_line(int fd)
{
	static t_list	*stash;
	char			*line;
	
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
		return (NULL);
	line = NULL;
	// 1. read from fd and add to linked list
	read_and_stash(fd, &stash);/*ptr to ptr to structrure &stash*/
	if (stash == NULL)
		return (NULL);
	// 2. extract from stash to line
	extract_line(stash, &line);
	// 3. clean up stash
	clean_stash(&stash);
	if (line[0] == '\0')
	{
		free_stash(stash);/*when everithing read we free stash*/
		stash = NULL;/*if you free smth it is not obligatory NULL by itself*/
		free(line);
		return (NULL);/*it over, nothing more to see*/
	}
	return (line);
}

/*Uses read() to add characters to stash*/

void	read_and_stash(int fd, t_list **stash)
{
	char	*buf;
	int		readed;
	
	while (!found_newline(*stash) && readed != 0)/*We dereference *stash no need to sent ptr to ptr.Found 1, not 0*/
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		readed = (int)read(fd, buf, BUFFER_SIZE);/*read returns int number of bytes, that was read 
		As it returns size_t casted to int */
		if ((*stash == NULL && readed == 0) || readed == -1)
	{
		free(buf);
		return ;
	}
	buf[readed] = '\0';/*to close our character list*/
	add_to_stash(stash, buf, readed);/*readed_ptr to know size of stash, not to BUFFER directly, because if BUFFER is 100000 it will take to much memory*/
	free(buf);
	}
}
/*adds the content of buffer to the end of the stash*/
void	add_to_stash(t_list **stash, char *buf, int readed)/*readed here is not ptr, because I sent the value*/
{
	int		i;
	t_list	*last;/*last element of stash*/
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;/*it is clearer to use VOID function in gnl no need return (0)*/
	new_node->next = NULL;/*initialize a pointer in my structure to NULL*/
	new_node->content = malloc(sizeof(char) * (readed + 1));/*store what was in my buffer
	Function receive reaed, no need in readed_ptr*/
	if (new_node->content == NULL)
		return ;/*I am out of here*/
	i = 0;/*initialize index i*/
	while(buf[i] && i < readed) /*only characters that was read*/
	{
		new_node->content[i] = buf[i];
		i++;/*copy everithing from BUFFER that has been read into new_node*/
	}
	new_node->content[i] = '\0';/*close as usual*/
	if (*stash == NULL) /*1)either nothing was read yet, 2)either everything was made already and fin added*/
	{
			*stash = new_node;
			return ;
	}
	last = ft_lst_get_last(*stash);/*last element of stash*/
	last->next = new_node;
	/*next element*/
}

/*Extracts all characters from the stash and stores them in out line
stoping after the first '\n' it encounters(extract line including\n)*/

void	extract_line(t_list *stash, char **line) /*takes pointer to *stash & ptr to ptr to **line */
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	generate_line(line, stash);/*to allocate enough memory for line 
	It will send ptr to ptr to **line 
	unless you would want only ptr to char, then derefference it: *line */
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break;
			}
			(*line)[j++] = stash->content[i++];/*if it is not \n Increment in single line to save space*/
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

/*After the line was read, we don't need those characters
clears the stash so only the characters,that have
not been returned at the end of get_next_line()remain in our static stash*/
	
void	clean_stash(t_list **stash)
{
	t_list *last;
	t_list *clean_node;
	int	i;
	int	j;

	clean_node = malloc(sizeof(t_list));/*for structure*/
	if (stash == NULL || clean_node == NULL)
		return ;/*he writes that it should be here not after malloc,
		but as we checked stash != NULL in get_next_line, that is not so important but I put before*/
	clean_node->next = NULL;
	last = ft_lst_get_last(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	clean_node->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i) + 1));
	/*for the lenth of all characters in the last element of my string, my stash + \n*/
	if (clean_node->content == NULL)
		return ; /*to do not have possibility to modify memory that has not been allocated*/
	j = 0;
	while(last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}


int	main()
{
	int		fd;
	char	*line;

	fd = open("tests/simple", O_RDONLY);
	while(1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break;
		printf("%s", line);
		free(line);
	}
	return (0);
}

