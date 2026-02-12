/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:34:08 by jesuserr          #+#    #+#             */
/*   Updated: 2026/02/13 00:12:18 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define BLUE       "\033[0;94m"
# define RED        "\033[0;31m"

# define ERROR_ARGS			0
# define ERROR_FILE     	1
# define ERROR_MEM			2
# define ERROR_EMPTY		3
# define ERROR_MAP			4
# define ERROR_SDL			5

# define WIDTH			1200
# define HEIGHT			900
# define ALLOWED_CHR	"-0123456789 ,xABCDEFabcdef"
# define DEF_COLOR		0xf26e04
# define HEX_BLUE		0x0000FF
# define HEX_CYAN		0x00FFFF
# define HEX_GREEN		0x00FF00
# define HEX_YELLOW		0xFFFF00
# define HEX_RED		0xFF0000
# define PI				3.141592654
# define INIT_SCALE		0.75
# define ROT_ANGLE		2
# define THREE_ROT_ANG	0.5
# define INIT_ZOOM		1
# define INC_ZOOM		1.1
# define ZOOM_MAX		40
# define ZOOM_MIN		0.1
# define INIT_OFFSET_X	0
# define INIT_OFFSET_Y	0
# define INC_OFFSET		5
# define INC_ZOOM_Z		1.1
# define MAX_Z_SCALES	50

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "libft/includes/get_next_line.h"
# include "libft/includes/ft_printf.h"
# include "libft/includes/libft.h"
# include <fcntl.h>						// for open
# include <math.h>
# ifdef __APPLE__
#  include <SDL.h>						// for SDL library (macOS)
#  include <SDL2_gfxPrimitives.h>		// for SDL2_gfx graph primitives (macOS)
# else
#  include <SDL2/SDL.h>					// for SDL library (Linux)
#  include <SDL2/SDL2_gfxPrimitives.h>	// for SDL2_gfx graph primitives (Linux)
# endif
# include <stdbool.h>					// for bool type

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_point
{
	float	x;
	float	y;
	float	z;
	int		color;
	int		color_gradient;
}	t_point;

typedef struct s_line
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		color0;
	int		color1;	
}	t_line;

typedef struct s_line_aux
{
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		error;	
}	t_line_aux;

typedef struct s_keys
{
	int		q_press;
	int		w_press;
	int		a_press;
	int		s_press;
	int		z_press;
	int		x_press;
	int		one_press;
	int		two_press;
	int		left_press;
	int		right_press;
	int		down_press;
	int		up_press;
	int		i_press;
	int		o_press;
	int		p_press;
	int		bar_press;
	int		mwb_press;
}	t_keys;

typedef struct s_sdl_window
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
}	t_sdl_window;

typedef struct s_fdf
{
	char			*raw_map;	
	int				x_elem;
	int				y_elem;
	int				z_max;
	int				z_min;
	float			scale_x;
	float			scale_y;
	float			scale_z;
	float			scale;
	float			angle_x;
	float			angle_y;
	float			angle_z;
	float			zoom;
	int				offset_x;
	int				offset_y;
	float			user_scale_z;
	int				num_scales_z;
	bool			render_only_points;
	bool			render_color_gradient;
	t_keys			key;
	t_point			*map;
	t_sdl_window	sdl;				// SDL window and renderer
}	t_fdf;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************** errors.c **********************************/
void	ft_error_handler(int error);
void	free_split(char **str);
void	free_and_exit(int error, char *ptr);
void	free_split_and_exit(char **str, int error, char *ptr);
void	free_map_and_exit(t_fdf *fdf, int error);

/********************************* graphics.c *********************************/
void	sdl_put_pixel(t_fdf *fdf, int x, int y, int color);
void	draw_line(t_line line, t_fdf *fdf);

/********************************** hooks.c ***********************************/
int		key_pressed(int keycode, t_fdf *fdf);
int		key_released(int keycode, t_fdf *fdf);
int		close_window(t_fdf *fdf);

/********************************* map_utils.c ********************************/
char	*read_map(char *file, t_fdf *fdf);
void	check_map(t_fdf *fdf);

/********************************** moves.c ***********************************/
void	apply_key_events(t_fdf *fdf);

/******************************** projections.c *******************************/
void	projection(t_fdf *fdf);

/********************************* rotations.c ********************************/
void	rotate(t_fdf *fdf);
void	unrotate(t_fdf *fdf);

/********************************** z-utils.c *********************************/
void	z_centering(t_fdf *fdf);
void	modify_height(t_fdf *fdf);
void	recover_height(t_fdf *fdf);

#endif
