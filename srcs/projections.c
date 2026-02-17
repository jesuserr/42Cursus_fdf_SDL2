/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 19:43:38 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/17 11:32:34 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	project_x_lines(t_fdf *fdf);
static void	project_y_lines(t_fdf *fdf);
static void	project_points(t_fdf *fdf);
static void	show_fps(t_fdf *fdf);

/* Scales figure according to screen size, INIT_SCALE value and user zoom */
/* Uses the worst case scale (smaller one) between x and y */
// Clears the renderer, draws either the full wireframe mesh or individual
// points based on render mode, optionally displays FPS counter and presents
// the final frame to the display buffer.
void	projection(t_fdf *fdf)
{
	SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(fdf->sdl.renderer);
	fdf->scale_x = (WIDTH * INIT_SCALE * fdf->zoom) / (fdf->x_elem - 1);
	fdf->scale_y = (HEIGHT * INIT_SCALE * fdf->zoom) / (fdf->y_elem - 1);
	if (fdf->scale_x < fdf->scale_y)
		fdf->scale = fdf->scale_x;
	else
		fdf->scale = fdf->scale_y;
	if (fdf->render_only_points)
		project_points(fdf);
	else
	{
		project_x_lines(fdf);
		project_y_lines(fdf);
	}
	if (fdf->show_fps)
		show_fps(fdf);
	SDL_RenderPresent(fdf->sdl.renderer);
}

/* Draws a line between each pair of horizontal points */
/* A copy of struct "line" is passed to draw_line since */
/* values are modified inside and would affect this function */
/* Offset used for translation of the figure by user */
static void	project_x_lines(t_fdf *fdf)
{
	t_line	line;
	int		i;

	i = 0;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		line.x0 = (fdf->map[i].x * fdf->scale) + (WIDTH / 2) + fdf->offset_x;
		line.y0 = (fdf->map[i].y * fdf->scale) + (HEIGHT / 2) + fdf->offset_y;
		if (fdf->map[i].color == DEF_COLOR && fdf->render_color_gradient)
			line.color0 = fdf->map[i].color_gradient;
		else
			line.color0 = fdf->map[i].color;
		if ((i % fdf->x_elem) != 0 && is_line_visible(&line))
			draw_line(line, fdf);
		line.x1 = line.x0;
		line.y1 = line.y0;
		i++;
	}
}

/* Draws a line between each pair of vertical points */
/* A copy of struct "line" is passed to draw_line since */
/* values are modified inside and would affect this function */
/* Offset used for translation of the figure by user */
static void	project_y_lines(t_fdf *fdf)
{
	t_line	line;
	int		i;

	i = 0;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		line.x0 = (fdf->map[i].x * fdf->scale) + (WIDTH / 2) + fdf->offset_x;
		line.y0 = (fdf->map[i].y * fdf->scale) + (HEIGHT / 2) + fdf->offset_y;
		if (fdf->map[i].color == DEF_COLOR && fdf->render_color_gradient)
			line.color0 = fdf->map[i].color_gradient;
		else
			line.color0 = fdf->map[i].color;
		if (i >= fdf->x_elem && is_line_visible(&line))
			draw_line(line, fdf);
		line.x1 = line.x0;
		line.y1 = line.y0;
		i = i + fdf->x_elem;
		if (i == ((fdf->x_elem * fdf->y_elem) + fdf->x_elem - 1))
			break ;
		if (i >= (fdf->x_elem * fdf->y_elem))
			i = i - ((fdf->x_elem * fdf->y_elem) - 1);
	}
}

/* Draws only the points, no lines */
/* Values outside screen boundaries are not printed */
/* Offset used for translation of the figure by user */
static void	project_points(t_fdf *fdf)
{
	int		i;
	float	x;
	float	y;

	i = 0;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		x = (fdf->map[i].x * fdf->scale) + (WIDTH / 2) + fdf->offset_x;
		y = (fdf->map[i].y * fdf->scale) + (HEIGHT / 2) + fdf->offset_y;
		if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT)
		{
			if (fdf->map[i].color == DEF_COLOR && fdf->render_color_gradient)
				sdl_put_pixel(fdf, x, y, fdf->map[i].color_gradient);
			else
				sdl_put_pixel(fdf, x, y, fdf->map[i].color);
		}
		i++;
	}
}

// Displays the current FPS counter in the top-left corner of the screen.
static void	show_fps(t_fdf *fdf)
{
	char		*fps;

	fps = NULL;
	SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, 0, 65, 10});
	stringColor(fdf->sdl.renderer, 0, 0, "FPS:", RGBA_WHITE);
	fps = ft_itoa(1000 / fdf->frame_time);
	if (fps)
	{
		stringColor(fdf->sdl.renderer, 40, 0, fps, RGBA_WHITE);
		free(fps);
	}
}
