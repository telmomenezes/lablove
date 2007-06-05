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

	static const char mClassName[];
        static Orbit<GridbrainComponent>::MethodType mMethods[];
	static Orbit<GridbrainComponent>::NumberGlobalType mNumberGlobals[];

        GridbrainComponent(lua_State* luaState);

	Type mType;
	float mInput;
	float mOutput;
	float mRecurrentInput;
	unsigned int mConnectionsCount;
	GridbrainConnection* mFirstConnection;
	float mParameter;
	float mState;
	bool mAggregator;
	bool mForwardFlag;
	bool mRecurrentFlag;
	unsigned int mOffset;
	unsigned int mPerceptionPosition;
	unsigned int mActionPosition;
	unsigned int mMolecule;

	unsigned long mColumn;
	unsigned long mRow;
	unsigned int mGrid;
};

#endif

