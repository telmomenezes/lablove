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
#include "Agent.h"

Brain* Gridbrain::recombine(Brain* brain)
{
    Gridbrain* child;

    switch (mRecombinationType)
    {
    case RT_TREE:
        child = treeRecombine((Gridbrain*)brain);
        break;
    case RT_UNIFORM:
        child = uniformRecombine((Gridbrain*)brain);
        break;
    }

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

Gridbrain* Gridbrain::treeRecombine(Gridbrain* brain)
{
    // TBD...
    Gridbrain* gbNew = (Gridbrain*)(this->clone());

    Gridbrain* child = (Gridbrain*)gbNew->clone();
    delete gbNew;

    return child;
}

void Gridbrain::clearConnRecombineInfo()
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        conn->mSelectionState = GridbrainConnection::SS_UNKNOWN;
        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
}

GridbrainComponent* Gridbrain::findEquivalentComponent(GridbrainComponent* comp, CompEquivalenceType eqType)
{
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        GridbrainComponent* comp2 = &mComponents[i];

        if (isCompEquivalent(comp, comp2, eqType))
        {
            return comp2;
        }
    }

    return NULL;
}

Gridbrain* Gridbrain::importConnection(Gridbrain* gb,
                                        GridbrainConnection* conn,
                                        bool &canAddComponent,
                                        bool &success,
                                        unsigned int &failsOrder,
                                        unsigned int &failsComp)
{
    Gridbrain* brain = gb;

    GridbrainComponent* orig = (GridbrainComponent*)(conn->mOrigComponent);
    GridbrainComponent* targ = (GridbrainComponent*)(conn->mTargComponent);

    GridbrainComponent* eqOrig = brain->findEquivalentComponent(orig, CET_NEW);
    GridbrainComponent* eqTarg = brain->findEquivalentComponent(targ, CET_NEW);

    if ((eqOrig == NULL) || (eqTarg == NULL))
    {
        if (!canAddComponent)
        {
            //printf("* components missing\n");
            failsComp++;
            success = false;
            return brain;
        }
    }

    llULINT origID;
    llULINT targID;

    if (eqOrig != NULL)
    {
        origID = eqOrig->mID;
        //printf("origin exists: ");
        //printComponent(eqOrig, true);
        //printf("\n");
    }
    if (eqTarg != NULL)
    {
        targID = eqTarg->mID;
        //printf("target exists: ");
        //printComponent(eqTarg, true);
        //printf("\n");
    }

    unsigned int origGrid = orig->mGrid;
    unsigned int targetGrid = targ->mGrid;

    if ((eqOrig == NULL)
        && (eqTarg == NULL))
    {
        //printf("* insert orig and target\n");
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

        origID = eqOrig->mID;
    }

    if (eqOrig == NULL)
    {
        //printf("* insert orig\n");
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
            }
            else
            {
                origColumn = grid->getWidth() - 1;
            }
        }

        while (eqOrig == NULL)
        {
            grid = brain->mGridsVec[origGrid];
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

        origID = eqOrig->mID;

        if (recalcTarg)
        {
            eqTarg = brain->getComponentByID(targID);
        }
    }

    if (eqTarg == NULL)
    {
        //printf("* insert target\n");
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

        targID = eqTarg->mID;

        if (recalcOrig)
        {
            eqOrig = brain->getComponentByID(origID);
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
        brain->addConnection(x1, y1, g1, x2, y2, g2, weight, conn->mTag);
        success = true;
        canAddComponent = false;
        //printf("SUCCESS\n");
    }
    else
    {
        failsOrder++;
        success = false;

        if (g1 == g2)
        {
            brain = brain->clone(false, ET_COLUMN_RANDOM, g1);
        }
        //printf("FAILURE\n");
    }

    return brain;
}

bool Gridbrain::checkTagGroup(llULINT group)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mTag.mGroupID == group)
        {
            return true;
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    return false;
}

bool Gridbrain::selectTagGroup(llULINT group, bool select)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mTag.mGroupID == group)
        {
            if (select)
            {
                conn->mSelectionState = GridbrainConnection::SS_SELECTED;
            }
            else
            {
                conn->mSelectionState = GridbrainConnection::SS_UNSELECTED;
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
}

Gridbrain* Gridbrain::uniformRecombine(Gridbrain* brain)
{
    //printf("\n\n=== START RECOMBINE ===\n");

    Gridbrain* gbNew = (Gridbrain*)(this->clone(false, ET_NONE, 0));
    Gridbrain* gb2 = brain;

    //printf("\n>>>> PARENT1\n");
    //gbNew->printDebug();
    //printf("\n>>>> PARENT2\n");
    //gb2->printDebug();

    gbNew->clearConnRecombineInfo();
    gb2->clearConnRecombineInfo();

    // Select connections, parent 1
    GridbrainConnection* conn = gbNew->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (gb2->checkTagGroup(conn->mTag.mGroupID))
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gbNew->selectTagGroup(conn->mTag.mGroupID, true);
                    gb2->selectTagGroup(conn->mTag.mGroupID, false);
                }
                else
                {
                    gbNew->selectTagGroup(conn->mTag.mGroupID, false);
                    gb2->selectTagGroup(conn->mTag.mGroupID, true);
                }
            }
            else
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gbNew->selectTagGroup(conn->mTag.mGroupID, true);
                }
                else
                {
                    gbNew->selectTagGroup(conn->mTag.mGroupID, false);
                }
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    // Select connections, parent 2
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (mDistRecombine->iuniform(0, 2) == 0)
            {
                gb2->selectTagGroup(conn->mTag.mGroupID, true);
            }
            else
            {
                gb2->selectTagGroup(conn->mTag.mGroupID, false);
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    // Remove unselected connections from child
    conn = gbNew->mConnections;
    while (conn != NULL)
    {
        GridbrainConnection* nextConn = (GridbrainConnection*)conn->mNextGlobalConnection;

        if (conn->mSelectionState == GridbrainConnection::SS_UNSELECTED)
        {
            gbNew->removeConnection(conn);
        }

        conn = nextConn;
    }

    // Import selected connections from parent 2
    bool done = false;
    unsigned int iteration = 0;
    bool canAddComponent = false;

    while (!done)
    {
        unsigned int failsOrder = 0;
        unsigned int failsComp = 0;

        conn = gb2->mConnections;
        while (conn != NULL)
        {
            if (conn->mSelectionState == GridbrainConnection::SS_SELECTED)
            {
                bool success;
                //printf("-> Import connection: ");
                //gb2->printConnection(conn);
                gbNew = gbNew->importConnection(gbNew, conn, canAddComponent, success, failsOrder, failsComp);

                if (success)
                {
                    conn->mSelectionState = GridbrainConnection::SS_UNSELECTED;
                    iteration = 0;
                }
            }

            conn = (GridbrainConnection*)conn->mNextGlobalConnection;
        }

        unsigned int fails = failsOrder + failsComp;

        if (fails > 0)
        {
            if ((failsOrder > 0) && (iteration < 10))
            {
                // Shuffle grid and retry
                iteration++;
                gbNew->update();
                gbNew->mutateSwapComponent(0.5f);
            }
            else if (!canAddComponent)
            {
                canAddComponent = true;
            }
            else
            {
                // OK, we give up...
                /*printf("=>>>>give up!\n");
                printf(">>> PARENT 1\n");
                printDebug();
                printf(">>> PARENT 2\n");
                gb2->printDebug();
                printf(">>> CHILD\n");
                gbNew->printDebug();*/
                done = true;
            }
        }
        else
        {
            // It's done!
            done = true;
        }
    }

    Gridbrain* child = (Gridbrain*)gbNew->clone();
    delete gbNew;
    
    //printf("\n>>>> CHILD\n");
    //child->printDebug();

    return child;
}

bool Gridbrain::isCompEquivalent(GridbrainComponent* comp1, GridbrainComponent* comp2, CompEquivalenceType eqType)
{
    if (comp1->isEqual(comp2))
    {
        if (comp1->isUnique())
        {
            return true;
        }
    }
    else
    {
        return false;
    }

    switch (eqType)
    {
    case CET_ORIGIN:
        if ((comp1->mInboundConnections == 0)
            && (comp2->mInboundConnections == 0)
            && (comp1->mConnectionsCount == 1)
            && (comp2->mConnectionsCount == 1)
            && comp1->isEqual(comp2))
        {
            return true;
        }
        break;
    case CET_TARGET:
        if ((comp1->mInboundConnections == 1)
            && (comp2->mInboundConnections == 1)
            && (comp1->mConnectionsCount == 0)
            && (comp2->mConnectionsCount == 0)
            && comp1->isEqual(comp2))
        {
            return true;
        }
        break;
    }

    GridbrainConnection* conn1 = (GridbrainConnection*)comp1->mFirstInConnection;
    while (conn1 != NULL)
    {
        GridbrainConnection* conn2 = (GridbrainConnection*)comp2->mFirstInConnection;
        while (conn2 != NULL)
        {
            if (conn1->mTag.isEquivalentOrigin(&(conn2->mTag)))
            {
                return true;
            }

            conn2 = (GridbrainConnection*)conn2->mNextInConnection;
        }
        conn1 = (GridbrainConnection*)conn1->mNextInConnection;
    }

    conn1 = (GridbrainConnection*)comp1->mFirstConnection;
    while (conn1 != NULL)
    {
        GridbrainConnection* conn2 = (GridbrainConnection*)comp2->mFirstConnection;
        while (conn2 != NULL)
        {
            if (conn1->mTag.isEquivalentTarget(&(conn2->mTag)))
            {
                return true;
            }

            conn2 = (GridbrainConnection*)conn2->mNextConnection;
        }
        conn1 = (GridbrainConnection*)conn1->mNextConnection;
    }

    return false;
}

GridbrainConnTag Gridbrain::findConnTag(GridbrainConnection* conn)
{
    GridbrainConnection* conn2 = mConnections;

    while (conn2 != NULL)
    {
        if (conn != conn2)
        {
            GridbrainComponent* orig1 = (GridbrainComponent*)conn->mOrigComponent;
            GridbrainComponent* targ1 = (GridbrainComponent*)conn->mTargComponent;
            GridbrainComponent* orig2 = (GridbrainComponent*)conn2->mOrigComponent;
            GridbrainComponent* targ2 = (GridbrainComponent*)conn2->mTargComponent;

            if (isCompEquivalent(orig1, orig2, CET_ORIGIN) && isCompEquivalent(targ1, targ2, CET_TARGET))
            {
                return conn2->mTag;
            }
        }

        conn2 = (GridbrainConnection*)conn2->mNextGlobalConnection;
    }

    return GridbrainConnTag();
}

void Gridbrain::popAdjust(vector<SimulationObject*>* popVec)
{
    for (unsigned int pos = 0; pos < mNumberOfComponents; pos++)
    {
        GridbrainComponent* comp = &mComponents[pos];

        GridbrainConnection* conn = comp->mFirstConnection;
        while (conn != NULL)
        {
            // If no tag assigned, look for equivalent connection
            if (conn->mTag.mID == 0)
            {
                conn->mTag = findConnTag(conn);

                unsigned int vecPos = 0;
                while ((conn->mTag.mID == 0) && (vecPos < popVec->size()))
                {
                    SimulationObject* obj = (*popVec)[vecPos];
                    Agent* agent = (Agent*)obj;
                    Gridbrain* gb = (Gridbrain*)agent->getBrain();
                    conn->mTag = gb->findConnTag(conn);
                    vecPos++;
                }
            }

            // If still no tag assigned, generate new one
            if (conn->mTag.mID == 0)
            {
                conn->mTag.generateID();
                conn->mTag.mGroupID = conn->mTag.mID;
            }

            conn = (GridbrainConnection*)conn->mNextConnection;
        }
    }
}

