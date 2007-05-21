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

#if !defined(__INCLUDE_OBJECT_SIMPLE_2D_H)
#define __INCLUDE_OBJECT_SIMPLE_2D_H

#include "Object.h"
#include "MoleculeRGB.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class ObjectSimple2D : public Object
{
public:
	ObjectSimple2D();
	ObjectSimple2D(ObjectSimple2D* obj);
	virtual ~ObjectSimple2D();
	
	virtual void on_cycle();

	virtual void set_pos(float x, float y);
	virtual void set_size(float size);
	virtual void set_rot(float rot);
	float get_x(){return _x;}
	float get_y(){return _y;}
	float get_size(){return _size;}
	float get_rot(){return _rot;}
	//virtual void after_draw();
	int get_cell_x(){return _cell_x;}
	int get_cell_y(){return _cell_y;}
	int get_cell_pos(){return _cell_pos;}

	virtual void place_random();

	void set_age_range(unsigned long low_age_limit, unsigned long high_age_limit);
	void set_metabolism(float metabolism){_metabolism = metabolism;}

	void set_color(MoleculeRGB* color){_color = MoleculeRGB(color);}
	MoleculeRGB* get_color(){return &_color;}

        int set_size(lua_State* L);
	int set_age_range(lua_State* L);
	int set_metabolism(lua_State* L);
	int set_color(lua_State* L);

	float _x;
	float _y;
	float _size;
	float _size_squared;
	float _rot;

	ObjectSimple2D* _next_cell_list;
	ObjectSimple2D* _prev_cell_list;

	int _cell_x;
	int _cell_y;
	int _cell_pos;

protected:
	unsigned long _low_age_limit;
	unsigned long _high_age_limit;
	unsigned long _max_age;
	float _metabolism;
	MoleculeRGB _color;
};
#endif

