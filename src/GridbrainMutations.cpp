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

long Gridbrain::MUTATIONS_ADD_CONN = 0;
long Gridbrain::MUTATIONS_REM_CONN = 0;
long Gridbrain::MUTATIONS_CHG_WGT = 0;
long Gridbrain::MUTATIONS_SPLIT_CONN = 0;
long Gridbrain::MUTATIONS_JOIN_CONN = 0;
long Gridbrain::MUTATIONS_CHG_COMP = 0;
long Gridbrain::MUTATIONS_SWP_COMP = 0;

void Gridbrain::mutate()
{
    /*printf("mMutateAddConnectionProb: %f; mMutateRemoveConnectionProb: %f; mMutateChangeConnectionWeightProb: %f; mWeightMutationStanDev: %f; mMutateSplitConnectionProb: %f; mMutateJoinConnectionsProb: %f; mMutateChangeComponentProb: %f; mMutateSwapComponentProb: %f\n",
    mMutateAddConnectionProb,
    mMutateRemoveConnectionProb,
    mMutateChangeConnectionWeightProb,
    mWeightMutationStanDev,
    mMutateSplitConnectionProb,
    mMutateJoinConnectionsProb,
    mMutateChangeComponentProb,
    mMutateSwapComponentProb);*/

    mutateSwapComponent();
    mutateChangeComponent();
    
    // targets per column may have changed
    calcConnectionCounts();

    mutateChangeConnectionWeight();

    unsigned int connCount = mConnectionsCount;

    mutateAddConnection(connCount);
    mutateRemoveConnection(connCount);

    mutateSplitConnection(connCount);
    mutateJoinConnections(connCount);

    update(); 
}

void Gridbrain::initRandomConnectionSequence(float selectionProb)
{
    // mConnSeqProb is the non-selection probability
    mConnSeqProb = 1.0f - selectionProb;
    mConnSeqCurrent = NULL;
}

GridbrainConnection* Gridbrain::nextRandomConnection()
{
    if (mConnSeqProb == 1.0f)
    {
        mConnSeqCurrent = NULL;
        return mConnSeqCurrent;
    }

    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    unsigned int deltaPos = (unsigned int)(ceil(log(prob) / log(mConnSeqProb)));

    for (unsigned int i = 0; i < deltaPos; i++)
    {
        if (mConnSeqCurrent == NULL)
        {
            mConnSeqCurrent = mConnections;
        }
        else
        {
            mConnSeqCurrent = (GridbrainConnection*)mConnSeqCurrent->mNextGlobalConnection;
        }

        if (mConnSeqCurrent == NULL)
        {
            return mConnSeqCurrent;
        }
    }

    return mConnSeqCurrent;
}

unsigned int Gridbrain::generateEventCount(float eventProb, unsigned int popSize)
{
    if (eventProb == 0.0f)
    {
        return 0;
    }

    float nonEventProb = 1.0f - eventProb;

    unsigned int count = 0;
    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    unsigned int nextPos = (unsigned int)(ceil(log(prob) / log(nonEventProb)));

    while (nextPos <= (double)popSize)
    {
        count++;
        prob = mDistMutationsProb->uniform(0.0f, 1.0f);
        nextPos += (unsigned int)(ceil(log(prob) / log(nonEventProb)));
    }

    return count;
}

void Gridbrain::initRandomComponentSequence(float selectionProb)
{
    // mCompSeqProb is the non-selection probability
    mCompSeqProb = 1.0f - selectionProb;
    mCompSeqPos = -1;
}

int Gridbrain::nextRandomComponent()
{
    if (mCompSeqProb == 1.0f)
    {
        mCompSeqPos = -1;
        return mCompSeqPos;
    }

    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    double nextPos = log(prob) / log(mCompSeqProb);

    mCompSeqPos += (unsigned int)(ceil(log(prob) / log(mCompSeqProb)));

    if (mCompSeqPos >= mNumberOfComponents)
    {
        mCompSeqPos = -1;
    }

    return mCompSeqPos;
}

GridbrainConnection* Gridbrain::getFirstMutableConnection(unsigned int initialPop)
{
    GridbrainConnection* conn = mConnections;

    if (mMutationScope == MS_ALL)
    {
        return conn;
    }

    unsigned int offset = 0;

    if (mConnectionsCount > initialPop)
    {
        offset = mConnectionsCount - initialPop;
    }

    for (unsigned int i = 0; i < offset; i++)
    {
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    return conn;
}

unsigned int Gridbrain::getMutableConnectionsCount(unsigned int initialPop)
{
    if (mMutationScope == MS_ALL)
    {
        return mConnectionsCount;
    }
    else
    {
        if (mConnectionsCount < initialPop)
        {
            return mConnectionsCount;
        }
        else
        {
            return initialPop;
        }
    }
}

void Gridbrain::removeRandomConnection(unsigned int initialPop)
{
    if (mConnectionsCount == 0)
    {
        return;
    }

    unsigned int pos;
    GridbrainConnection* conn = getFirstMutableConnection(initialPop);
    pos = mDistConnections->iuniform(0, getMutableConnectionsCount(initialPop));
    
    unsigned int i = 0;
    while (i < pos)
    {
        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
        i++;
    }

    removeConnection(conn);
}

bool Gridbrain::isSplitable(GridbrainConnection* conn)
{
    unsigned int x1 = conn->mColumnOrig;
    unsigned int g1 = conn->mGridOrig;
    unsigned int x2 = conn->mColumnTarg;
    unsigned int g2 = conn->mGridTarg;

    if (g1 == g2)
    {
        if ((x2 - x1) > 1)
        {
            return true;
        }
    }
    else
    {
        if (x2 > 0)
        {
            return true;
        }

        Grid* grid = mGridsVec[g1];
        if ((grid->getWidth() - x1) > 1)
        {
            return true;
        }
    }

    return false;
}

GridbrainConnection* Gridbrain::selectSplitableConnection(unsigned int initialPop)
{
    GridbrainConnection* conn = getFirstMutableConnection(initialPop);
    unsigned int count = 0;
    while (conn != NULL)
    {
        if (isSplitable(conn))
        {
            count++;
        }
        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
    }

    if (count == 0)
    {
        return NULL;
    }

    unsigned int pos = mDistConnections->iuniform(0, count);

    conn = getFirstMutableConnection(initialPop);
    unsigned int i = 0;
    while (i <= pos)
    {
        if (isSplitable(conn))
        {
            i++;
        }
        if (i <= pos)
        {
            conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
        }
    }

    return conn;
}

GridbrainConnection* Gridbrain::selectJoinableConnection(unsigned int initialPop)
{
    GridbrainConnection* conn = getFirstMutableConnection(initialPop);
    unsigned int count = 0;
    while (conn != NULL)
    {
        unsigned int x = conn->mColumnTarg;
        unsigned int y = conn->mRowTarg;
        unsigned int g = conn->mGridTarg;

        GridbrainComponent* comp = getComponent(x, y, g);

        if (comp->mConnectionsCount > 0)
        {
            count++;
        }
        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
    }
    
    if (count == 0)
    {
        return NULL;
    }

    unsigned int pos = mDistConnections->iuniform(0, count);

    conn = getFirstMutableConnection(initialPop);
    unsigned int i = 0;
    while (i <= pos)
    {
        unsigned int x = conn->mColumnTarg;
        unsigned int y = conn->mRowTarg;
        unsigned int g = conn->mGridTarg;

        GridbrainComponent* comp = getComponent(x, y, g);

        if (comp->mConnectionsCount > 0)
        {
            i++;
        }

        if (i <= pos)
        {
            conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
        }
    }

    return conn;
}

void Gridbrain::mutateAddConnection(unsigned int popSize)
{
    // Allways create a connection if none exist
    // otherwise a 0 connections grid is an evolutionary dead-end
    if (mConnectionsCount == 0)
    {
        addRandomConnections(1);
        return;
    }

    unsigned int count = generateEventCount(mMutateAddConnectionProb, popSize);

    MUTATIONS_ADD_CONN += count;
    addRandomConnections(count);
}

void Gridbrain::mutateRemoveConnection(unsigned int popSize)
{
    unsigned int count = generateEventCount(mMutateRemoveConnectionProb, popSize);

    MUTATIONS_REM_CONN += count;

    for (unsigned int i = 0; i < count; i++)
    {
        removeRandomConnection(popSize);
    }
}

void Gridbrain::mutateChangeConnectionWeight()
{
    initRandomConnectionSequence(mMutateChangeConnectionWeightProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_CHG_WGT++;
        GridbrainConnection* conn = mConnSeqCurrent;

        float newWeight = conn->mWeight;
        newWeight += mDistWeights->normal(0.0f, mWeightMutationStanDev);
        if (newWeight > 1.0f)
        {
            newWeight = 1.0f;
        }
        else if (newWeight < -1.0f)
        {
            newWeight = -1.0f;
        }

        conn->mWeight = newWeight;
        applyWeight(conn);
    }
}

void Gridbrain::mutateSplitConnection(unsigned int popSize)
{
    unsigned int count = generateEventCount(mMutateSplitConnectionProb, popSize);

    for (unsigned int i = 0; i < count; i++)
    {
        GridbrainConnection* conn = selectSplitableConnection(popSize);

        if (conn == NULL)
        {
            return;
        }

        unsigned int g1 = conn->mGridOrig;
        unsigned int g2 = conn->mGridTarg;
        Grid* gridOrig = mGridsVec[g1];
        Grid* gridTarg = mGridsVec[g2];
        unsigned int x1 = conn->mColumnOrig; 
        unsigned int y1 = conn->mRowOrig; 
        unsigned int x2 = conn->mColumnTarg; 
        unsigned int y2 = conn->mRowTarg;
        int g3 = -1;
        int x3 = -1;
        int y3 = -1;

        if (g1 == g2)
        {
            int deltaX = x2 - x1;
            int absDeltaX = deltaX;
            if (absDeltaX < 0)
            {
                absDeltaX = -deltaX;
            }
            if (absDeltaX > 1)
            {
                unsigned int middleColumn = mDistConnections->iuniform(1, absDeltaX);
                if (deltaX < 0)
                {
                    middleColumn = -middleColumn;
                }
                x3 = x1 + middleColumn;
                y3 = mDistConnections->iuniform(0, gridOrig->getHeight());
                g3 = g1;
            }
        }
        else
        {
            int candidateCols1 = 0;
            int candidateCols2 = 0;

            if (x1 < (gridOrig->getWidth() - 1))
            {
                candidateCols1 = gridOrig->getWidth() - x1 - 1;
            }
            if (x2 < (gridTarg->getWidth() - 1))
            {
                candidateCols2 = x2;
            }

            if ((candidateCols1 != 0) || (candidateCols2 != 0))
            {
                unsigned int candidateCells1 = candidateCols1 * gridOrig->getHeight();
                unsigned int candidateCells2 = candidateCols2 * gridTarg->getHeight();
                unsigned int totalCells = candidateCells1 + candidateCells2;
                unsigned int pivotCell = mDistConnections->iuniform(0, totalCells);
                Grid* pivotGrid;

                if (pivotCell < candidateCells1)
                {
                    g3 = g1;
                    pivotGrid = gridOrig;
                    pivotCell = pivotGrid->getSize() - pivotCell - 1;
                }
                else
                {
                    g3 = g2;
                    pivotCell -= candidateCells1;
                    pivotGrid = gridTarg;
                }

                x3 = pivotCell / pivotGrid->getHeight();
                y3 = pivotCell % pivotGrid->getHeight();
            }
        }

        bool valid = false;

        if (g3 != -1)
        {
            valid = true;

            // test 1->3 connection
            valid &= isConnectionValid(x1, y1, g1, x3, y3, g3);
            // test 2->3 connection
            valid &= isConnectionValid(x3, y3, g3, x2, y2, g2);
        }

        if (valid)
        {
            MUTATIONS_SPLIT_CONN++;
            GridbrainGeneTag tag1 = conn->mGeneTag;
            float weight = conn->mWeight;

            // Current connection is going to be delted, advance to next one
            mConnSeqCurrent = (GridbrainConnection*)conn->mNextGlobalConnection;

            // remove 1->2 connection
            removeConnection(x1, y1, g1, x2, y2, g2);

            float weight1 = weight;
            float weight2 = weight;

            unsigned int weightSelector = mDistWeights->iuniform(0, 2);
            if (weightSelector == 0)
            {
                weight1 = mDistWeights->uniform(-1.0f, 1.0f);
            }
            weightSelector = mDistWeights->iuniform(0, 2);
            if (weightSelector == 0)
            {
                weight2 = mDistWeights->uniform(-1.0f, 1.0f);
            }

            GridbrainGeneTag tag2;
            GridbrainGeneTag tag3;

            llULINT geneID = tag1.mGeneID;

            if (geneID == 0)
            {
                geneID = GridbrainGeneTag::generateID();
            }

            tag2.mGeneID = geneID;
            tag3.mGeneID = geneID;
            tag2.mOrigID = tag1.mOrigID;
            tag3.mTargID = tag1.mTargID;
            tag2.mTargID = GridbrainGeneTag::generateID();
            tag3.mOrigID = GridbrainGeneTag::generateID();

            /*printf(">>> split tags!\n");
            tag2.print();
            printf("\n");
            tag3.print();
            printf("\n");*/

            // create 1->3 connection
            addConnection(x1, y1, g1, x3, y3, g3, weight1, tag2);
            // create 3->2 connection
            addConnection(x3, y3, g3, x2, y2, g2, weight2, tag3);

            /*printf("SPLIT\n");
            printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x2, y2, g2);
            printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x3, y3, g3);
            printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x3, y3, g3, x2, y2, g2);*/
        }
    }
}

void Gridbrain::mutateJoinConnections(unsigned int popSize)
{
    unsigned int count = generateEventCount(mMutateJoinConnectionsProb, popSize);

    for (unsigned int i = 0; i < count; i++)
    {
        GridbrainConnection* conn1 = selectJoinableConnection(popSize);

        if (conn1 == NULL)
        {
            return;
        }

        unsigned int x = conn1->mColumnTarg;
        unsigned int y = conn1->mRowTarg;
        unsigned int g = conn1->mGridTarg;

        GridbrainComponent* comp = getComponent(x, y, g);

        unsigned int pos = mDistWeights->iuniform(0, comp->mConnectionsCount);

        GridbrainConnection* conn2 = comp->mFirstConnection;

        for (unsigned j = 0; j < pos; j++)
        {
            conn2 = (GridbrainConnection*)conn2->mNextConnection;
        }

        MUTATIONS_JOIN_CONN++;
        float weight = 0;
        unsigned int weightSelector = mDistWeights->iuniform(0, 3);
        switch (weightSelector)
        {
        case 0:
            weight = conn1->mWeight;
        case 1:
            weight = conn2->mWeight;
        default:
            weight = mDistWeights->uniform(-1.0f, 1.0f);
        }

        /*printf("JOIN\n");
        printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x2, y2, g2);
        printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);
        printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", iterConn->mColumnOrig, iterConn->mRowOrig, iterConn->mGridOrig, iterConn->mColumnTarg, iterConn->mRowTarg, iterConn->mGridTarg);*/

        addConnection(conn1->mColumnOrig,
                        conn1->mRowOrig,
                        conn1->mGridOrig,
                        conn2->mColumnTarg,
                        conn2->mRowTarg,
                        conn2->mGridTarg,
                        weight);
        removeConnection(conn1);
        removeConnection(conn2);
    }
}

void Gridbrain::mutateChangeComponent()
{
    initRandomComponentSequence(mMutateChangeComponentProb);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_CHG_COMP++;
        int pos = mCompSeqPos;

        unsigned int gridNumber = mComponents[pos].mGrid;
        Grid* grid = mGridsVec[gridNumber];

        GridbrainComponent* comp = grid->getRandomComponent(mOwner, mComponents);
        mComponents[pos].copyDefinitions(comp);
    }

    cleanInvalidConnections();
}

void Gridbrain::mutateSwapComponent(float prob)
{
    float swapProb = prob;

    if (swapProb == 0.0f)
    {
        swapProb = mMutateSwapComponentProb;
    }

    initRandomComponentSequence(swapProb);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_SWP_COMP++;
        int pos = mCompSeqPos;

        GridbrainComponent* comp1 = &mComponents[pos];

        unsigned int gridNumber = comp1->mGrid;
        Grid* grid = mGridsVec[gridNumber];

        // No point in swapping components in a grid with less than 2 columns
        if (grid->getWidth() > 1)
        {
            unsigned int x1 = comp1->mColumn;
            unsigned int y1 = comp1->mRow;

            unsigned int x2 = mDistComponents->iuniform(0, grid->getWidth() - 1);
            if (x2 >= comp1->mColumn)
            {
                x2++;
            }
            unsigned int y2 = mDistComponents->iuniform(0, grid->getHeight());

            GridbrainComponent* comp2 = getComponent(x2, y2, gridNumber);

            bool valid = true;

            // These swaps are not desirable
            if (comp1->getConnectorType() == GridbrainComponent::CONN_IN)
            {
                if (x1 > x2)
                {
                    valid = false;
                }
            }
            if (comp2->getConnectorType() == GridbrainComponent::CONN_IN)
            {
                if (x1 < x2)
                {
                    valid = false;
                }
            }
            if (comp1->getConnectorType() == GridbrainComponent::CONN_OUT)
            {
                if (x1 < x2)
                {
                    valid = false;
                }
            }
            if (comp2->getConnectorType() == GridbrainComponent::CONN_OUT)
            {
                if (x1 > x2)
                {
                    valid = false;
                }
            }

            // If valid, do swap
            if (valid)
            {
                swapComponents(comp1, comp2);
                //printf("SWAP (%d, %d, %d) / (%d, %d, %d)\n", x1, y1, gridNumber, x2, y2, gridNumber);
            }
        }
    }
}

void Gridbrain::debugMutationsCount()
{
    printf("CON+:%d CON-:%d CWG:%d SPL:%d JOI: %d CHG:%d SWP:%d\n",
            MUTATIONS_ADD_CONN,
            MUTATIONS_REM_CONN,
            MUTATIONS_CHG_WGT,
            MUTATIONS_SPLIT_CONN,
            MUTATIONS_JOIN_CONN,
            MUTATIONS_CHG_COMP,
            MUTATIONS_SWP_COMP);
    fflush(stdout);
}

