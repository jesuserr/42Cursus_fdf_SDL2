/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:25:20 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/13 00:16:39 by jesuserr         ###   ########.fr       */
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
	if (fdf->scale_z < 0.05)
		fdf->scale_z = 0.05;
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
// 5 colors: Blue->Cyan->Green->Yellow->Red. Determines which segment the height
// falls into (0 to 3), calculates the offset within that segment and then uses
// linear interpolation to blend the RGB components of the two adjacent colors,
// creating smooth color transitions across the height range.
static int	get_height_color(float normalized)
{
	int		colors[5];
	int		rgb[3];
	int		segment;
	float	offset;

	ft_memcpy(colors, (int [5]){HEX_BLUE, HEX_CYAN, HEX_GREEN, HEX_YELLOW, \
	HEX_RED}, sizeof(int) * 5);
	segment = (int)(normalized * 4);
	if (segment >= 4)
		return (colors[4]);
	offset = (normalized * 4) - segment;
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
	if (fdf->key.one_press == 1 && fdf->num_scales_z < MAX_Z_SCALES)
	{
		while (i < (fdf->x_elem * fdf->y_elem))
			fdf->map[i++].z *= fdf->user_scale_z;
		fdf->num_scales_z++;
	}
	else if (fdf->key.two_press == 1 && fdf->num_scales_z > -MAX_Z_SCALES)
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
