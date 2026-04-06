/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:54:26 by jesuserr          #+#    #+#             */
/*   Updated: 2026/04/06 12:29:32 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	key_pressed_aux(int keycode, t_fdf *fdf);
static void	key_pressed_aux_2(int keycode, t_fdf *fdf);
static void	key_released_aux(int keycode, t_fdf *fdf);

void	key_pressed(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_ESCAPE)
		fdf->running = false;
	else if (keycode == SDLK_q)
		fdf->key.q_press = true;
	else if (keycode == SDLK_w)
		fdf->key.w_press = true;
	else if (keycode == SDLK_a)
		fdf->key.a_press = true;
	else if (keycode == SDLK_s)
		fdf->key.s_press = true;
	else if (keycode == SDLK_z)
		fdf->key.z_press = true;
	else if (keycode == SDLK_x)
		fdf->key.x_press = true;
	else if (keycode == SDLK_LEFT)
		fdf->key.left_press = true;
	else if (keycode == SDLK_RIGHT)
		fdf->key.right_press = true;
	else if (keycode == SDLK_DOWN)
		fdf->key.down_press = true;
	else if (keycode == SDLK_UP)
		fdf->key.up_press = true;
	else if (keycode == SDLK_i)
		fdf->key.i_press = true;
	key_pressed_aux(keycode, fdf);
}

static void	key_pressed_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_o)
		fdf->key.o_press = true;
	else if (keycode == SDLK_p)
		fdf->key.p_press = true;
	else if (keycode == SDLK_SPACE)
		fdf->key.bar_press = !fdf->key.bar_press;
	else if (keycode == SDLK_1)
		fdf->key.one_press = true;
	else if (keycode == SDLK_2)
		fdf->key.two_press = true;
	else if (keycode == SDLK_e && fdf->zoom < ZOOM_MAX)
		fdf->zoom *= INC_ZOOM;
	else if (keycode == SDLK_d && fdf->zoom > ZOOM_MIN)
		fdf->zoom /= INC_ZOOM;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = true;
	else if (keycode == SDLK_r)
		fdf->render_only_points = !fdf->render_only_points;
	else if (keycode == SDLK_g)
		fdf->render_colors = !fdf->render_colors;
	else if (keycode == SDLK_f)
		fdf->show_fps = !fdf->show_fps;
	else if (keycode == SDLK_v && !fdf->reverse_z)
		fdf->reverse_z = true;
	key_pressed_aux_2(keycode, fdf);
}

static void	key_pressed_aux_2(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_F12)
		fdf->take_screenshot = true;
	else if (keycode == SDLK_t)
		fdf->show_angles = !fdf->show_angles;
	else if (keycode == SDLK_F1)
		fdf->show_help = !fdf->show_help;
	else if (keycode == SDLK_3 && fdf->line_thickness < MAX_LINE_THK)
		fdf->line_thickness += 2;
	else if (keycode == SDLK_4 && fdf->line_thickness > DEF_LINE_THK)
		fdf->line_thickness -= 2;
	else if (keycode == SDLK_b)
		fdf->key.b_press = true;
	else if (keycode == SDLK_n)
		fdf->key.n_press = true;
	else if (keycode == SDLK_m)
		fdf->key.m_press = true;
}

void	key_released(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_q)
		fdf->key.q_press = false;
	else if (keycode == SDLK_w)
		fdf->key.w_press = false;
	else if (keycode == SDLK_a)
		fdf->key.a_press = false;
	else if (keycode == SDLK_s)
		fdf->key.s_press = false;
	else if (keycode == SDLK_z)
		fdf->key.z_press = false;
	else if (keycode == SDLK_x)
		fdf->key.x_press = false;
	else if (keycode == SDLK_LEFT)
		fdf->key.left_press = false;
	else if (keycode == SDLK_RIGHT)
		fdf->key.right_press = false;
	else if (keycode == SDLK_DOWN)
		fdf->key.down_press = false;
	else if (keycode == SDLK_UP)
		fdf->key.up_press = false;
	else if (keycode == SDLK_i)
		fdf->key.i_press = false;
	else if (keycode == SDLK_o)
		fdf->key.o_press = false;
	key_released_aux(keycode, fdf);
}

static void	key_released_aux(int keycode, t_fdf *fdf)
{
	if (keycode == SDLK_p)
		fdf->key.p_press = false;
	else if (keycode == SDLK_1)
		fdf->key.one_press = false;
	else if (keycode == SDLK_2)
		fdf->key.two_press = false;
	else if (keycode == SDLK_c)
		fdf->key.mwb_press = false;
	else if (keycode == SDLK_v)
		fdf->reverse_z = false;
	else if (keycode == SDLK_b)
		fdf->key.b_press = false;
	else if (keycode == SDLK_n)
		fdf->key.n_press = false;
	else if (keycode == SDLK_m)
		fdf->key.m_press = false;
}
