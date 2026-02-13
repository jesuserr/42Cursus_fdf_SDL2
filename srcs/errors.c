/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:10:39 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/13 12:46:46 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_error_handler(int error)
{
	if (error == ERROR_ARGS)
		ft_printf ("%sInvalid arguments - One single map expected\n", RED);
	else if (error == ERROR_FILE)
		ft_printf ("%sError opening file\n", RED);
	else if (error == ERROR_MEM)
		ft_printf ("%sError allocating memory\n", RED);
	else if (error == ERROR_EMPTY)
		ft_printf ("%sError - Empty map\n", RED);
	else if (error == ERROR_MAP)
		ft_printf ("%sInvalid map format\n", RED);
	else if (error == ERROR_SDL)
		ft_printf ("%sError starting SDL instances\n", RED);
	exit(EXIT_FAILURE);
}

/* To be used mainly with double pointers used on ft_split */
void	free_split(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

/* Another free function to save space */
void	free_split_and_exit(char **str, int error, t_fdf *fdf)
{
	size_t	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	munmap(fdf->raw_map, fdf->raw_map_size);
	ft_error_handler(error);
}

/* Function to exit in controlled way when there is an error during */
/* the SDL init */
void	free_map_and_exit(t_fdf *fdf, int error)
{
	if (fdf->sdl.renderer)
		SDL_DestroyRenderer(fdf->sdl.renderer);
	if (fdf->sdl.window)
		SDL_DestroyWindow(fdf->sdl.window);
	SDL_Quit();
	munmap(fdf->raw_map, fdf->raw_map_size);
	free(fdf->map);
	ft_error_handler(error);
}
