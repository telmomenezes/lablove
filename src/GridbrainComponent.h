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

#ifndef _INCLUDE_GRIDBRAIN_COMPONENT_H
#define _INCLUDE_GRIDBRAIN_COMPONENT_H

#include "GridbrainConnection.h"

#include "Orbit.h"


class GridbrainComponent
{
public:
	enum Type {NUL, PER, STA, ACT, THR, AGG, MAX, MUL, NOT};

	GridbrainComponent();
	virtual ~GridbrainComponent();

	static const char class_name[];
        static Orbit<GridbrainComponent>::MethodType methods[];
	static Orbit<GridbrainComponent>::NumberGlobalType number_globals[];

        GridbrainComponent(lua_State* L);

	Type _type;
	float _input;
	float _output;
	float _recurrent_input;
	unsigned int _connections_count;
	GridbrainConnection* _first_connection;
	float _parameter;
	float _state;
	bool _aggregator;
	bool _forward_flag;
	bool _recurrent_flag;
	unsigned int _offset;
	unsigned int _perception_position;
	unsigned int _action_position;
	unsigned int _molecule;

	unsigned long _column;
	unsigned long _row;
	unsigned int _grid;
};

#endif

