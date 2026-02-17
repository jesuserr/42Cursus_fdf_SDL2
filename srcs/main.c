/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:40:52 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/17 16:36:22 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_map(char *file, t_fdf *fdf)
{
	fdf->raw_map = read_map(file, fdf);
	fdf->z_max = INT_MIN;
	fdf->z_min = INT_MAX;
	fdf->zoom = INIT_ZOOM;
	fdf->offset_x = INIT_OFFSET_X;
	fdf->offset_y = INIT_OFFSET_Y;
	fdf->user_scale_z = INC_ZOOM_Z;
	fdf->render_color_gradient = true;
	fdf->frame_time = 1000 / FPS_LIMIT;
	fdf->smooth_frame_time = 1000 / FPS_LIMIT;
	ft_printf ("%sOK!\nAnalyzing Map... ", BLUE);
	verify_and_parse_map(fdf);
	ft_printf ("%sOK!\n", BLUE);
}

void	init_win(t_fdf *fdf)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		free_map_and_exit(fdf, ERROR_SDL);
	fdf->sdl.window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, \
	SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
	if (fdf->sdl.window == NULL)
		free_map_and_exit(fdf, ERROR_SDL);
	fdf->sdl.renderer = SDL_CreateRenderer(fdf->sdl.window, -1, \
	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (fdf->sdl.renderer == NULL)
		free_map_and_exit(fdf, ERROR_SDL);
}

void	iso_view(t_fdf *fdf)
{
	fdf->angle_x = 45;
	fdf->angle_y = 35;
	fdf->angle_z = 30;
	rotate(fdf);
	projection(fdf);
}

// Main loop function that processes all input events and renders a frame.
// Polls SDL events (keyboard, mouse wheel, mouse buttons) and dispatches them
// to appropriate handlers. Then applies transformations (rotation/translation)
// to the 3D model, clears the screen, projects the wireframe, and presents
// the final rendered frame with FPS limiting.
void	process_input_events_and_render(t_fdf *fdf)
{
	SDL_Event	event;

	fdf->frame_start = SDL_GetTicks();
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			key_pressed(event.key.keysym.sym, fdf);
		else if (event.type == SDL_KEYUP)
			key_released(event.key.keysym.sym, fdf);
		else if (event.type == SDL_MOUSEWHEEL)
			mouse_wheel_scrolled(event.wheel, fdf);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			mouse_button_pressed(event.button, fdf);
		else if (event.type == SDL_MOUSEBUTTONUP)
			mouse_button_released(event.button, fdf);
	}
	apply_input_events(fdf);
	projection(fdf);
	fdf->frame_time = SDL_GetTicks() - fdf->frame_start;
	while (fdf->frame_time < (1000 / FPS_LIMIT))
	{
		SDL_Delay(1);
		fdf->frame_time = SDL_GetTicks() - fdf->frame_start;
	}
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_fdf	fdf;

	ft_bzero(&fdf, sizeof(t_fdf));
	if (argc != 2)
		ft_error_handler(ERROR_ARGS);
	init_map(argv[1], &fdf);
	init_win(&fdf);
	iso_view(&fdf);
	while (1)
		process_input_events_and_render(&fdf);
}
