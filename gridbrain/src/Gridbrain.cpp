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

#include "Gridbrain.h"
#include "ComponentSet.h"

#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <float.h>
#include <math.h>

namespace gb
{

mt_distribution* Gridbrain::mDistConnections = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistMutationsProb = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistComponents = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistRecombine = gDistManager.getNewDistribution();
mt_distribution* Gridbrain::mDistGridbrain = gDistManager.getNewDistribution();

Gridbrain::Gridbrain()
{
    mMaxInputDepth = 50;
    mNumberOfComponents = 0;
    mConnections = NULL;
    mConnectionsCount = 0;
    mFirstBetaIndex = 0;
    mTotalPossibleConnections = 0;
    mBetaComponentsCount = 0;
    mGridsCount = 0;

    mMutateAddConnectionProb = 0.0f;
    mMutateRemoveConnectionProb = 0.0f;
    mMutateChangeParamProb = 0.0f;
    mParamMutationStanDev = 0.0f;
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
}

Gridbrain::~Gridbrain()
{
    for (vector<Component*>::iterator iterComp = mComponents.begin();
            iterComp != mComponents.end();
            iterComp++)
    {
        delete(*iterComp);
    }
    mComponents.clear();

    while (mConnections != NULL)
    {
        Connection* conn = mConnections;
        mConnections = (Connection*)conn->mNextGlobalConnection;
        free(conn);
    }
    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        if (!mGridsVec[i]->mCreatedByScript)
        {
            delete mGridsVec[i];
        }
    }
    mGridsCount = 0;

    clearInterfaces();
}

Gridbrain* Gridbrain::clone(bool grow, ExpansionType expansion, unsigned int targetGrid, Coord* gc)
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
    gb->mMutateChangeParamProb = mMutateChangeParamProb;
    gb->mParamMutationStanDev = mParamMutationStanDev;
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

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* oldGrid = mGridsVec[g];
        Grid* newGrid = new Grid(*oldGrid);
    
        if (grow)
        {
            int leftInactive = 0;
            int rightInactive = 0;
            int jump = 9999999;

            for (unsigned int x = 0;
                x < oldGrid->getWidth();
                x++)
            {
                bool activeCol = false;
                unsigned int minConnJump = 9999999;
                Coord colCoord = oldGrid->getColumnCoord(x);

                for (unsigned int y = 0;
                    y < oldGrid->getHeight();
                    y++)
                {
                    Component* comp = getComponent(x, y, g);
                    if (comp->mActive)
                    {
                        activeCol = true;

                        Connection* conn = comp->mFirstConnection;
                        while (conn != NULL)
                        {
                            if (conn->mGridOrig == conn->mGridTarg)
                            {
                                unsigned int connJump = conn->mColumnTarg - conn->mColumnOrig;
                                if (connJump < minConnJump)
                                {
                                    minConnJump = connJump;
                                }
                            }
                            conn = (Connection*)conn->mNextConnection;
                        }
                    }
                }

                if (activeCol)
                {
                    leftInactive = -1;
                    rightInactive = 0;
                    jump = minConnJump;
                    
                    if (x == 0)
                    {
                        Coord newCol = colCoord.leftOf();
                        newGrid->addColumn(&newCol);
                    }

                    if ((x == (oldGrid->getWidth() - 1))
                        || (minConnJump == 1))
                    {
                        Coord newCol = newGrid->getColCoordAfter(colCoord);
                        newGrid->addColumn(&newCol);
                        jump++;
                    }
                }
                else
                {
                    bool deleteCol = true;

                    if ((x == (oldGrid->getWidth() - 1))
                        && (rightInactive == 0))
                    {
                        deleteCol = false;
                    }

                    if (leftInactive == 0)
                    {
                        deleteCol = false;
                    }

                    if (jump == 2)
                    {
                        deleteCol = false;
                    }

                    if (deleteCol)
                    {
                        newGrid->deleteColumn(colCoord);
                    }
                    else
                    {
                        if (leftInactive >= 0)
                        {
                            leftInactive++;
                        }
                        rightInactive++;
                        jump--;    
                    }
                }
            }

////////////////////
            int botInactive = 0;

            for (unsigned int y = 0;
                y < oldGrid->getHeight();
                y++)
            {
                bool activeRow = false;
                Coord rowCoord = oldGrid->getRowCoord(y);

                for (unsigned int x = 0;
                    x < oldGrid->getWidth();
                    x++)
                {
                    Component* comp = getComponent(x, y, g);
                    if (comp->mActive)
                    {
                        activeRow = true;
                    }
                }

                if (activeRow)
                {
                    botInactive = 0;

                    if (y == (oldGrid->getHeight() - 1))
                    {
                        newGrid->addRow();
                    }
                }
                else
                {
                    botInactive++;

                    bool deleteRow = true;

                    if ((y == (oldGrid->getHeight() - 1))
                        && (botInactive <= 1))
                    {
                        deleteRow = false;
                    }

                    if (deleteRow)
                    {
                        newGrid->deleteRow(rowCoord);
                    }
                }
            }
        }

        if (newGrid->getSize() == 0)
        {
            newGrid->addColumn();
            newGrid->addRow();
        }
        
        if (targetGrid == g)
        {
            if (expansion == ET_COLUMN)
            {
                newGrid->addColumn(gc);
            }
            else if (expansion == ET_ROW)
            {
                newGrid->addRow(gc);
            }
        }

        gb->addGrid(newGrid, "");
    }

    unsigned int offset = 0;

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* oldGrid = mGridsVec[g];
        Grid* newGrid = gb->mGridsVec[g];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            Coord xCoord = newGrid->getColumnCoord(x);
            int oldX = oldGrid->getColumnByCoord(xCoord);

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                Coord yCoord = newGrid->getRowCoord(y);
                int oldY = oldGrid->getRowByCoord(yCoord);

                Component* oldComponent = NULL;
                if ((oldX >= 0)
                    && (oldY >= 0))
                {
                    oldComponent = getComponent(oldX, oldY, g);
                }

                Component* newComponent;

                if (oldComponent != NULL)
                {
                    newComponent = oldComponent->clone();
                }
                else
                {
                    newComponent = new CompNUL();
                }

                newComponent->mOffset = offset;
                newComponent->mColumn = x;
                newComponent->mRow = y;
                newComponent->mGrid = g;

                gb->mComponents.push_back(newComponent);

                offset++;
            }
        }

        newGrid->getComponentSet()->update(gb,
                                            newGrid->getOffset(),
                                            newGrid->getOffset() + newGrid->getSize());
        //printf("FIRST UPDATE**************\n");
        //newGrid->getComponentSet()->print();
    }

    unsigned int pos = 0;
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
                Component* comp = gb->getComponent(x, y, g);
                if (comp->isNUL())
                {
                    Component* newComp = newGrid->getRandomComponent();
                    gb->replaceComponent(pos, newComp);
                    #ifdef GB_VALIDATE
                    gb->isValid();
                    #endif
                }
                pos++;
            }
        }
    }

    gb->calcConnectionCounts();

    unsigned int lostConnections = 0;

    Connection* conn = mConnections;
    while (conn != NULL)
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

        Coord x1Coord = oldGridOrig->getColumnCoord(x1);
        Coord y1Coord = oldGridOrig->getRowCoord(y1);
        Coord x2Coord = oldGridTarg->getColumnCoord(x2);
        Coord y2Coord = oldGridTarg->getRowCoord(y2);

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
                conn->mGene);
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }

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
    if (mComponents.size() == 0)
    {
        for (unsigned int i = 0; i < mNumberOfComponents; i++)
        {
            mComponents.push_back(new CompNUL());
        }

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
                    mComponents[pos]->mOffset = pos;
                    mComponents[pos]->mColumn = x;
                    mComponents[pos]->mRow = y;
                    mComponents[pos]->mGrid = i;

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

    #ifdef GB_VALIDATE
    isValid();
    #endif
}

Component* Gridbrain::setComponent(unsigned int x, unsigned int y, unsigned int g, Component& comp)
{
    Component* oldComp = getComponent(x, y, g);
    return replaceComponent(oldComp->mOffset, &comp);
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
            list<Component*>* interface;

            interface = new list<Component*>();
            mInputInterfacesVector.push_back(interface);

            for (unsigned int j = 0;
                j < grid->mComponentSequenceSize;
                j++)
            {
                Component* comp = grid->mComponentSequence[j];
                if (comp->isInput())
                {
                    int perPos = grid->addPerception(comp);
                    comp->mPerceptionPosition = perPos;

                    if (perPos >= interface->size())
                    {
                        interface->push_back(comp);
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
                Component* comp = grid->mComponentSequence[j];

                if (comp->isOutput())
                {
                    comp->mActionPosition = grid->addAction(comp);
                    mOutputInterface.push_back(comp);
                }
            }

            grid->initOutputVector();
        }
    }
}

Component* Gridbrain::getComponent(unsigned int x,
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
    return mComponents[pos];
}

Component* Gridbrain::replaceComponent(unsigned int pos, Component* comp)
{
    Component* oldComp = mComponents[pos];
    Grid* grid = mGridsVec[oldComp->mGrid];
    ComponentSet* compSet = grid->getComponentSet();

    compSet->enable(oldComp);

    Component* newComp = comp->clone();
    newComp->copyPosition(oldComp);
    newComp->copyConnections(oldComp);
    delete oldComp;

    mComponents[pos] = newComp;

    compSet->disable(comp);

    // Update connections
    Connection* conn = (Connection*)newComp->mFirstConnection;
    while (conn != NULL)
    {
        conn->mOrigComponent = newComp;
        conn = (Connection*)conn->mNextConnection;
    }

    conn = (Connection*)newComp->mFirstInConnection;
    while (conn != NULL)
    {
        conn->mTargComponent = newComp;
        conn = (Connection*)conn->mNextInConnection;
    }

    return newComp;
}

void Gridbrain::updateComponentSets()
{
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];
        grid->getComponentSet()->update(this,
                                        grid->getOffset(),
                                        grid->getOffset() + grid->getSize());
    }
}

void Gridbrain::addConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                Gene tag)
{
    /*printf("add connection: %d,%d,%d -> %d,%d,%d\n",
            xOrig,
            yOrig,
            gOrig,
            xTarg,
            yTarg,
            gTarg);*/

    Grid* gridOrig = mGridsVec[gOrig];
    Grid* gridTarg = mGridsVec[gTarg];

    if ((xOrig >= gridOrig->getWidth())
        || (xTarg >= gridTarg->getWidth())
        || (yOrig >= gridOrig->getHeight())
        || (yTarg >= gridTarg->getHeight()))
    {
        char buffer[500];
        sprintf(buffer,
                    "Parameter(s) out of range in gridbrain add connection: %d,%d,%d -> %d,%d,%d",
                    xOrig,
                    yOrig,
                    gOrig,
                    xTarg,
                    yTarg,
                    gTarg);
        throw std::runtime_error(buffer);
    }
    
    if (!isConnectionValid(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return;
    }
    if (connectionExists(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
    {
        return;
    }

    unsigned int orig = (xOrig * gridOrig->getHeight()) + yOrig + gridOrig->getOffset();
    unsigned int target = (xTarg * gridTarg->getHeight()) + yTarg + gridTarg->getOffset();

    Component* comp = mComponents[orig];
    Component* targComp = mComponents[target];
    Connection* conn = (Connection*)malloc(sizeof(Connection));
    conn->mColumnOrig = comp->mColumn;
    conn->mRowOrig = comp->mRow;
    conn->mGridOrig = comp->mGrid;
    conn->mColumnTarg = targComp->mColumn;
    conn->mRowTarg = targComp->mRow;
    conn->mGridTarg = targComp->mGrid;
    conn->mOrigComponent = comp;
    conn->mTargComponent = targComp;
    conn->mGene = tag;

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

        Connection* iterConn = comp->mFirstConnection;
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
                    ((Connection*)(conn->mPrevConnection))->mNextConnection = conn;
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

            iterConn = (Connection*)(iterConn->mNextConnection);
        }
    }

    (comp->mConnectionsCount)++;

    // Insert in the beginning of the target's incoming connections list
    Connection* nextConn = targComp->mFirstInConnection;
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

void Gridbrain::removeConnection(Connection* conn)
{
    Component* comp = (Component*)conn->mOrigComponent;
    Component* targComp = (Component*)conn->mTargComponent;

    comp->mConnectionsCount--;

    if (conn->mPrevConnection)
    {
        ((Connection*)conn->mPrevConnection)->mNextConnection = conn->mNextConnection;
    }
    else
    {
        comp->mFirstConnection = (Connection*)conn->mNextConnection;
    }
    if (conn->mNextConnection)
    {
        ((Connection*)conn->mNextConnection)->mPrevConnection = conn->mPrevConnection;
    }

    targComp->mInboundConnections--;

    if (conn->mPrevInConnection)
    {
        ((Connection*)conn->mPrevInConnection)->mNextInConnection = conn->mNextInConnection;
    }
    else
    {
        targComp->mFirstInConnection = (Connection*)conn->mNextInConnection;
    }
    if (conn->mNextInConnection)
    {
        ((Connection*)conn->mNextInConnection)->mPrevInConnection = conn->mPrevInConnection;
    }

    mConnectionsCount--;
    if (conn->mPrevGlobalConnection)
    {
        ((Connection*)conn->mPrevGlobalConnection)->mNextGlobalConnection =
            conn->mNextGlobalConnection;
    }
    else
    {
        mConnections = (Connection*)conn->mNextGlobalConnection;
    }
    if (conn->mNextGlobalConnection)
    {
        ((Connection*)conn->mNextGlobalConnection)->mPrevGlobalConnection =
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
    Connection* conn = getConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg);
    if (conn != NULL)
    {
        removeConnection(conn);
    }
}

Connection* Gridbrain::getConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    if (gOrig >= mGridsCount)
    {
        return NULL;
    }

    Grid* grid = mGridsVec[gOrig];

    if (xOrig >= grid->getWidth())
    {
        return NULL;
    }
    if (yOrig >= grid->getHeight())
    {
        return NULL;
    }

    unsigned int orig = (xOrig * grid->getHeight()) + yOrig + grid->getOffset();
    grid = mGridsVec[gTarg];

    Component* comp = mComponents[orig];
    Connection* conn = comp->mFirstConnection;

    unsigned int i = 0;
    while ((conn) && (i < comp->mConnectionsCount))
    {
        if ((conn->mColumnTarg == xTarg)
            && (conn->mRowTarg == yTarg)
            && (conn->mGridTarg == gTarg))
        {
            return conn;
        }

        conn = (Connection*)conn->mNextConnection;
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
    Connection* conn = getConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg);
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
                Component* comp = getComponent(col, row, g);
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
    Component* origComp = getComponent(x1, y1, g1);
    Connection* conn = origComp->mFirstConnection;

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

        conn = (Connection*)(conn->mNextConnection);
    }

    // Get target component coordinates from relative offset
    Component* targComp = getCompByRelativeOffset(origComp, targPos);
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
    Component* targetComp = getComponent(x2, y2, g2);

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
                    Component* comp = getComponent(x, y, g);

                    if (comp->getConnectorType() != Component::CONN_IN)
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
            Component* comp = getComponent(x, y, g2);

            if (comp->getConnectorType() != Component::CONN_IN)
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
                    Component* comp = getComponent(x1, y1, g1);

                    if ((comp->getConnectorType() != Component::CONN_IN)
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
            Component* comp = getComponent(x1, y1, g1);

            if ((comp->getConnectorType() != Component::CONN_IN)
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
    Component* origComp = getComponent(x1, y1, g1);

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
                Component* comp = getComponent(x2, y2, g2);

                if ((comp->getConnectorType() != Component::CONN_OUT)
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
            Component* comp = getComponent(x2, y2, g2);

            if ((comp->getConnectorType() != Component::CONN_OUT)
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

unsigned int Gridbrain::getRelativeOffset(Component* compOrig,
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
        Component* comp = getComponent(targX, row, targG);
        if (comp->getConnectorType() != Component::CONN_OUT)
        {
            offset++;
        }
    }

    return offset;
}

Component* Gridbrain::getCompByRelativeOffset(Component* compOrig, unsigned int offset)
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
        Component* comp = getComponent(col, row, g);
        if (comp->getConnectorType() != Component::CONN_OUT)
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

    for (unsigned int i = 0; i < count; i++)
    {
        if (selectRandomConnection(x1, y1, g1, x2, y2, g2))
        {
            addConnection(x1, y1, g1, x2, y2, g2);
        }
    }
}

void Gridbrain::cycle()
{
    //printf("========== BEGIN CYCLE ==========\n");

    // Reset beta grid
    // Assume beta grid at the end
    Grid* grid = mGridsVec[mGridsCount - 1];

    unsigned int endIndex = grid->mComponentSequenceSize;

    for (unsigned int i = 0; i < endIndex; i++)
    {
        Component* comp = grid->mComponentSequence[i];
        comp->reset(0, 0);
    }

    // Evaluate grids
    for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
    {
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
            bool firstAlpha = false;
            if (pass < (passCount - 1))
            {
                firstAlpha = true;
            }

            unsigned int inputDepthOffset = 0;

            for (unsigned int i = 0; i < inputDepth; i++)
            {
                unsigned int inputID = 0;

                if (grid->getType() == Grid::ALPHA)
                {
                    inputID = grid->getInputID(i);

                    for (unsigned int i = 0; i < endIndex; i++)
                    {
                        Component* comp = grid->mComponentSequence[i];
                        if (comp->isInput())
                        {
                            comp->mOutput =
                                inputMatrix[comp->mPerceptionPosition
                                        + inputDepthOffset];
                        }

                        comp->reset(pass, inputID);
                    }
                }

                for (unsigned int j = 0; j < endIndex; j++)
                {
                    Component* comp = grid->mComponentSequence[j];

                    // compute component output
                    float output = comp->output();
                    if (isnan(output))
                    {
                        output = 0.0f;
                    }

                    //comp->print();
                    //printf(" => %f\n", output);

                    if (comp->isOutput())
                    {
                        outputVector[comp->mActionPosition] = output;
                    }

                    // propagate outputs (inside grid if fist alpha)
                    Connection* conn = comp->mFirstConnection;
                    for (unsigned int k = 0; k < comp->mConnectionsCount; k++)
                    {
                        if ((!firstAlpha) || (!conn->mInterGrid))
                        {
                            Component* targetComp = (Component*)conn->mTargComponent;
                            targetComp->input(output, comp->mOffset);
                        }
                        conn = (Connection*)conn->mNextConnection;
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
                Component* comp = getComponent(col, row, g);

                if (comp->getConnectorType() != Component::CONN_OUT)
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
                Component* comp = getComponent(col, row, g);
                unsigned int compPossibleConnections = 0;

                if (comp->getConnectorType() != Component::CONN_IN)
                {
                    compPossibleConnections = possibleConnections;
                }
                comp->mPossibleConnections = compPossibleConnections;
                mTotalPossibleConnections += compPossibleConnections;
            }
        }
    }
}

void Gridbrain::calcActiveComponents()
{
    // Reset component active/producer/consumer state
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                Component* comp = getComponent(x, y, g);
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
                Component* comp = getComponent(x, y, g);
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
                    if (comp->isInput())
                    {
                        mActivePerceptions++;
                    }
                    else if (comp->isOutput())
                    {
                        mActiveActions++;
                    }
                    mActiveComponents++;
                    sequenceSize++;
                }
            }
        }

        grid->mComponentSequenceSize = sequenceSize;
    }
}

void Gridbrain::calcActive()
{
    calcActiveComponents();

    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* grid = mGridsVec[g];

        if (grid->mComponentSequence != NULL)
        {
            free(grid->mComponentSequence);
            grid->mComponentSequence = NULL;
        }
        grid->mComponentSequence = (Component**)malloc(grid->mComponentSequenceSize * sizeof(Component*));

        unsigned int pos = 0;
        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                Component* comp = getComponent(x, y, g);
                if (comp->mActive)
                {
                    grid->mComponentSequence[pos] = comp;
                    pos++;
                }
            }
        }
    }

    Connection* conn = mConnections;

    mActiveConnections = 0;
    while (conn != NULL)
    {
        Component* origComp = (Component*)conn->mOrigComponent;
        Component* targComp = (Component*)conn->mTargComponent;

        if (origComp->mActive && targComp->mActive)
        {
            mActiveConnections++;
            conn->mActive = true;
        }
        else
        {
            conn->mActive = false;
        }

        conn = (Connection*)conn->mNextGlobalConnection;
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
                Component* origComp = getComponent(x, y, g);

                Connection* conn = origComp->mFirstConnection;

                while (conn != NULL)
                {
                    Component* targComp = (Component*)conn->mTargComponent;

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

                    conn = (Connection*)conn->mNextConnection;
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
                Component* comp = getComponent(x, y, g);

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

float* Gridbrain::getInputBuffer(unsigned int channel, unsigned int id)
{
    return mGridsVec[channel]->getInputBuffer(id);
}

float* Gridbrain::getOutputBuffer()
{
    return mGridsVec[mGridsCount - 1]->getOutputVector();
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

    return false;
}

bool Gridbrain::isConnectionValid(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg)
{
    if ((gOrig == gTarg) && (xOrig >= xTarg))
    {
        return false;
    }

    Component* compOrig = getComponent(xOrig, yOrig, gOrig);
    if (compOrig->getConnectorType() == Component::CONN_IN)
    {
        return false;
    }
    Component* compTarg = getComponent(xTarg, yTarg, gTarg);
    if (compTarg->getConnectorType() == Component::CONN_OUT)
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
    Connection* nextConn = mConnections;

    while (nextConn != NULL)
    {
        Connection* conn = nextConn;
        nextConn = (Connection*)nextConn->mNextGlobalConnection;
        if (!isConnectionValid(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg))
        {
            removeConnection(conn);
        }
    }
}

bool Gridbrain::isValid()
{
    Connection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGridOrig == conn->mGridTarg)
        {
            if (conn->mColumnOrig >= conn->mColumnTarg)
            {
                throw("Invalid connection\n");
                return false;
            }
        }
        else
        {
            if (mGridsVec[conn->mGridOrig]->getType() != Grid::ALPHA)
            {
                printConnection(conn);
                throw("Invalid connection\n");
                return false;
            }
            if (mGridsVec[conn->mGridTarg]->getType() != Grid::BETA)
            {
                throw("Invalid connection\n");
                return false;
            }
        }

        if (!isConnectionValid(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg))
        {

            throw("Invalid connection\n");
            return false;
        }

        Component* compOrig = getComponent(conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig);
        Component* compTarg = getComponent(conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);

        if (conn->mOrigComponent != compOrig)
        {
            throw("Invalid component\n");
            return false;
        }
        if (conn->mTargComponent != compTarg)
        {
            throw("Invalid component\n");
            return false;
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        if (!mGridsVec[i]->isValid())
        {
            throw("Invalid grid\n");
            return false;
        }
    }

    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        Component* comp1 = mComponents[i];

        if (comp1->isUnique())
        {
            for (unsigned int j = 0; j < mNumberOfComponents; j++)
            {
                if (j != i)
                {
                    Component* comp2 = mComponents[j];

                    if (comp1->isEqual(comp2))
                    {
                        printf("Repeated unique component: (%d) ", i);
                        comp1->print();
                        printf(" (%d) ", j);
                        comp2->print();
                        printf("\n");
                        throw("Repeated unique component");
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

float Gridbrain::getDistance(Gridbrain* brain)
{
    Gridbrain* gb = (Gridbrain*)brain;

    float maxSize = (float)mActiveConnections;
    float ac2 = (float)gb->mActiveConnections;

    if (ac2 > maxSize)
    {
        maxSize = ac2;
    }

    if (maxSize == 0.0f)
    {
        return 0.0f;
    }

    float match = 0.0f;

    Connection* conn1 = mConnections;

    while (conn1 != NULL)
    {
        if (conn1->mActive)
        {
            Gene* g1 = &(conn1->mGene);
            Connection* conn2 = gb->mConnections;
            
            bool done = false;

            while ((conn2 != NULL) && (!done))
            {
                if (conn2->mActive)
                {
                    Gene* g2 = &(conn2->mGene);

                    if (g1->mGeneID == g2->mGeneID)
                    {
                        if ((g1->mOrigID == g2->mOrigID)
                            && (g1->mTargID == g2->mTargID))
                        {
                            match += 1.0f;
                            done = true;
                        }
                        else if ((g1->mOrigID == g2->mOrigID)
                            || (g1->mTargID == g2->mTargID))
                        {
                            match += 0.5f;
                            done = true;
                        }
                    }
                }

                conn2 = (Connection*)conn2->mNextGlobalConnection;
            }
        }

        conn1 = (Connection*)conn1->mNextGlobalConnection;
    }

    match /= maxSize;

    return (1.0f - match);
}

void Gridbrain::clearInterfaces()
{
    for (vector<list<Component*>*>::iterator iterInterface = mInputInterfacesVector.begin();
        iterInterface != mInputInterfacesVector.end();
        iterInterface++)
    {
        list<Component*>* interface = (*iterInterface);

        interface->clear();
        delete interface;
    }
    mInputInterfacesVector.clear();

    mOutputInterface.clear();
}

list<Component*>* Gridbrain::getInputInterface(unsigned int channel)
{
    return mInputInterfacesVector[channel];
}

list<Component*>* Gridbrain::getOutputInterface()
{
    return &mOutputInterface;
}

int Gridbrain::getChannelByName(string name)
{
    if (mChannels.count(name) == 0)
    {
        return -1;
    }

    return mChannels[name];
}

string Gridbrain::getChannelName(int chan)
{
    for (map<string, int>::iterator iterChan = mChannels.begin();
        iterChan != mChannels.end();
        iterChan++)
    {
        if ((*iterChan).second == chan)
        {
            return (*iterChan).first;
        }
    }

    return "";
}

}

