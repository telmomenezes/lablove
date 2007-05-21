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

#if !defined(__INCLUDE_KEYBOARD_MOUSE_HANDLER_H)
#define __INCLUDE_KEYBOARD_MOUSE_HANDLER_H

class KeyboardMouseHandler
{
public:
	virtual bool on_key_down(int keycode){return false;}
	virtual bool on_key_up(int keycode){return false;}
	virtual bool on_mouse_button_down(int button, int x, int y){return false;}
	virtual bool on_mouse_button_up(int button, int x, int y){return false;}
	virtual bool on_mouse_move(int x, int y){return false;}
};
#endif

