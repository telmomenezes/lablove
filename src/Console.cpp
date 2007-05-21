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

#include "Console.h"
#include "Love.h"
#include <string.h>

Console::Console(unsigned int width, unsigned int height)
{
	_char_width = width;
	_char_height = height;
	_buffer_size = _char_width * _char_height;
	_char_buffer = (char*)malloc(sizeof(char) * _buffer_size);
	_color_buffer = (unsigned int*)malloc(sizeof(unsigned int) * _buffer_size);

	for (unsigned int i = 0; i < _buffer_size; i++)
	{
		_char_buffer[i] = ' ';
		_color_buffer[i] = 0xFFFFFFFF;
	}

	_width = (_char_width * 8) + 10;
	_height = (_char_height * 8) + 10;

	_current_position = 0;

	_number_buffer = (char*)malloc(sizeof(char) * 255);

#ifdef __LOVE_GRAPHICS
	_font.init();
#endif
}

Console::~Console()
{
	if (_char_buffer != NULL)
	{
		free(_char_buffer);
		_char_buffer = NULL;
	}

	if (_color_buffer != NULL)
	{
		free(_color_buffer);
		_color_buffer = NULL;
	}

	if (_number_buffer != NULL)
	{
		free(_number_buffer);
		_number_buffer = NULL;
	}
}

void Console::draw()
{
#ifdef __LOVE_GRAPHICS
	unsigned int start_x = 0;
	unsigned int start_y = 0;
	unsigned int end_x = _width;
	unsigned int end_y = _height;

	glColor4f(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(start_x, start_y);
	glVertex2f(end_x, start_y);
	glVertex2f(end_x, end_y);
	glVertex2f(start_x, end_y);
	glEnd();

	for (unsigned int y = 0; y < _char_height; y++)
	{
		for (unsigned int x = 0; x < _char_width; x++)
		{
			unsigned int buffer_pos = (y * _char_width) + x;
			if (_char_buffer[buffer_pos] != ' ')
			{
				glColor3f(1.0, 1.0, 1.0);
				//_color_buffer[buffer_pos]
				_font.print((x * 8) + start_x + 5,
						(y * 8) + start_y + 5,
						"%c",
						_char_buffer[buffer_pos]);
			}
		}
	}
#endif
}

void Console::write(char* text, unsigned int color)
{
	int length = strlen(text);
	int space_left = _buffer_size - _current_position;
	int space_to_free = length - space_left;
	int space_pad_count = 0;

	if (space_to_free > 0)
	{
		space_pad_count = _char_width - (space_to_free % _char_width);
		space_to_free += space_pad_count;

		int size_to_move = _current_position - space_to_free;

		_current_position -= space_to_free;

		memmove(_char_buffer, &_char_buffer[space_to_free], size_to_move * sizeof(char));
		memmove(_color_buffer, &_color_buffer[space_to_free], size_to_move * sizeof(unsigned int));
	}

	strncpy(&_char_buffer[_current_position], text, length * sizeof(char));

	unsigned int end_position = _current_position + length;
	for (unsigned int i = _current_position; i < end_position; i++)
	{
		_color_buffer[i] = color;
	}

	int start_padding = _buffer_size - space_pad_count;
	for (unsigned int i = start_padding; i < _buffer_size; i++)
	{
		_char_buffer[i] = ' ';
		_color_buffer[i] = color;
	}

	_current_position += length;
}

void Console::write_ln(char* text, unsigned int color)
{
	write(text, color);
	new_line();
}

void Console::write_int(int value, unsigned int color)
{
	sprintf(_number_buffer, "%d", value);
	write(_number_buffer, color);
}

void Console::write_float(float value, unsigned int color)
{
	sprintf(_number_buffer, "%.4f", value);
	write(_number_buffer, color);
}

void Console::write_double(double value, unsigned int color)
{
	sprintf(_number_buffer, "%.4f", value);
	write(_number_buffer, color);
}

void Console::new_line()
{
	int chars_to_next_line= _char_width - (_current_position % _char_width);
	int chars_to_write = chars_to_next_line + _char_width;
	
	int space_left = _buffer_size - _current_position;
	int space_to_free = chars_to_write - space_left;

	if (space_to_free > 0)
	{
		int size_to_move = _current_position - space_to_free;
		_current_position -= space_to_free;
		
		memmove(_char_buffer, &_char_buffer[space_to_free], size_to_move * sizeof(char));
		memmove(_color_buffer, &_color_buffer[space_to_free], size_to_move * sizeof(unsigned int));
	}

	unsigned int end_clear = _current_position + chars_to_write;
	for (unsigned int i = _current_position; i < end_clear; i++)
	{
		_char_buffer[i] = ' ';
		_color_buffer[i] = COLOR_CONSOLE_TEXT;
	}

	_current_position += chars_to_next_line;
}

