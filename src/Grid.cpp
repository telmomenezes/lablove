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

Grid::Grid(lua_State* luaState)
{
	mCreatedByScript = true;

	mType = 0;
	mWidth = 0;
	mHeight = 0;
	mNumber = 0;
	mSize = 0;
	mOffset = 0;
	mInputMatrix = NULL;
	mOutputVector = NULL;
	mInputDepth = 0;
	mPerceptionsCount = 0;
	mActionsCount = 0;
}

Grid::Grid(int type, unsigned int width, unsigned int height)
{
	mType = type;
	mWidth = width;
	mHeight = height;
	mNumber = 0;
	mSize = 0;
	mOffset = 0;
	mInputMatrix = NULL;
	mOutputVector = NULL;
	mInputDepth = 0;
	mPerceptionsCount = 0;
	mActionsCount = 0;
}

Grid::Grid(Grid* grid)
{
	mType = grid->mType;
	mWidth = grid->mWidth;
	mHeight = grid->mHeight;
	mNumber = grid->mNumber;
	mSize = grid->mSize;
	mOffset = grid->mOffset;
	mInputDepth = 0;
	mPerceptionsCount = 0;
	mActionsCount = 0;
	mInputMatrix = NULL;
	mOutputVector = NULL;

	std::list<GridbrainComponentSet*>::iterator iterSet;
	for (iterSet = grid->mComponentSet.begin();
		iterSet != grid->mComponentSet.end();
		iterSet++)
	{
		mComponentSet.push_back(*iterSet);
	}

	std::list<int>::iterator iterEndColumn;
	for (iterEndColumn = grid->mComponentSetEndColumn.begin();
		iterEndColumn != grid->mComponentSetEndColumn.end();
		iterEndColumn++)
	{
		mComponentSetEndColumn.push_back(*iterEndColumn);
	}

	for (unsigned int i = 0; i < mHeight; i++)
	{
		unsigned long code = grid->mRowsVec[i];
		mRowsVec.push_back(code);
		mRowsMap[code] = i;
	}
	for (unsigned int i = 0; i < mWidth; i++)
	{
		unsigned long code = grid->mColumnsVec[i];
		mColumnsVec.push_back(code);
		mColumnsMap[code] = i;
		mColumnsConnectionsCountVec.push_back(grid->mColumnsConnectionsCountVec[i]);
	}
}



Grid::~Grid()
{
	if (mInputMatrix != NULL)
	{
		free(mInputMatrix);
		mInputMatrix = NULL;
	}
	if (mOutputVector != NULL)
	{
		free(mOutputVector);
		mOutputVector = NULL;
	}
}

void Grid::init()
{
	for (unsigned int i = 0; i < mWidth; i++)
	{
		mColumnsVec.push_back(CURRENT_COLUMN_ID);
		mColumnsMap[CURRENT_COLUMN_ID] = i;
		CURRENT_COLUMN_ID++;
		mColumnsConnectionsCountVec.push_back(0);
	}
	for (unsigned int i = 0; i < mHeight; i++)
	{
		mRowsVec.push_back(CURRENT_ROW_ID);
		mRowsMap[CURRENT_ROW_ID] = i;
		CURRENT_ROW_ID++;
	}

	mSize = mWidth * mHeight;
}

void Grid::addComponentSet(GridbrainComponentSet* componentSet, int endColumn)
{
	mComponentSet.push_back(componentSet);
	mComponentSetEndColumn.push_back(endColumn);
}

GridbrainComponent* Grid::getRandomComponent(unsigned int pos)
{
	unsigned int column = getXByOffset(pos);

	std::list<GridbrainComponentSet*>::iterator iterSet = mComponentSet.begin();
	std::list<int>::iterator iterEndColumn = mComponentSetEndColumn.begin();

	while (iterSet != mComponentSet.end())
	{
		int endColumn = (*iterEndColumn);

		if ((column <= endColumn) || (endColumn == -1))
		{
			return (*iterSet)->getRandom();
		}

		iterSet++;
		iterEndColumn++;
	}

	// TODO: error!
	return NULL;
}

void Grid::setInput(unsigned int number, unsigned int depth, float value)
{
	mInputMatrix[(depth * mPerceptionsCount) + number] = value;
}

unsigned int Grid::addPerception(GridbrainComponent* per)
{
	for (unsigned int i = 0; i < mPerceptionsVec.size(); i++)
	{
		GridbrainComponent* curPer = mPerceptionsVec[i];

		if (curPer->mParameter == per->mParameter)
		{
			return i;
		}
	}

	mPerceptionsVec.push_back(per);
	return (mPerceptionsVec.size() - 1);
}

unsigned int Grid::addAction(GridbrainComponent* act)
{
	mActionsVec.push_back(act);
	return (mActionsVec.size() - 1);
}

void Grid::initInputMatrix(unsigned int maxInputDepth)
{
	mPerceptionsCount = mPerceptionsVec.size();
	mMaxInputDepth = maxInputDepth;
	unsigned int inputSize = mPerceptionsCount * mMaxInputDepth;

	if (inputSize >  0)
	{
		mInputMatrix = (float*)malloc(inputSize * sizeof(float));
	}
}

void Grid::initOutputVector()
{
	mActionsCount = mActionsVec.size();

	if (mActionsCount > 0)
	{
		mOutputVector = (float*)malloc(mActionsCount * sizeof(float));
	}
}

unsigned int Grid::getPerceptionType(unsigned int number)
{
	GridbrainComponent* comp = mPerceptionsVec[number];
	return (unsigned int)comp->mParameter;
}

float Grid::getOutput(unsigned int number)
{
	return mOutputVector[number];
}

unsigned int Grid::getActionType(unsigned int number)
{
	GridbrainComponent* comp = mActionsVec[number];
	return (unsigned int)comp->mParameter;
}

void Grid::removeInputOutput()
{
	if (mInputMatrix != NULL)
	{
		free(mInputMatrix);
		mInputMatrix = NULL;
	}
	if (mOutputVector != NULL)
	{
		free(mOutputVector);
		mOutputVector = NULL;
	}

	mPerceptionsCount = 0;
	mActionsCount = 0;

	mPerceptionsVec.clear();
	mActionsVec.clear();
}

unsigned int Grid::getXByOffset(unsigned int offset)
{
	unsigned int internalOffset = offset - mOffset;
	return (internalOffset / mHeight);
}

const char Grid::mClassName[] = "Grid";

Orbit<Grid>::MethodType Grid::mMethods[] = {
	{"addComponentSet", &Grid::addComponentSet},
	{"setWidth", &Grid::setWidth},
	{"setHeight", &Grid::setHeight},
        {0,0}
};

Orbit<Grid>::NumberGlobalType Grid::mNumberGlobals[] = {{0,0}};

int Grid::addComponentSet(lua_State* luaState)
{
        GridbrainComponentSet* set = (GridbrainComponentSet*)Orbit<Grid>::pointer(luaState, 1);
        int endColumn = luaL_checkint(luaState, 2);
        addComponentSet(set, endColumn);
        return 0;
}

int Grid::setWidth(lua_State* luaState)
{
	setWidth(luaL_checkint(luaState, 1));
	return 0;
}

int Grid::setHeight(lua_State* luaState)
{
	setHeight(luaL_checkint(luaState, 1));
	return 0;
}

