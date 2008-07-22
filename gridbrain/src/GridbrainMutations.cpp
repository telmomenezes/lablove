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

#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <float.h>
#include <math.h>

namespace gb
{

long Gridbrain::MUTATIONS_ADD_CONN = 0;
long Gridbrain::MUTATIONS_REM_CONN = 0;
long Gridbrain::MUTATIONS_SPLIT_CONN = 0;
long Gridbrain::MUTATIONS_JOIN_CONN = 0;
long Gridbrain::MUTATIONS_CHG_COMP = 0;
long Gridbrain::MUTATIONS_CHG_IN_COMP = 0;
long Gridbrain::MUTATIONS_CHG_PARAM = 0;
long Gridbrain::MUTATIONS_SWP_COMP = 0;

void Gridbrain::mutate(float factor)
{
    /*printf("mMutateAddConnectionProb: %f; mMutateRemoveConnectionProb: %f; mMutateChangeParamProb: %f; mParamMutationStanDev: %f; mMutateSplitConnectionProb: %f; mMutateJoinConnectionsProb: %f; mMutateChangeComponentProb: %f; mMutateSwapComponentProb: %f\n",
    mMutateAddConnectionProb,
    mMutateRemoveConnectionProb,
    mMutateChangeParamProb,
    mParamMutationStanDev,
    mMutateSplitConnectionProb,
    mMutateJoinConnectionsProb,
    mMutateChangeComponentProb,
    mMutateSwapComponentProb);*/

    mutateChangeComponent(mMutateChangeComponentProb * factor);
    mutateChangeInactiveComponent(mMutateChangeInactiveComponentProb * factor);
    mutateChangeParam(mMutateChangeParamProb * factor);
    
    // targets per column may have changed
    calcConnectionCounts();

    unsigned int connCount = mConnectionsCount;

    mutateAddConnection(connCount, mMutateAddConnectionProb * factor);
    mutateRemoveConnection(connCount, mMutateRemoveConnectionProb * factor);

    mutateSplitConnection(mMutateSplitConnectionProb * factor);
    mutateJoinConnections(mMutateJoinConnectionsProb * factor);

    update(); 
}

void Gridbrain::initRandomConnectionSequence(float selectionProb)
{
    // mConnSeqProb is the non-selection probability
    mConnSeqProb = 1.0f - selectionProb;
    mConnSeqCurrent = NULL;
}

Connection* Gridbrain::nextRandomConnection()
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
            mConnSeqCurrent = (Connection*)mConnSeqCurrent->mNextGlobalConnection;
        }

        if (mConnSeqCurrent == NULL)
        {
            return mConnSeqCurrent;
        }
    }

    return mConnSeqCurrent;
}

Connection* Gridbrain::skipNextRandomConnection()
{
    if (mConnSeqCurrent == NULL)
    {
        return NULL;
    }
    mConnSeqCurrent = (Connection*)mConnSeqCurrent->mNextGlobalConnection;
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

Connection* Gridbrain::getRandomConnection()
{
    if (mConnectionsCount == 0)
    {
        return NULL;
    }

    unsigned int pos;
    Connection* conn = mConnections;
    pos = mDistConnections->iuniform(0, mConnectionsCount);
    
    unsigned int i = 0;
    while (i < pos)
    {
        conn = (Connection*)(conn->mNextGlobalConnection);
        i++;
    }

    return conn;
}

void Gridbrain::mutateAddConnection(unsigned int popSize, float prob)
{
    // Allways create a connection if none exist
    // otherwise a 0 connections grid is an evolutionary dead-end
    if (mConnectionsCount == 0)
    {
        addRandomConnections(1);
        return;
    }

    unsigned int count = generateEventCount(prob, popSize);

    MUTATIONS_ADD_CONN += count;
    addRandomConnections(count);
}

void Gridbrain::mutateRemoveConnection(unsigned int popSize, float prob)
{
    unsigned int count = generateEventCount(prob, popSize);

    MUTATIONS_REM_CONN += count;

    for (unsigned int i = 0; i < count; i++)
    {
        Connection* conn = getRandomConnection();
        if (conn != NULL)
        {
            removeConnection(conn);
        }
    }
}

void Gridbrain::mutateSplitConnection(float prob)
{
    initRandomConnectionSequence(prob);

    Connection* nextConn = nextRandomConnection();
    while (nextConn != NULL)
    {
        Connection* conn = nextConn;
        nextConn = nextRandomConnection();

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

        if (g3 != -1)
        {
            MUTATIONS_SPLIT_CONN++;
            Gene tag1 = conn->mGene;

            // remove 1->2 connection
            removeConnection(conn);

            Gene tag2;
            Gene tag3;

            llULINT geneID = tag1.mGeneID;

            if (geneID == 0)
            {
                geneID = Gene::generateID();
            }

            tag2.mGeneID = geneID;
            tag3.mGeneID = geneID;
            tag2.mOrigID = tag1.mOrigID;
            tag3.mTargID = tag1.mTargID;
            tag2.mTargID = Gene::generateID();
            tag3.mOrigID = Gene::generateID();

            /*printf(">>> split tags!\n");
            tag2.print();
            printf("\n");
            tag3.print();
            printf("\n");*/

            // create 1->3 connection
            addConnection(x1, y1, g1, x3, y3, g3, tag2);
            // create 3->2 connection
            addConnection(x3, y3, g3, x2, y2, g2, tag3);

            /*printf("SPLIT\n");
            printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x2, y2, g2);
            printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x3, y3, g3);
            printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x3, y3, g3, x2, y2, g2);*/
        }
    }
}

void Gridbrain::mutateJoinConnections(float prob)
{
    initRandomConnectionSequence(prob);

    Connection* nextConn = nextRandomConnection();
    while (nextConn != NULL)
    {
        Connection* conn1 = nextConn;
        nextConn = nextRandomConnection();

        unsigned int x = conn1->mColumnTarg;
        unsigned int y = conn1->mRowTarg;
        unsigned int g = conn1->mGridTarg;

        Component* comp = getComponent(x, y, g);

        Connection* conn2 = comp->mFirstConnection;
        while ((conn2 != NULL)
                && (conn1->mGene.mGeneID != conn2->mGene.mGeneID))
        {
            conn2 = (Connection*)conn2->mNextConnection;
        }

        if (conn2 != NULL)
        {
            MUTATIONS_JOIN_CONN++;

            /*printf("JOIN\n");
            printf("add: (%d, %d, %d)->(%d, %d, %d)\n", x1, y1, g1, x2, y2, g2);
            printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", conn->mColumnOrig, conn->mRowOrig, conn->mGridOrig, conn->mColumnTarg, conn->mRowTarg, conn->mGridTarg);
            printf("remove: (%d, %d, %d)->(%d, %d, %d)\n", iterConn->mColumnOrig, iterConn->mRowOrig, iterConn->mGridOrig, iterConn->mColumnTarg, iterConn->mRowTarg, iterConn->mGridTarg);*/

            if (nextConn == conn2)
            {
                nextConn = skipNextRandomConnection();
            }

            Gene gt;
            gt.mGeneID = conn1->mGene.mGeneID;
            gt.mOrigID = conn1->mGene.mOrigID;
            gt.mTargID = conn2->mGene.mTargID;

            addConnection(conn1->mColumnOrig,
                        conn1->mRowOrig,
                        conn1->mGridOrig,
                        conn2->mColumnTarg,
                        conn2->mRowTarg,
                        conn2->mGridTarg,
                        gt);
            removeConnection(conn1);
            removeConnection(conn2);
        }
    }
}

void Gridbrain::mutateChangeComponent(float prob)
{
    initRandomComponentSequence(prob);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_CHG_COMP++;
        int pos = mCompSeqPos;

        unsigned int gridNumber = mComponents[pos]->mGrid;
        Grid* grid = mGridsVec[gridNumber];

        Component* comp = grid->getRandomComponent();
        replaceComponent(pos, comp);
    }

    cleanInvalidConnections();
}

void Gridbrain::mutateChangeInactiveComponent(float prob)
{
    initRandomComponentSequence(prob);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_CHG_IN_COMP++;
        int pos = mCompSeqPos;

        Component* comp = mComponents[pos];

        if (!comp->mActive)
        {
            unsigned int gridNumber = comp->mGrid;
            Grid* grid = mGridsVec[gridNumber];

            Component* comp2 = grid->getRandomComponent();
            replaceComponent(pos, comp2);
        }
    }

    cleanInvalidConnections();
}

void Gridbrain::mutateChangeParam(float prob)
{
    initRandomComponentSequence(prob);

    while (nextRandomComponent() >= 0)
    {
        MUTATIONS_CHG_PARAM++;
        int pos = mCompSeqPos;

        unsigned int gridNumber = mComponents[pos]->mGrid;
        Grid* grid = mGridsVec[gridNumber];

        Component* comp = mComponents[pos];
        float param = comp->mParam;
        param += mDistComponents->normal(0.0f, mParamMutationStanDev);
        if (param > 1.0f)
        {
            param = 1.0f;
        }
        else if (param < 0.0f)
        {
            param = 0.0f;
        }
        comp->mParam = param;
    }
}

void Gridbrain::debugMutationsCount()
{
    printf("CON+:%d CON-:%d SPL:%d JOI:%d CHG:%d CHIN:%d SWP:%d\n",
            MUTATIONS_ADD_CONN,
            MUTATIONS_REM_CONN,
            MUTATIONS_SPLIT_CONN,
            MUTATIONS_JOIN_CONN,
            MUTATIONS_CHG_COMP,
            MUTATIONS_CHG_IN_COMP,
            MUTATIONS_SWP_COMP);
    fflush(stdout);
}

}

