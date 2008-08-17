/*
 * Gridbrain
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

namespace gb
{

mt_distribution* Grid::mDistRowsAndColumns = gDistManager.getNewDistribution();
mt_distribution* Grid::mDistXover = gDistManager.getNewDistribution();

Grid::Grid()
{
    mType = ALPHA;
    mWidth = 0;
    mHeight = 0;
    mSize = 0;
    mOffset = 0;
    mInputMatrix = NULL;
    mInputIDs = NULL;
    mOutputVector = NULL;
    mMaxInputDepth = 50;
    mInputDepth = 0;
    mPerceptionsCount = 0;
    mActionsCount = 0;
    mWriteX = 0;
    mWriteY = 0;
    mComponentSequence = NULL;
    mComponentSequenceSize = 0;

    mComponentSet = new ComponentSet();

    mMaxDepth = 0;
    mMaxActiveCol = 0;
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
    mInputIDs = NULL;
    mOutputVector = NULL;
    mWriteX = 0;
    mWriteY = 0;
    mComponentSequence = NULL;
    mComponentSequenceSize = 0;
    
    mComponentSet = new ComponentSet(grid.mComponentSet);

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnTargetCountVec.push_back(grid.mColumnTargetCountVec[i]);
        Coord gc = grid.mColumnCoords[i];
        mColumnCoords.push_back(gc);
    }
    for (unsigned int i = 0; i < mHeight; i++)
    {
        Coord gc = grid.mRowCoords[i];
        mRowCoords.push_back(gc);
    }

    mMaxDepth = 0;
    mMaxActiveCol = 0;

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
    if (mInputIDs != NULL)
    {
        free(mInputIDs);
        mInputIDs = NULL;
    }
    if (mOutputVector != NULL)
    {
        free(mOutputVector);
        mOutputVector = NULL;
    }
    if (mComponentSequence != NULL)
    {
        free(mComponentSequence);
        mComponentSequence = NULL;
    }
    if (mComponentSet != NULL)
    {
        delete mComponentSet;
        mComponentSet = NULL;
    }
}

void Grid::init(Type type, unsigned int width, unsigned int height)
{
    mType = type;
    mWidth = width;
    mHeight = height;

    for (unsigned int i = 0; i < mWidth; i++)
    {
        mColumnTargetCountVec.push_back(0);

        if (i == 0)
        {
            mColumnCoords.push_back(Coord());
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
            mRowCoords.push_back(Coord());
        }
        else
        {
            mRowCoords.push_back(mRowCoords[i - 1].rightOf());
        }
    }

    mSize = mWidth * mHeight;
}

void Grid::setComponentSet(ComponentSet* componentSet)
{
    mComponentSet = componentSet;
}

Component* Grid::getRandomComponent()
{
    return mComponentSet->getRandom();
}

void Grid::setInput(unsigned int number, unsigned int depth, float value)
{
    mInputMatrix[(depth * mPerceptionsCount) + number] = value;
}

unsigned int Grid::addPerception(Component* per)
{
    for (unsigned int i = 0; i < mPerceptionsVec.size(); i++)
    {
        Component* curPer = mPerceptionsVec[i];

        if (curPer->isEqual(per))
        {
            return i;
        }
    }

    mPerceptionsVec.push_back(per);
    return (mPerceptionsVec.size() - 1);
}

unsigned int Grid::addAction(Component* act)
{
    mActionsVec.push_back(act);
    return (mActionsVec.size() - 1);
}

void Grid::initInputMatrix(unsigned int maxInputDepth)
{
    mPerceptionsCount = mPerceptionsVec.size();
    mMaxInputDepth = maxInputDepth;
    unsigned int inputSize = mPerceptionsCount * mMaxInputDepth;

    if (inputSize > 0)
    {
        mInputMatrix = (float*)malloc(inputSize * sizeof(float));
        mInputIDs = (unsigned int*)malloc(mMaxInputDepth * sizeof(unsigned int));
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

Component* Grid::getPerception(unsigned int number)
{
    return mPerceptionsVec[number];
}

Component* Grid::getAction(unsigned int number)
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
    if (mInputIDs != NULL)
    {
        free(mInputIDs);
        mInputIDs = NULL;
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

float* Grid::getInputBuffer(unsigned int id)
{
    if (mInputDepth >= mMaxInputDepth)
    {
        return NULL;
    }
    float* buffer = mInputMatrix + (mInputDepth * mPerceptionsCount);

    if (mPerceptionsCount > 0)
    {
        mInputIDs[mInputDepth] = id;
    }
    mInputDepth++;

    return buffer;
}

unsigned int Grid::getInputID(unsigned int depth)
{
    if (mPerceptionsCount == 0)
    {
        return 0;
    }

    return mInputIDs[depth];
}

void Grid::addColumn(Coord* gc)
{
    mWidth++;
    mSize = mWidth * mHeight;
    mColumnTargetCountVec.push_back(0);

    if (gc == NULL)
    {
        Coord ngc;

        if (mWidth == 1)
        {
            ngc = Coord();
            mColumnCoords.push_back(ngc);
        }
        else
        {
            ngc = mColumnCoords[mWidth - 2].rightOf();
            mColumnCoords.push_back(ngc);
        }
    }
    else
    {   
        Coord ngc = *gc;

        vector<Coord>::iterator iterCoord = mColumnCoords.begin();
        while ((iterCoord != mColumnCoords.end())
                && ((*iterCoord).position(ngc) != 1))
        {
            iterCoord++;
        }
        mColumnCoords.insert(iterCoord, ngc);
    }
}

void Grid::addRow(Coord* gc)
{
    mHeight++;
    mSize = mWidth * mHeight;

    if (gc == NULL)
    {
        Coord ngc;

        if (mHeight == 1)
        {
            ngc = Coord();
        }
        else
        {
            ngc = mRowCoords[mHeight - 2].rightOf();
        }
        mRowCoords.push_back(ngc);
    }
    else
    {   
        Coord ngc = *gc;

        vector<Coord>::iterator iterCoord = mRowCoords.begin();
        while ((iterCoord != mRowCoords.end())
                && ((*iterCoord).position(ngc) != 1))
        {
            iterCoord++;
        }
        mRowCoords.insert(iterCoord, ngc);
    }
}

void Grid::deleteColumn(Coord col)
{
    if (mWidth == 0)
    {
        return;
    }

    vector<Coord>::iterator iterCol = mColumnCoords.begin();
    vector<unsigned int>::iterator iterColTarg = mColumnTargetCountVec.begin();
    
    while ((iterCol != mColumnCoords.end())
        && (!((*iterCol) == col)))
    {
        iterCol++;
        iterColTarg++;
    }

    mColumnCoords.erase(iterCol);
    mColumnTargetCountVec.erase(iterColTarg);

    mWidth--;
    mSize = mWidth * mHeight;
}

void Grid::deleteRow(Coord row)
{
    if (mHeight == 0)
    {
        return;
    }

    vector<Coord>::iterator iterRow = mRowCoords.begin();
    
    while ((iterRow != mRowCoords.end())
        && (!((*iterRow) == row)))
    {
        iterRow++;
    }

    mRowCoords.erase(iterRow);

    mHeight--;
    mSize = mWidth * mHeight;
}

Coord Grid::getColCoordAfter(Coord col)
{
    int x = getColumnByCoord(col);
    Coord col1 = mColumnCoords[x];

    if (x == (mWidth - 1))
    {
        return col1.rightOf();
    }

    Coord col2 = mColumnCoords[x + 1];

    if (col1.getDepth() >= col2.getDepth())
    {
        return col1.rightOf();
    }
    else
    {
        return col2.leftOf();
    }
}

int Grid::getColumnByCoord(Coord coord)
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

int Grid::getRowByCoord(Coord coord)
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

}

