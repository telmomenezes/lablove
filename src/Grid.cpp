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

Grid::Grid(lua_State* luaState)
{
    mType = ALPHA;
    mNumber = 0;
    mWidth = 0;
    mHeight = 0;
    mSize = 0;
    mOffset = 0;
    mInputMatrix = NULL;
    mOutputVector = NULL;
    mMaxInputDepth = 50;
    mInputDepth = 0;
    mPerceptionsCount = 0;
    mActionsCount = 0;
    mWriteX = 0;
    mWriteY = 0;
    mConnDensity = 0.0f;
    mAverageJump = 0.0f;

    if (luaState)
    {
        mCreatedByScript = true;
    }
}

Grid::Grid(const Grid& grid)
{
    mType = grid.mType;
    mWidth = grid.mWidth;
    mHeight = grid.mHeight;
    mNumber = grid.mNumber;
    mSize = grid.mSize;
    mOffset = grid.mOffset;
    mInputDepth = 0;
    mPerceptionsCount = 0;
    mActionsCount = 0;
    mInputMatrix = NULL;
    mOutputVector = NULL;
    mWriteX = 0;
    mWriteY = 0;
    mConnDensity = 0.0f;
    mAverageJump = 0.0f;

    Grid* auxGrid = (Grid*)&grid;

    list<GridbrainComponentSet*>::iterator iterSet;
    for (iterSet = auxGrid->mComponentSetsList.begin();
        iterSet != auxGrid->mComponentSetsList.end();
        iterSet++)
    {
        mComponentSetsList.push_back(*iterSet);
    }

    vector<GridbrainComponentSet*>::iterator iterComp;
    for (iterComp = auxGrid->mComponentSetByColumn.begin();
        iterComp != auxGrid->mComponentSetByColumn.end();
        iterComp++)
    {
        mComponentSetByColumn.push_back(*iterComp);
    }

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnsConnectionsCountVec.push_back(grid.mColumnsConnectionsCountVec[i]);
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

void Grid::init(Type type, unsigned int width, unsigned int height)
{
    mType = type;
    mWidth = width;
    mHeight = height;

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnsConnectionsCountVec.push_back(0);
        mComponentSetByColumn.push_back(NULL);
    }

    mSize = mWidth * mHeight;
}

void Grid::addComponentSet(GridbrainComponentSet* componentSet,
                            int startColumn,
                            int endColumn)
{
    mComponentSetsList.push_back(componentSet);

    int c1 = 0;
    int c2 = mWidth - 1;

    if (startColumn >= 0)
    {
        c1 = startColumn;
    }
    if (endColumn >= 0)
    {
        c2 = endColumn;
    }

    for (unsigned int i = c1; i <= c2; i++)
    {
        mComponentSetByColumn[i] = componentSet;
    }
}

GridbrainComponent* Grid::getRandomComponent(unsigned int pos)
{
    unsigned int column = getXByOffset(pos);
    GridbrainComponentSet* set = mComponentSetByColumn[column];

    if (set)
    {
        return mComponentSetByColumn[column]->getRandom();
    }
    else
    {
        return GridbrainComponent::getNullComponent();
    }
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

        if ((curPer->mSubType == per->mSubType)
            && (curPer->mOrigSymTable == per->mOrigSymTable)
            && (curPer->mTargetSymTable == per->mTargetSymTable)
            && (curPer->mOrigSymIndex == per->mOrigSymIndex)
            && (curPer->mTargetSymIndex == per->mTargetSymIndex))
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

GridbrainComponent* Grid::getPerception(unsigned int number)
{
    return mPerceptionsVec[number];
}

GridbrainComponent* Grid::getAction(unsigned int number)
{
    return mActionsVec[number];
}

float Grid::getOutput(unsigned int number)
{
    return mOutputVector[number];
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

float* Grid::getInputBuffer()
{
    float* buffer = mInputMatrix + (mInputDepth * mPerceptionsCount);
    if (mInputDepth < (mMaxInputDepth - 1))
    {
        mInputDepth++;
    }
    return buffer;
}

const char Grid::mClassName[] = "Grid";

Orbit<Grid>::MethodType Grid::mMethods[] = {
    {"addComponentSet", &Grid::addComponentSet},
    {"init", &Grid::init},
    {0,0}
};

Orbit<Grid>::NumberGlobalType Grid::mNumberGlobals[] = {
    {"ALPHA", ALPHA},
    {"BETA", BETA},
    {0,0}
};

int Grid::addComponentSet(lua_State* luaState)
{
    GridbrainComponentSet* set = (GridbrainComponentSet*)Orbit<Grid>::pointer(luaState, 1);
    int startColumn = luaL_optint(luaState, 2, -1);
    int endColumn = luaL_optint(luaState, 3, -1);
    addComponentSet(set, startColumn, endColumn);
    return 0;
}

int Grid::init(lua_State* luaState)
{
    Type type = (Type)luaL_checkint(luaState, 1);
    unsigned int width = luaL_checkint(luaState, 2);
    unsigned int height = luaL_checkint(luaState, 3);
    init(type, width, height);
    return 0;
}

