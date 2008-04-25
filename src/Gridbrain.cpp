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

llULINT Gridbrain::CURRENT_MEM_ID = 1;

mt_distribution* Gridbrain::mDistConnections = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistMutationsProb = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistWeights = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistComponents = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistRecombine = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistGridbrain = gDistManager.getNewDistribution();

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
    mMutateSplitConnectionProb = 0.0f;
    mMutateJoinConnectionsProb = 0.0f;
    mMutateChangeComponentProb = 0.0f;
    mMutateChangeInactiveComponentProb = 0.0f;
    mMutateSwapComponentProb = 0.0f;

    mConnSeqProb = 1.0f;
    mConnSeqCurrent = NULL;
    mCompSeqProb = 1.0f;
    mCompSeqPos = -1;

    mActiveComponents = 0;
    mActivePerceptions = 0;
    mActiveActions = 0;
    mActiveConnections = 0;
    mAllActive = false;

    mGrowMethod = GM_PRESSURE;
    mCloneConnectionsMode = CC_ALL_PLUS;
    mMutationScope = MS_ALL;

    mGeneGrouping = false;
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
    gb->mActiveComponents = 0;
    gb->mActivePerceptions = 0;
    gb->mActiveActions = 0;
    gb->mActiveConnections = 0;
    gb->mAllActive = mAllActive;

    gb->mMutateAddConnectionProb = mMutateAddConnectionProb;
    gb->mMutateRemoveConnectionProb = mMutateRemoveConnectionProb;
    gb->mMutateChangeConnectionWeightProb = mMutateChangeConnectionWeightProb;
    gb->mWeightMutationStanDev = mWeightMutationStanDev;
    gb->mMutateSplitConnectionProb = mMutateSplitConnectionProb;
    gb->mMutateJoinConnectionsProb = mMutateJoinConnectionsProb;
    gb->mMutateChangeComponentProb = mMutateChangeComponentProb;
    gb->mMutateChangeInactiveComponentProb = mMutateChangeInactiveComponentProb;
    gb->mMutateSwapComponentProb = mMutateSwapComponentProb;

    for (map<string, int>::iterator iterChannel = mChannels.begin();
            iterChannel != mChannels.end();
            iterChannel++)
    {
        gb->mChannels[(*iterChannel).first] = (*iterChannel).second;
    }

    gb->mGrowMethod = mGrowMethod;
    gb->mCloneConnectionsMode = mCloneConnectionsMode;
    gb->mMutationScope = mMutationScope;

    gb->mOwner = mOwner;

    gb->mGeneGrouping = mGeneGrouping;

    return gb;
}

Brain* Gridbrain::clone()
{
    return clone(true, ET_NONE, 0);
}

Gridbrain* Gridbrain::clone(bool grow, ExpansionType expansion, unsigned int targetGrid, unsigned int pos)
{
    Gridbrain* gb = baseClone();

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* oldGrid = mGridsVec[g];
        Grid* newGrid = new Grid(*oldGrid);
    
        if (grow)
        {
            unsigned int col = 0;

            for (unsigned int x = 0;
                x < oldGrid->getWidth();
                x++)
            {
                bool deleteCol = true;

                for (unsigned int y = 0;
                    y < oldGrid->getHeight();
                    y++)
                {
                    GridbrainComponent* comp = getComponent(x, y, g);
                    if (comp->mActive)
                    {
                        deleteCol = false;
                    }
                }

                if (deleteCol)
                {
                    newGrid->deleteColumn(col);
                }
                else
                {
                    col++;
                }
            }

            unsigned int row = 0;

            for (unsigned int y = 0;
                y < oldGrid->getHeight();
                y++)
            {

                bool deleteRow = true;

                for (unsigned int x = 0;
                    x < oldGrid->getWidth();
                    x++)
                {
                    GridbrainComponent* comp = getComponent(x, y, g);
                    if (comp->mActive)
                    {
                        deleteRow = false;
                    }
                }

                if (deleteRow)
                {
                    newGrid->deleteRow(row);
                }
                else
                {
                    row++;
                }
            }

            if (mGrowMethod == GM_FREE)
            {
                newGrid->addColumn();
                newGrid->addRow();
            }
            else if (mGrowMethod == GM_PRESSURE)
            {
                if (oldGrid->mMaxDepth == newGrid->getWidth())
                {
                    newGrid->addColumn();
                }
                if (oldGrid->mMaxActiveCol == newGrid->getHeight())
                {
                    newGrid->addRow();
                }
            }
        }
        else if (targetGrid == g)
        {
            switch (expansion)
            {
            case ET_COLUMN_FIRST:
                newGrid->addColumn(Grid::CP_FIRST);
                break;
            case ET_COLUMN_LAST:
                newGrid->addColumn(Grid::CP_LAST);
                break;
            case ET_COLUMN_RANDOM:
                newGrid->addColumn(Grid::CP_RANDOM);
                break;
            case ET_COLUMN_BEFORE:
                newGrid->addColumn(Grid::CP_BEFORE, pos);
                break;
            case ET_COLUMN_AFTER:
                newGrid->addColumn(Grid::CP_AFTER, pos);
                break;
            case ET_ROW:
                newGrid->addRow();
                break;
            }
        }

        gb->addGrid(newGrid, "");
    }

    gb->mComponents = (GridbrainComponent*)malloc(gb->mNumberOfComponents * sizeof(GridbrainComponent));

    unsigned int offset = 0;

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* oldGrid = mGridsVec[g];
        Grid* newGrid = gb->mGridsVec[g];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            GridCoord xCoord = newGrid->getColumnCoord(x);
            int oldX = oldGrid->getColumnByCoord(xCoord);

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                GridCoord yCoord = newGrid->getRowCoord(y);
                int oldY = oldGrid->getRowByCoord(yCoord);

                GridbrainComponent* oldComponent = NULL;
                if ((oldX >= 0) && (oldY >= 0))
                {
                    oldComponent = getComponent(oldX, oldY, g);
                }

                GridbrainComponent* newComponent = gb->getComponent(x, y, g);

                newComponent->clearDefinitions();
                newComponent->clearConnections();
                newComponent->clearMetrics();
                
                if (oldComponent != NULL)
                {
                    newComponent->copyDefinitions(oldComponent);
                }

                newComponent->mOffset = offset;
                newComponent->mColumn = x;
                newComponent->mRow = y;
                newComponent->mGrid = g;

                offset++;
            }
        }
    }

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* newGrid = gb->mGridsVec[g];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                GridbrainComponent* comp = gb->getComponent(x, y, g);
                if (comp->mType == GridbrainComponent::NUL)
                {
                    GridbrainComponent* newComp = newGrid->getRandomComponent(mOwner, gb->mComponents);
                    comp->copyDefinitions(newComp);
                }
            }
        }
    }

    gb->calcConnectionCounts();

    unsigned int lostConnections = 0;

    GridbrainConnection* conn = mConnections;
    while (conn != NULL)
    {
        if ((mCloneConnectionsMode == CC_ALL)
            || (mCloneConnectionsMode == CC_ALL_PLUS)
            || (conn->mActive))
        {
            int x1 = conn->mColumnOrig;
            int y1 = conn->mRowOrig;
            int g1 = conn->mGridOrig;
            int x2 = conn->mColumnTarg;
            int y2 = conn->mRowTarg;
            int g2 = conn->mGridTarg;

            Grid* oldGridOrig = mGridsVec[g1];
            Grid* oldGridTarg = mGridsVec[g2];
            Grid* newGridOrig = gb->mGridsVec[g1];
            Grid* newGridTarg = gb->mGridsVec[g2];

            GridCoord x1Coord = oldGridOrig->getColumnCoord(x1);
            GridCoord y1Coord = oldGridOrig->getRowCoord(y1);
            GridCoord x2Coord = oldGridTarg->getColumnCoord(x2);
            GridCoord y2Coord = oldGridTarg->getRowCoord(y2);

            x1 = newGridOrig->getColumnByCoord(x1Coord);
            y1 = newGridOrig->getRowByCoord(y1Coord);
            x2 = newGridTarg->getColumnByCoord(x2Coord);
            y2 = newGridTarg->getRowByCoord(y2Coord);

            if ((x1 >= 0)
                && (y1 >= 0)
                && (x2 >= 0)
                && (y2 >= 0))
            {
                gb->addConnection(x1,
                    y1,
                    g1,
                    x2,
                    y2,
                    g2,
                    conn->mWeight,
                    conn->mGeneTag);
            }
            else
            {
                lostConnections++;
            }
        }
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    if (mCloneConnectionsMode == CC_ALL_PLUS)
    {
        gb->addRandomConnections(lostConnections);
    }

    // Clone memory
    for (map<llULINT, GridbrainMemCell>::iterator iterCell = mMemory.begin();
            iterCell != mMemory.end();
            iterCell++)
    {
        gb->mMemory[(*iterCell).first] = (*iterCell).second;
    }

    for (unsigned int i = 0; i < gb->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &(gb->mComponents[i]);

        if (comp->isMemory())
        {
            gb->mMemory[comp->mOrigSymID].mUsed = true;
        }
    }

    map<llULINT, GridbrainMemCell>::iterator iterCell = gb->mMemory.begin();
    while(iterCell != gb->mMemory.end())
    {
        map<llULINT, GridbrainMemCell>::iterator iterNext = iterCell;   
        iterNext++;
        if (!(*iterCell).second.mUsed)
        {
            gb->mMemory.erase(iterCell);
        }
        iterCell = iterNext;
    }

    gb->mMemory[CURRENT_MEM_ID++] = GridbrainMemCell();

    gb->update();

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

        // Init grids with random components
        pos = 0;

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

    update(); 
}

void Gridbrain::update()
{
    calcActive();
    calcDensityMetrics();
    calcConnectionCounts();
    initGridsIO();
    initGridWritePositions();
    linkMemory();
}

void Gridbrain::setComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber,
                GridbrainComponent::Type type,
                int subType,
                InterfaceItem::TableLinkType linkType,
                int origSymTable,
                llULINT origSymID,
                int targetSymTable,
                llULINT targetSymID)
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
    clearInterfaces();

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        grid->removeInputOutput();
    
        if (grid->getType() == Grid::ALPHA)
        {
            list<InterfaceItem*>* interface;

            interface = new list<InterfaceItem*>();
            mInputInterfacesVector.push_back(interface);

            for (unsigned int j = 0;
                j < grid->mComponentSequenceSize;
                j++)
            {
                GridbrainComponent* comp = grid->mComponentSequence[j];
                if (comp->mType == GridbrainComponent::PER)
                {
                    int perPos = grid->addPerception(comp);
                    comp->mPerceptionPosition = perPos;

                    if (perPos >= interface->size())
                    {
                        InterfaceItem* item = new InterfaceItem();
                        item->mType = comp->mSubType;
                        item->mOrigSymTable = comp->mOrigSymTable;
                        item->mTargetSymTable = comp->mTargetSymTable;
                        item->mOrigSymID = comp->mOrigSymID;
                        item->mTargetSymID = comp->mTargetSymID;
                        item->mTableLinkType = comp->mTableLinkType;
                        interface->push_back(item);
                    }
                }
            }

            grid->initInputMatrix(mMaxInputDepth);
        }
        else
        {
            for (unsigned int j = 0;
                j < grid->mComponentSequenceSize;
                j++)
            {
                GridbrainComponent* comp = grid->mComponentSequence[j];

                if (comp->mType == GridbrainComponent::ACT)
                {
                    comp->mActionPosition = grid->addAction(comp);
                    InterfaceItem* item = new InterfaceItem();
                    item->mType = comp->mSubType;
                    mOutputInterface.push_back(item);
                }
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
        throw std::runtime_error("Trying to get component from invalid grid column");
    }
    if (y >= grid->getHeight())
    {
        throw std::runtime_error("Trying to get component from invalid grid row");
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
                GridbrainGeneTag tag)
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
    conn->mWeight = weight;
    applyWeight(conn);
    conn->mOrigComponent = comp;
    conn->mTargComponent = targComp;
    conn->mGeneTag = tag;

    if (conn->mGridOrig == conn->mGridTarg)
    {
        conn->mInterGrid = false;
    }
    else
    {
        conn->mInterGrid = true;
    }

    // Insert in component's connection list, ordered by relative offset
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

    // Insert in the beginning of the target's incoming connections list
    GridbrainConnection* nextConn = targComp->mFirstInConnection;
    conn->mNextInConnection = nextConn;
    conn->mPrevInConnection = NULL;
    targComp->mFirstInConnection = conn;

    if (nextConn != NULL)
    {
        nextConn->mPrevInConnection = conn;
    }

    (targComp->mInboundConnections)++;

    // Insert in the beginning of the global connections list
    nextConn = mConnections;
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
                float realWeight,
                GridbrainGeneTag tag)
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

    addConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg, weight, tag);
}

void Gridbrain::removeConnection(GridbrainConnection* conn)
{
    GridbrainComponent* comp = (GridbrainComponent*)conn->mOrigComponent;
    GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;

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

    targComp->mInboundConnections--;

    if (conn->mPrevInConnection)
    {
        ((GridbrainConnection*)conn->mPrevInConnection)->mNextInConnection = conn->mNextInConnection;
    }
    else
    {
        targComp->mFirstInConnection = (GridbrainConnection*)conn->mNextInConnection;
    }
    if (conn->mNextInConnection)
    {
        ((GridbrainConnection*)conn->mNextInConnection)->mPrevInConnection = conn->mPrevInConnection;
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

    GridbrainComponent* comp = &(mComponents[orig]);
    GridbrainConnection* conn = comp->mFirstConnection;

    unsigned int i = 0;
    while ((conn) && (i < comp->mConnectionsCount))
    {
        if ((conn->mColumnTarg == xTarg)
            && (conn->mRowTarg == yTarg)
            && (conn->mGridTarg == gTarg))
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

bool Gridbrain::selectRandomOrigin(unsigned int &x1,
                unsigned int &y1,
                unsigned int &g1,
                unsigned int x2,
                unsigned int y2,
                unsigned int g2)
{
    unsigned int origins = 0;
    GridbrainComponent* targetComp = getComponent(x2, y2, g2);

    Grid* gridTarg = getGrid(g2);

    if (gridTarg->getType() == Grid::BETA)
    {
        for (unsigned int g = 0; g < (mGridsCount - 1); g++)
        {
            Grid* grid = getGrid(g);

            for (unsigned int x = 0; x < grid->getWidth(); x++)
            {
                for (unsigned int y = 0; y < grid->getHeight(); y++)
                {
                    GridbrainComponent* comp = getComponent(x, y, g);

                    if (comp->getConnectorType() != GridbrainComponent::CONN_IN)
                    {
                        origins++;
                    }
                }
            }
        }
    }

    for (unsigned int x = 0; x < x2; x++)
    {
        for (unsigned int y = 0; y < gridTarg->getHeight(); y++)
        {
            GridbrainComponent* comp = getComponent(x, y, g2);

            if (comp->getConnectorType() != GridbrainComponent::CONN_IN)
            {
                origins++;
            }
        }
    }

    unsigned int possibleOrigins = origins - targetComp->mInboundConnections;

    if (possibleOrigins == 0)
    {
        return false;
    }

    unsigned int origin = mDistConnections->iuniform(0, possibleOrigins);
    unsigned int curPos = 0;

    if (gridTarg->getType() == Grid::BETA)
    {
        for (g1 = 0; g1 < (mGridsCount - 1); g1++)
        {
            Grid* grid = getGrid(g1);

            for (x1 = 0; x1 < grid->getWidth(); x1++)
            {
                for (y1 = 0; y1 < grid->getHeight(); y1++)
                {
                    GridbrainComponent* comp = getComponent(x1, y1, g1);

                    if ((comp->getConnectorType() != GridbrainComponent::CONN_IN)
                        && (!connectionExists(x1, y1, g1, x2, y2, g2)))
                    {
                        curPos++;
                    }
                    if (curPos > origin)
                    {
                        return true;
                    }
                }
            }
        }
    }

    g1 = g2;
    for (x1 = 0; x1 < x2; x1++)
    {
        for (y1 = 0; y1 < gridTarg->getHeight(); y1++)
        {
            GridbrainComponent* comp = getComponent(x1, y1, g1);

            if ((comp->getConnectorType() != GridbrainComponent::CONN_IN)
                && (!connectionExists(x1, y1, g1, x2, y2, g2)))
            {
                curPos++;
            }
            if (curPos > origin)
            {
                return true;
            }
        }
    }

    return false;
}

bool Gridbrain::selectRandomTarget(unsigned int x1,
                unsigned int y1,
                unsigned int g1,
                unsigned int &x2,
                unsigned int &y2,
                unsigned int &g2)
{
    unsigned int targets = 0;
    GridbrainComponent* origComp = getComponent(x1, y1, g1);

    Grid* gridOrig = getGrid(g1);

    if (gridOrig->getType() == Grid::ALPHA)
    {
        unsigned int g = mGridsCount - 1;
        {
            Grid* grid = getGrid(g);

            for (unsigned int x = 0; x < grid->getWidth(); x++)
            {
                targets += grid->getColTargCount(x);
            }
        }
    }

    for (unsigned int x = x1 + 1; x < gridOrig->getWidth(); x++)
    {
        targets += gridOrig->getColTargCount(x);
    }

    unsigned int possibleTargets = targets - origComp->mConnectionsCount;

    if (possibleTargets == 0)
    {
        return false;
    }

    unsigned int target = mDistConnections->iuniform(0, possibleTargets);
    unsigned int curPos = 0;

    if (gridOrig->getType() == Grid::ALPHA)
    {
        g2 = mGridsCount - 1;
        Grid* grid = getGrid(g2);

        for (x2 = 0; x2 < grid->getWidth(); x2++)
        {
            for (y2 = 0; y2 < grid->getHeight(); y2++)
            {
                GridbrainComponent* comp = getComponent(x2, y2, g2);

                if ((comp->getConnectorType() != GridbrainComponent::CONN_OUT)
                    && (!connectionExists(x1, y1, g1, x2, y2, g2)))
                {
                    curPos++;
                }
                if (curPos > target)
                {
                    return true;
                }
            }
        }
    }

    g2 = g1;
    for (x2 = x1 + 1; x2 < gridOrig->getWidth(); x2++)
    {
        for (y2 = 0; y2 < gridOrig->getHeight(); y2++)
        {
            GridbrainComponent* comp = getComponent(x2, y2, g2);

            if ((comp->getConnectorType() != GridbrainComponent::CONN_OUT)
                && (!connectionExists(x1, y1, g1, x2, y2, g2)))
            {
                curPos++;
            }
            if (curPos > target)
            {
                return true;
            }
        }
    }

    return false;
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
    for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
    {
        //printf("----> GRID: %d\n", gridNumber);
        Grid* grid = mGridsVec[gridNumber];

        unsigned int endIndex = grid->mComponentSequenceSize;

        for (unsigned int i = 0; i < endIndex; i++)
        {
            comp = grid->mComponentSequence[i];

            comp->mInput = 0;
            comp->mOutput = 0;
            comp->mState = 0;
            comp->mCycleFlag = false;
            comp->mForwardFlag = false;
        }
    }

    // Evaluate grids
    for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
    {
        //printf("----> GRID: %d\n", gridNumber);
        Grid* grid = mGridsVec[gridNumber];

        unsigned int endIndex = grid->mComponentSequenceSize;

        unsigned int inputDepth = 1;
        int passCount = 1;
        unsigned int perceptionsCount = 0;
        if (grid->getType() == Grid::ALPHA)
        {
            passCount = 2;
            perceptionsCount = grid->getPerceptionsCount();
            inputDepth = grid->getInputDepth();
        }


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
                    for (unsigned int i = 0; i < endIndex; i++)
                    {
                        comp = grid->mComponentSequence[i];

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

                for (unsigned int j = 0; j < endIndex; j++)
                {
                    comp = grid->mComponentSequence[j];
                    conn = comp->mFirstConnection;

                    // compute component output
                    float output;
                    float input;

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
                        else
                        {
                            output = 0.0f;
                        }
                        break;
                    case GridbrainComponent::STHR:
                        //printf("STHR ");
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
                    case GridbrainComponent::INV:
                        //printf("INV ");
                        output = comp->mInput;
                        if (output > 1.0f)
                        {
                            output = 1.0f;
                        }
                        else if (output < -1.0f)
                        {
                            output = -1.0f;
                        }

                        if (output >= 0)
                        {
                            output = 1.0f - output;
                        }
                        else
                        {
                            output = -1.0f - output;
                        }

                        break;
                    case GridbrainComponent::RAND:
                        //printf("RAND ");
                        output = mDistGridbrain->uniform(-1.0f, 1.0f);

                        break;
                    case GridbrainComponent::CLK:
                        //printf("CLK ");

                        input = comp->mInput;
                        if (input < 0.0f)
                        {
                            input = -input;
                        }
                        if (input > 1.0f)
                        {
                            input = 1.0f;
                        }

                        if (input != comp->mLastInput)
                        {
                            comp->mLastInput = input;

                            if (input > 0.0f)
                            {
                                comp->mTriggerInterval = (llULINT)(powf(5.0f, input * 10.0f)); 
                                comp->mTimeToTrigger = 0;
                            }
                        }

                        output = 0.0f;

                        if (comp->mTriggerInterval > 0)
                        {
                            if (comp->mTimeToTrigger == 0)
                            {
                                output = 1.0f;
                                comp->mTimeToTrigger = comp->mTriggerInterval;
                            }
                            else
                            {
                                comp->mTimeToTrigger--;
                            }
                        }

                        break;
                    case GridbrainComponent::MEMA:
                        //printf("MEMA ");
                        input = comp->mInput;
                        if (input > 1.0f)
                        {
                            input = 1.0f;
                        }
                        else if (input < -1.0f)
                        {
                            input = -1.0f;
                        }

                        comp->mMemCell->mAdd += input;

                        output = comp->mMemCell->mValue;

                        break;
                    case GridbrainComponent::MEMC:
                        //printf("MEMC ");
                        if (comp->mInput != 0.0f)
                        {
                            comp->mMemCell->mClear = true;
                        }

                        output = comp->mMemCell->mValue;

                        break;
                    case GridbrainComponent::MEMT:
                        //printf("MEMT ");
                        input = comp->mInput;
                        if (input > 1.0f)
                        {
                            input = 1.0f;
                        }
                        else if (input < -1.0f)
                        {
                            input = -1.0f;
                        }

                        comp->mMemCell->mToggle += input;

                        output = comp->mMemCell->mValue;

                        break;
                    case GridbrainComponent::MEMW:
                        //printf("MEMW ");
                        input = comp->mInput;
                        if (input > 1.0f)
                        {
                            input = 1.0f;
                        }
                        else if (input < -1.0f)
                        {
                            input = -1.0f;
                        }

                        comp->mMemCell->mWrite += input;

                        output = comp->mMemCell->mValue;

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
                            GridbrainComponent* targetComp = (GridbrainComponent*)conn->mTargComponent;
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

    // Process memory
    for (map<llULINT, GridbrainMemCell>::iterator iterCell = mMemory.begin();
            iterCell != mMemory.end();
            iterCell++)
    {
        (*iterCell).second.cycle();
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

void Gridbrain::calcActive()
{
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, g);
                comp->mActive = false;
                comp->mProducer = false;
                comp->mConsumer = false;
            }
        }
    }

    mActiveComponents = 0;
    mActivePerceptions = 0;
    mActiveActions = 0;
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];
        unsigned int sequenceSize = 0;

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, g);
                if (mAllActive)
                {
                    comp->mActive = true;
                }
                else
                {
                    comp->calcActive();
                }
                if (comp->mActive)
                {
                    if (comp->mType == GridbrainComponent::PER)
                    {
                        mActivePerceptions++;
                    }
                    else if (comp->mType == GridbrainComponent::ACT)
                    {
                        mActiveActions++;
                    }
                    mActiveComponents++;
                    sequenceSize++;
                }
            }
        }

        if (grid->mComponentSequence != NULL)
        {
            free(grid->mComponentSequence);
            grid->mComponentSequence = NULL;
        }
        grid->mComponentSequence = (GridbrainComponent**)malloc(sequenceSize * sizeof(GridbrainComponent*));
        grid->mComponentSequenceSize = sequenceSize;

        unsigned int pos = 0;
        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, g);
                if (comp->mActive)
                {
                    grid->mComponentSequence[pos] = comp;
                    pos++;
                }
            }
        }
    }

    GridbrainConnection* conn = mConnections;

    mActiveConnections = 0;
    while (conn != NULL)
    {
        GridbrainComponent* origComp = (GridbrainComponent*)conn->mOrigComponent;
        GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;

        if (origComp->mActive && targComp->mActive)
        {
            mActiveConnections++;
            conn->mActive = true;
        }
        else
        {
            conn->mActive = false;
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
}

void Gridbrain::calcDensityMetrics()
{
    // calc component depths
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* origComp = getComponent(x, y, g);

                GridbrainConnection* conn = origComp->mFirstConnection;

                while (conn != NULL)
                {
                    GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;

                    if (origComp->mActive && targComp->mActive)
                    {
                        if (origComp->mDepth == 0)
                        {
                            origComp->mDepth = 1;
                        }

                        unsigned int newDepth = 1;

                        if (conn->mGridOrig == conn->mGridTarg)
                        {
                            newDepth = origComp->mDepth + 1;
                        }

                        if (targComp->mDepth < newDepth)
                        {
                            targComp->mDepth = newDepth;
                        }
                    }

                    conn = (GridbrainConnection*)conn->mNextConnection;
                }
            }
        }
    }

    // Calc max depth & max active column per grid
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];

        unsigned int maxDepth = 0;
        unsigned int maxActiveCol = 0;

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            unsigned int rowActive = 0;

            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, g);

                if (comp->mDepth > maxDepth)
                {
                    maxDepth = comp->mDepth;
                }

                if (comp->mActive)
                {
                    rowActive++;
                }
            }

            if (rowActive > maxActiveCol)
            {
                maxActiveCol = rowActive;
            }
        }

        //printf("g: %d; maxDepth: %d; maxActiveCol: %d\n", g, maxDepth, maxActiveCol);

        grid->mMaxDepth = maxDepth;
        grid->mMaxActiveCol = maxActiveCol;
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
    else if (fieldName == "gb_active_connections")
    {
        value = mActiveConnections;
        return true;
    }
    else if (fieldName == "gb_active_perceptions")
    {
        value = mActivePerceptions;
        return true;
    }
    else if (fieldName == "gb_active_actions")
    {
        value = mActiveActions;
        return true;
    }
    else if (fieldName.substr(0,  28) == "gb_active_perceptions_table_")
    {
        string strTableCode = fieldName.substr(28, fieldName.size() - 28);
        int tableCode;
        sscanf(strTableCode.c_str(), "%d", &tableCode);

        value = 0;
        for (unsigned int i = 0; i < mNumberOfComponents; i++)
        {
            if (mComponents[i].mActive
                && (mComponents[i].mType == GridbrainComponent::PER)
                && (mComponents[i].mOrigSymTable == tableCode))
            {
                value++;
            }
        }

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
                printConnection(conn);
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

        GridbrainComponent* compOrig = getComponent(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig);
        GridbrainComponent* compTarg = getComponent(conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);

        if (conn->mOrigComponent != compOrig)
        {
            return false;
        }
        if (conn->mTargComponent != compTarg)
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

bool Gridbrain::symbolUsed(int tableID, llULINT symbolID)
{

    printf("xixi %d\n", symbolID);

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

bool Gridbrain::swapComponents(GridbrainComponent* comp1, GridbrainComponent* comp2)
{
    unsigned int x1 = comp1->mColumn;
    unsigned int y1 = comp1->mRow;
    unsigned int x2 = comp2->mColumn;
    unsigned int y2 = comp2->mRow;
    unsigned int gridNumber = comp1->mGrid;

    // Check if swap is possible
    bool valid = true;
    GridbrainConnection* conn = mConnections;
    while (valid && (conn != NULL))
    {
        if (!conn->mInterGrid)
        {
            unsigned int connX1 = conn->mColumnOrig;
            unsigned int connY1 = conn->mRowOrig;
            unsigned int connX2 = conn->mColumnTarg;
            unsigned int connY2 = conn->mRowTarg;
            unsigned int connG1 = conn->mGridOrig;
            unsigned int connG2 = conn->mGridTarg;

            if ((connX1 == x1) 
                && (connY1 == y1)
                && (connG1 == gridNumber))
            {
                connX1 = x2;
                connY1 = y2;
            }
            else if ((connX1 == x2)
                && (connY1 == y2)
                && (connG1 == gridNumber))
            {
                connX1 = x1;
                connY1 = y1;
            }
            if ((connX2 == x1)
                && (connY2 == y1)
                && (connG2 == gridNumber))
            {
                connX2 = x2;
                connY2 = y2;
            }
            else if ((connX2 == x2)
                && (connY2 == y2)
                && (connG2 == gridNumber))
            {
                connX2 = x1;
                connY2 = y1;
            }

            if (connX1 >= connX2)
            {
                valid = false;
            }
        }
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    if (!valid)
    {
        return false;
    }

    // do swap

    GridbrainComponent auxComp;
    auxComp.copyDefinitions(comp1);
    comp1->copyDefinitions(comp2);
    comp2->copyDefinitions(&auxComp);
    conn = mConnections;

    list<GridbrainConnection*> addInTheEnd;

    while (conn != NULL)
    {
        unsigned int newX1 = conn->mColumnOrig;
        unsigned int newY1 = conn->mRowOrig;
        unsigned int newG1 = conn->mGridOrig;
        unsigned int newX2 = conn->mColumnTarg;
        unsigned int newY2 = conn->mRowTarg;
        unsigned int newG2 = conn->mGridTarg;
        float newWeight = conn->mWeight;
        bool change = false;
        GridbrainConnection* curConn = conn;

        if ((conn->mColumnOrig == x1) 
            && (conn->mRowOrig == y1)
            && (conn->mGridOrig == gridNumber))
        {
            newX1 = x2;
            newY1 = y2;
            change = true;
        }
        else if ((conn->mColumnOrig == x2)
            && (conn->mRowOrig == y2)
            && (conn->mGridOrig == gridNumber))
        {
            newX1 = x1;
            newY1 = y1;
            change = true;
        }
        if ((conn->mColumnTarg == x1)
            && (conn->mRowTarg == y1)
            && (conn->mGridTarg == gridNumber))
        {
            newX2 = x2;
            newY2 = y2;
            change = true;
        }
        else if ((conn->mColumnTarg == x2)
            && (conn->mRowTarg == y2)
            && (conn->mGridTarg == gridNumber))
        {
            newX2 = x1;
            newY2 = y1;
            change = true;
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;

        if (change)
        {
            GridbrainGeneTag tag = curConn->mGeneTag;
            removeConnection(curConn);

            if (connectionExists(newX1, newY1, newG1, newX2, newY2, newG2))
            {
                GridbrainConnection* addConn = new GridbrainConnection();
                addConn->mColumnOrig = newX1;
                addConn->mRowOrig = newY1;
                addConn->mGridOrig = newG1;
                addConn->mColumnTarg = newX2;
                addConn->mRowTarg = newY2;
                addConn->mGridTarg = newG2;
                addConn->mWeight = newWeight;
                addConn->mGeneTag = tag;
                addInTheEnd.push_back(addConn);
            }
            else
            {
                addConnection(newX1, newY1, newG1, newX2, newY2, newG2, newWeight, tag);
            }
        }
    }

    list<GridbrainConnection*>::iterator iterConn;

    for (iterConn = addInTheEnd.begin();
            iterConn != addInTheEnd.end();
            iterConn++)
    {
        GridbrainConnection* addConn = (*iterConn);
                    
        addConnection(addConn->mColumnOrig,
                        addConn->mRowOrig,
                        addConn->mGridOrig,
                        addConn->mColumnTarg,
                        addConn->mRowTarg,
                        addConn->mGridTarg,
                        addConn->mWeight,
                        addConn->mGeneTag);

        delete addConn;
    }

    addInTheEnd.clear();

    return true;
}

void Gridbrain::linkMemory()
{
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &mComponents[i];

        if (comp->isMemory())
        {
            if (comp->mOrigSymID == 0)
            {
                unsigned int pos = mDistComponents->iuniform(0, mMemory.size());           
                map<llULINT, GridbrainMemCell>::iterator iterCell = mMemory.begin();
                for (unsigned int j = 0; j < pos; j++)
                {
                    iterCell++;
                }
                
                comp->mOrigSymID = (*iterCell).first;
            }

            if (comp->mMemCell == NULL)
            {
                comp->mMemCell = &mMemory[comp->mOrigSymID];
            }
        }
    }
}

const char Gridbrain::mClassName[] = "Gridbrain";

Orbit<Gridbrain>::MethodType Gridbrain::mMethods[] = {
    {"init", &Gridbrain::init},
    {"setComponent", &Gridbrain::setComponent},
    {"addGrid", &Gridbrain::addGrid},
    {"addConnection", &Gridbrain::addConnection},
    {"addConnectionReal", &Gridbrain::addConnectionReal},
    {"addRandomConnections", &Gridbrain::addRandomConnections},
    {"setGrowMethod", &Gridbrain::setGrowMethod},
    {"setCloneConnectionsMode", &Gridbrain::setCloneConnectionsMode},
    {"setMutationScope", &Gridbrain::setMutationScope},
    {"setMutateAddConnectionProb", &Gridbrain::setMutateAddConnectionProb},
    {"setMutateRemoveConnectionProb", &Gridbrain::setMutateRemoveConnectionProb},
    {"setMutateChangeConnectionWeightProb", &Gridbrain::setMutateChangeConnectionWeightProb},
    {"setWeightMutationStanDev", &Gridbrain::setWeightMutationStanDev},
    {"setMutateSplitConnectionProb", &Gridbrain::setMutateSplitConnectionProb},
    {"setMutateJoinConnectionsProb", &Gridbrain::setMutateJoinConnectionsProb},
    {"setMutateChangeComponentProb", &Gridbrain::setMutateChangeComponentProb},
    {"setMutateChangeInactiveComponentProb", &Gridbrain::setMutateChangeInactiveComponentProb},
    {"setMutateSwapComponentProb", &Gridbrain::setMutateSwapComponentProb},
    {"setGeneGrouping", &Gridbrain::setGeneGrouping},
    {0,0}
};

Orbit<Gridbrain>::NumberGlobalType Gridbrain::mNumberGlobals[] = {
    {"GM_FREE", GM_FREE},
    {"GM_PRESSURE", GM_PRESSURE},
    {"CC_ALL", CC_ALL},
    {"CC_ALL_PLUS", CC_ALL_PLUS},
    {"CC_ACTIVE", CC_ACTIVE},
    {"MS_ALL", MS_ALL},
    {"MS_ACTIVE", MS_ACTIVE},
    {0,0}};

int Gridbrain::init(lua_State* luaState)
{
        init();
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

int Gridbrain::setGrowMethod(lua_State* luaState)
{
    unsigned int val = luaL_checkint(luaState, 1);
    setGrowMethod((GrowMethod)val);
    return 0;
}

int Gridbrain::setCloneConnectionsMode(lua_State* luaState)
{
    unsigned int val = luaL_checkint(luaState, 1);
    setCloneConnectionsMode((CloneConnectionsMode)val);
    return 0;
}

int Gridbrain::setMutationScope(lua_State* luaState)
{
    unsigned int val = luaL_checkint(luaState, 1);
    setMutationScope((MutationScope)val);
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

int Gridbrain::setMutateChangeInactiveComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setMutateChangeInactiveComponentProb(prob);
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

int Gridbrain::setGeneGrouping(lua_State* luaState)
{
    bool val = luaL_checkbool(luaState, 1);
    setGeneGrouping(val);
    return 0;
}

