/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:54:26 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/12 11:42:19 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	key_pressed_aux(int keycode, t_fdf *fdf);
static int	key_released_aux(int keycode, t_fdf *fdf);

int	key_pressed(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_ESCAPE)
		close_window(fdf);
	else if (keycode == SDLK_q)
		fdf->key.q_press = 1;
	else if (keycode == SDLK_w)
		fdf->key.w_press = 1;
	else if (keycode == SDLK_a)
		fdf->key.a_press = 1;
	else if (keycode == SDLK_s)
		fdf->key.s_press = 1;
	else if (keycode == SDLK_z)
		fdf->key.z_press = 1;
	else if (keycode == SDLK_x)
		fdf->key.x_press = 1;
	else if (keycode == SDLK_LEFT)
		fdf->key.left_press = 1;
	else if (keycode == SDLK_RIGHT)
		fdf->key.right_press = 1;
	else if (keycode == SDLK_DOWN)
		fdf->key.down_press = 1;
	else if (keycode == SDLK_UP)
		fdf->key.up_press = 1;
	else if (keycode == SDLK_i)
		fdf->key.i_press = 1;
	return (key_pressed_aux(keycode, fdf));
}

static int	key_pressed_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_o)
		fdf->key.o_press = 1;
	else if (keycode == SDLK_p)
		fdf->key.p_press = 1;
	else if (keycode == SDLK_SPACE)
		fdf->key.bar_press = 1;
	else if (keycode == SDLK_1)
		fdf->key.one_press = 1;
	else if (keycode == SDLK_2)
		fdf->key.two_press = 1;
	else if (keycode == SDLK_e)
		fdf->zoom *= INC_ZOOM;
	else if (keycode == SDLK_d)
		fdf->zoom /= INC_ZOOM;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = 1;
	return (0);
}

int	key_released(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_q)
		fdf->key.q_press = 0;
	else if (keycode == SDLK_w)
		fdf->key.w_press = 0;
	else if (keycode == SDLK_a)
		fdf->key.a_press = 0;
	else if (keycode == SDLK_s)
		fdf->key.s_press = 0;
	else if (keycode == SDLK_z)
		fdf->key.z_press = 0;
	else if (keycode == SDLK_x)
		fdf->key.x_press = 0;
	else if (keycode == SDLK_LEFT)
		fdf->key.left_press = 0;
	else if (keycode == SDLK_RIGHT)
		fdf->key.right_press = 0;
	else if (keycode == SDLK_DOWN)
		fdf->key.down_press = 0;
	else if (keycode == SDLK_UP)
		fdf->key.up_press = 0;
	else if (keycode == SDLK_i)
		fdf->key.i_press = 0;
	else if (keycode == SDLK_o)
		fdf->key.o_press = 0;
	return (key_released_aux(keycode, fdf));
}

static int	key_released_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_p)
		fdf->key.p_press = 0;
	else if (keycode == SDLK_SPACE)
		fdf->key.bar_press = 0;
	else if (keycode == SDLK_1)
		fdf->key.one_press = 0;
	else if (keycode == SDLK_2)
		fdf->key.two_press = 0;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = 0;
	return (0);
}

int	close_window(t_fdf *fdf)
{
	free(fdf->raw_map);
	free(fdf->map);
	if (fdf->sdl.renderer)
		SDL_DestroyRenderer(fdf->sdl.renderer);
	if (fdf->sdl.window)
		SDL_DestroyWindow(fdf->sdl.window);
	SDL_Quit();
	exit (EXIT_SUCCESS);
}
