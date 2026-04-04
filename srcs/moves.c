/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 11:47:50 by jesuserr          #+#    #+#             */
/*   Updated: 2026/04/04 17:05:06 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	key_action_1(t_fdf *fdf);
static void	key_action_2(t_fdf *fdf);
static void	key_action_3(t_fdf *fdf);
static void	normalize_angles(t_fdf *fdf);

void	apply_input_events(t_fdf *fdf)
{
	unrotate(fdf);
	if (fdf->bending_factor != 0)
		bending_effect(fdf, UNDO_EFFECT);
	key_action_1(fdf);
	key_action_2(fdf);
	key_action_3(fdf);
	if (fdf->key.mlb_press)
	{
		fdf->angle_x -= fdf->mouse_delta_y;
		fdf->angle_y -= fdf->mouse_delta_x;
		fdf->mouse_delta_x = 0;
		fdf->mouse_delta_y = 0;
	}
	normalize_angles(fdf);
	if (fdf->bending_factor != 0)
		bending_effect(fdf, APPLY_EFFECT);
	rotate(fdf);
	projection(fdf);
}

/* Rotations and arrow keys movements */
static void	key_action_1(t_fdf *fdf)
{
	if (fdf->key.w_press)
		fdf->angle_x += ROT_ANGLE;
	if (fdf->key.q_press)
		fdf->angle_x -= ROT_ANGLE;
	if (fdf->key.s_press)
		fdf->angle_y += ROT_ANGLE;
	if (fdf->key.a_press)
		fdf->angle_y -= ROT_ANGLE;
	if (fdf->key.z_press)
		fdf->angle_z -= ROT_ANGLE;
	if (fdf->key.x_press)
		fdf->angle_z += ROT_ANGLE;
	if (fdf->key.left_press)
		fdf->offset_x += INC_OFFSET;
	if (fdf->key.right_press)
		fdf->offset_x -= INC_OFFSET;
	if (fdf->key.down_press)
		fdf->offset_y -= INC_OFFSET;
	if (fdf->key.up_press)
		fdf->offset_y += INC_OFFSET;
	if (fdf->key.b_press)
		fdf->bending_factor += BENDING_FACTOR;
	if (fdf->key.n_press)
		fdf->bending_factor -= BENDING_FACTOR;
}

/* Deals with the three available views */
static void	key_action_2(t_fdf *fdf)
{
	if (fdf->key.m_press)
		fdf->bending_factor = 0;
	if (fdf->key.i_press || fdf->key.o_press || fdf->key.p_press)
	{
		fdf->zoom = INIT_ZOOM;
		fdf->offset_x = 0;
		fdf->offset_y = 0;
		fdf->angle_z = 0;
	}
	if (fdf->key.i_press)
	{
		fdf->angle_x = 0;
		fdf->angle_y = 0;
	}
	else if (fdf->key.o_press)
	{
		fdf->angle_x = 90;
		fdf->angle_y = 0;
	}
	else if (fdf->key.p_press)
	{
		fdf->angle_x = 90;
		fdf->angle_y = 90;
	}
}

/* Three-axxis rotation at the same time */
static void	key_action_3(t_fdf *fdf)
{
	if (fdf->key.bar_press)
	{
		fdf->angle_x -= THREE_ROT_ANG;
		fdf->angle_y += THREE_ROT_ANG;
		fdf->angle_z -= THREE_ROT_ANG;
	}
	if (fdf->key.one_press || fdf->key.two_press)
		modify_height(fdf);
	if (fdf->key.mwb_press)
	{
		fdf->zoom = INIT_ZOOM;
		fdf->offset_x = 0;
		fdf->offset_y = 0;
		fdf->angle_x = 45;
		fdf->angle_y = 35;
		fdf->angle_z = 30;
		fdf->bending_factor = 0;
		recover_height(fdf);
		if (fdf->z_is_reversed)
			reverse_height(fdf);
		fdf->line_thickness = DEF_LINE_THK;
	}
	if (fdf->reverse_z)
		reverse_height(fdf);
}

// In case angles are printed it will show always values between 0-359 degrees,
// instead of negative or bigger than 360 values
/* Keeps angles always between 0-359 degrees */
static void	normalize_angles(t_fdf *fdf)
{
	if (fdf->angle_x >= 360)
		fdf->angle_x = fdf->angle_x - 360;
	else if (fdf->angle_x <= -1)
		fdf->angle_x = fdf->angle_x + 360;
	if (fdf->angle_y >= 360)
		fdf->angle_y = fdf->angle_y - 360;
	else if (fdf->angle_y <= -1)
		fdf->angle_y = fdf->angle_y + 360;
	if (fdf->angle_z >= 360)
		fdf->angle_z = fdf->angle_z - 360;
	else if (fdf->angle_z <= -1)
		fdf->angle_z = fdf->angle_z + 360;
}
