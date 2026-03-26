/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 11:14:43 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/26 18:12:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	show_fps(t_fdf *fdf);
static void	show_angles(t_fdf *fdf);
static void	show_help(t_fdf *fdf);
static void	show_help_aux(SDL_Renderer *rend, int txt_x, int txt_y);

// Renders all HUD overlay elements on top of the 3D scene. Sets blend mode
// to BLEND at the start for semi-transparent backgrounds, displays the [F1]
// help hint, and conditionally renders FPS counter, angle display, and full
// help overlay based on user toggles. Resets blend mode to NONE when done.
void	render_hud(t_fdf *fdf)
{
	SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, FPS_TRANSP);
	SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, HEIGHT - 10, 30, 10});
	stringColor(fdf->sdl.renderer, 0, HEIGHT - 10, "[F1]", RGBA_WHITE);
	if (fdf->show_fps)
		show_fps(fdf);
	if (fdf->show_angles)
		show_angles(fdf);
	if (fdf->show_help && WIDTH > HELP_BOX_W * 1.5 && HEIGHT > HELP_BOX_H * 1.5)
		show_help(fdf);
	SDL_SetRenderDrawBlendMode(fdf->sdl.renderer, SDL_BLENDMODE_NONE);
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
		SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, FPS_TRANSP);
		SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, 0, 60, 10});
		stringColor(fdf->sdl.renderer, 0, 0, "FPS:", RGBA_WHITE);
		stringColor(fdf->sdl.renderer, 35, 0, fps, RGBA_WHITE);
	}
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
	SDL_SetRenderDrawColor(fdf->sdl.renderer, 0, 0, 0, FPS_TRANSP);
	SDL_RenderFillRect(fdf->sdl.renderer, &(SDL_Rect){0, 10, 60, 30});
	stringColor(fdf->sdl.renderer, 0, 10, "  x:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 10, angle_x, RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 0, 20, "  y:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 20, angle_y, RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 0, 30, "  z:", RGBA_WHITE);
	stringColor(fdf->sdl.renderer, 35, 30, angle_z, RGBA_WHITE);
}

// Displays a centered help overlay box with all keyboard control mappings.
// Draws a white border frame around a black background box for visibility,
// then renders control descriptions in a formatted list. Triggered by F1 key
// and split into two functions (show_help + show_help_aux) for Norminette.
void	show_help(t_fdf *fdf)
{
	int				box_x;
	int				box_y;
	int				txt_x;
	int				txt_y;
	SDL_Renderer	*rend;

	rend = fdf->sdl.renderer;
	box_x = WIDTH / 2 - HELP_BOX_W / 2;
	box_y = HEIGHT / 2 - HELP_BOX_H / 2;
	txt_x = box_x + 10;
	txt_y = box_y + 5;
	SDL_SetRenderDrawColor(rend, 255, 255, 255, OPACITY_MAX);
	SDL_RenderFillRect(rend, &(SDL_Rect){box_x - 5, box_y - 5, \
	HELP_BOX_W + 10, HELP_BOX_H + 10});
	SDL_SetRenderDrawColor(rend, 0, 0, 0, OPACITY_MAX);
	SDL_RenderFillRect(rend, &(SDL_Rect){box_x, box_y, HELP_BOX_W, HELP_BOX_H});
	stringColor(rend, txt_x, txt_y += 5, "Rotate X:       Q/W", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Rotate Y:       A/S", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Rotate Z:       Z/X", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Move:           Arrows", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Zoom:           E/D", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Scale Z:        1/2", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Reverse Z:      V", RGBA_WHITE);
	show_help_aux(rend, txt_x, txt_y);
}

void	show_help_aux(SDL_Renderer *rend, int txt_x, int txt_y)
{
	stringColor(rend, txt_x, txt_y += 10, "Reset View:     C", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Alt. Views:     I/O/P", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Screenshot:     F12", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Wireframe:      R", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Color Grad.:    G", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "FPS Display:    F", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Angles Display: T", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Animation:      Space", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Thickness:      3/4", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 10, "Exit:           ESC", RGBA_WHITE);
	stringColor(rend, txt_x, txt_y += 20, "------ CONTROLS ------", RGBA_WHITE);
}
