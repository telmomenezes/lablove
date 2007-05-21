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

#ifndef __INCLUDE_GRIDBRAIN_H
#define __INCLUDE_GRIDBRAIN_H

#include <stdio.h>

#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainConnection.h"
#include "GridbrainComponentSet.h"

#include <vector>
using namespace std;

class Gridbrain
{
public:
	Gridbrain();
	virtual ~Gridbrain();

	Gridbrain* clone(bool full=true);

	void add_grid(Grid* grid);
	void init();

	bool is_aggregator(int type);

	void set_component(unsigned int x,
				unsigned int y,
				unsigned int grid_number,
				int type,
				float parameter);
	void add_connection(unsigned int x_orig,
				unsigned int y_orig,
				unsigned int g_orig,
				unsigned int x_targ,
				unsigned int y_targ,
				unsigned int g_targ,
				float weight);
	bool connection_exists(unsigned int x_orig,
				unsigned int y_orig,
				unsigned int g_orig,
				unsigned int x_targ,
				unsigned int y_targ,
				unsigned int g_targ);
	void select_random_connection(unsigned int &x1,
					unsigned int &y1,
					unsigned int &g1,
					unsigned int &x2,
					unsigned int &y2,
					unsigned int &g2);
	void add_random_connection();
	void set_max_input_depth(unsigned int max_input_depth){_max_input_depth = max_input_depth;}

	void eval();
	
	Grid* get_grid(unsigned int number);

	void init_grids_input_output();

	void mutate_add_connection();
	void mutate_remove_connection();
	void mutate_change_connection_weight();
	void mutate_change_component();

protected:
	void init_grid_input_output(Grid* grid);
	void calc_connection_counts();

	vector<Grid*> _grids_vec;

        GridbrainComponent* _components;
	unsigned int _max_input_depth;
	unsigned int _number_of_components;
	GridbrainConnection* _connections;
	unsigned int _connections_count;
	unsigned int _grids_count;
	unsigned int _first_beta_index;
	unsigned int _total_possible_connections;
	unsigned int _beta_components_count;
};

#endif

