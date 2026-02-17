/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:03:40 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/17 11:40:51 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	line_direction(t_line *line, t_line_aux *line_aux);

void	sdl_put_pixel(t_fdf *fdf, int x, int y, int color)
{
	Uint8	r;
	Uint8	g;
	Uint8	b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	SDL_SetRenderDrawColor(fdf->sdl.renderer, r, g, b, 255);
	SDL_RenderDrawPoint(fdf->sdl.renderer, x, y);
}

/* Uses Bresenham's line algorithm (extended to work in any octant) */
/* Pixels outside screen boundaries are not printed */
// Since line is composed by just one color, SDL_SetRenderDrawColor is called
// just once here and not every time we render a pixel.
void	draw_line(t_line line, t_fdf *fdf)
{
	t_line_aux	line_aux;

	SDL_SetRenderDrawColor(fdf->sdl.renderer, (line.color0 >> 16) & 0xFF, \
	(line.color0 >> 8) & 0xFF, line.color0 & 0xFF, 255);
	line_direction (&line, &line_aux);
	line_aux.dx = abs(line.x1 - line.x0);
	line_aux.dy = -abs(line.y1 - line.y0);
	line_aux.error = line_aux.dx + line_aux.dy;
	while (!(line.x0 == line.x1 && line.y0 == line.y1))
	{
		if (line.x0 >= 0 && line.y0 >= 0 && line.x0 < WIDTH && line.y0 < HEIGHT)
			SDL_RenderDrawPoint(fdf->sdl.renderer, line.x0, line.y0);
		if ((2 * line_aux.error) >= line_aux.dy)
		{
			line_aux.error = line_aux.error + line_aux.dy;
			line.x0 = line.x0 + line_aux.sx;
		}
		else
		{
			line_aux.error = line_aux.error + line_aux.dx;
			line.y0 = line.y0 + line_aux.sy;
		}
	}
}

static void	line_direction(t_line *line, t_line_aux *line_aux)
{
	if (line->x0 < line->x1)
		line_aux->sx = 1;
	else
		line_aux->sx = -1;
	if (line->y0 < line->y1)
		line_aux->sy = 1;
	else
		line_aux->sy = -1;
}

// Quick visibility check to skip drawing lines completely outside viewport
// by rejecting cases where both endpoints are on the same side of screen.
// Some diagonal lines outside the visible field won't be caught due to the
// algorithm's simplicity but the performance gain is still significant.
bool	is_line_visible(t_line *line)
{
	if (!((line->x0 < 0 && line->x1 < 0) || (line->y0 < 0 && line->y1 < 0) || \
	(line->x0 >= WIDTH && line->x1 >= WIDTH) || \
	(line->y0 >= HEIGHT && line->y1 >= HEIGHT)))
		return (true);
	return (false);
}
