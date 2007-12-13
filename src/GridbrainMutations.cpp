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
    mutateAddConnection();
    mutateRemoveConnection();
    mutateChangeConnectionWeight();
    mutateSplitConnection();
    mutateJoinConnections();
    mutateChangeComponent();
    mutateSwapComponent();
}

void Gridbrain::initRandomConnectionSequence(float selectionProb)
{
    // mConnSeqProb is the non-selection probability
    mConnSeqProb = 1.0f - selectionProb;
    mConnSeqCurrent = mConnections;
    mConnSeqPos = 0;
}

GridbrainConnection* Gridbrain::nextRandomConnection()
{
    if (mConnSeqProb == 1.0f)
    {
        mConnSeqCurrent = NULL;
    }
    if (mConnSeqCurrent == NULL)
    {
        return NULL;
    }

    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    double nextPos = trunc(log(prob) / log(mConnSeqProb));

    while (mConnSeqPos < nextPos)
    {
        mConnSeqCurrent = (GridbrainConnection*)mConnSeqCurrent->mNextGlobalConnection;
        mConnSeqPos++;

        if (mConnSeqCurrent == NULL)
        {
            return mConnSeqCurrent;
        }
    }

    return mConnSeqCurrent;
}

void Gridbrain::initRandomComponentSequence(float selectionProb)
{
    // mCompSeqProb is the non-selection probability
    mCompSeqProb = 1.0f - selectionProb;
    mCompSeqPos = 0;
}

int Gridbrain::nextRandomComponent()
{
    if (mCompSeqProb == 1.0f)
    {
        mCompSeqPos = -1;
        return mCompSeqPos;
    }

    float prob = mDistMutationsProb->uniform(0.0f, 1.0f);
    double nextPos = trunc(log(prob) / log(mCompSeqProb));

    mCompSeqPos += (int)nextPos;

    if (mCompSeqPos >= mNumberOfComponents)
    {
        mCompSeqPos = -1;
    }

    return mCompSeqPos;
}

void Gridbrain::mutateAddConnection()
{
    initRandomConnectionSequence(mMutateAddConnectionProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_ADD_CONN++;
        addRandomConnections(1);
    }
}

void Gridbrain::mutateRemoveConnection()
{
    initRandomConnectionSequence(mMutateRemoveConnectionProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_REM_CONN++;
        GridbrainConnection* conn = mConnSeqCurrent;
        mConnSeqCurrent = (GridbrainConnection*)mConnSeqCurrent->mNextGlobalConnection;
        removeConnection(conn);
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

void Gridbrain::mutateSplitConnection()
{
    initRandomConnectionSequence(mMutateSplitConnectionProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_SPLIT_CONN++;
        GridbrainConnection* conn = mConnSeqCurrent;

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
            valid &= !connectionExists(x1, y1, g1, x3, y3, g3);
            // test 2->3 connection
            valid &= !connectionExists(x3, y3, g3, x2, y2, g2);
        }

        if (valid)
        {
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

            // create 1->3 connection
            addConnection(x1, y1, g1, x3, y3, g3, weight1);
            // create 3->2 connection
            addConnection(x3, y3, g3, x2, y2, g2, weight2);
        }
    }
}

void Gridbrain::mutateJoinConnections()
{
    initRandomConnectionSequence(mMutateJoinConnectionsProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_JOIN_CONN++;
        GridbrainConnection* conn = mConnSeqCurrent;

        GridbrainComponent* pivot = getComponent(conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);
        GridbrainConnection* iterConn = pivot->mFirstConnection;
        unsigned int candidateConnections = 0;

        while (iterConn != NULL)
        {
            if (conn->mFeedForward == iterConn->mFeedForward)
            {
                candidateConnections++;
            }
            iterConn = (GridbrainConnection*)(iterConn->mNextConnection);
        }

        unsigned int pos = 0;
        if (candidateConnections > 0)
        {
            pos = mDistConnections->iuniform(1, candidateConnections + 1);
        }

        if (pos >= 1)
        {
            unsigned int currentPos = 0;
            iterConn = NULL;
            while (currentPos < pos)
            {
                if (iterConn == NULL)
                {
                    iterConn = pivot->mFirstConnection;
                }
                else
                {
                    iterConn = (GridbrainConnection*)(iterConn->mNextConnection);
                }
                if (conn->mFeedForward == iterConn->mFeedForward)
                {
                    currentPos++;
                }
            }

            unsigned int x1 = conn->mColumnOrig;
            unsigned int y1 = conn->mRowOrig;
            unsigned int g1 = conn->mGridOrig;
            unsigned int x2 = iterConn->mColumnTarg;
            unsigned int y2 = iterConn->mRowTarg;
            unsigned int g2 = iterConn->mGridTarg;

            if (!connectionExists(x1, y1, g1, x2, y2, g2))
            {
                float weight = 0;
                unsigned int weightSelector = mDistWeights->iuniform(0, 3);
                switch (weightSelector)
                {
                case 0:
                    weight = conn->mWeight;
                case 1:
                    weight = iterConn->mWeight;
                default:
                    weight = mDistWeights->uniform(-1.0f, 1.0f);
                }

                // Current connection is going to be delted, advance to next one
                GridbrainConnection* nextConn = (GridbrainConnection*)conn->mNextGlobalConnection;
                if (nextConn == iterConn)
                {
                    nextConn = (GridbrainConnection*)nextConn->mNextGlobalConnection;
                }

                removeConnection(conn);
                removeConnection(iterConn);
                addConnection(x1, y1, g1, x2, y2, g2, weight);
                
                mConnSeqCurrent = nextConn;
            }
        }
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

        GridbrainComponent* comp = grid->getRandomComponent(pos);
        mComponents[pos].copyDefinitions(comp);
    }
}

void Gridbrain::mutateSwapComponent()
{
    initRandomComponentSequence(mMutateSwapComponentProb);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_SWP_COMP++;
        int pos = mCompSeqPos;

        unsigned int gridNumber = mComponents[pos].mGrid;
        Grid* grid = mGridsVec[gridNumber];

        GridbrainComponent* comp1 = &mComponents[pos];

        unsigned int x1 = comp1->mColumn;
        unsigned int y1 = comp1->mRow;

        unsigned int x2 = mDistComponents->iuniform(0, grid->getWidth() - 1);
        if (x2 >= comp1->mColumn)
        {
            x2++;
        }
        unsigned int y2 = mDistComponents->iuniform(0, grid->getHeight());

        GridbrainComponent* comp2 = getComponent(x2, y2, gridNumber);

        GridbrainConnection* conn = mConnections;

        while (conn != NULL)
        {
            
            if ((conn->mColumnOrig == x1) 
                && (conn->mRowOrig == y1)
                && (conn->mGridOrig == gridNumber))
            {
                conn->mColumnOrig = x2;
                conn->mRowOrig = y2;
            }
            else if ((conn->mColumnOrig == x2)
                && (conn->mRowOrig == y2)
                && (conn->mGridOrig == gridNumber))
            {
                conn->mColumnOrig = x1;
                conn->mRowOrig = y1;
            }

            if ((conn->mColumnTarg == x1)
                && (conn->mRowTarg == y1)
                && (conn->mGridTarg == gridNumber))
            {
                conn->mColumnTarg = x2;
                conn->mRowTarg = y2;
            }
            else if ((conn->mColumnTarg == x2)
                && (conn->mRowTarg == y2)
                && (conn->mGridTarg == gridNumber))
            {
                conn->mColumnTarg = x1;
                conn->mRowTarg = y1;
            }

            if (((!mRecurrentAllowed)
                || (grid->getType() == Grid::ALPHA))
                && (conn->mColumnOrig >= conn->mColumnTarg))
            {
                GridbrainConnection* nextConn = (GridbrainConnection*)conn->mNextGlobalConnection;
                removeConnection(conn);
                conn = nextConn;
            }
            else
            {
                conn = (GridbrainConnection*)conn->mNextGlobalConnection;
            }
        }

        GridbrainComponent auxComp;
        auxComp.copyDefinitions(comp1);
        comp1->copyDefinitions(comp2);
        comp2->copyDefinitions(&auxComp);
    }
}

void Gridbrain::debugMutationsCount()
{
    printf("CON+:%d CON-:%d WGT:%d SPL:%d JOI:%d CHG:%d SWP:%d\n",
            MUTATIONS_ADD_CONN,
            MUTATIONS_REM_CONN,
            MUTATIONS_CHG_WGT,
            MUTATIONS_SPLIT_CONN,
            MUTATIONS_JOIN_CONN,
            MUTATIONS_CHG_COMP,
            MUTATIONS_SWP_COMP);
    fflush(stdout);
}

