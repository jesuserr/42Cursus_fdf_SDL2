/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:37:33 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/14 13:56:20 by jesuserr         ###   ########.fr       */
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
}

void	mouse_button_released(SDL_MouseButtonEvent button, t_fdf *fdf)
	{
	if (button.button == SDL_BUTTON_MIDDLE)
		fdf->key.mwb_press = 0;
}
