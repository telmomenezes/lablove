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

mt_distribution* Grid::mDistRowsAndColumns = gDistManager.getNewDistribution();
mt_distribution* Grid::mDistXover = gDistManager.getNewDistribution();

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
        mColumnCoords.push_back(grid.mColumnCoords[i]);
    }
    for (unsigned int i = 0; i < mHeight; i++)
    {
        mRowCoords.push_back(grid.mRowCoords[i]);
    }

    /*printf("column coords: ");
    for (unsigned int i = 0; i < mWidth; i++)
    {
        printf("%s ", mColumnCoords[i].toString().c_str());
    }
    printf("\n");
    printf("row coords: ");
    for (unsigned int i = 0; i < mHeight; i++)
    {
        printf("%s ", mRowCoords[i].toString().c_str());
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

void Grid::crossover(Grid* gridA, Grid* gridB)
{
    int xoverType = mDistXover->iuniform(0, 2);

    Grid* grid1;
    Grid* grid2;
    char firstGrid;
    char secondGrid;

    firstGrid = mDistXover->iuniform(1, 3);
    if (firstGrid == 1)
    {
        grid1 = gridA;
        grid2 = gridB;
        secondGrid = 2;
    }
    else
    {
        grid1 = gridB;
        grid2 = gridA;
        secondGrid = 1;
    }

    int xoverCol = mDistXover->iuniform(0, grid1->mWidth);
    int xoverRow = mDistXover->iuniform(0, grid1->mHeight);

    //printf("xoverCol: %d; xoverRow: %d\n", xoverCol, xoverRow);

    GridCoord xoverColCoord = grid1->mColumnCoords[xoverCol];
    GridCoord xoverRowCoord = grid1->mRowCoords[xoverRow];

    mColumnCoords.clear();
    unsigned int i = 0;
    while ((grid1->mColumnCoords[i].position(xoverColCoord) == -1)
            && (i < grid1->mWidth))
    {
        GridCoord gc = grid1->mColumnCoords[i];
        gc.mXoverOrigin = 0;
        if (xoverType == 0)
        {
            gc.mXoverOrigin = firstGrid;
        }
        mColumnCoords.push_back(gc);
        i++;
    }

    i = 0;
    while ((grid2->mColumnCoords[i].position(xoverColCoord) == -1)
            && (i < grid2->mWidth))
    {
        i++;
    }
    while (i < grid2->mWidth)
    {
        GridCoord gc = grid2->mColumnCoords[i];
        gc.mXoverOrigin = 0;
        if (xoverType == 0)
        {
            gc.mXoverOrigin = secondGrid;
        }
        mColumnCoords.push_back(gc);
        i++;
    }

    firstGrid = mDistXover->iuniform(1, 3);
    if (firstGrid == 1)
    {
        grid1 = gridA;
        grid2 = gridB;
        secondGrid = 2;
    }
    else
    {
        grid1 = gridB;
        grid2 = gridA;
        secondGrid = 1;
    }

    mRowCoords.clear();
    i = 0;
    while ((grid1->mRowCoords[i].position(xoverRowCoord) == -1)
            && (i < grid1->mHeight))
    {
        GridCoord gc = grid1->mRowCoords[i];
        gc.mXoverOrigin = 0;
        if (xoverType == 1)
        {
            gc.mXoverOrigin = firstGrid;
        }
        mRowCoords.push_back(gc);
        i++;
    }

    i = 0;
    while ((grid2->mRowCoords[i].position(xoverRowCoord) == -1)
            && (i < grid2->mHeight))
    {
        i++;
    }
    while (i < grid2->mHeight)
    {
        GridCoord gc = grid2->mRowCoords[i];
        gc.mXoverOrigin = 0;
        if (xoverType == 1)
        {
            gc.mXoverOrigin = secondGrid;
        }
        mRowCoords.push_back(gc);
        i++;
    }

    mWidth = mColumnCoords.size();
    mHeight = mRowCoords.size();
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

        if (i == 0)
        {
            mColumnCoords.push_back(GridCoord());
        }
        else
        {
            mColumnCoords.push_back(mColumnCoords[i - 1].rightOf());
        }
    }

    for (unsigned int i = 0; i < mHeight; i++)
    {
        if (i == 0)
        {
            mRowCoords.push_back(GridCoord());
        }
        else
        {
            mRowCoords.push_back(mRowCoords[i - 1].rightOf());
        }
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
            mRowCoords.push_back(GridCoord());
        }

        mWidth++;
        mSize = mWidth * mHeight;
        mNewColumn = mDistRowsAndColumns->iuniform(0, mWidth);
        mColumnsConnectionsCountVec.push_back(0);

        vector<GridCoord>::iterator iterCoord = mColumnCoords.begin();
        for (unsigned int i = 0; i < mNewColumn; i++)
        {
            iterCoord++;
        }

        GridCoord gc;

        if (mWidth == 1)
        {
            gc = GridCoord();
        }
        else if (mNewColumn == 0)
        {
            gc = mColumnCoords[0].leftOf();
        }
        else if (mNewColumn == (mWidth - 1))
        {
            gc = mColumnCoords[mWidth - 1].rightOf();
        }
        else
        {
            unsigned int d1 = mColumnCoords[mNewColumn - 1].getDepth();
            unsigned int d2 = mColumnCoords[mNewColumn].getDepth();

            if (d1 > d2)
            {
                gc = mColumnCoords[mNewColumn - 1].rightOf();
            }
            else
            {
                gc = mColumnCoords[mNewColumn].leftOf();
            }
        }

        mColumnCoords.insert(iterCoord, gc);
    }
    else
    {
        // If width is 0, also create new column
        if (mWidth == 0)
        {
            mWidth = 1;
            mNewColumn = 0;
            mColumnsConnectionsCountVec.push_back(0);
            mColumnCoords.push_back(GridCoord());
        }

        mHeight++;
        mSize = mWidth * mHeight;
        mNewRow = mDistRowsAndColumns->iuniform(0, mHeight);

        vector<GridCoord>::iterator iterCoord = mRowCoords.begin();
        for (unsigned int i = 0; i < mNewRow; i++)
        {
            iterCoord++;
        }

        GridCoord gc;

        if (mHeight == 1)
        {
            gc = GridCoord();
        }
        else if (mNewRow == 0)
        {
            gc = mRowCoords[0].leftOf();
        }
        else if (mNewRow == (mHeight - 1))
        {
            gc = mRowCoords[mHeight - 1].rightOf();
        }
        else
        {
            unsigned int d1 = mRowCoords[mNewRow - 1].getDepth();
            unsigned int d2 = mRowCoords[mNewRow].getDepth();

            if (d1 > d2)
            {
                gc = mRowCoords[mNewRow - 1].rightOf();
            }
            else
            {
                gc = mRowCoords[mNewRow].leftOf();
            }
        }

        mRowCoords.insert(iterCoord, gc);
    }
}

int Grid::getColumnByCoord(GridCoord coord)
{
    for (unsigned int i = 0; i < mWidth; i++)
    {
        if (mColumnCoords[i] == coord)
        {
            return i;
        }
    }

    return -1;
}

int Grid::getRowByCoord(GridCoord coord)
{
    for (unsigned int i = 0; i < mHeight; i++)
    {
        if (mRowCoords[i] == coord)
        {
            return i;
        }
    }

    return -1;
}

bool Grid::isValid()
{
    for (unsigned int i = 0; i < mWidth; i++)
    {
        if (!mColumnCoords[i].isValid())
        {
            return false;
        }
        if (i > 0)
        {
            if (mColumnCoords[i].position(mColumnCoords[i - 1]) != 1)
            {
                return false;
            }
        }
    }
    for (unsigned int i = 0; i < mHeight; i++)
    {
        if (!mRowCoords[i].isValid())
        {
            return false;
        }
        if (i > 0)
        {
            if (mRowCoords[i].position(mRowCoords[i - 1]) != 1)
            {
                return false;
            }
        }
    }
    return true;
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

