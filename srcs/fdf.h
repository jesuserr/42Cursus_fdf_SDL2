/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:34:08 by jesuserr          #+#    #+#             */
/*   Updated: 2026/04/08 23:32:26 by jesuserr         ###   ########.fr       */
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

# define WIDTH			1920
# define HEIGHT			1080
# define ALLOWED_CHR	"-0123456789 ,xABCDEFabcdef"
# define DEF_COLOR		0xf26e04
# define HEX_ABYSS		0x08306B			// Deep navy (abyssal depths)
# define HEX_OCEAN		0x2166AC			// Ocean blue (deep water)
# define HEX_SEA		0x4393C3			// Mid-depth blue
# define HEX_COAST		0x92C5DE			// Coastal/shallow water
# define HEX_LOWLAND	0x006837			// Dark forest green (sea level)
# define HEX_PLAINS		0x1A9641			// Forest green (lowlands)
# define HEX_MEADOW		0x78C679			// Light green (plains)
# define HEX_HILLS		0xFECC5C			// Amber yellow (hills)
# define HEX_MOUNTAIN	0xBD4A0E			// Terra cotta (mountains)
# define HEX_PEAKS		0xF5F5F5			// Snow white (peaks)
# define RGBA_WHITE		0xFFFFFFFF			// White color in RGBA format
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
# define FPS_LIMIT		60
# define BACKG_TRANSP	125				// Alpha value for background
# define OPACITY_MAX	255
# define EMA_ALPHA		0.25			// Smoothing factor for FPS display
# define NBR_SHOTS		50				// Max number of screenshots (255 limit)
# define SHOT_DELAY		50000			// Microsecs delay after screenshot
# define SHOT_COLOR		175				// Flash color for screenshot effect
# define HELP_BOX_W		200				// Width of the help box
# define HELP_BOX_H		230				// Height of the help box
# define DEF_LINE_THK	1				// Line default (and minimum) thickness
# define MAX_LINE_THK	7				// Max line thickness
# define BENDING_FACTOR	0.05			// Amplitude of the bending effect
# define BENDING_WAVES	3				// Bending waves across the map
# define UNDO_EFFECT	-1				// Undo the bending effect
# define APPLY_EFFECT	1				// Apply the bending effect

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "libft/includes/ft_printf.h"
# include "libft/includes/libft.h"
# include <fcntl.h>						// for open
# include <math.h>
# ifdef __APPLE__
#  include <SDL.h>						// for SDL library (macOS)
#  include <SDL2_gfxPrimitives.h>		// for SDL2_gfx graph primitives (macOS)
#  include <SDL_image.h>				// for IMG_SavePNG (macOS)
# else
#  include <SDL2/SDL.h>					// for SDL library (Linux)
#  include <SDL2/SDL2_gfxPrimitives.h>	// for SDL2_gfx graph primitives (Linux)
#  include <SDL2/SDL_image.h>			// for IMG_SavePNG (Linux)
# endif
# include <stdbool.h>					// for bool type
# include <sys/stat.h>					// for fstat
# include <sys/mman.h>					// for mmap/munmap
# include <limits.h>					// for INT_MAX/INT_MIN

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
	int		color_backup;
}	t_point;

typedef struct s_line
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		color0;	
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
	bool	q_press;
	bool	w_press;
	bool	a_press;
	bool	s_press;
	bool	z_press;
	bool	x_press;
	bool	one_press;
	bool	two_press;
	bool	left_press;
	bool	right_press;
	bool	down_press;
	bool	up_press;
	bool	i_press;
	bool	o_press;
	bool	p_press;
	bool	bar_press;
	bool	b_press;
	bool	n_press;
	bool	m_press;
	bool	mwb_press;					// Mouse wheel button pressed
	bool	mlb_press;					// Mouse left button pressed
}	t_keys;

typedef struct s_sdl_window
{
	SDL_Window		*window;			// SDL window
	SDL_Renderer	*renderer;			// SDL renderer for drawing
	SDL_Texture		*texture;			// SDL texture for pixel manipulation
	uint32_t		*argb_pixels;		// Pointer for direct pixel access
	int				pitch;				// Length of a row of pixels (in bytes)
	int				pixels_per_row;		// Length of a row of pixels (in pixels)
}	t_sdl_window;

typedef struct s_fdf
{
	char			*raw_map;
	size_t			raw_map_size;
	int				x_elem;
	int				y_elem;
	int				total_points;
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
	bool			render_colors;
	bool			prev_state_render_colors;
	bool			show_fps;
	bool			running;
	bool			reverse_z;
	bool			z_is_reversed;
	bool			take_screenshot;
	bool			img_init_success;
	bool			show_angles;
	bool			show_help;
	bool			render_half_points;
	bool			skip_point;
	Uint32			frame_start;
	Uint32			frame_time;
	Uint32			smooth_frame_time;
	Uint8			shot_nbr;
	Uint8			line_thickness;
	int				mouse_delta_x;
	int				mouse_delta_y;
	float			bending_factor;
	float			bending_freq;
	t_keys			key;
	t_point			*map;
	t_sdl_window	sdl;				// SDL window and renderer
}	t_fdf;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************* effects.c **********************************/
void	bending_effect(t_fdf *fdf, int action);
void	take_screenshot(t_fdf *fdf);
void	delay_screenshot_effect(t_fdf *fdf);
void	swap_render_color(t_fdf *fdf);

/********************************** errors.c **********************************/
void	ft_error_handler(int error);
void	free_split(char **str);
void	free_split_and_exit(char **str, int error, t_fdf *fdf);
void	free_map_and_exit(t_fdf *fdf, int error);
void	release_resources(t_fdf *fdf);

/********************************* graphics.c *********************************/
void	sdl_put_pixel(t_fdf *fdf, int x, int y, int color);
void	draw_line(t_line line, t_fdf *fdf);
bool	is_line_visible(t_line *line);

/********************************** hooks.c ***********************************/
void	key_pressed(int keycode, t_fdf *fdf);
void	key_released(int keycode, t_fdf *fdf);

/*********************************** hud.c ************************************/
void	render_hud(t_fdf *fdf);

/********************************* map_utils.c ********************************/
char	*read_map(char *file, t_fdf *fdf);
void	verify_and_parse_map(t_fdf *fdf);

/********************************** mouse.c ***********************************/
void	mouse_wheel_scrolled(SDL_MouseWheelEvent wheel, t_fdf *fdf);
void	mouse_button_pressed(SDL_MouseButtonEvent button, t_fdf *fdf);
void	mouse_button_released(SDL_MouseButtonEvent button, t_fdf *fdf);
void	mouse_motion(SDL_MouseMotionEvent motion, t_fdf *fdf);

/********************************** moves.c ***********************************/
void	apply_input_events(t_fdf *fdf);

/******************************** projections.c *******************************/
void	projection(t_fdf *fdf);

/********************************* rotations.c ********************************/
void	rotate(t_fdf *fdf);
void	unrotate(t_fdf *fdf);

/********************************** z-utils.c *********************************/
void	z_centering(t_fdf *fdf);
void	modify_height(t_fdf *fdf);
void	recover_height(t_fdf *fdf);
void	reverse_height(t_fdf *fdf);

#endif
