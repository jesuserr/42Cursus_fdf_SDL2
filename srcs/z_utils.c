/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:25:20 by jesuserr          #+#    #+#             */
/*   Updated: 2023/12/08 14:27:48 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* Applied scale_z depends on z_max to z_min range */
/* Line equation used for that purpose */
/* For 42.fdf applied scale is 0.1 // Minimum 0.05 */
/* Alt: fdf->scale_z = (-0.00072 * (fdf->z_max - fdf->z_min)) + 0.10792; */
/* (inside while) fdf->map[i].z -= ((fdf->z_max + fdf->z_min) / 2.0); */
void	z_centering(t_fdf *fdf)
{
	int	i;

	i = 0;
	fdf->scale_z = (-0.0004 * (fdf->z_max - fdf->z_min)) + 0.1044;
	if (fdf->scale_z < 0.05)
		fdf->scale_z = 0.05;
	while (i < (fdf->x_elem * fdf->y_elem))
	{
		fdf->map[i].z *= fdf->scale_z;
		i++;
	}
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
