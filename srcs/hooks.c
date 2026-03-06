/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:54:26 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/06 16:40:55 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	key_pressed_aux(int keycode, t_fdf *fdf);
static void	key_released_aux(int keycode, t_fdf *fdf);

void	key_pressed(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_ESCAPE)
		fdf->running = false;
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
	key_pressed_aux(keycode, fdf);
}

static void	key_pressed_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_o)
		fdf->key.o_press = 1;
	else if (keycode == SDLK_p)
		fdf->key.p_press = 1;
	else if (keycode == SDLK_SPACE)
		fdf->key.bar_press = !fdf->key.bar_press;
	else if (keycode == SDLK_1)
		fdf->key.one_press = 1;
	else if (keycode == SDLK_2)
		fdf->key.two_press = 1;
	else if (keycode == SDLK_e && fdf->zoom < ZOOM_MAX)
		fdf->zoom *= INC_ZOOM;
	else if (keycode == SDLK_d && fdf->zoom > ZOOM_MIN)
		fdf->zoom /= INC_ZOOM;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = 1;
	else if (keycode == SDLK_r)
		fdf->render_only_points = !fdf->render_only_points;
	else if (keycode == SDLK_g)
		fdf->render_color_gradient = !fdf->render_color_gradient;
	else if (keycode == SDLK_f)
		fdf->show_fps = !fdf->show_fps;
	else if (keycode == SDLK_v && !fdf->reverse_z)
		fdf->reverse_z = true;
}

void	key_released(int keycode, t_fdf *fdf)
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
	key_released_aux(keycode, fdf);
}

static void	key_released_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_p)
		fdf->key.p_press = 0;
	else if (keycode == SDLK_1)
		fdf->key.one_press = 0;
	else if (keycode == SDLK_2)
		fdf->key.two_press = 0;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = 0;
	else if (keycode == SDLK_v)
		fdf->reverse_z = false;
}
