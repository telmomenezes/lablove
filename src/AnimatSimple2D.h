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

#if !defined(__INCLUDE_ANIMAT_SIMPLE_2D_H)
#define __INCLUDE_ANIMAT_SIMPLE_2D_H

#include "ObjectSimple2D.h"
#include "Gridbrain.h"
#include "MoleculeTable.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class AnimatSimple2D : public ObjectSimple2D
{
public:
	AnimatSimple2D();
	AnimatSimple2D(AnimatSimple2D* anim, bool full=true);
	virtual ~AnimatSimple2D();
	virtual SimulationObject* clone(bool full=true);
	virtual void init();
	void init_test();

	void set_alpha_objects_grid(Grid* grid);
	void set_beta_grid(Grid* grid);

	virtual void set_rot(float rot);
	virtual void perception_step();
	virtual void action_step();
	virtual void on_cycle();
	virtual void computation_step();

	//virtual AnimatSimple2D* crossover(Animat* other_parent);
	virtual void mutate();

	void set_view_range(float range);
	void set_view_angle(float angle);
	float get_view_range(){return _view_range;}
	float get_view_angle(){return _view_angle;}

	void go_front(float distance);
	void rotate(float angle);
	void eat();

	void set_go_cost(float cost){_go_cost = cost;}
	void set_rotate_cost(float cost){_rotate_cost = cost;}

#ifdef __LOVE_GRAPHICS
	virtual void draw();
#endif

	bool _action_go;
	float _action_go_param;
	bool _action_rotate;
	float _action_rotate_param;
	bool _action_eat;
	float _action_eat_param;

	static const char className[];
        static Luna<AnimatSimple2D>::RegType methods[];

        AnimatSimple2D(lua_State* L);
        int set_view_range(lua_State* L);
	int set_view_angle(lua_State* L);
	int set_alpha_objects_grid(lua_State* L);
	int set_beta_grid(lua_State* L);

	int set_go_cost(lua_State* L);
	int set_rotate_cost(lua_State* L);

protected:
	virtual void on_scan_object(SimulationObject* obj, bool visible, bool contact, float angle, float distance);
	void scan_cell(int cell_x, int cell_y);
	virtual void end_cycle();

	Gridbrain* _gridbrain;

	unsigned int _current_input_depth;
	unsigned int _number_of_perceptions;
	unsigned int _max_input_depth;
	unsigned int _initial_connections;

	float _view_range;
	float _view_angle;
	float _half_view_angle;
	float _low_limit_view_angle;
	float _high_limit_view_angle;

	float _go_cost;
	float _rotate_cost;

	SimulationObject* _nearest_food;
	float _distance_to_nearest_food;

	MoleculeTable* _color_molecule_table;
};
#endif

