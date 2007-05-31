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

#include "Gridbrain.h"
#include "functions.h"
#include "random.h"

#include <stdlib.h>
#include <string.h>

Gridbrain::Gridbrain()
{
	_max_input_depth = 50;
        _components = NULL;
	_number_of_components = 0;
	_connections = NULL;
	_connections_count = 0;
	_first_beta_index = 0;
	_total_possible_connections = 0;
	_beta_components_count = 0;
}

Gridbrain::~Gridbrain()
{
	while (_connections != NULL)
	{
		GridbrainConnection* conn = _connections;
		_connections = (GridbrainConnection*)conn->_next_global_connection;
		free(conn);
	}
	if (_components != NULL)
	{
        	free(_components);
		_components = NULL;
	}
	for (unsigned int i = 0; i < _grids_count; i++)
	{
		if (!_grids_vec[i]->_created_by_script)
		{
			delete _grids_vec[i];
		}
	}
	_grids_count = 0;
}

Gridbrain* Gridbrain::clone(bool full)
{
	Gridbrain* gb = new Gridbrain();

	gb->_max_input_depth = _max_input_depth;
	gb->_number_of_components = _number_of_components;
	gb->_connections = NULL;
	gb->_connections_count = 0;
	gb->_grids_count = _grids_count;
	gb->_first_beta_index = _first_beta_index;
	gb->_total_possible_connections = _total_possible_connections;
	gb->_beta_components_count = _beta_components_count;

	for (unsigned int i = 0; i < _grids_count; i++)
	{
		Grid* grid = _grids_vec[i];
		gb->_grids_vec.push_back(new Grid(grid));
	}

	if (full)
	{
		gb->_components = (GridbrainComponent*)malloc(_number_of_components * sizeof(GridbrainComponent));

		unsigned int index = 0;

		for (unsigned int index = 0; index < _number_of_components; index++)
		{
			gb->_components[index]._input = 0;
			gb->_components[index]._output = 0;
			gb->_components[index]._state = 0;
			gb->_components[index]._perception_position = 0;
			gb->_components[index]._action_position = 0;
			gb->_components[index]._connections_count = 0;
			gb->_components[index]._recurrent_input = 0;
			gb->_components[index]._forward_flag = false;
			gb->_components[index]._recurrent_flag = false;
			gb->_components[index]._first_connection = NULL;
			gb->_components[index]._type = _components[index]._type;
			gb->_components[index]._parameter = _components[index]._parameter;
			gb->_components[index]._offset = _components[index]._offset;
			gb->_components[index]._aggregator = _components[index]._aggregator;
			gb->_components[index]._column = _components[index]._column;
			gb->_components[index]._row = _components[index]._row;
			gb->_components[index]._grid = _components[index]._grid;
			gb->_components[index]._molecule = _components[index]._molecule;
		}

		for (unsigned int index = 0; index < _number_of_components; index++)
		{
			GridbrainConnection* conn = _components[index]._first_connection;
			while (conn != NULL)
			{
				Grid* grid_orig = _grids_vec[conn->_grid_orig];
				Grid* grid_targ = _grids_vec[conn->_grid_targ];
				gb->add_connection(grid_orig->get_x_by_code(conn->_column_orig),
					grid_orig->get_y_by_code(conn->_row_orig),
					conn->_grid_orig,
					grid_targ->get_x_by_code(conn->_column_targ),
					grid_targ->get_y_by_code(conn->_row_targ),
					conn->_grid_targ,
					conn->_weight);
				conn = (GridbrainConnection*)conn->_next_connection;
			}
		}

		gb->init_grids_input_output();
	}

	return gb;
}

void Gridbrain::add_grid(Grid* grid)
{
	grid->set_number(_grids_vec.size());
	grid->set_offset(_number_of_components);
	unsigned int grid_component_count = grid->get_height() * grid->get_width();
	_number_of_components += grid_component_count;

	if (grid->get_type() == Grid::BETA)
	{
		_first_beta_index = _number_of_components;
		_beta_components_count = grid_component_count;
	}

	grid->init();
	_grids_vec.push_back(grid);
}

void Gridbrain::init()
{
	_grids_count = _grids_vec.size();

	calc_connection_counts();
	
        _components = (GridbrainComponent*)malloc(_number_of_components * sizeof(GridbrainComponent));

	// Init grids with random components

	unsigned int pos = 0;

	for (unsigned int i = 0; i < _grids_count; i++)
	{
		Grid* grid = _grids_vec[i];

		for (unsigned int x = 0;
			x < grid->get_width();
			x++)
		{
			for (unsigned int y = 0;
				y < grid->get_height();
				y++)
			{
				GridbrainComponent* comp = grid->get_random_component(pos);
				_components[pos]._input = 0;
				_components[pos]._output = 0;
				_components[pos]._recurrent_input = 0;
				_components[pos]._connections_count = 0;
				_components[pos]._first_connection = NULL;
				_components[pos]._state = 0;
				_components[pos]._forward_flag = false;
				_components[pos]._recurrent_flag = false;
				_components[pos]._perception_position = 0;
				_components[pos]._action_position = 0;
				_components[pos]._type = comp->_type;
				_components[pos]._parameter = comp->_parameter;
				_components[pos]._offset = pos;
				_components[pos]._aggregator = is_aggregator(comp->_type);
				_components[pos]._column = grid->get_column_code(x);
				_components[pos]._row = grid->get_row_code(y);
				_components[pos]._grid = i;
				_components[pos]._molecule = random() % 5;

				pos++;
			}
		}
	}

	init_grids_input_output();
}

void Gridbrain::init_grids_input_output()
{
	for (unsigned int i = 0; i < _grids_count; i++)
	{
		Grid* grid = _grids_vec[i];
		init_grid_input_output(grid);		
	}
}

void Gridbrain::init_grid_input_output(Grid* grid)
{
	grid->remove_input_output();
	unsigned int pos = grid->get_offset();

	for (unsigned int j = 0;
		j < grid->get_size();
		j++)
	{
		if (grid->get_type() == Grid::ALPHA)
		{
			if (_components[pos]._type == GridbrainComponent::PER)
			{
				_components[pos]._perception_position = grid->add_perception(&_components[pos]);
			}
		}
		else
		{
			if (_components[pos]._type == GridbrainComponent::ACT)
			{
				_components[pos]._action_position = grid->add_action(&_components[pos]);
			}
		}
		pos++;
	}

	if (grid->get_type() == Grid::ALPHA)
	{
		grid->init_input_matrix(_max_input_depth);
	}
	else
	{
		grid->init_output_vector();
	}
}

bool Gridbrain::is_aggregator(int type)
{
	return ((type == GridbrainComponent::AGG ) || (type == GridbrainComponent::MAX));
}

void Gridbrain::set_component(unsigned int x,
				unsigned int y,
				unsigned int grid_number,
				GridbrainComponent::Type type,
				float parameter)
{
	Grid* grid = _grids_vec[grid_number];

	unsigned int pos = (x * grid->get_height()) + y + grid->get_offset();

	_components[pos]._input = 0;
	_components[pos]._output = 0;
	_components[pos]._recurrent_input = 0;
	_components[pos]._connections_count = 0;
	_components[pos]._first_connection = NULL;
	_components[pos]._state = 0;
	_components[pos]._forward_flag = false;
	_components[pos]._recurrent_flag = false;
	_components[pos]._perception_position = 0;
	_components[pos]._action_position = 0;
	_components[pos]._type = type;
	_components[pos]._parameter = parameter;
	_components[pos]._aggregator = is_aggregator(type);
	_components[pos]._column = grid->get_column_code(x);
	_components[pos]._row = grid->get_row_code(y);
}

void Gridbrain::add_connection(unsigned int x_orig,
				unsigned int y_orig,
				unsigned int g_orig,
				unsigned int x_targ,
				unsigned int y_targ,
				unsigned int g_targ,
				float weight)
{
	// TODO: disallow invalid connections?
	if (connection_exists(x_orig, y_orig, g_orig, x_targ, y_targ, g_targ))
	{
		return;
	}

	Grid* grid = _grids_vec[g_orig];
	unsigned int orig = (x_orig * grid->get_height()) + y_orig + grid->get_offset();
	grid = _grids_vec[g_targ];
	unsigned int target = (x_targ * grid->get_height()) + y_targ + grid->get_offset();

	GridbrainComponent* comp = &(_components[orig]);
	GridbrainComponent* targ_comp = &(_components[target]);
	GridbrainConnection* conn = (GridbrainConnection*)malloc(sizeof(GridbrainConnection));
	conn->_column_orig = comp->_column;
	conn->_row_orig = comp->_row;
	conn->_grid_orig = comp->_grid;
	conn->_column_targ = targ_comp->_column;
	conn->_row_targ = targ_comp->_row;
	conn->_grid_targ = targ_comp->_grid;
	conn->_target = target;
	conn->_weight = weight;
	conn->_orig_component = comp;
	conn->_feed_forward = true;

	if (conn->_grid_orig == conn->_grid_targ)
	{
		conn->_inter_grid = false;

		if (x_orig >= x_targ)
		{
			conn->_feed_forward = false;
		}
	}
	else
	{
		conn->_inter_grid = true;
	}

	if (comp->_connections_count == 0)
	{
		comp->_first_connection = conn;
		conn->_prev_connection = NULL;
		conn->_next_connection = NULL;
	}
	else
	{
		GridbrainConnection* next_conn = comp->_first_connection;
		conn->_prev_connection = NULL;
		conn->_next_connection = next_conn;
		comp->_first_connection = conn;
		next_conn->_prev_connection = conn;
	}

	(comp->_connections_count)++;

	GridbrainConnection* next_conn = _connections;
	conn->_next_global_connection = next_conn;
	conn->_prev_global_connection = NULL;
	_connections = conn;

	if (next_conn != NULL)
	{
		next_conn->_prev_global_connection = conn;
	}

	_connections_count++;
}

bool Gridbrain::connection_exists(unsigned int x_orig,
				unsigned int y_orig,
				unsigned int g_orig,
				unsigned int x_targ,
				unsigned int y_targ,
				unsigned int g_targ)
{
	Grid* grid = _grids_vec[g_orig];
	unsigned int orig = (x_orig * grid->get_height()) + y_orig + grid->get_offset();
	grid = _grids_vec[g_targ];
	unsigned int target = (x_targ * grid->get_height()) + y_targ + grid->get_offset();

	GridbrainComponent* comp = &(_components[orig]);
	GridbrainConnection* conn = comp->_first_connection;

	unsigned int i = 0;
	while ((conn) && (i < comp->_connections_count))
	{
		if (conn->_target == target)
		{
			return true;
		}

		conn = (GridbrainConnection*)conn->_next_connection;
		i++;
	}

	return false;
}

void Gridbrain::select_random_connection(unsigned int &x1,
					unsigned int &y1,
					unsigned int &g1,
					unsigned int &x2,
					unsigned int &y2,
					unsigned int &g2)
{
	// TODO: don't assume beta is the last grid

	unsigned int conn_pos = rand() % _total_possible_connections;
	unsigned int cur_pos = 0;
	unsigned int col_conn_count = 0;


	Grid* grid_orig;

	bool found = false;
	for (unsigned g = 0;
		(g < _grids_count) && (!found);
		g++)
	{
		grid_orig = _grids_vec[g];
		unsigned int width = grid_orig->get_width();
		
		for (unsigned int col = 0;
			(col < width) && (!found);
			col++)
		{
			col_conn_count = grid_orig->get_col_conn_count(col);
			cur_pos += col_conn_count;

			if (cur_pos >= conn_pos)
			{
				g1 = grid_orig->get_number();
				x1 = col;
				y1 = random_uniform_int(0, grid_orig->get_height() - 1);
				found = true;
			}
		}
	}

	

	if (grid_orig->get_type() == Grid::ALPHA)
	{
		unsigned int targ_conn_pos = rand() % col_conn_count;

		cur_pos = 0;
		unsigned int width = grid_orig->get_width();
		for (unsigned int col = (x1 + 1); col < width; col++)
		{
			cur_pos += grid_orig->get_col_conn_count(col);

			if (cur_pos >= targ_conn_pos)
			{
				g2 = grid_orig->get_number();
				x2 = col;
				y2 = random_uniform_int(0, grid_orig->get_height() - 1);
				return;
			}
		}

		// Assumes one beta at the end
		g2 = _grids_count - 1;
	}
	else
	{
		g2 = g1;
	}

	Grid* targ_grid = _grids_vec[g2];

	x2 = random_uniform_int(0, targ_grid->get_width() - 1);
	y2 = random_uniform_int(0, targ_grid->get_height() - 1);
}

void Gridbrain::add_random_connection()
{
	unsigned int x1;
	unsigned int x2;
	unsigned int y1;
	unsigned int y2;
	unsigned int g1;
	unsigned int g2;
	float weight;

	select_random_connection(x1, y1, g1, x2, y2, g2);
	
	weight = random_uniform_probability();
	
	if (random_uniform_bool())
	{
		weight = -weight;
	}
	
	add_connection(x1, y1, g1, x2, y2, g2, weight);
}

void Gridbrain::eval()
{
	GridbrainComponent* comp;
	GridbrainConnection* conn;

	// Reset all components
	for (unsigned int i = 0; i < _number_of_components; i++)
	{
		comp = &(_components[i]);

		comp->_input = 0;
		comp->_state = 0;
		comp->_forward_flag = false;
		comp->_recurrent_flag = false;

		// Transfer recurrent inputs to direct inputs
		if (i >= _first_beta_index)
		{
			comp->_input = _components[i]._recurrent_input;
			comp->_recurrent_input = 0;
		}
	}

	// Evaluate grids
	for (unsigned int grid_number = 0; grid_number < _grids_count; grid_number++)
	{
		Grid* grid = _grids_vec[grid_number];

		unsigned int input_depth = 1;
		int pass_count = 1;
		unsigned int perceptions_count = 0;
		if (grid->get_type() == Grid::ALPHA)
		{
			pass_count = 2;
			perceptions_count = grid->get_perceptions_count();
			input_depth = grid->get_input_depth();
		}

		unsigned int start_index;
		unsigned int end_index;

		start_index = grid->get_offset();
		end_index = start_index + grid->get_size();

		float* input_matrix = grid->get_input_matrix();
		float* output_vector = grid->get_output_vector();

		for (unsigned int pass = 0; pass < pass_count; pass++)
		{
			bool first_alpha = false;
			if (pass < (pass_count - 1))
			{
				first_alpha = true;
			}

			unsigned int input_depth_offset = 0;

			for (unsigned int i = 0; i < input_depth; i++)
			{
				if (grid->get_type() == Grid::ALPHA)
				{
					// reset alpha components except aggregators
					for (unsigned int i = start_index; i < end_index; i++)
					{
						comp = &(_components[i]);

						if (!comp->_aggregator)
						{
							if (comp->_type == GridbrainComponent::PER)
							{
								// apply perceptions for this input depth
								comp->_state =
									input_matrix[comp->_perception_position
											+ input_depth_offset];
							}
							else
							{
								comp->_state = 0;
							}
							comp->_forward_flag = false;
							comp->_recurrent_flag = false;
						}
						comp->_input = 0;
					}
				}

				for (unsigned int j = start_index; j < end_index; j++)
				{
					comp = &(_components[j]);
					conn = comp->_first_connection;

					// compute component output
					float output;
					if (comp->_type == GridbrainComponent::NUL)
					{
						//printf("NUL ");
						output = 0.0f;
					}
					else if (comp->_type == GridbrainComponent::PER)
					{
						//printf("PER ");
						output = comp->_state;
					}
					else if (comp->_type == GridbrainComponent::STA)
					{
						//printf("STA ");
						output = comp->_state;
					}
					else if (comp->_type == GridbrainComponent::ACT)
					{
						//printf("ACT ");
						// TODO: return 1 if the action was executed?
						output_vector[comp->_action_position] = comp->_input;
						output = 0.0f;
					}
					else if (comp->_type == GridbrainComponent::THR)
					{
						//printf("THR ");
						if ((comp->_input > 0.1)
							|| (comp->_input < -0.1))
						{
							output = comp->_input;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->_type == GridbrainComponent::AGG)
					{
						//printf("AGG ");
						if ((comp->_input > 0.1)
							|| (comp->_input < -0.1))
						{
							output = comp->_input;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->_type == GridbrainComponent::MAX)
					{
						//printf("MAX ");
						if (comp->_input >= comp->_state)
						{
							output = 1.0f;
							comp->_state = comp->_input;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->_type == GridbrainComponent::MUL)
					{
						//printf("MUL ");
						output = comp->_input;
					}
					else if (comp->_type == GridbrainComponent::NOT)
					{
						//printf("NOT ");
						if (comp->_input == 0.0f)
						{
							output = 1.0f;
						}
						else
						{
							output = 0.0f;
						}
					}
					else {
							// TODO: this is an error. how to inform?
							output = 0.0f;
					}

					if (output > 1.0f)
					{
						output = 1.0f;
					}
					else if (output < -1.0f)
					{
						output = -1.0f;
					}
					comp->_output = output;

					//printf("%f => %f\n", comp->_input, output);

					// propagate outputs (inside grid if fist alpha)
					for (unsigned int k = 0; k < comp->_connections_count; k++)
					{
						if ((!first_alpha) || (!conn->_inter_grid))
						{
							GridbrainComponent* target_comp = &(_components[conn->_target]);
							float input = output * conn->_weight;

							/*printf("(%d, %d, %d)[%f] -> (%d, %d, %d)[%f]\n",
								conn->_column_orig,
								conn->_row_orig,
								conn->_grid_orig,
								output,
								conn->_column_targ,
								conn->_row_targ,
								conn->_grid_targ,
								input);*/
							
							// Feed forward
							if (conn->_feed_forward)
							{
								if (target_comp->_type == GridbrainComponent::MUL)
								{
									if (!target_comp->_forward_flag)
									{
										target_comp->_input = input;
										target_comp->_forward_flag = true;
									}
									else
									{
										target_comp->_input *= input;
									}
								}
								else
								{
									target_comp->_input += input;
								}
							}
							// Recurent
							else
							{
								if (target_comp->_type == GridbrainComponent::MUL)
								{
									if (!target_comp->_recurrent_flag)
									{
										target_comp->_recurrent_input = input;
										target_comp->_recurrent_flag = true;
									}
									else
									{
										target_comp->_recurrent_input *= input;
									}
								}
								else
								{
									target_comp->_recurrent_input += input;
								}
							}
						}
						conn = (GridbrainConnection*)conn->_next_connection;
					}
				}

				input_depth_offset += perceptions_count;
			}
		}
	}
}

void Gridbrain::mutate_add_connection()
{
	add_random_connection();
}

void Gridbrain::mutate_remove_connection()
{
	if (_connections_count > 0) 
	{
		unsigned int connection_pos = random_uniform_int(0, _connections_count - 1);

		GridbrainConnection* conn = _connections;
		for (unsigned int i = 0; i < connection_pos; i++)
		{
			conn = (GridbrainConnection*)conn->_next_global_connection;
		}

		GridbrainComponent* comp = (GridbrainComponent*)conn->_orig_component;
		comp->_connections_count--;

		if (conn->_prev_connection)
		{
			((GridbrainConnection*)conn->_prev_connection)->_next_connection = conn->_next_connection;
		}
		else
		{
			comp->_first_connection = (GridbrainConnection*)conn->_next_connection;
		}
		if (conn->_next_connection)
		{
			((GridbrainConnection*)conn->_next_connection)->_prev_connection = conn->_prev_connection;
		}

		_connections_count--;
		if (conn->_prev_global_connection)
		{
			((GridbrainConnection*)conn->_prev_global_connection)->_next_global_connection = conn->_next_global_connection;
		}
		else
		{
			_connections = (GridbrainConnection*)conn->_next_global_connection;
		}
		if (conn->_next_global_connection)
		{
			((GridbrainConnection*)conn->_next_global_connection)->_prev_global_connection = conn->_prev_global_connection;
		}

		free(conn);
	}
}

void Gridbrain::mutate_change_connection_weight()
{
	if (_connections_count > 0) 
	{
		unsigned int connection_pos = random_uniform_int(0, _connections_count - 1);

		GridbrainConnection* conn = _connections;
		for (unsigned int i = 0; i < connection_pos; i++)
		{
			conn = (GridbrainConnection*)conn->_next_global_connection;
		}
		
		float new_weight = conn->_weight;
		new_weight += random_uniform_float(-1.0f, 1.0f);
		if (new_weight > 1.0f)
		{
			new_weight = 1.0f;
		}
		else if (new_weight < -1.0f)
		{
			new_weight = -1.0f;
		}

		conn->_weight = new_weight;
	}
}

void Gridbrain::mutate_change_component()
{
	unsigned int pos = random_uniform_int(0, _number_of_components - 1);

	Grid* grid = _grids_vec[_components[pos]._grid];

	GridbrainComponent* comp = grid->get_random_component(pos);

	_components[pos]._input = 0;
	_components[pos]._output = 0;
	_components[pos]._recurrent_input = 0;
	_components[pos]._state = 0;
	_components[pos]._forward_flag = false;
	_components[pos]._recurrent_flag = false;
	_components[pos]._perception_position = 0;
	_components[pos]._action_position = 0;
	_components[pos]._type = comp->_type;
	_components[pos]._parameter = comp->_parameter;
	_components[pos]._aggregator = is_aggregator(comp->_type);
	_components[pos]._molecule = random() % 5;

	init_grid_input_output(grid);
}

Grid* Gridbrain::get_grid(unsigned int number)
{
	return _grids_vec[number];
}

void Gridbrain::calc_connection_counts()
{
	_total_possible_connections = 0;

	for (unsigned int g = 0; g < _grids_count; g++)
	{
		Grid* grid = _grids_vec[g];
		unsigned int width = grid->get_width();
		unsigned int height = grid->get_height();

		for (unsigned int col = 0; col < width; col++)
		{
			unsigned int possible_connections;

			possible_connections = _beta_components_count * height;

			if (grid->get_type() == Grid::ALPHA)
			{
				possible_connections += (width - col) * height * height;
			}
			
			grid->set_col_conn_count(col, possible_connections);
			_total_possible_connections += possible_connections;
		}
	}
}

