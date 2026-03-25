/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 11:14:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/25 23:10:43 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// Captures the current frame buffer to a PNG file with an auto-incremented name
// (screenshot_X.png). Creates a temporary SDL surface directly from locked
// texture pixels for efficient saving, then cleans up resources and triggers
// the white flash screen effect. Surface is created as RGB888 to ignore the
// alpha channel and avoid checkerboard patterns in saved image.
void	take_screenshot(t_fdf *fdf)
{
	char			shot_nbr_str[8];
	char			filename[20];
	SDL_Surface		*surface;

	surface = SDL_CreateRGBSurfaceWithFormatFrom(fdf->sdl.argb_pixels, \
	WIDTH, HEIGHT, 24, fdf->sdl.pitch, SDL_PIXELFORMAT_RGB888);
	if (surface == NULL)
		free_map_and_exit(fdf, ERROR_SDL);
	ft_strlcpy(filename, "screenshot_", sizeof(filename));
	snprintf(shot_nbr_str, sizeof(shot_nbr_str), "%03d", fdf->shot_nbr);
	ft_strlcat(filename, shot_nbr_str, sizeof(filename));
	ft_strlcat(filename, ".png", sizeof(filename));
	if (IMG_SavePNG(surface, filename) != 0)
		ft_printf("%sScreenshot failed: %s\n", RED, IMG_GetError());
	else
		ft_printf("%s%s saved\n", BLUE, filename);
	fdf->shot_nbr = (fdf->shot_nbr + 1) % NBR_SHOTS;
	SDL_FreeSurface(surface);
	memset(fdf->sdl.argb_pixels, SHOT_COLOR, fdf->sdl.pitch * HEIGHT);
}

void	delay_screenshot_effect(t_fdf *fdf)
{
	fdf->take_screenshot = false;
	usleep(SHOT_DELAY);
}
