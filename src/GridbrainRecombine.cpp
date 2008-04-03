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

Brain* Gridbrain::recombine(Brain* brain)
{
    switch (mRecombinationType)
    {
    case RT_TREE:
        return treeRecombine((Gridbrain*)brain);
        break;
    case RT_UNIFORM:
        return uniformRecombine((Gridbrain*)brain);
        break;
    }
}

GridbrainComponent* Gridbrain::findEquivalentComponent(GridbrainComponent* comp)
{
    GridbrainComponent* eqComp = NULL;

    Grid* grid = mGridsVec[comp->mGrid];
    unsigned int start = grid->getOffset();
    unsigned int end = start + grid->getSize();

    for (unsigned int i = start; i < end; i++)
    {
        GridbrainComponent* curComp = &(mComponents[i]);

        if ((comp->mType == GridbrainComponent::PER) || (comp->mType == GridbrainComponent::ACT))
        {
            if ((comp->mType == curComp->mType)
                && (comp->mSubType == curComp->mSubType)
                && (comp->mOrigSymTable == curComp->mOrigSymTable)
                && (comp->mTargetSymTable == curComp->mTargetSymTable)
                && (comp->mOrigSymID == curComp->mOrigSymID)
                && (comp->mTargetSymID == curComp->mTargetSymID))
            {
                return curComp;
            }
            else
            {
                if (comp->mID == curComp->mID)
                {
                    eqComp = curComp;
                }
            }
        }
        else
        {
            if (comp->mID == curComp->mID)
            {
                return curComp;
            }
        }
    }

    return eqComp;
}

void Gridbrain::clearCompRecombineInfo(bool selected)
{
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        mComponents[i].mPivotCandidate = false;
        mComponents[i].mSelected = selected;
        mComponents[i].mUsed = false;
    }
}

void Gridbrain::spreadSelected(GridbrainComponent* comp, bool selected)
{
    if (comp->mSelected != selected)
    {
        comp->mSelected = selected;

        GridbrainConnection* conn = comp->mFirstConnection;

        while (conn != NULL)
        {
            GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
            spreadSelected(targComp, selected);
            conn = (GridbrainConnection*)conn->mNextConnection;
        }
    }
}

Gridbrain* Gridbrain::crossoverComp(Gridbrain* gb, GridbrainComponent* pivot, unsigned int &failed)
{
    Gridbrain* brain = gb;
    GridbrainComponent* orig = pivot;
    GridbrainConnection* conn = orig->mFirstConnection;

    while (conn != NULL)
    {
        GridbrainComponent* targ = (GridbrainComponent*)(conn->mTargComponent);

        GridbrainComponent* eqOrig = brain->findEquivalentComponent(orig);
        GridbrainComponent* eqTarg = brain->findEquivalentComponent(targ);

        bool recalcOrig = false;

        if (eqTarg == NULL)
        {
            unsigned int origGrid = orig->mGrid;
            unsigned int targetGrid = targ->mGrid;
            unsigned int targetColumn = 0;

            if (origGrid == targetGrid)
            {
                targetColumn = eqOrig->mColumn + 1;
            }

            // Grid width needs to be expanded?
            if (brain->mGridsVec[targetGrid]->getWidth() == targetColumn)
            {
                Gridbrain* newBrain = brain->clone(false, ET_COLUMN_LAST, targetGrid);
                delete brain;
                brain = newBrain;
                recalcOrig = true;
            }

            while (eqTarg == NULL)
            {
                Grid* grid = brain->mGridsVec[targetGrid];

                unsigned int y = 0;
                while ((eqTarg == NULL) && (y < grid->getHeight()))
                {
                    GridbrainComponent* curComp = brain->getComponent(targetColumn, y, targetGrid);
                    y++;
                    if (!curComp->isUsed())
                    {
                        curComp->copyDefinitions(targ);
                        eqTarg = curComp;
                    }
                }

                // Grid height needs to be expanded?
                if (eqTarg == NULL)
                {
                    Gridbrain* newBrain = brain->clone(false, ET_ROW, targetGrid);
                    delete brain;
                    brain = newBrain;
                    recalcOrig = true;
                }
            }
        }

        if (recalcOrig)
        {
            eqOrig = brain->findEquivalentComponent(orig);
        }

        unsigned int x1 = eqOrig->mColumn;
        unsigned int y1 = eqOrig->mRow;
        unsigned int g1 = eqOrig->mGrid;
        unsigned int x2 = eqTarg->mColumn;
        unsigned int y2 = eqTarg->mRow;
        unsigned int g2 = eqTarg->mGrid;
        float weight = conn->mWeight;

        if ((brain->isConnectionValid(x1, y1, g1, x2, y2, g2))
            && ((g1 != g2) || (x1 < x2)))
        {
            brain->addConnection(x1, y1, g1, x2, y2, g2, weight);
        }
        else
        {
            failed++;
        }

        eqOrig->mUsed = true;
        eqTarg->mUsed = true;
        brain = crossoverComp(brain, targ, failed);

        conn = (GridbrainConnection*)(conn->mNextConnection);
    }

    return brain;
}

Gridbrain* Gridbrain::treeRecombine(Gridbrain* brain)
{
    Gridbrain* gbNew = (Gridbrain*)(this->clone());
    Gridbrain* gb2 = (Gridbrain*)(brain->clone());

    gbNew->clearCompRecombineInfo(true);
    gb2->clearCompRecombineInfo(false);

    // find initial equivalents and pivot candidates
    unsigned int possiblePivots = 0;

    for (unsigned int i = 0; i < gbNew->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &(gbNew->mComponents[i]);
        GridbrainComponent* eqComp = gb2->findEquivalentComponent(comp);

        if ((eqComp != NULL)
            && (comp->mActive || eqComp->mActive))
        {
            possiblePivots++;
            comp->mPivotCandidate = true;
        }
    }

    if (possiblePivots == 0)
    {
        delete gb2;
        return gbNew;
    }

    // find pivot
    unsigned int pivotPos = mDistRecombine->iuniform(0, possiblePivots);

    unsigned int i = 0;
    unsigned int curPos = 0;
    GridbrainComponent* pivot1;

    while (curPos <= pivotPos)
    {
        pivot1 = &(gbNew->mComponents[i]);

        if (pivot1->mPivotCandidate)
        {
            curPos++;
        }
        i++;
    }

    GridbrainComponent* pivot2 = gb2->findEquivalentComponent(pivot1);

    gbNew->spreadSelected(pivot1, false);
    gb2->spreadSelected(pivot2, true);

    // Remove unselected connections
    for (unsigned int i = 0; i < gbNew->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &(gbNew->mComponents[i]);

        if (!comp->mSelected)
        {
            GridbrainConnection* conn = comp->mFirstConnection;

            while (conn != NULL)
            {
                GridbrainConnection* remConn = conn;
                conn = (GridbrainConnection*)(conn->mNextConnection);
                gbNew->removeConnection(remConn);
            }
        }
    } 
    gbNew->update();

    // Insert second brain's connections and components
    // Use swap operator to reorder components and attempt to add more
    // connections from second parent if needed
    int iteration = 0;
    while (iteration < 10)
    {
        unsigned int failed = 0;
        gbNew = gbNew->crossoverComp(gbNew, pivot2, failed);

        if (failed > 0)
        {
            iteration++;
            gbNew->mutateSwapComponent(0.5f);
        }
        else
        {
            iteration = 999999;
        }

        gbNew->update();
    }

    delete gb2;

    Gridbrain* child = (Gridbrain*)gbNew->clone();
    delete gbNew;

    return child;
}

void Gridbrain::setSelectedSymbols(SimulationObject* obj)
{
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = obj->mSymbolTables.begin();
        iterTables != obj->mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;
        int tableID = table->getID();

        map<llULINT, Symbol*>::iterator iterSymbol;
        for (iterSymbol = table->getSymbolMap()->begin();
            iterSymbol != table->getSymbolMap()->end();
            iterSymbol++)
        {
            Symbol* sym = (*iterSymbol).second;
            llULINT symID = sym->mID;

            if (!sym->mFixed)
            {
                sym->mSelected = false;

                for (unsigned int i = 0;
                    (i < mNumberOfComponents) && (!sym->mSelected);
                    i++)
                {
                    GridbrainComponent* comp = &mComponents[i];

                    if ((tableID == comp->mOrigSymTable)
                        && (symID == comp->mOrigSymID))
                    {
                        sym->mSelected = true;
                    }
                }
            }
        }
    }
}

void Gridbrain::clearConnRecombineInfo(bool selected)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        conn->mSelected = selected;
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
}

GridbrainConnection* Gridbrain::findEquivalentConnection(GridbrainConnection* conn)
{
    GridbrainComponent* origComp = findEquivalentComponent((GridbrainComponent*)conn->mOrigComponent);
    if (origComp == NULL)
    {
        return NULL;
    }
    GridbrainComponent* targComp = findEquivalentComponent((GridbrainComponent*)conn->mTargComponent);
    if (targComp == NULL)
    {
        return NULL;
    }
    
    llULINT origID = origComp->mID;
    llULINT targID = targComp->mID;

    GridbrainConnection* conn2 = mConnections;

    while (conn2 != NULL)
    {
        llULINT origID2 = ((GridbrainComponent*)conn2->mOrigComponent)->mID;
        llULINT targID2 = ((GridbrainComponent*)conn2->mTargComponent)->mID;

        if ((origID == origID2)
            && (targID == targID2))
        {
            return conn2;
        }
   
        conn2 = (GridbrainConnection*)conn2->mNextGlobalConnection;
    }

    return NULL;
}

Gridbrain* Gridbrain::importConnection(Gridbrain* gb, GridbrainConnection* conn, unsigned int &failed)
{
    Gridbrain* brain = gb;

    GridbrainComponent* orig = (GridbrainComponent*)(conn->mOrigComponent);
    GridbrainComponent* targ = (GridbrainComponent*)(conn->mTargComponent);

    GridbrainComponent* eqOrig = brain->findEquivalentComponent(orig);
    GridbrainComponent* eqTarg = brain->findEquivalentComponent(targ);

    unsigned int origGrid = orig->mGrid;
    unsigned int targetGrid = targ->mGrid;


    if ((eqOrig == NULL)
        && (eqTarg == NULL))
    {
        while (eqOrig == NULL)
        {
            Grid* grid = brain->mGridsVec[origGrid];

            for (unsigned int x = 0;
                    (x < grid->getWidth()) && (eqOrig == NULL);
                    x++)
            {
                for (unsigned int y = 0;
                        (y < grid->getHeight()) && (eqOrig == NULL);
                        y++)
                {
                    GridbrainComponent* curComp = brain->getComponent(x, y, origGrid);

                    if (!curComp->isUsed())
                    {
                        curComp->copyDefinitions(orig);
                        eqOrig = curComp;
                    }
                }
            }

            // Grid width needs to be expanded?
            if (eqOrig == NULL)
            {
                Gridbrain* newBrain = brain->clone(false, ET_COLUMN_RANDOM, origGrid);
                delete brain;
                brain = newBrain;
            }
        }
    }

    if (eqOrig == NULL)
    {
        Grid* grid = brain->mGridsVec[origGrid];

        bool recalcTarg = false;
        
        unsigned int origColumn;

        if (origGrid == targetGrid)
        {
            if (eqTarg->mColumn == 0)
            {
                origColumn = 0;

                // Grid width needs to be expanded
                Gridbrain* newBrain = brain->clone(false, ET_COLUMN_FIRST, origGrid);
                delete brain;
                brain = newBrain;
                recalcTarg = true;
            }
            else
            {
                origColumn = eqTarg->mColumn - 1;
            }
        }
        else
        {
            if (grid->getWidth() == 0)
            {
                origColumn = 0;

                // Grid width needs to be expanded
                Gridbrain* newBrain = brain->clone(false, ET_COLUMN_FIRST, origGrid);
                delete brain;
                brain = newBrain;
                recalcTarg = true;
                grid = brain->mGridsVec[origGrid];
            }
            else
            {
                origColumn = grid->getWidth() - 1;
            }
        }

        while (eqOrig == NULL)
        {
            unsigned int y = 0;
            while ((eqOrig == NULL) && (y < grid->getHeight()))
            {
                GridbrainComponent* curComp = brain->getComponent(origColumn, y, origGrid);
                y++;
                if (!curComp->isUsed())
                {
                    curComp->copyDefinitions(orig);
                    eqOrig = curComp;
                }
            }

            // Grid height needs to be expanded?
            if (eqOrig == NULL)
            {
                Gridbrain* newBrain = brain->clone(false, ET_ROW, origGrid);
                delete brain;
                brain = newBrain;
                recalcTarg = true;
            }
        }

        if (recalcTarg)
        {
            eqTarg = brain->findEquivalentComponent(targ);
        }
    }

    if (eqTarg == NULL)
    {
        bool recalcOrig = false;
        
        unsigned int targetColumn = 0;

        if (origGrid == targetGrid)
        {
            targetColumn = eqOrig->mColumn + 1;
        }

        // Grid width needs to be expanded?
        if (brain->mGridsVec[targetGrid]->getWidth() == targetColumn)
        {
            Gridbrain* newBrain = brain->clone(false, ET_COLUMN_LAST, targetGrid);
            delete brain;
            brain = newBrain;
            recalcOrig = true;
        }

        while (eqTarg == NULL)
        {
            Grid* grid = brain->mGridsVec[targetGrid];

            unsigned int y = 0;
            while ((eqTarg == NULL) && (y < grid->getHeight()))
            {
                GridbrainComponent* curComp = brain->getComponent(targetColumn, y, targetGrid);
                y++;
                if (!curComp->isUsed())
                {
                    curComp->copyDefinitions(targ);
                    eqTarg = curComp;
                }
            }

            // Grid height needs to be expanded?
            if (eqTarg == NULL)
            {
                Gridbrain* newBrain = brain->clone(false, ET_ROW, targetGrid);
                delete brain;
                brain = newBrain;
                recalcOrig = true;
            }
        }

        if (recalcOrig)
        {
            eqOrig = brain->findEquivalentComponent(orig);
        }
    }

    unsigned int x1 = eqOrig->mColumn;
    unsigned int y1 = eqOrig->mRow;
    unsigned int g1 = eqOrig->mGrid;
    unsigned int x2 = eqTarg->mColumn;
    unsigned int y2 = eqTarg->mRow;
    unsigned int g2 = eqTarg->mGrid;
    float weight = conn->mWeight;

    if ((brain->isConnectionValid(x1, y1, g1, x2, y2, g2))
        && ((g1 != g2) || (x1 < x2)))
    {
        brain->addConnection(x1, y1, g1, x2, y2, g2, weight);
        //printf("-> Conn imported (2)\n");
    }
    else
    {
        failed++;
        //printf("-> Import failed (2)\n");
    }

    eqOrig->mUsed = true;
    eqTarg->mUsed = true;

    return brain;
}

Gridbrain* Gridbrain::uniformRecombine(Gridbrain* brain)
{
    //printf("\n\n=== START RECOMBINE ===\n");

    Gridbrain* gbNew = (Gridbrain*)(this->clone());
    Gridbrain* gb2 = brain;

    gb2->clearConnRecombineInfo(true);

    // Crossover components in first brain
    for (unsigned int i = 0; i < gbNew->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp2 = gb2->getComponentByID(gbNew->mComponents[i].mID);

        if ((comp2 != NULL)
            && (mDistRecombine->iuniform(0, 2) == 0))
        {
            gbNew->mComponents[i].copyDefinitions(comp2);
        }
    }

    // Crossover connections, parent 1
    GridbrainConnection* conn = gbNew->mConnections;
    bool nextConn;

    while (conn != NULL)
    {
        nextConn = true;

        GridbrainConnection* conn2 = gb2->findEquivalentConnection(conn);
       
        if (conn2 == NULL)
        {
            if (mDistRecombine->iuniform(0, 2) == 0)
            {
                //printf("-> Remove conn (1)\n");
                GridbrainConnection* rmConn = conn;
                conn = (GridbrainConnection*)conn->mNextGlobalConnection;
                gbNew->removeConnection(rmConn);
                nextConn = false;
            }
            else
            {
                //printf("-> Keep conn (1)\n");
            }
        }
        else
        {
            conn2->mSelected = false;

            if (mDistRecombine->iuniform(0, 2) == 0)
            {
                conn->mWeight = conn2->mWeight;
                conn->mRealWeight = conn2->mRealWeight;
                //printf("-> Use other conn weights (1)\n");
            }
            else
            {
                //printf("-> Keep conn weight (1)\n");
            }
        }

        if (nextConn)
        {
            conn = (GridbrainConnection*)conn->mNextGlobalConnection;
        }
    }

    // Crossover connections, parent 2
    conn = gb2->mConnections;

    // Connections that only exist on parent2 have a 50% chance of being selected
    while (conn != NULL)
    {
        if (conn->mSelected)
        {
            if (mDistRecombine->iuniform(0, 2) == 0)
            {
                //printf("-> Do no include conn (2)\n");
                conn->mSelected = false;
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    int iteration = 0;
    while (iteration < 10)
    {
        unsigned int failed = 0;
        conn = gb2->mConnections;

        while (conn != NULL)
        {
            if (conn->mSelected)
            {
                gbNew = importConnection(gbNew, conn, failed);
            }

            conn = (GridbrainConnection*)conn->mNextGlobalConnection;
        }

        if (failed > 0)
        {
            iteration++;
            gbNew->mutateSwapComponent(0.5f);
        }
        else
        {
            iteration = 999999;
        }

        gbNew->update();
    }
    
    Gridbrain* child = (Gridbrain*)gbNew->clone();
    delete gbNew;

    return child;
}

