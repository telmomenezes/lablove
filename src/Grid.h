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

#ifndef __INCLUDE_GRID_H
#define __INCLUDE_GRID_H

#include "GridbrainComponentSet.h"

#include <vector>
#include <list>
#include <map>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class Grid
{
public:
	static const int ALPHA;
	static const int BETA;

	Grid(int type, unsigned int width, unsigned int height);
	Grid(Grid* grid);
	virtual ~Grid();

	void init();
	void set_type(int type){_type = type;}
	void set_number(unsigned int number){_number = number;}
	void set_width(unsigned int width){_width = width;}
	void set_height(unsigned int height){_height = height;}
	void add_component_set(GridbrainComponentSet* component_set, int end_column=-1);
	void set_offset(unsigned int offset){_offset = offset;}
	int get_type(){return _type;}
	unsigned int get_number(){return _number;}
	unsigned int get_width(){return _width;}
	unsigned int get_height(){return _height;}
	unsigned int get_size(){return _size;}
	unsigned int get_offset(){return _offset;}
	GridbrainComponent* get_random_component(unsigned int pos);
	void set_input(unsigned int number, unsigned int depth, float value);
	unsigned int add_perception(GridbrainComponent* per);
	unsigned int add_action(GridbrainComponent* act);
	void init_input_matrix(unsigned int max_input_depth);
	float* get_input_matrix(){return _input_matrix;}
	void init_output_vector();
	float* get_output_vector(){return _output_vector;}
	unsigned int get_perception_type(unsigned int number);
	unsigned int get_perception_molecule(unsigned int number);
	void set_input_depth(unsigned int input_depth){_input_depth = input_depth;}
	unsigned int get_input_depth(){return _input_depth;}
	unsigned int get_max_input_depth(){return _max_input_depth;}
	unsigned int get_perceptions_count(){return _perceptions_count;}
	unsigned int get_actions_count(){return _actions_count;}
	float get_output(unsigned int number);
	unsigned int get_action_type(unsigned int number);
	unsigned long get_row_code(unsigned int pos){return _rows_vec[pos];}
	unsigned long get_column_code(unsigned int pos){return _columns_vec[pos];}
	unsigned int get_x_by_code(unsigned long code){return _columns_map[code];}
	unsigned int get_y_by_code(unsigned long code){return _rows_map[code];}
	unsigned int get_x_by_offset(unsigned int offset);
	void remove_input_output();
	unsigned int get_col_conn_count(unsigned int col){return _columns_connections_count_vec[col];}
	void set_col_conn_count(unsigned int col, unsigned int count){_columns_connections_count_vec[col] = count;}

	static const char className[];
        static Luna<Grid>::RegType methods[];

        Grid(lua_State* L);
        int add_component_set(lua_State* L);
	int set_width(lua_State* L);
	int set_height(lua_State* L);

	static unsigned int CURRENT_COLUMN_ID;
	static unsigned int CURRENT_ROW_ID;

protected:
	int _type;
	unsigned int _number;
	unsigned int _width;
	unsigned int _height;
	unsigned int _size;
	unsigned int _offset;
	list<GridbrainComponentSet*> _component_set;
	list<int> _component_set_end_column;

	vector<unsigned long> _rows_vec;
	vector<unsigned long> _columns_vec;
	map<unsigned long, unsigned int> _rows_map;
	map<unsigned long, unsigned int> _columns_map;

	float* _input_matrix;
	float* _output_vector;
	unsigned int _max_input_depth;
	unsigned int _input_depth;
	vector<GridbrainComponent*> _perceptions_vec;
	vector<GridbrainComponent*> _actions_vec;
	unsigned int _perceptions_count;
	unsigned int _actions_count;

	vector<unsigned int> _columns_connections_count_vec;
};
#endif

