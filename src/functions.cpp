/*
 * LOVE Lab
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "functions.h"
#include "Love.h"
#include <math.h>

void console_write(char* text, unsigned int color)
{
	Love::get_instance().get_console()->write(text, color);
}

void console_write_ln(char* text, unsigned int color)
{
	Love::get_instance().get_console()->write_ln(text, color);
}

void console_write_int(int value, unsigned int color)
{
	Love::get_instance().get_console()->write_int(value, color);
}

void console_write_float(float value, unsigned int color)
{
	Love::get_instance().get_console()->write_float(value, color);
}

void console_write_double(double value, unsigned int color)
{
	Love::get_instance().get_console()->write_double(value, color);
}

void console_new_line()
{
	Love::get_instance().get_console()->new_line();
}

float normalize_angle(float angle)
{
	float PI2 = M_PI * 2.0f;
	while(angle > M_PI)
	{
		angle -= PI2;
	}
	while(angle <= -M_PI)
	{
		angle += PI2;
	}

	return angle;
}

