/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:03:40 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/24 10:47:11 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	line_direction(t_line *line, t_line_aux *line_aux);

// Writes a single pixel directly to the locked texture buffer in ARGB8888
// format. Bounds limits are checked by caller. Transparency set to 0xFF (255).
void	sdl_put_pixel(t_fdf *fdf, int x, int y, int color)
{
	fdf->sdl.argb_pixels[(y * fdf->sdl.pixels_per_row) + x] = \
	color | 0xFF000000;
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

// Captures the current frame buffer to a PNG file with an auto-incremented name
// (screenshot_X.png). Creates a temporary SDL surface directly from locked
// texture pixels for efficient saving, then cleans up resources and triggers
// the white flash screen effect. Surface is created as RGB888 to ignore the
// alpha channel and avoid checkerboard patterns in saved image.
void	take_screenshot(t_fdf *fdf)
{
	char			filename[20];
	SDL_Surface		*surface;

	surface = SDL_CreateRGBSurfaceWithFormatFrom(fdf->sdl.argb_pixels, \
	WIDTH, HEIGHT, 24, fdf->sdl.pitch, SDL_PIXELFORMAT_RGB888);
	if (surface == NULL)
		free_map_and_exit(fdf, ERROR_SDL);
	ft_strlcpy(filename, "screenshot_", sizeof(filename));
	fdf->shot_nbr_str = ft_itoa(fdf->shot_nbr);
	if (fdf->shot_nbr_str)
	{
		ft_strlcat(filename, fdf->shot_nbr_str, sizeof(filename));
		free(fdf->shot_nbr_str);
		ft_strlcat(filename, ".png", sizeof(filename));
		if (IMG_SavePNG(surface, filename) != 0)
			ft_printf("%sScreenshot failed: %s\n", RED, IMG_GetError());
		else
			ft_printf("%s%s saved\n", BLUE, filename);
		fdf->shot_nbr = (fdf->shot_nbr + 1) % NBR_SHOTS;
	}
	else
		ft_printf("%sFailed to generate screenshot filename\n", RED);
	SDL_FreeSurface(surface);
	fdf->take_screenshot = false;
	memset(fdf->sdl.argb_pixels, 200, fdf->sdl.pitch * HEIGHT);
}
