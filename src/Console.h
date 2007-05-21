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

#if !defined(__INCLUDE_CONSOLE_H)
#define __INCLUDE_CONSOLE_H

#ifdef __LOVE_GRAPHICS
#include "FontVeraMono8.h"
#endif

// Temp hack to keep this working
#define COLOR_CONSOLE_TEXT 0

class Console
{
public:
	Console(unsigned int width, unsigned int height);
	virtual ~Console();

	virtual void draw();
	void write(char* text, unsigned int color=COLOR_CONSOLE_TEXT);
	void write_ln(char* text, unsigned int color=COLOR_CONSOLE_TEXT);
	void write_int(int value, unsigned int color=COLOR_CONSOLE_TEXT);
	void write_float(float value, unsigned int color=COLOR_CONSOLE_TEXT);
	void write_double(double value, unsigned int color=COLOR_CONSOLE_TEXT);
	void new_line();

private:
	unsigned int _char_width;
	unsigned int _char_height;
	unsigned int _buffer_size;
	unsigned int _current_position;
	char* _char_buffer;
	unsigned int* _color_buffer;
	unsigned int _width;
	unsigned int _height;
	char* _number_buffer;
#ifdef __LOVE_GRAPHICS
	FontVeraMono8 _font;
#endif
};
#endif

