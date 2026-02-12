/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:03:40 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:23 by jesuserr         ###   ########.fr       */
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
void	draw_line(t_line line, t_fdf *fdf)
{
	t_line_aux	line_aux;

	line_direction (&line, &line_aux);
	line_aux.dx = abs(line.x1 - line.x0);
	line_aux.dy = -abs(line.y1 - line.y0);
	line_aux.error = line_aux.dx + line_aux.dy;
	while (!(line.x0 == line.x1 && line.y0 == line.y1))
	{
		if (line.x0 >= 0 && line.y0 >= 0 && line.x0 < WIDTH && line.y0 < HEIGHT)
			sdl_put_pixel(fdf, line.x0, line.y0, line.color0);
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
