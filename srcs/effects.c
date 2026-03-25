/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 11:14:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/25 19:37:48 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	show_fps(t_fdf *fdf);
static void	show_angles(t_fdf *fdf);

void	render_hud(t_fdf *fdf)
{
	if (fdf->show_fps)
		show_fps(fdf);
	if (fdf->show_angles)
		show_angles(fdf);
}

// Displays the current frames per second (FPS) in the top-left corner.
// Uses exponential moving average (EMA) to smooth frame time fluctuations
// and prevent jittery display. Draws a black background rectangle before
// rendering the FPS text for better visibility.
// EMA_ALPHA controls smoothing strength, 0.1 = very smooth, slow to respond to
// changes // 0.5 = balanced // 0.9 = minimal smoothing, fast response.
static void	show_fps(t_fdf *fdf)
{
	char		fps[8];

	if (fdf->frame_time > 0 && fdf->smooth_frame_time > 0)
	{
		fdf->smooth_frame_time = (EMA_ALPHA * fdf->frame_time) + \
		((1 - EMA_ALPHA) * fdf->smooth_frame_time);
		snprintf(fps, sizeof(fps), "%3d", (int)(1000 / fdf->smooth_frame_time));
		SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, FPS_TRANSP);
		SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, 0, 60, 10});
		stringColor(fdf->sdl.renderer, 0, 0, "FPS:", RGBA_WHITE);
		stringColor(fdf->sdl.renderer, 35, 0, fps, RGBA_WHITE);
		SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_NONE);
	}
}

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

// Similar to show_fps, this function displays the current rotation angles
// (angle_x, angle_y, angle_z) in the top-left corner. Draws a semi-transparent
// black background rectangle for better visibility, then renders each angle
// with labels.
static void	show_angles(t_fdf *fdf)
{
	char		angle_x[8];
	char		angle_y[8];
	char		angle_z[8];

	snprintf(angle_x, sizeof(angle_x), "%3d", (int)fdf->angle_x);
	snprintf(angle_y, sizeof(angle_y), "%3d", (int)fdf->angle_y);
	snprintf(angle_z, sizeof(angle_z), "%3d", (int)fdf->angle_z);
	SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, FPS_TRANSP);
	SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, 10, 60, 30});
	stringColor(fdf->sdl.renderer, 0, 10, "  x:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 10, angle_x, RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 0, 20, "  y:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 20, angle_y, RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 0, 30, "  z:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 30, angle_z, RGBA_WHITE);
	SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_NONE);
}
