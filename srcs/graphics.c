/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:03:40 by jesuserr          #+#    #+#             */
/*   Updated: 2026/04/08 23:32:26 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	line_direction(t_line *line, t_line_aux *line_aux);

// If line thickness is default (1), just writes a single pixel directly to the
// locked texture buffer in ARGB8888 format and exits. For thicker lines, 
// calculates a circular brush pattern around the target pixel and also checks
// that none of these new pixels exceed the screen boundaries. Pixel bounds
// limits are checked by the caller. Transparency set to 0xFF (255 - Opaque).
void	sdl_put_pixel(t_fdf *fdf, int x, int y, int color)
{
	int	off_x;
	int	off_y;
	int	off;

	if (fdf->line_thickness == DEF_LINE_THK)
	{
		fdf->sdl.argb_pixels[(y * fdf->sdl.pixels_per_row) + x] = \
		color | 0xFF000000;
		return ;
	}
	off = fdf->line_thickness / 2;
	off_x = -off;
	while (off_x <= off)
	{
		off_y = -off;
		while (off_y <= off)
		{
			if ((off_x * off_x + off_y * off_y <= off * off) && x + off_x >= 0 \
			&& y + off_y >= 0 && x + off_x < WIDTH && y + off_y < HEIGHT)
				fdf->sdl.argb_pixels[((y + off_y) * fdf->sdl.pixels_per_row) \
				+ x + off_x] = color | 0xFF000000;
			off_y++;
		}
		off_x++;
	}
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
		if (fdf->render_half_points)
			fdf->skip_point = !fdf->skip_point;
		if ((!fdf->render_half_points || !fdf->skip_point) && line.x0 >= 0 && \
		line.y0 >= 0 && line.x0 < WIDTH && line.y0 < HEIGHT)
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
