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

#include "GridbrainComponent.h"

GridbrainComponent::GridbrainComponent()
{
	mInput = 0;
	mOutput = 0;
	mRecurrentInput = 0;
	mState = 0;
	mForwardFlag = false;
	mRecurrentFlag = false;
	mPerceptionPosition = 0;
	mActionPosition = 0;
	mType = NUL;
	mParameter = 0.0f;
	mOffset = 0;
	mAggregator = false;
	mColumn = 0;
	mRow = 0;
	mGrid = 0;
}

GridbrainComponent::GridbrainComponent(lua_State* luaState)
{
	mInput = 0;
	mOutput = 0;
	mRecurrentInput = 0;
	mState = 0;
	mForwardFlag = false;
	mRecurrentFlag = false;
	mPerceptionPosition = 0;
	mActionPosition = 0;
	mType = NUL;
	mParameter = 0.0f;
	mOffset = 0;
	mAggregator = false;
	mColumn = 0;
	mRow = 0;
	mGrid = 0;
}

GridbrainComponent::~GridbrainComponent()
{
}

void GridbrainComponent::clear(bool clearConnections)
{
	mInput = 0;
	mOutput = 0;
	mRecurrentInput = 0;
	mState = 0;
	mForwardFlag = false;
	mRecurrentFlag = false;
	mPerceptionPosition = 0;
	mActionPosition = 0;

	if (clearConnections)
	{
		mConnectionsCount = 0;
		mFirstConnection = NULL;
	}
}

void GridbrainComponent::copy(GridbrainComponent* comp)
{
	clear();

	mType = comp->mType;
	mParameter = comp->mParameter;
	mOffset = comp->mOffset;
	mAggregator = comp->mAggregator;
	mColumn = comp->mColumn;
	mRow = comp->mRow;
	mGrid = comp->mGrid;

}

bool GridbrainComponent::isAggregator()
{
	return ((mType == GridbrainComponent::AGG )
		|| (mType == GridbrainComponent::MAX));
}

const char GridbrainComponent::mClassName[] = "GridbrainComponent";

Orbit<GridbrainComponent>::MethodType GridbrainComponent::mMethods[] = {{0,0}};

Orbit<GridbrainComponent>::NumberGlobalType GridbrainComponent::mNumberGlobals[] = {
	{"NUL", NUL},
	{"PER", PER},
	{"STA", STA},
	{"ACT", ACT},
	{"THR", THR},
	{"AGG", AGG},
	{"MAX", MAX},
	{"MUL", MUL},
	{"NOT", NOT},
	{0,0}
};

