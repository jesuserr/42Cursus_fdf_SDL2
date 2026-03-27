/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:37:33 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/27 21:47:38 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	mouse_wheel_scrolled(SDL_MouseWheelEvent wheel, t_fdf *fdf)
{
	if (wheel.y > 0 && fdf->zoom < ZOOM_MAX)
		fdf->zoom *= INC_ZOOM;
	else if (wheel.y < 0 && fdf->zoom > ZOOM_MIN)
		fdf->zoom /= INC_ZOOM;
}

void	mouse_button_pressed(SDL_MouseButtonEvent button, t_fdf *fdf)
{
	if (button.button == SDL_BUTTON_MIDDLE)
		fdf->key.mwb_press = 1;
	else if (button.button == SDL_BUTTON_LEFT)
		fdf->key.mlb_press = 1;
}

void	mouse_button_released(SDL_MouseButtonEvent button, t_fdf *fdf)
{
	if (button.button == SDL_BUTTON_MIDDLE)
		fdf->key.mwb_press = 0;
	else if (button.button == SDL_BUTTON_LEFT)
		fdf->key.mlb_press = 0;
}

// xrel and yrel are the relative motion in the x and y direction and their
// values are -1 or 1 depending on the direction of the mouse movement (0 if no
// movement).
void	mouse_motion(SDL_MouseMotionEvent motion, t_fdf *fdf)
{
	if (motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		fdf->offset_x += motion.xrel;
		fdf->offset_y += motion.yrel;
	}
	else if (motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		fdf->key.mouse_delta_x = motion.xrel;
		fdf->key.mouse_delta_y = motion.yrel;
	}
}
