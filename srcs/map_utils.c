/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 14:23:42 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/14 13:58:21 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	count_x_elem(char *line);
static void	parse_map(t_fdf *fdf, char *line);
static int	get_hex_color(char *color);

// Uses 'mmap' to map the entire file into memory in one shot. Way more 
// efficient than reading the file multiple times with GNL. File size is kept
// for the 'munmap' function to know how many bytes to unmap when the program 
// finishes. Also counts the number of elements in the first line to determine
// later if the map is valid.
char	*read_map(char *file, t_fdf *fdf)
{
	int			fd;
	char		*raw_map;
	struct stat	file_stat;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		ft_error_handler(ERROR_FILE);
	if (fstat(fd, &file_stat) < 0 || file_stat.st_size == 0)
	{
		close(fd);
		ft_error_handler(ERROR_FILE);
	}
	fdf->raw_map_size = file_stat.st_size;
	ft_printf ("%sReading Map..... ", BLUE);
	raw_map = mmap(NULL, fdf->raw_map_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (raw_map == MAP_FAILED)
	{
		close(fd);
		ft_error_handler(ERROR_FILE);
	}
	fdf->x_elem = count_x_elem(raw_map);
	close (fd);
	return (raw_map);
}

/* Returns the number of elements on a given line (checks until \n or \0) */
/* Similar to function used in ft_split modified to use spaces as delimiters */
static int	count_x_elem(char *line)
{
	int	flag;
	int	n;

	flag = 0;
	n = 0;
	while (*line && *line != '\n')
	{
		if (*line == ' ')
			flag = 0;
		if (*line != ' ' && flag == 0)
		{
			flag = 1;
			n++;
		}
		line++;
	}
	return (n);
}

/* Verifies that all lines have the same number of elements */
/* and that no line contains any forbidden characters (ALLOWED_CHR) */
/* If there is only one line (no 3D element) -> finishes too */
/* After map parsing, centers and scales Z coordinates, figure 100% centered */
void	verify_and_parse_map(t_fdf *fdf)
{
	char	**split;
	char	*line;
	int		i;

	split = ft_split(fdf->raw_map, '\n');
	fdf->y_elem = 0;
	while (split[fdf->y_elem])
	{
		line = split[fdf->y_elem++];
		if (ft_strlen(line) != ft_strspn(line, ALLOWED_CHR)
			|| count_x_elem(line) != fdf->x_elem)
			free_split_and_exit(split, ERROR_MAP, fdf);
	}
	if (fdf->y_elem < 2)
		free_split_and_exit(split, ERROR_MAP, fdf);
	fdf->map = malloc (sizeof(t_point) * fdf->y_elem * fdf->x_elem);
	if (!fdf->map)
		free_split_and_exit(split, ERROR_MEM, fdf);
	i = 0;
	ft_printf ("%sOK!\nParsing Map..... ", BLUE);
	while (split[i])
		parse_map(fdf, split[i++]);
	z_centering(fdf);
	free_split(split);
}

/* Parses the map in such way that X = 0 and Y = 0 are in the center */
/* Determines max and min values of Z to center all heights later */
/* Note: k can be replaced by (i + (j * fdf->x_elem)) */
/*       harder to read but can help to save a couple of lines if needed */
static void	parse_map(t_fdf *fdf, char *line)
{
	char		**split;
	int			i;
	static int	j = 0;
	static int	k = 0;

	split = ft_split(line, ' ');
	i = 0;
	while (i < fdf->x_elem)
	{
		fdf->map[k].x = i + ((fdf->x_elem / -2.0) + 0.5);
		fdf->map[k].y = j + ((fdf->y_elem / -2.0) + 0.5);
		fdf->map[k].z = ft_atoi(split[i]);
		if (fdf->map[k].z > fdf->z_max)
			fdf->z_max = fdf->map[k].z;
		if (fdf->map[k].z < fdf->z_min)
			fdf->z_min = fdf->map[k].z;
		if (ft_strstr(split[i], ",0x"))
			fdf->map[k].color = get_hex_color(split[i]);
		else
			fdf->map[k].color = DEF_COLOR;
		k++;
		i++;
	}
	j++;
	free_split(split);
}

/* Converts the str containing the hex color to an int value */
/* similar philosophy as ft_atoi */
static int	get_hex_color(char *hex_color)
{
	char	**split;
	int		color;
	int		i;
	char	*str;

	i = 0;
	color = 0;
	split = ft_split(hex_color, ',');
	str = split[1] + 2;
	while (i < 6 && str[i] != '\0')
	{
		color = color * 16;
		if (str [i] >= '0' && str [i] <= '9')
			color = color + str [i] - '0';
		else if (str [i] >= 'a' && str [i] <= 'f')
			color = color + str [i] - 87;
		else if (str [i] >= 'A' && str [i] <= 'F')
			color = color + str [i] - 55;
		i++;
	}
	free_split(split);
	return (color);
}
