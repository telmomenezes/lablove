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

#include "Gridbrain.h"

#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <float.h>

mt_distribution* Gridbrain::mDistConnections = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistMutationsProb = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistWeights = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistComponents = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistRecombine = gDistManager.getNewDistribution();

Gridbrain::Gridbrain(lua_State* luaState)
{
    mMaxInputDepth = 10;
    mComponents = NULL;
    mNumberOfComponents = 0;
    mConnections = NULL;
    mConnectionsCount = 0;
    mFirstBetaIndex = 0;
    mTotalPossibleConnections = 0;
    mBetaComponentsCount = 0;
    mGridsCount = 0;

    mMutateAddConnectionProb = 0.0f;
    mMutateRemoveConnectionProb = 0.0f;
    mMutateChangeConnectionWeightProb = 0.0f;
    mWeightMutationStanDev = 0.0f;
    mMutateNewConnectionWeightProb = 0.0f;
    mMutateMoveConnectionOriginProb = 0.0f;
    mMutateSplitConnectionProb = 0.0f;
    mMutateJoinConnectionsProb = 0.0f;
    mMutateChangeComponentProb = 0.0f;
    mMutateSwapComponentProb = 0.0f;

    mMinimumFreeComponentRatio = 0.0f;

    mConnSeqProb = 1.0f;
    mConnSeqCurrent = NULL;
    mCompSeqProb = 1.0f;
    mCompSeqPos = -1;
}

Gridbrain::~Gridbrain()
{
    while (mConnections != NULL)
    {
        GridbrainConnection* conn = mConnections;
        mConnections = (GridbrainConnection*)conn->mNextGlobalConnection;
        free(conn);
    }
    if (mComponents != NULL)
    {
        free(mComponents);
        mComponents = NULL;
    }
    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        if (!mGridsVec[i]->mCreatedByScript)
        {
            delete mGridsVec[i];
        }
    }
    mGridsCount = 0;
}

Gridbrain* Gridbrain::baseClone()
{
    Gridbrain* gb = new Gridbrain();

    gb->mMaxInputDepth = mMaxInputDepth;
    gb->mNumberOfComponents = 0;
    gb->mConnections = NULL;
    gb->mConnectionsCount = 0;
    gb->mGridsCount = 0;
    gb->mFirstBetaIndex = 0;
    gb->mTotalPossibleConnections = 0;
    gb->mBetaComponentsCount = 0;

    gb->mMutateAddConnectionProb = mMutateAddConnectionProb;
    gb->mMutateRemoveConnectionProb = mMutateRemoveConnectionProb;
    gb->mMutateChangeConnectionWeightProb = mMutateChangeConnectionWeightProb;
    gb->mWeightMutationStanDev = mWeightMutationStanDev;
    gb->mMutateNewConnectionWeightProb = mMutateNewConnectionWeightProb;
    gb->mMutateMoveConnectionOriginProb = mMutateMoveConnectionOriginProb;
    gb->mMutateSplitConnectionProb = mMutateSplitConnectionProb;
    gb->mMutateJoinConnectionsProb = mMutateJoinConnectionsProb;
    gb->mMutateChangeComponentProb = mMutateChangeComponentProb;
    gb->mMutateSwapComponentProb = mMutateSwapComponentProb;

    gb->mMinimumFreeComponentRatio = mMinimumFreeComponentRatio;

    for (map<string, int>::iterator iterChannel = mChannels.begin();
            iterChannel != mChannels.end();
            iterChannel++)
    {
        gb->mChannels[(*iterChannel).first] = (*iterChannel).second;
    }

    return gb;
}

Brain* Gridbrain::clone(bool randomize)
{
    Gridbrain* gb = baseClone();

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];
        Grid* newGrid = new Grid(*grid);

        if (grid->mAddRowOrColumn)
        {
            newGrid->addRowOrColumn();
        }

        gb->addGrid(newGrid, "");
    }

    gb->mComponents = (GridbrainComponent*)malloc(gb->mNumberOfComponents * sizeof(GridbrainComponent));

    unsigned int newIndex = 0;
    unsigned int oldIndex = 0;

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];
        Grid* newGrid = gb->mGridsVec[i];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                gb->mComponents[newIndex].clearDefinitions();
                gb->mComponents[newIndex].clearConnections();
                gb->mComponents[newIndex].clearMetrics();
                
                if ((x == newGrid->mNewColumn)
                    || (y == newGrid->mNewRow))
                {
                    GridbrainComponent* comp = grid->getRandomComponent(mOwner, mComponents);
                    gb->mComponents[newIndex].copyDefinitions(comp);
                }
                else
                {
                    if (randomize)
                    {
                        GridbrainComponent* comp = grid->getRandomComponent(mOwner, gb->mComponents);
                        gb->mComponents[newIndex].copyDefinitions(comp);
                    }
                    else
                    {
                        gb->mComponents[newIndex].copyDefinitions(&mComponents[oldIndex]);
                    }

                    oldIndex++;
                }
                gb->mComponents[newIndex].mOffset = newIndex;
                gb->mComponents[newIndex].mColumn = x;
                gb->mComponents[newIndex].mRow = y;
                gb->mComponents[newIndex].mGrid = i;
                newIndex++;
            }
        }
    }

    gb->calcConnectionCounts();

    GridbrainConnection* conn = mConnections;
    while (conn != NULL)
    {
        if (randomize)
        {
            gb->addRandomConnections(1);
        }
        else
        {
            int rowOrig = conn->mRowOrig;
            int rowTarg = conn->mRowTarg;
            int columnOrig = conn->mColumnOrig;
            int columnTarg = conn->mColumnTarg;

            Grid* gridOrig = gb->mGridsVec[conn->mGridOrig];
            Grid* gridTarg = gb->mGridsVec[conn->mGridTarg];

            if ((gridOrig->mNewColumn >= 0)
                && (columnOrig >= gridOrig->mNewColumn))
            {
                columnOrig++;
            }
            if ((gridTarg->mNewColumn >= 0)
                && (columnTarg >= gridTarg->mNewColumn))
            {
                columnTarg++;
            }
            if ((gridOrig->mNewRow >= 0)
                && (rowOrig >= gridOrig->mNewRow))
            {
                rowOrig++;
            }
            if ((gridTarg->mNewRow >= 0)
                && (rowTarg >= gridTarg->mNewRow))
            {
                rowTarg++;
            }

            gb->addConnection(columnOrig,
                rowOrig,
                conn->mGridOrig,
                columnTarg,
                rowTarg,
                conn->mGridTarg,
                conn->mWeight,
                conn->mAge + 1.0f);
        }
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    return gb;
}

void Gridbrain::addGrid(Grid* grid, string name)
{
    int number = mGridsVec.size();
    grid->setOffset(mNumberOfComponents);
    unsigned int gridComponentCount = grid->getHeight() * grid->getWidth();
    mNumberOfComponents += gridComponentCount;

    if (grid->getType() == Grid::BETA)
    {
        mFirstBetaIndex = mNumberOfComponents;
        mBetaComponentsCount = gridComponentCount;
    }

    mGridsVec.push_back(grid);

    mGridsCount++;

    mChannels[name] = number;
}

Grid* Gridbrain::getGrid(string name)
{
    return mGridsVec[mChannels[name]];
}

void Gridbrain::init()
{

    if (mComponents == NULL)
    {
        initEmpty();

        // Init grids with random components
        unsigned int pos = 0;

        for (unsigned int i = 0; i < mGridsCount; i++)
        {
            Grid* grid = mGridsVec[i];

            for (unsigned int x = 0;
                x < grid->getWidth();
                x++)
            {
                for (unsigned int y = 0;
                    y < grid->getHeight();
                    y++)
                {
                    GridbrainComponent* comp = grid->getRandomComponent(mOwner, mComponents);
                    mComponents[pos].copyDefinitions(comp);

                    pos++;
                }
            }
        }
    }

    calcConnectionCounts();
}

void Gridbrain::onAdd()
{
    initGridsIO();
    initGridWritePositions();
    calcConnectionDensities();
}

void Gridbrain::initEmpty()
{
    mComponents = (GridbrainComponent*)malloc(mNumberOfComponents * sizeof(GridbrainComponent));

    // Init grids with NUL components
    unsigned int pos = 0;

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        for (unsigned int x = 0;
            x < grid->getWidth();
            x++)
        {
            for (unsigned int y = 0;
                y < grid->getHeight();
                y++)
            {
                mComponents[pos].clearConnections();
                mComponents[pos].clearDefinitions();
                mComponents[pos].clearMetrics();

                mComponents[pos].mOffset = pos;
                mComponents[pos].mColumn = x;
                mComponents[pos].mRow = y;
                mComponents[pos].mGrid = i;

                pos++;
            }
        }
    }

    calcConnectionCounts();
}

void Gridbrain::setComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber,
                GridbrainComponent::Type type,
                int subType,
                InterfaceItem::TableLinkType linkType,
                int origSymTable,
                unsigned long origSymID,
                int targetSymTable,
                unsigned long targetSymID)
{
    GridbrainComponent* comp = getComponent(x, y, gridNumber);

    comp->mType = type;
    comp->mSubType = subType;
    comp->mOrigSymTable = origSymTable;
    comp->mTargetSymTable = targetSymTable;
    comp->mOrigSymID = origSymID;
    comp->mTargetSymID = targetSymID;
    comp->mTableLinkType = linkType;
}

void Gridbrain::initGridsIO()
{
    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        grid->removeInputOutput();
        unsigned int pos = grid->getOffset();

    
        if (grid->getType() == Grid::ALPHA)
        {
            list<InterfaceItem*>* interface;

            interface = new list<InterfaceItem*>();
            mInputInterfacesVector.push_back(interface);

            int maxPerPos = -1;

            for (unsigned int j = 0;
                j < grid->getSize();
                j++)
            {
                if (mComponents[pos].mType == GridbrainComponent::PER)
                {
                    int perPos = grid->addPerception(&mComponents[pos]);
                    mComponents[pos].mPerceptionPosition = perPos;
                    if (perPos > maxPerPos)
                    {
                        maxPerPos = perPos;
                        InterfaceItem* item = new InterfaceItem();
                        item->mType = mComponents[pos].mSubType;
                        item->mOrigSymTable = mComponents[pos].mOrigSymTable;
                        item->mTargetSymTable = mComponents[pos].mTargetSymTable;
                        item->mOrigSymID = mComponents[pos].mOrigSymID;
                        item->mTargetSymID = mComponents[pos].mTargetSymID;
                        item->mTableLinkType = mComponents[pos].mTableLinkType;
                        interface->push_back(item);
                    }
                }

                pos++;
            }

            grid->initInputMatrix(mMaxInputDepth);
        }
        else
        {
            for (unsigned int j = 0;
                j < grid->getSize();
                j++)
            {
                if (mComponents[pos].mType == GridbrainComponent::ACT)
                {
                    mComponents[pos].mActionPosition = grid->addAction(&mComponents[pos]);
                    InterfaceItem* item = new InterfaceItem();
                    item->mType = mComponents[pos].mSubType;
                    mOutputInterface.push_back(item);
                }

                pos++;
            }

            grid->initOutputVector();
        }
    }
}

GridbrainComponent* Gridbrain::getComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber)
{
    if (gridNumber >= mGridsCount)
    {
        throw std::runtime_error("Trying to get component from inexistent grid");
    }

    Grid* grid = mGridsVec[gridNumber];

    if (x >= grid->getWidth())
    {
        throw std::runtime_error("Trying to get component from invalid grid position");
    }
    if (y >= grid->getHeight())
    {
        throw std::runtime_error("Trying to get component from invalid grid position");
    }

    unsigned int pos = (x * grid->getHeight()) + y + grid->getOffset();
    return &(mComponents[pos]);
}

void Gridbrain::addConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                float weight,
                double age)
{
    /*printf("add connection: %d,%d,%d -> %d,%d,%d [%f]\n",
            xOrig,
            yOrig,
            gOrig,
            xTarg,
            yTarg,
            gTarg,
            weight);*/

    Grid* gridOrig = mGridsVec[gOrig];
    Grid* gridTarg = mGridsVec[gTarg];

    if ((xOrig >= gridOrig->getWidth())
        || (xTarg >= gridTarg->getWidth())
        || (yOrig >= gridOrig->getHeight())
        || (yTarg >= gridTarg->getHeight())
        || (weight < -1.0f)
        || (weight > 1.0f))
    {
        char buffer[500];
        sprintf(buffer,
                    "Parameter(s) out of range in gridbrain add connection: %d,%d,%d -> %d,%d,%d [%f]",
                    xOrig,
                    yOrig,
                    gOrig,
                    xTarg,
                    yTarg,
                    gTarg,
                    weight);
        throw std::runtime_error(buffer);
    }

    if ((gOrig == gTarg) && (xOrig >= xTarg))
    {
        char buffer[500];
        sprintf(buffer,
                    "Only feedforward connections allowed in the gridbrain. Invalid add connection: %d,%d,%d -> %d,%d,%d [%f]",
                    xOrig,
                    yOrig,
                    gOrig,
                    xTarg,
                    yTarg,
                    gTarg,
                    weight);
        throw std::runtime_error(buffer);
    }

    if (connectionExists(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return;
    }
    if (!isConnectionValid(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return;
    }

    unsigned int orig = (xOrig * gridOrig->getHeight()) + yOrig + gridOrig->getOffset();
    unsigned int target = (xTarg * gridTarg->getHeight()) + yTarg + gridTarg->getOffset();

    GridbrainComponent* comp = &(mComponents[orig]);
    GridbrainComponent* targComp = &(mComponents[target]);
    GridbrainConnection* conn = (GridbrainConnection*)malloc(sizeof(GridbrainConnection));
    conn->mColumnOrig = comp->mColumn;
    conn->mRowOrig = comp->mRow;
    conn->mGridOrig = comp->mGrid;
    conn->mColumnTarg = targComp->mColumn;
    conn->mRowTarg = targComp->mRow;
    conn->mGridTarg = targComp->mGrid;
    conn->mTarget = target;
    conn->mWeight = weight;
    conn->mAge = age;
    applyWeight(conn);
    conn->mOrigComponent = comp;

    if (conn->mGridOrig == conn->mGridTarg)
    {
        conn->mInterGrid = false;
    }
    else
    {
        conn->mInterGrid = true;
    }

    // Insert in component list, ordered by relative offset
    if (comp->mConnectionsCount == 0)
    {
        comp->mFirstConnection = conn;
        conn->mPrevConnection = NULL;
        conn->mNextConnection = NULL;
    }
    else
    {
        unsigned int offset = getRelativeOffset(comp, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);

        GridbrainConnection* iterConn = comp->mFirstConnection;
        bool inserted = false;
        while ((!inserted) && (iterConn != NULL))
        {
            unsigned int iterOffset = getRelativeOffset(comp, iterConn->mColumnTarg, iterConn->mRowTarg, iterConn->mGridTarg);

            if (iterOffset > offset)
            {
                conn->mNextConnection = iterConn;
                conn->mPrevConnection = iterConn->mPrevConnection;
                iterConn->mPrevConnection = conn;

                if (conn->mPrevConnection == NULL)
                {
                    comp->mFirstConnection = conn;
                }
                else
                {
                    ((GridbrainConnection*)(conn->mPrevConnection))->mNextConnection = conn;
                }

                inserted = true;
            }
            else if (iterConn->mNextConnection == NULL)
            {
                iterConn->mNextConnection = conn;
                conn->mPrevConnection = iterConn;
                conn->mNextConnection = NULL;

                inserted = true;
            }

            iterConn = (GridbrainConnection*)(iterConn->mNextConnection);
        }
    }

    (comp->mConnectionsCount)++;

    GridbrainConnection* nextConn = mConnections;
    conn->mNextGlobalConnection = nextConn;
    conn->mPrevGlobalConnection = NULL;
    mConnections = conn;


    if (nextConn != NULL)
    {
        nextConn->mPrevGlobalConnection = conn;
    }

    mConnectionsCount++;
}

void Gridbrain::addConnectionReal(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                float realWeight)
{
    float weight;

    if (realWeight < 0.0f)
    {
        weight = (1.0f / (1.0f - realWeight)) - 1.0f;
    }
    else
    {
        weight = -(1.0f / (realWeight + 1.0f)) + 1.0f;
    }

    addConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg, weight);
}

void Gridbrain::removeConnection(GridbrainConnection* conn)
{
    GridbrainComponent* comp = (GridbrainComponent*)conn->mOrigComponent;
    comp->mConnectionsCount--;

    if (conn->mPrevConnection)
    {
        ((GridbrainConnection*)conn->mPrevConnection)->mNextConnection = conn->mNextConnection;
    }
    else
    {
        comp->mFirstConnection = (GridbrainConnection*)conn->mNextConnection;
    }
    if (conn->mNextConnection)
    {
        ((GridbrainConnection*)conn->mNextConnection)->mPrevConnection = conn->mPrevConnection;
    }

    mConnectionsCount--;
    if (conn->mPrevGlobalConnection)
    {
        ((GridbrainConnection*)conn->mPrevGlobalConnection)->mNextGlobalConnection =
            conn->mNextGlobalConnection;
    }
    else
    {
        mConnections = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
    if (conn->mNextGlobalConnection)
    {
        ((GridbrainConnection*)conn->mNextGlobalConnection)->mPrevGlobalConnection =
            conn->mPrevGlobalConnection;
    }

    free(conn);
}

void Gridbrain::removeConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    GridbrainConnection* conn = getConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg);
    if (conn != NULL)
    {
        removeConnection(conn);
    }
}

GridbrainConnection* Gridbrain::getConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    Grid* grid = mGridsVec[gOrig];
    unsigned int orig = (xOrig * grid->getHeight()) + yOrig + grid->getOffset();
    grid = mGridsVec[gTarg];
    unsigned int target = (xTarg * grid->getHeight()) + yTarg + grid->getOffset();

    GridbrainComponent* comp = &(mComponents[orig]);
    GridbrainConnection* conn = comp->mFirstConnection;

    unsigned int i = 0;
    while ((conn) && (i < comp->mConnectionsCount))
    {
        if (conn->mTarget == target)
        {
            return conn;
        }

        conn = (GridbrainConnection*)conn->mNextConnection;
        i++;
    }

    return NULL;
}

bool Gridbrain::connectionExists(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    GridbrainConnection* conn = getConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg);
    return conn != NULL;
}

bool Gridbrain::selectRandomConnection(unsigned int &x1,
                    unsigned int &y1,
                    unsigned int &g1,
                    unsigned int &x2,
                    unsigned int &y2,
                    unsigned int &g2)
{
    unsigned int possibleConnections = mTotalPossibleConnections - mConnectionsCount;
    //printf("mTotalPossibleConnections: %d; mConnectionsCount: %d; possibleConnections: %d\n", mTotalPossibleConnections, mConnectionsCount, possibleConnections);

    // No free connection exists, return with false
    if (possibleConnections == 0)
    {
        return false;
    }

    // Randomly select a free connection index
    unsigned int connPos = mDistConnections->iuniform(0, possibleConnections);

    // Find the origin component of the selected connection
    unsigned int curPos = 0;
    unsigned int colConnCount = 0;
    unsigned int targPos = 0;
    Grid* gridOrig = NULL;
    bool found = false;

    for (unsigned g = 0;
        (g < mGridsCount) && (!found);
        g++)
    {
        gridOrig = mGridsVec[g];
        unsigned int width = gridOrig->getWidth();
        unsigned int height = gridOrig->getHeight();
        
        for (unsigned int col = 0;
            (col < width) && (!found);
            col++)
        {
            for (unsigned int row = 0;
                (row < height) && (!found);
                row++)
            {
                GridbrainComponent* comp = getComponent(col, row, g);
                // Increment current position by the number of free connections in the curent component
                curPos += comp->mPossibleConnections - comp->mConnectionsCount;

                if (curPos > connPos)
                {
                    g1 = g;
                    x1 = col;
                    y1 = row;
                    found = true;
                    targPos = curPos - connPos - 1;
                }
            }
        }
    }

    // This should never happen
    if (!found)
    {
        throw std::runtime_error("Failed internal assertion: Could not find selected origin component in Gridbrain::selectRandomComponent()");
    }

    // Determine relative offset (to origin component) of target
    GridbrainComponent* origComp = getComponent(x1, y1, g1);
    GridbrainConnection* conn = origComp->mFirstConnection;

    while (conn != NULL)
    {
        unsigned int targX = conn->mColumnTarg;
        unsigned int targY = conn->mRowTarg;
        unsigned int targG = conn->mGridTarg;

        unsigned int offset = getRelativeOffset(origComp, targX, targY, targG);

        // This works because connections lists are ordered by relative offsets in components
        if (targPos >= offset)
        {
            targPos++;
        }

        conn = (GridbrainConnection*)(conn->mNextConnection);
    }

    // Get target component coordinates from relative offset
    GridbrainComponent* targComp = getCompByRelativeOffset(origComp, targPos);
    x2 = targComp->mColumn;
    y2 = targComp->mRow;
    g2 = targComp->mGrid;

    //printf("selected connection: %d, %d, %d -> %d, %d, %d\n", x1, y1, g1, x2, y2, g2);

    return true;
}

unsigned int Gridbrain::getRelativeOffset(GridbrainComponent* compOrig,
                                            unsigned int targX,
                                            unsigned int targY,
                                            unsigned int targG)
{
    unsigned int offset = 0;
    
    unsigned int x1 = compOrig->mColumn;
    unsigned int y1 = compOrig->mRow;
    unsigned int g1 = compOrig->mGrid;

    Grid* gridOrig = getGrid(g1);

    if (targG != g1)
    {
        Grid* gridTarg = getGrid(targG);

        for (unsigned int col = x1 + 1; col < gridOrig->getWidth(); col++)
        {
            offset += gridOrig->getColTargCount(col);
        }
        for (unsigned int col = 0; col < targX; col++)
        {
            offset += gridTarg->getColTargCount(col);
        }
    }
    else
    {
        for (unsigned int col = x1 + 1; col < targX; col++)
        {
            offset += gridOrig->getColTargCount(col);
        }
    }

    for (unsigned int row = 0; row < targY; row++)
    {
        GridbrainComponent* comp = getComponent(targX, row, targG);
        if (comp->getConnectorType() != GridbrainComponent::CONN_OUT)
        {
            offset++;
        }
    }

    return offset;
}

GridbrainComponent* Gridbrain::getCompByRelativeOffset(GridbrainComponent* compOrig, unsigned int offset)
{
    unsigned int x = compOrig->mColumn;
    unsigned int y = compOrig->mRow;
    unsigned int g = compOrig->mGrid;

    Grid* grid = getGrid(g);

    bool colFound = false;
    unsigned int col = x + 1;
    unsigned int curOffset = 0;

    while ((!colFound) && (col < grid->getWidth()))
    {
        curOffset += grid->getColTargCount(col);

        if (curOffset > offset)
        {
            colFound = true;
        }
        else
        {
            col++;
        }
    }

    if ((!colFound) && (grid->getType() == Grid::ALPHA))
    {
        // Assumes one BETA grid at the end
        g = mGridsCount - 1;
        grid = getGrid(g);
        col = 0;
        while ((!colFound) && (col < grid->getWidth()))
        {
            curOffset += grid->getColTargCount(col);

            if (curOffset > offset)
            {
                colFound = true;
            }
            else
            {
                col++;
            }
        }
    }

    if (!colFound)
    {
        return NULL;
    }

    unsigned int rowOffset = offset - (curOffset - grid->getColTargCount(col));
    unsigned int row = 0;
    curOffset = 0;

    while (row < grid->getHeight())
    {
        GridbrainComponent* comp = getComponent(col, row, g);
        if (comp->getConnectorType() != GridbrainComponent::CONN_OUT)
        {
            curOffset++;
        }
        if (curOffset > rowOffset)
        {
            return comp;
        }
        row++;
    }

    return NULL;
}

void Gridbrain::addRandomConnections(unsigned int count)
{
    // If all grids has 0 size, no connection can be created
    if (mTotalPossibleConnections == 0)
    {
        return;
    }

    unsigned int x1;
    unsigned int x2;
    unsigned int y1;
    unsigned int y2;
    unsigned int g1;
    unsigned int g2;
    float weight;

    for (unsigned int i = 0; i < count; i++)
    {
        if (selectRandomConnection(x1, y1, g1, x2, y2, g2))
        {
            weight = mDistWeights->uniform(-1.0f, 1.0f);
            addConnection(x1, y1, g1, x2, y2, g2, weight);
        }
    }
}

void Gridbrain::cycle()
{
    //printf("======= BEGIN CYCLE ======\n");
    GridbrainComponent* comp;
    GridbrainConnection* conn;
    GridbrainComponent::InputType inputType;

    // Reset all components
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        comp = &(mComponents[i]);

        comp->mInput = 0;
        comp->mOutput = 0;
        comp->mState = 0;
        comp->mCycleFlag = false;
        comp->mForwardFlag = false;
    }

    // Evaluate grids
    for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
    {
        //printf("----> GRID: %d\n", gridNumber);
        Grid* grid = mGridsVec[gridNumber];

        unsigned int inputDepth = 1;
        int passCount = 1;
        unsigned int perceptionsCount = 0;
        if (grid->getType() == Grid::ALPHA)
        {
            passCount = 2;
            perceptionsCount = grid->getPerceptionsCount();
            inputDepth = grid->getInputDepth();
        }

        unsigned int startIndex;
        unsigned int endIndex;

        startIndex = grid->getOffset();
        endIndex = startIndex + grid->getSize();

        float* inputMatrix = grid->getInputMatrix();
        float* outputVector = grid->getOutputVector();

        for (unsigned int pass = 0; pass < passCount; pass++)
        {
            //printf("----> PASS: %d\n", pass);
            bool firstAlpha = false;
            if (pass < (passCount - 1))
            {
                firstAlpha = true;
            }

            unsigned int inputDepthOffset = 0;

            for (unsigned int i = 0; i < inputDepth; i++)
            {
                if (grid->getType() == Grid::ALPHA)
                {
                    // reset alpha components except aggregators
                    for (unsigned int i = startIndex; i < endIndex; i++)
                    {
                        comp = &(mComponents[i]);

                        if (!comp->isAggregator())
                        {
                            if (comp->mType == GridbrainComponent::PER)
                            {
                                // apply perceptions for this input depth
                                comp->mState =
                                    inputMatrix[comp->mPerceptionPosition
                                            + inputDepthOffset];
                            }
                            else
                            {
                                comp->mState = 0;
                            }
                        }
                        comp->mInput = 0;
                        comp->mForwardFlag = false;
                    }
                }

                for (unsigned int j = startIndex; j < endIndex; j++)
                {
                    comp = &(mComponents[j]);
                    conn = comp->mFirstConnection;

                    // compute component output
                    float output;
                    switch(comp->mType)
                    {
                    case GridbrainComponent::NUL:
                        //printf("NUL ");
                        output = 0.0f;
                        break;
                    case GridbrainComponent::PER:
                        //printf("PER ");
                        output = comp->mState;
                        break;
                    case GridbrainComponent::ACT:
                        //printf("ACT ");
                        // TODO: return 1 if the action was executed?
                        output = comp->mInput;
                        if (output > 1.0f)
                        {
                            output = 1.0f;
                        }
                        else if (output < -1.0f)
                        {
                            output = -1.0f;
                        }
                        outputVector[comp->mActionPosition] = output;

                        break;
                    case GridbrainComponent::THR:
                        //printf("THR ");
                        if (comp->mInput > GB_THRESHOLD)
                        {
                            output = 1.0f;
                        }
                        else if (comp->mInput < -GB_THRESHOLD)
                        {
                            output = -1.0f;
                        }
                        else
                        {
                            output = 0.0f;
                        }
                        break;
                    case GridbrainComponent::MAX:
                    case GridbrainComponent::MMAX:
                        //printf("MAX/MMAX ");
                        if ((comp->mType != GridbrainComponent::MMAX) || (comp->mInput != 0))
                        {
                            if (!comp->mCycleFlag)
                            {
                                output = 1.0f;
                                comp->mState = comp->mInput;
                                comp->mCycleFlag = true;
                            }
                            else
                            {
                                if (comp->mInput >= comp->mState)
                                {
                                    output = 1.0f;
                                    comp->mState = comp->mInput;
                                    if (pass > 0)
                                    {
                                        comp->mState += 1.0f;
                                    }
                                }
                                else
                                {
                                    output = 0.0f;
                                }
                            }
                        }
                        else
                        {
                            output = 0.0f;
                        }
                        break;
                    case GridbrainComponent::MUL:
                        //printf("MUL ");
                        output = comp->mInput;
                        break;
                    case GridbrainComponent::AND:
                    case GridbrainComponent::TAND:
                        //printf("AND/TAND ");
                        if (comp->mInput > 0.0f)
                        {
                            output = 1.0f;
                        }
                        else if (comp->mInput < 0.0f)
                        {
                            output = -1.0f;
                        }
                        else
                        {
                            output = 0.0f;
                        }
                        break;
                    case GridbrainComponent::NOT:
                    case GridbrainComponent::TNAND:
                        //printf("NOT/TNAND ");
                        if (comp->mInput == 0.0f)
                        {
                            output = 1.0f;
                        }
                        else
                        {
                            output = 0.0f;
                        }
                        break;
                    default:
                        // TODO: this is an error. how to inform?
                        output = 0.0f;
                        break;
                    }
                            

                    if (output > 1.0f)
                    {
                        output = 1.0f;
                    }
                    else if (output < -1.0f)
                    {
                        output = -1.0f;
                    }
                    comp->mOutput = output;

                    //printf("%f => %f\n", comp->mInput, output);

                    // propagate outputs (inside grid if fist alpha)
                    for (unsigned int k = 0; k < comp->mConnectionsCount; k++)
                    {
                        if ((!firstAlpha) || (!conn->mInterGrid))
                        {
                            GridbrainComponent* targetComp = &(mComponents[conn->mTarget]);
                            float input = output * conn->mRealWeight;

                            COMPONENT_INPUT_TYPE(targetComp->mType, inputType)

                            switch(inputType)
                            {
                            case GridbrainComponent::IN_MUL:
                                if (!targetComp->mForwardFlag)
                                {
                                    targetComp->mInput = input;
                                    targetComp->mForwardFlag = true;
                                }
                                else
                                {
                                    targetComp->mInput *= input;
                                }
                                break;
                            case GridbrainComponent::IN_TMUL:
                                if ((input < GB_THRESHOLD)
                                    && (input > -GB_THRESHOLD))
                                {
                                    input = 0.0f;
                                }
                                if (!targetComp->mForwardFlag)
                                {
                                   targetComp->mInput = input;
                                   targetComp->mForwardFlag = true;
                                }
                                else
                                {
                                    targetComp->mInput *= input;
                                }
                                break;
                            case GridbrainComponent::IN_SUM:
                                targetComp->mInput += input;
                                break;
                            }

                            /*printf("(%d, %d, %d)[%f] -> (%d, %d, %d)[%f]\n",
                                conn->mColumnOrig,
                                conn->mRowOrig,
                                conn->mGridOrig,
                                output,
                                conn->mColumnTarg,
                                conn->mRowTarg,
                                conn->mGridTarg,
                                input);*/
                        }
                        conn = (GridbrainConnection*)conn->mNextConnection;
                    }
                }

                inputDepthOffset += perceptionsCount;
            }
        }
    }

    // Reset input depths
    for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
    {
        mGridsVec[gridNumber]->setInputDepth(0);
    }
}

Grid* Gridbrain::getGrid(unsigned int number)
{
    if (mGridsCount <= number)
    {
        return NULL;
    }

    return mGridsVec[number];
}

void Gridbrain::calcConnectionCounts()
{
    // Determine target count for each column and beta layer
    unsigned int betaTargets = 0;
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];
        unsigned int width = grid->getWidth();
        unsigned int height = grid->getHeight();

        for (unsigned int col = 0; col < width; col++)
        {
            unsigned int targetCount = 0;

            for (unsigned int row = 0; row < height; row++)
            {
                GridbrainComponent* comp = getComponent(col, row, g);

                if (comp->getConnectorType() != GridbrainComponent::CONN_OUT)
                {
                    targetCount++;
                }
            } 
            grid->setColTargCount(col, targetCount);

            if (grid->getType() == Grid::BETA)
            {
                betaTargets += targetCount;
            }
        }
    }

    // Determine possible connections, total and per component
    mTotalPossibleConnections = 0;

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];
        unsigned int width = grid->getWidth();
        unsigned int height = grid->getHeight();

        for (unsigned int col = 0; col < width; col++)
        {
            unsigned int possibleConnections = 0;

            for (unsigned int col2 = col + 1; col2 < width; col2++)
            {
                possibleConnections += grid->getColTargCount(col2);
            }

            if (grid->getType() == Grid::ALPHA)
            {
                possibleConnections += betaTargets;
            }

            for (unsigned int row = 0; row < height; row++)
            {
                GridbrainComponent* comp = getComponent(col, row, g);
                unsigned int compPossibleConnections = 0;

                if (comp->getConnectorType() != GridbrainComponent::CONN_IN)
                {
                    compPossibleConnections = possibleConnections;
                }
                comp->mPossibleConnections = compPossibleConnections;
                mTotalPossibleConnections += compPossibleConnections;
            }
        }
    }
}

void Gridbrain::calcConnectionDensities()
{
    GridbrainConnection* conn = mConnections;

    // Calc inbound connections for every component
    while (conn != NULL)
    {
        (getComponent(conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg)->mInboundConnections) += 1;
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        unsigned int startOffset = grid->getOffset();
        unsigned int endOffset = startOffset + grid->getSize();

        float freeComponents = 0.0f;

        for (unsigned int pos = startOffset;
            pos < endOffset;
            pos++)
        {
            unsigned int totalConns = mComponents[pos].mConnectionsCount + mComponents[pos].mInboundConnections;
            if (totalConns == 0)
            {
                freeComponents += 1.0f;
            }
        }

        float freeComponentRatio;
        float gridSize = (float)grid->getSize();

        if (gridSize == 0.0f)
        {
            freeComponentRatio = 0.0f;
        }
        else
        {
            freeComponentRatio = freeComponents / ((float)grid->getSize());
        }

        if (freeComponentRatio < mMinimumFreeComponentRatio)
        {
            grid->mAddRowOrColumn = true;
        }
        //printf("freeComponentRatio[%d] => %f (free: %f; width: %d; height: %d)\n", i, freeComponentRatio, freeComponents, grid->getWidth(), grid->getHeight());
    }
}

float* Gridbrain::getInputBuffer(unsigned int channel)
{
    return mGridsVec[channel]->getInputBuffer();
}

float* Gridbrain::getOutputBuffer()
{
    return mGridsVec[mGridsCount - 1]->getOutputVector();
}

void Gridbrain::applyWeight(GridbrainConnection* conn)
{
    if (conn->mWeight <= -1.0f)
    {
        conn->mRealWeight = -FLT_MAX;
    }
    else if (conn->mWeight >= 1.0f)
    {
        conn->mRealWeight = FLT_MAX;
    }
    else if (conn->mWeight < 0.0f)
    {
        conn->mRealWeight = -(1.0f / (1.0f + conn->mWeight)) + 1.0f;
    }
    else
    {
        conn->mRealWeight = (1.0f / (1.0f - conn->mWeight)) - 1.0f;
    }
}

bool Gridbrain::getFieldValue(string fieldName, float& value)
{
    if (fieldName == "gb_connections")
    {
        value = mConnectionsCount;
        return true;
    }
    else if (fieldName == "gb_components")
    {
        value = mNumberOfComponents;
        return true;
    }
    else if (fieldName.substr(0,  14) == "gb_grid_width_")
    {
        string gridName = fieldName.substr(14, fieldName.size() - 14);
        Grid* grid = getGrid(gridName);
        if (grid)
        {
            value = grid->getWidth();
        }
        return true;
    }
    else if (fieldName.substr(0,  15) == "gb_grid_height_")
    {
        string gridName = fieldName.substr(15, fieldName.size() - 15);
        Grid* grid = getGrid(gridName);
        if (grid)
        {
            value = grid->getHeight();
        }
        return true;
    }

    return Brain::getFieldValue(fieldName, value);
}

bool Gridbrain::isConnectionValid(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    GridbrainComponent* compOrig = getComponent(xOrig, yOrig, gOrig);
    if (compOrig->getConnectorType() == GridbrainComponent::CONN_IN)
    {
        return false;
    }
    GridbrainComponent* compTarg = getComponent(xTarg, yTarg, gTarg);
    if (compTarg->getConnectorType() == GridbrainComponent::CONN_OUT)
    {
        return false;
    }
    return true;
}

bool Gridbrain::canCreateConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    if (!isConnectionValid(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return false;
    }
    if (connectionExists(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return false;
    }
    return true;
}

void Gridbrain::cleanInvalidConnections()
{
    GridbrainConnection* nextConn = mConnections;

    while (nextConn != NULL)
    {
        GridbrainConnection* conn = nextConn;
        nextConn = (GridbrainConnection*)nextConn->mNextGlobalConnection;
        if (!isConnectionValid(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg))
        {
            removeConnection(conn);
        }
    }
}

bool Gridbrain::isValid()
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGridOrig == conn->mGridTarg)
        {
            if (conn->mColumnOrig >= conn->mColumnTarg)
            {
                return false;
            }
        }
        else
        {
            if (mGridsVec[conn->mGridOrig]->getType() != Grid::ALPHA)
            {
                return false;
            }
            if (mGridsVec[conn->mGridTarg]->getType() != Grid::BETA)
            {
                return false;
            }
        }

        if (!isConnectionValid(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg))
        {
            return false;
        }
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        if (!mGridsVec[i]->isValid())
        {
            return false;
        }
    }

    return true;
}

bool Gridbrain::symbolUsed(int tableID, unsigned long symbolID)
{
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &mComponents[i];

        if ((comp->mOrigSymTable == tableID)
            && (comp->mOrigSymID == symbolID))
        {
            return true;
        }
    }

    return false;
}

const char Gridbrain::mClassName[] = "Gridbrain";

Orbit<Gridbrain>::MethodType Gridbrain::mMethods[] = {
    {"initEmpty", &Gridbrain::initEmpty},
    {"setComponent", &Gridbrain::setComponent},
    {"addGrid", &Gridbrain::addGrid},
    {"addConnection", &Gridbrain::addConnection},
    {"addConnectionReal", &Gridbrain::addConnectionReal},
    {"addRandomConnections", &Gridbrain::addRandomConnections},
    {"setMutateAddConnectionProb", &Gridbrain::setMutateAddConnectionProb},
    {"setMutateRemoveConnectionProb", &Gridbrain::setMutateRemoveConnectionProb},
    {"setMutateChangeConnectionWeightProb", &Gridbrain::setMutateChangeConnectionWeightProb},
    {"setWeightMutationStanDev", &Gridbrain::setWeightMutationStanDev},
    {"setMutateNewConnectionWeightProb", &Gridbrain::setMutateNewConnectionWeightProb},
    {"setMutateMoveConnectionOriginProb", &Gridbrain::setMutateMoveConnectionOriginProb},
    {"setMutateSplitConnectionProb", &Gridbrain::setMutateSplitConnectionProb},
    {"setMutateJoinConnectionsProb", &Gridbrain::setMutateJoinConnectionsProb},
    {"setMutateChangeComponentProb", &Gridbrain::setMutateChangeComponentProb},
    {"setMutateSwapComponentProb", &Gridbrain::setMutateSwapComponentProb},
    {"setMinimumFreeComponentRatio", &Gridbrain::setMinimumFreeComponentRatio},
    {0,0}
};

Orbit<Gridbrain>::NumberGlobalType Gridbrain::mNumberGlobals[] = {{0,0}};

int Gridbrain::initEmpty(lua_State* luaState)
{
        initEmpty();
        return 0;
}

int Gridbrain::setComponent(lua_State* luaState)
{
    unsigned int x = luaL_checkint(luaState, 1);
    unsigned int y = luaL_checkint(luaState, 2);
    unsigned int gridNumber = luaL_checkint(luaState, 3);
    GridbrainComponent::Type type = (GridbrainComponent::Type)luaL_checkint(luaState, 4);
    unsigned int subType = luaL_optint(luaState, 5, -1);
    InterfaceItem::TableLinkType linkType = (InterfaceItem::TableLinkType)(luaL_optint(luaState, 6, InterfaceItem::NO_LINK));
    int origSymTable = luaL_optint(luaState, 7, -1);
    int origSymIndex = luaL_optint(luaState, 8, -1);
    int targetSymTable = luaL_optint(luaState, 9, -1);
    int targetSymIndex = luaL_optint(luaState, 10, -1);

    setComponent(x, y, gridNumber, type, subType, linkType, origSymTable, origSymIndex, targetSymTable, targetSymIndex);
    return 0;
}

int Gridbrain::addGrid(lua_State* luaState)
{
        Grid* grid = (Grid*)(Orbit<Gridbrain>::pointer(luaState, 1));
        string name = luaL_checkstring(luaState, 2);
        addGrid(grid, name);
        return 0;
}

int Gridbrain::addConnection(lua_State* luaState)
{
    unsigned int xOrig = luaL_checkint(luaState, 1);
    unsigned int yOrig = luaL_checkint(luaState, 2);
    unsigned int gOrig = luaL_checkint(luaState, 3);
    unsigned int xTarg = luaL_checkint(luaState, 4);
    unsigned int yTarg = luaL_checkint(luaState, 5);
    unsigned int gTarg = luaL_checkint(luaState, 6);
    float weight = luaL_checknumber(luaState, 7);

    addConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg, weight);
    return 0;
}

int Gridbrain::addConnectionReal(lua_State* luaState)
{
    unsigned int xOrig = luaL_checkint(luaState, 1);
    unsigned int yOrig = luaL_checkint(luaState, 2);
    unsigned int gOrig = luaL_checkint(luaState, 3);
    unsigned int xTarg = luaL_checkint(luaState, 4);
    unsigned int yTarg = luaL_checkint(luaState, 5);
    unsigned int gTarg = luaL_checkint(luaState, 6);
    float weight = luaL_checknumber(luaState, 7);

    addConnectionReal(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg, weight);
    return 0;
}

int Gridbrain::addRandomConnections(lua_State* luaState)
{
    unsigned int count = luaL_checkint(luaState, 1);
    addRandomConnections(count);
    return 0;
}

int Gridbrain::setMutateAddConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateAddConnectionProb(prob);
    return 0;
}

int Gridbrain::setMutateRemoveConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateRemoveConnectionProb(prob);
    return 0;
}

int Gridbrain::setMutateChangeConnectionWeightProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateChangeConnectionWeightProb(prob);
    return 0;
}

int Gridbrain::setMutateNewConnectionWeightProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateNewConnectionWeightProb(prob);
    return 0;
}

int Gridbrain::setMutateMoveConnectionOriginProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateMoveConnectionOriginProb(prob);
    return 0;
}

int Gridbrain::setMutateSplitConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateSplitConnectionProb(prob);
    return 0;
}

int Gridbrain::setMutateJoinConnectionsProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateJoinConnectionsProb(prob);
    return 0;
}

int Gridbrain::setMutateChangeComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateChangeComponentProb(prob);
    return 0;
}

int Gridbrain::setMutateSwapComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateSwapComponentProb(prob);
    return 0;
}

int Gridbrain::setWeightMutationStanDev(lua_State* luaState)
{
    float sd = luaL_checknumber(luaState, 1);
    setWeightMutationStanDev(sd);
    return 0;
}

int Gridbrain::setMinimumFreeComponentRatio(lua_State* luaState)
{
    float rate = luaL_checknumber(luaState, 1);
    setMinimumFreeComponentRatio(rate);
    return 0;
}

