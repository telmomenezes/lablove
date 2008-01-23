/*
 * LabLOVE
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

unsigned long Grid::CURRENT_COLUMN_CODE = 0;
unsigned long Grid::CURRENT_ROW_CODE = 0;
mt_distribution* Grid::mDistRowsAndColumns = gDistManager.getNewDistribution();

Grid::Grid(lua_State* luaState)
{
    mType = ALPHA;
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
    mNewRow = -1;
    mNewColumn = -1;
    mAddRowOrColumn = false;

    mComponentSet = NULL;

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
    mSize = grid.mSize;
    mOffset = grid.mOffset;
    mInputDepth = 0;
    mPerceptionsCount = 0;
    mActionsCount = 0;
    mInputMatrix = NULL;
    mOutputVector = NULL;
    mWriteX = 0;
    mWriteY = 0;
    mNewRow = -1;
    mNewColumn = -1;
    mAddRowOrColumn = false;

    mComponentSet = grid.mComponentSet;

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnsConnectionsCountVec.push_back(grid.mColumnsConnectionsCountVec[i]);
        mColumnCodes.push_back(grid.mColumnCodes[i]);
    }
    for (unsigned int i = 0; i < mHeight; i++)
    {
        mRowCodes.push_back(grid.mRowCodes[i]);
    }

    /*printf("column codes: ");
    for (unsigned int i = 0; i < mWidth; i++)
    {
        printf("%d ", mColumnCodes[i]);
    }
    printf("\n");
    printf("row codes: ");
    for (unsigned int i = 0; i < mHeight; i++)
    {
        printf("%d ", mRowCodes[i]);
    }
    printf("\n");*/
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

void Grid::crossoverColumn(Grid* grid2, int xoverCol1, int xoverCol2)
{
    vector<unsigned long> columnCodes;

    for (unsigned int i = 0; i < xoverCol1; i++)
    {
        columnCodes.push_back(mColumnCodes[i]);
    }
    for (unsigned int i = xoverCol2; i < grid2->getWidth(); i++)
    {
        columnCodes.push_back(grid2->mColumnCodes[i]);
    }

    mColumnCodes.clear();
    for (unsigned int i = 0; i < columnCodes.size(); i++)
    {
        mColumnCodes.push_back(columnCodes[i]);
    }

    mWidth = mColumnCodes.size();
    mSize = mWidth * mHeight;

    mColumnsConnectionsCountVec.clear();

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnsConnectionsCountVec.push_back(0);
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
        mColumnCodes.push_back(CURRENT_COLUMN_CODE++);
    }

    for (unsigned int i = 0; i < mHeight; i++)
    {
        mRowCodes.push_back(CURRENT_ROW_CODE++);
    }

    mSize = mWidth * mHeight;
}

void Grid::setComponentSet(GridbrainComponentSet* componentSet)
{
    mComponentSet = componentSet;
}

GridbrainComponent* Grid::getRandomComponent()
{
    if (mComponentSet)
    {
        return mComponentSet->getRandom();
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

void Grid::addRowOrColumn()
{
    if (mDistRowsAndColumns->iuniform(0, 2) == 0)
    {
        // If height is 0, also create new row
        if (mHeight == 0)
        {
            mHeight = 1;
            mNewRow = 0;
            mRowCodes.push_back(CURRENT_ROW_CODE++);
        }

        mWidth++;
        mSize = mWidth * mHeight;
        mNewColumn = mDistRowsAndColumns->iuniform(0, mWidth);
        mColumnsConnectionsCountVec.push_back(0);

        vector<unsigned long>::iterator iterCode = mColumnCodes.begin();
        for (unsigned int i = 0; i < mNewColumn; i++)
        {
            iterCode++;
        }
        mColumnCodes.insert(iterCode, CURRENT_COLUMN_CODE++);
    }
    else
    {
        // If width is 0, also create new column
        if (mWidth == 0)
        {
            mWidth = 1;
            mNewColumn = 0;
            mColumnsConnectionsCountVec.push_back(0);
            mColumnCodes.push_back(CURRENT_COLUMN_CODE++);
        }

        mHeight++;
        mSize = mWidth * mHeight;
        mNewRow = mDistRowsAndColumns->iuniform(0, mHeight);

        vector<unsigned long>::iterator iterCode = mRowCodes.begin();
        for (unsigned int i = 0; i < mNewRow; i++)
        {
            iterCode++;
        }
        mRowCodes.insert(iterCode, CURRENT_ROW_CODE++);
    }
}

int Grid::getColumnByCode(unsigned long code)
{
    for (unsigned int i = 0; i < mWidth; i++)
    {
        if (mColumnCodes[i] == code)
        {
            return i;
        }
    }

    return -1;
}

int Grid::getRowByCode(unsigned long code)
{
    for (unsigned int i = 0; i < mHeight; i++)
    {
        if (mRowCodes[i] == code)
        {
            return i;
        }
    }

    return -1;
}

const char Grid::mClassName[] = "Grid";

Orbit<Grid>::MethodType Grid::mMethods[] = {
    {"setComponentSet", &Grid::setComponentSet},
    {"init", &Grid::init},
    {0,0}
};

Orbit<Grid>::NumberGlobalType Grid::mNumberGlobals[] = {
    {"ALPHA", ALPHA},
    {"BETA", BETA},
    {0,0}
};

int Grid::setComponentSet(lua_State* luaState)
{
    GridbrainComponentSet* set = (GridbrainComponentSet*)Orbit<Grid>::pointer(luaState, 1);
    setComponentSet(set);
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

