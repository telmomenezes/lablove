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

#include "Grid.h"

const int Grid::ALPHA = 0;
const int Grid::BETA = 1;

unsigned int Grid::CURRENT_COLUMN_ID = 0;
unsigned int Grid::CURRENT_ROW_ID = 0;

Grid::Grid(int type, unsigned int width, unsigned int height)
{
	_type = type;
	_width = width;
	_height = height;
	_number = 0;
	_size = 0;
	_offset = 0;
	_input_matrix = NULL;
	_output_vector = NULL;
	_input_depth = 0;
	_perceptions_count = 0;
	_actions_count = 0;
}


Grid::Grid(Grid* grid)
{
	_type = grid->_type;
	_width = grid->_width;
	_height = grid->_height;
	_number = grid->_number;
	_size = grid->_size;
	_offset = grid->_offset;
	_input_depth = 0;
	_perceptions_count = 0;
	_actions_count = 0;
	_input_matrix = NULL;
	_output_vector = NULL;

	list<GridbrainComponentSet*>::iterator iter_set;
	for (iter_set = grid->_component_set.begin();
		iter_set != grid->_component_set.end();
		iter_set++)
	{
		_component_set.push_back(*iter_set);
	}

	list<int>::iterator iter_end_column;
	for (iter_end_column = grid->_component_set_end_column.begin();
		iter_end_column != grid->_component_set_end_column.end();
		iter_end_column++)
	{
		_component_set_end_column.push_back(*iter_end_column);
	}

	for (unsigned int i = 0; i < _height; i++)
	{
		unsigned long code = grid->_rows_vec[i];
		_rows_vec.push_back(code);
		_rows_map[code] = i;
	}
	for (unsigned int i = 0; i < _width; i++)
	{
		unsigned long code = grid->_columns_vec[i];
		_columns_vec.push_back(code);
		_columns_map[code] = i;
		_columns_connections_count_vec.push_back(grid->_columns_connections_count_vec[i]);
	}
}

Grid::~Grid()
{
	if (_input_matrix != NULL)
	{
		free(_input_matrix);
		_input_matrix = NULL;
	}
	if (_output_vector != NULL)
	{
		free(_output_vector);
		_output_vector = NULL;
	}
}

void Grid::init()
{
	for (unsigned int i = 0; i < _width; i++)
	{
		_columns_vec.push_back(CURRENT_COLUMN_ID);
		_columns_map[CURRENT_COLUMN_ID] = i;
		CURRENT_COLUMN_ID++;
		_columns_connections_count_vec.push_back(0);
	}
	for (unsigned int i = 0; i < _height; i++)
	{
		_rows_vec.push_back(CURRENT_ROW_ID);
		_rows_map[CURRENT_ROW_ID] = i;
		CURRENT_ROW_ID++;
	}

	_size = _width * _height;
}

void Grid::add_component_set(GridbrainComponentSet* component_set, int end_column)
{
	_component_set.push_back(component_set);
	_component_set_end_column.push_back(end_column);
}

GridbrainComponent* Grid::get_random_component(unsigned int pos)
{
	unsigned int column = get_x_by_offset(pos);

	list<GridbrainComponentSet*>::iterator iter_set = _component_set.begin();
	list<int>::iterator iter_end_column = _component_set_end_column.begin();

	while (iter_set != _component_set.end())
	{
		int end_column = (*iter_end_column);

		if ((column <= end_column) || (end_column == -1))
		{
			return (*iter_set)->get_random();
		}

		iter_set++;
		iter_end_column++;
	}

	// TODO: error!
	return NULL;
}

void Grid::set_input(unsigned int number, unsigned int depth, float value)
{
	_input_matrix[(depth * _perceptions_count) + number] = value;
}

unsigned int Grid::add_perception(GridbrainComponent* per)
{
	for (unsigned int i = 0; i < _perceptions_vec.size(); i++)
	{
		GridbrainComponent* cur_per = _perceptions_vec[i];

		if ((cur_per->_parameter == per->_parameter) && (cur_per->_molecule == per->_molecule))
		{
			return i;
		}
	}

	_perceptions_vec.push_back(per);
	return (_perceptions_vec.size() - 1);
}

unsigned int Grid::add_action(GridbrainComponent* act)
{
	_actions_vec.push_back(act);
	return (_actions_vec.size() - 1);
}

void Grid::init_input_matrix(unsigned int max_input_depth)
{
	_perceptions_count = _perceptions_vec.size();
	_max_input_depth = max_input_depth;
	unsigned int input_size = _perceptions_count * _max_input_depth;

	if (input_size >  0)
	{
		_input_matrix = (float*)malloc(input_size * sizeof(float));
	}
}

void Grid::init_output_vector()
{
	_actions_count = _actions_vec.size();

	if (_actions_count > 0)
	{
		_output_vector = (float*)malloc(_actions_count * sizeof(float));
	}
}

unsigned int Grid::get_perception_type(unsigned int number)
{
	GridbrainComponent* comp = _perceptions_vec[number];
	return (unsigned int)comp->_parameter;
}

unsigned int Grid::get_perception_molecule(unsigned int number)
{
	GridbrainComponent* comp = _perceptions_vec[number];
	return (unsigned int)comp->_molecule;
}

float Grid::get_output(unsigned int number)
{
	return _output_vector[number];
}

unsigned int Grid::get_action_type(unsigned int number)
{
	GridbrainComponent* comp = _actions_vec[number];
	return (unsigned int)comp->_parameter;
}

void Grid::remove_input_output()
{
	if (_input_matrix != NULL)
	{
		free(_input_matrix);
		_input_matrix = NULL;
	}
	if (_output_vector != NULL)
	{
		free(_output_vector);
		_output_vector = NULL;
	}

	_perceptions_count = 0;
	_actions_count = 0;

	_perceptions_vec.clear();
	_actions_vec.clear();
}

unsigned int Grid::get_x_by_offset(unsigned int offset)
{
	unsigned int internal_offset = offset - _offset;
	return (internal_offset / _height);
}

const char Grid::className[] = "Grid";

Lunar<Grid>::RegType Grid::methods[] = {
	{"add_component_set", &Grid::add_component_set},
	{"set_width", &Grid::set_width},
	{"set_height", &Grid::set_height},
        {0,0}
};

Grid::Grid(lua_State* L)
{
	_created_by_script = true;

	_type = 0;
	_width = 0;
	_height = 0;
	_number = 0;
	_size = 0;
	_offset = 0;
	_input_matrix = NULL;
	_output_vector = NULL;
	_input_depth = 0;
	_perceptions_count = 0;
	_actions_count = 0;
}

int Grid::add_component_set(lua_State *L)
{
        GridbrainComponentSet* set = (GridbrainComponentSet*)Lunar<Grid>::pointer(L, 1);
        int end_column = luaL_checkint(L, 2);
        add_component_set(set, end_column);
        return 0;
}

int Grid::set_width(lua_State* L)
{
	set_width(luaL_checkint(L, 1));
	return 0;
}

int Grid::set_height(lua_State* L)
{
	set_height(luaL_checkint(L, 1));
	return 0;
}

