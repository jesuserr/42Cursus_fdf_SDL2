/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:25:20 by jesuserr          #+#    #+#             */
/*   Updated: 2026/03/28 14:16:51 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	get_height_color(float normalized);

// Calculates z-axis scaling based on the map's height using linear regression
// equation derived from empirical testing to ensure proper visualization.
// For points without explicit colors, it assigns gradient colors based on their
// normalized height (0.0-1.0). Finally, applies the calculated scale_z to all
// Z coordinates to center the map vertically in screen.
void	z_centering(t_fdf *fdf)
{
	int		i;	
	int		map_height;
	float	norm_value;

	i = 0;
	map_height = fdf->z_max - fdf->z_min;
	fdf->scale_z = (-0.0004 * map_height) + 0.1044;
	if (fdf->scale_z < 0.01)
		fdf->scale_z = 0.01;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		if (fdf->map[i].color == DEF_COLOR)
		{
			if (fdf->z_max != fdf->z_min)
				norm_value = (fdf->map[i].z - fdf->z_min) / map_height;
			else
				norm_value = 0.5;
			fdf->map[i].color_gradient = get_height_color(norm_value);
		}
		fdf->map[i].z *= fdf->scale_z;
		i++;
	}
}

// Returns a color based on normalized height (0.0 to 1.0) using a gradient of
// 10 colors: Abyss->Ocean->Sea->Coast->Lowland->Plains->Meadow->Hills->
// Mountain->Snow. Determines which segment the height falls into (from 0 to 8),
// calculates the offset linear interpolation to blend the RGB components of the
// two adjacent colors, creating smooth color transitions across the height
// range.
static int	get_height_color(float normalized)
{
	int		colors[10];
	int		rgb[3];
	int		segment;
	float	offset;

	ft_memcpy(colors, (int [10]){HEX_ABYSS, HEX_OCEAN, HEX_SEA, HEX_COAST, \
	HEX_LOWLAND, HEX_PLAINS, HEX_MEADOW, HEX_HILLS, HEX_MOUNTAIN, HEX_PEAKS}, \
	sizeof(int) * 10);
	segment = (int)(normalized * 9);
	if (segment >= 9)
		return (colors[9]);
	offset = (normalized * 9) - segment;
	rgb[0] = ((colors[segment] >> 16) & 0xFF) * (1 - offset) + \
	((colors[segment + 1] >> 16) & 0xFF) * offset;
	rgb[1] = ((colors[segment] >> 8) & 0xFF) * (1 - offset) + \
	((colors[segment + 1] >> 8) & 0xFF) * offset;
	rgb[2] = (colors[segment] & 0xFF) * (1 - offset) + \
	((colors[segment + 1] & 0xFF) * offset);
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

/* Modifies the height of the object by the scale given in user_scale_z */
/* Keeps track of the number of height modifications in order to be */
/* able to recover the original view when the ISO or Parallel views */
/* are recalled (num_scales_z) */
void	modify_height(t_fdf *fdf)
{
	int	i;

	i = 0;
	if (fdf->key.one_press && fdf->num_scales_z < MAX_Z_SCALES)
	{
		while (i < (fdf->x_elem * fdf->y_elem))
			fdf->map[i++].z *= fdf->user_scale_z;
		fdf->num_scales_z++;
	}
	else if (fdf->key.two_press && fdf->num_scales_z > -MAX_Z_SCALES)
	{
		while (i < (fdf->x_elem * fdf->y_elem))
			fdf->map[i++].z /= fdf->user_scale_z;
		fdf->num_scales_z--;
	}
}

/* Uses the variable num_scales_z to recover the original height of the */
/* object. Used by ISO an Parallel calls. */
void	recover_height(t_fdf *fdf)
{
	int	i;

	while (fdf->num_scales_z)
	{
		i = 0;
		if (fdf->num_scales_z > 0)
		{
			while (i < (fdf->x_elem * fdf->y_elem))
				fdf->map[i++].z /= fdf->user_scale_z;
			fdf->num_scales_z--;
		}
		if (fdf->num_scales_z < 0)
		{
			while (i < (fdf->x_elem * fdf->y_elem))
				fdf->map[i++].z *= fdf->user_scale_z;
			fdf->num_scales_z++;
		}
	}
}

void	reverse_height(t_fdf *fdf)
{
	int64_t	i;

	i = 0;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		fdf->map[i].z = -fdf->map[i].z;
		i++;
	}
	fdf->reverse_z = false;
	fdf->z_is_reversed = !fdf->z_is_reversed;
}
