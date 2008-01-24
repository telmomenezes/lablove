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
long Gridbrain::MUTATIONS_NEW_WGT = 0;
long Gridbrain::MUTATIONS_MOV_ORI = 0;
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

    mutateChangeConnectionWeight();
    mutateNewConnectionWeight();

    unsigned int connCount = mConnectionsCount;
    mutateAddConnection(connCount);
    mutateRemoveConnection(connCount);

    mutateMoveConnectionOrigin();
    mutateSplitConnection();
    mutateJoinConnections();

    mutateChangeComponent();
    mutateSwapComponent();
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

void Gridbrain::removeRandomConnection()
{
    if (mConnectionsCount == 0)
    {
        return;
    }

    unsigned int pos = mDistConnections->iuniform(0, mConnectionsCount);
    GridbrainConnection* conn = mConnections;
    unsigned int i = 0;
    while (i < pos)
    {
        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
        i++;
    }

    removeConnection(conn);
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
        removeRandomConnection();
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

void Gridbrain::mutateNewConnectionWeight()
{
    initRandomConnectionSequence(mMutateNewConnectionWeightProb);

    while (nextRandomConnection() != NULL) 
    {
        MUTATIONS_NEW_WGT++;
        GridbrainConnection* conn = mConnSeqCurrent;

        float newWeight = mDistWeights->uniform(-1.0f, 1.0f);
        conn->mWeight = newWeight;
        applyWeight(conn);
    }
}

void Gridbrain::mutateMoveConnectionOrigin()
{
    // WARNING: This code assume one beta grid in the end

    initRandomConnectionSequence(mMutateMoveConnectionOriginProb);

    while (nextRandomConnection() != NULL) 
    {
        GridbrainConnection* conn = mConnSeqCurrent;

        Grid* gridTarg = mGridsVec[conn->mGridTarg];

        int sameGridOrigins = gridTarg->getHeight() * conn->mColumnTarg;
        int possibleOrigins = sameGridOrigins;

        if (gridTarg->getType() == Grid::BETA)
        {
            for (unsigned int i = 0; i < mGridsCount - 1; i++)
            {
                possibleOrigins += mGridsVec[i]->getSize();
            }
        }

        int pos = mDistConnections->iuniform(0, possibleOrigins);

        unsigned int newX;
        unsigned int newY;
        unsigned int newG;
       
        if (gridTarg->getType() == Grid::BETA)
        {
            unsigned int i = 0;
            unsigned int currentLimit = mGridsVec[i]->getSize();

            while (pos >= currentLimit)
            {
                i++;    
                currentLimit += mGridsVec[i]->getSize();
            }

            newG = i;
            pos = pos - currentLimit + mGridsVec[i]->getSize();
        }
        else
        {
            newG = conn->mGridTarg;
        }

        gridTarg = mGridsVec[newG];

        newX = pos / gridTarg->getHeight();
        newY = pos - (newX * gridTarg->getHeight());

        if (!connectionExists(newX,
                                newY,
                                newG,
                                conn->mColumnTarg,
                                conn->mRowTarg,
                                conn->mGridTarg))
        {
            MUTATIONS_MOV_ORI++;

            //printf("move connection: \nfrom: %d, %d, %d -> %d, %d, %d\n", newX, newY, newG, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);
            //printf("  to: %d, %d, %d -> %d, %d, %d\n", conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);

            addConnection(newX,
                            newY,
                            newG,
                            conn->mColumnTarg,
                            conn->mRowTarg,
                            conn->mGridTarg,
                            conn->mWeight,
                            conn->mAge);

            removeConnection(conn);
        }
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
        unsigned int candidateConnections = pivot->mConnectionsCount;

        unsigned int pos = 0;
        if (candidateConnections > 0)
        {
            pos = mDistConnections->iuniform(1, candidateConnections + 1);
        }

        if (pos >= 1)
        {
            unsigned int currentPos = 0;
            GridbrainConnection* iterConn = pivot->mFirstConnection;
            while (currentPos < pos)
            {
                iterConn = (GridbrainConnection*)(iterConn->mNextConnection);
                currentPos++;
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

        GridbrainComponent* comp = grid->getRandomComponent();
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

            // Check if swap is possible
            bool valid = true;
            GridbrainConnection* conn = mConnections;
            while (valid && (conn != NULL))
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
                conn = (GridbrainConnection*)conn->mNextGlobalConnection;
            }

            // If valid, do swap
            if (valid)
            {
                conn = mConnections;
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

                    conn = (GridbrainConnection*)conn->mNextGlobalConnection;
                }

                GridbrainComponent auxComp;
                auxComp.copyDefinitions(comp1);
                comp1->copyDefinitions(comp2);
                comp2->copyDefinitions(&auxComp);
            }
        }
    }
}

void Gridbrain::debugMutationsCount()
{
    printf("CON+:%d CON-:%d CWG:%d NWG:%d MOR:%d SPL:%d JOI: %d CHG:%d SWP:%d\n",
            MUTATIONS_ADD_CONN,
            MUTATIONS_REM_CONN,
            MUTATIONS_CHG_WGT,
            MUTATIONS_NEW_WGT,
            MUTATIONS_MOV_ORI,
            MUTATIONS_SPLIT_CONN,
            MUTATIONS_JOIN_CONN,
            MUTATIONS_CHG_COMP,
            MUTATIONS_SWP_COMP);
    fflush(stdout);
}

