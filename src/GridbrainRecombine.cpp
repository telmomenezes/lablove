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

void Gridbrain::clearRecombineInfo()
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
    int bestEq = 0;
    GridbrainComponent* eqComp = NULL;

    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        GridbrainComponent* comp2 = &mComponents[i];

        int eq = compEquivalence(comp, comp2, eqType);

        if (eq > bestEq)
        {
            bestEq = eq;
            eqComp = comp2;
        }

        if (bestEq == 2)
        {
            return eqComp;
        }
    }

    return eqComp;
}

bool Gridbrain::correctOrder(int& x1, int& y1, int& x2, int& y2, int g)
{
    GridbrainComponent* comp1 = getComponent(x1, y1, g);
    GridbrainComponent* comp2 = getComponent(x2, y2, g);

    if ((x1 != x2) && (swapComponents(comp1, comp2)))
    {
        unsigned int tx = x1;
        unsigned int ty = y1;
        x1 = x2;
        y1 = y2;
        x2 = tx;
        y2 = ty;
        return true;
    }

    Grid* grid = mGridsVec[g];

    for (unsigned int x = x1 + 1; x < grid->getWidth(); x++)
    {
        for (unsigned int y = 0; y < grid->getHeight(); y++)
        {
            GridbrainComponent* compSwap = getComponent(x, y, g);

            if (swapComponents(comp2, compSwap))
            {
                x2 = x;
                y2 = y;
                return true;
            }
        }
    }

    for (int x = x2 - 1; x >= 0; x--)
    {
        for (unsigned int y = 0; y < grid->getHeight(); y++)
        {
            GridbrainComponent* compSwap = getComponent(x, y, g);

            if (swapComponents(comp1, compSwap))
            {
                x1 = x;
                y1 = y;
                return true;
            }
        }
    }

    return false;
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

    ;if ((eqOrig == NULL) || (eqTarg == NULL))
    {
        if (!canAddComponent)
        {
            //printf("* components missing\n");
            failsComp++;
            success = false;
            return brain;
        }
    }

    unsigned int origX;
    unsigned int origY;
    unsigned int origG;
    unsigned int targX;
    unsigned int targY;
    unsigned int targG;

    if (eqOrig != NULL)
    {
        origX = eqOrig->mColumn;
        origY = eqOrig->mRow;
        origG = eqOrig->mGrid;
        //printf("origin exists: ");
        //printComponent(eqOrig);
        //printf("\n");
    }
    if (eqTarg != NULL)
    {
        targX = eqTarg->mColumn;
        targY = eqTarg->mRow;
        targG = eqTarg->mGrid;
        //printf("target exists: ");
        //printComponent(eqTarg);
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
                eqOrig = brain->findEquivalentComponent(orig, CET_NEW);
            }
        }

        origX = eqOrig->mColumn;
        origY = eqOrig->mRow;
        origG = eqOrig->mGrid;
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
                if (targG == origG)
                {
                    targX++;
                }
                eqOrig = brain->findEquivalentComponent(orig, CET_NEW);
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
                eqOrig = brain->findEquivalentComponent(orig, CET_NEW);
            }
        }

        origX = eqOrig->mColumn;
        origY = eqOrig->mRow;
        origG = eqOrig->mGrid;

        if (recalcTarg)
        {
            eqTarg = brain->getComponent(targX, targY, targG);
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
            eqTarg = brain->findEquivalentComponent(targ, CET_NEW);
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
                eqTarg = brain->findEquivalentComponent(targ, CET_NEW);
            }
        }

        if (recalcOrig)
        {
            eqOrig = brain->getComponent(origX, origY, origG);
        }
    }

    bool fail = false;

    int x1 = eqOrig->mColumn;
    int y1 = eqOrig->mRow;
    int g1 = eqOrig->mGrid;
    int x2 = eqTarg->mColumn;
    int y2 = eqTarg->mRow;
    int g2 = eqTarg->mGrid;
    float weight = conn->mWeight;

    if (brain->isConnectionValid(x1, y1, g1, x2, y2, g2) && (g1 <= g2))
    {
        if ((g1 == g2) && (x1 >= x2))
        {
            //printf("* reverse order\n");
            if (!brain->correctOrder(x1, y1, x2, y2, g1))
            {
                Gridbrain* newBrain = brain->clone(false, ET_COLUMN_BEFORE, g1, x2);
                delete brain;
                brain = newBrain;
                x1++;
                x2++;
                if (!brain->correctOrder(x1, y1, x2, y2, g1))
                {
                    Gridbrain* newBrain = brain->clone(false, ET_COLUMN_AFTER, g1, x1);
                    delete brain;
                    brain = newBrain;
                    
                    if (!brain->correctOrder(x1, y1, x2, y2, g1))
                    {
                        fail = true;
                    }
                }
            }
        }
    }
    else
    {
        fail = true;
    }

    if (fail)
    {
        failsOrder++;
        success = false;

        //printf("FAILURE\n");
    }
    else
    {
        brain->addConnection(x1, y1, g1, x2, y2, g2, weight, conn->mGeneTag);
        eqOrig = brain->getComponent(x1, y1, g1);
        eqTarg = brain->getComponent(x2, y2, g2);
        canAddComponent = false;
        success = true;

        //printf("SUCCESS\n");
    }

    return brain;
}

bool Gridbrain::checkGene(llULINT geneID)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
        {
            return true;
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    return false;
}

bool Gridbrain::selectGene(llULINT geneID, bool select)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
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

Brain* Gridbrain::recombine(Brain* brain)
{
    //printf("\n\n=== START RECOMBINE ===\n");

    Gridbrain* gbNew = (Gridbrain*)(this->clone(false, ET_NONE, 0));
    Gridbrain* gb2 = (Gridbrain*)brain;

    //printf("\n>>>> PARENT1\n");
    //gbNew->printDebug();
    //printf("\n>>>> PARENT2\n");
    //gb2->printDebug();

    gbNew->clearRecombineInfo();
    gb2->clearRecombineInfo();

    // Select connections, parent 1
    GridbrainConnection* conn = gbNew->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (gb2->checkGene(conn->mGeneTag.mGeneID))
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gbNew->selectGene(conn->mGeneTag.mGeneID, true);
                    gb2->selectGene(conn->mGeneTag.mGeneID, false);
                }
                else
                {
                    gbNew->selectGene(conn->mGeneTag.mGeneID, false);
                    gb2->selectGene(conn->mGeneTag.mGeneID, true);
                }
            }
            else
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gbNew->selectGene(conn->mGeneTag.mGeneID, true);
                }
                else
                {
                    gbNew->selectGene(conn->mGeneTag.mGeneID, false);
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
                gb2->selectGene(conn->mGeneTag.mGeneID, true);
            }
            else
            {
                gb2->selectGene(conn->mGeneTag.mGeneID, false);
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
    bool canAddComponent = false;

    while (!done)
    {
        unsigned int failsOrder = 0;
        unsigned int failsComp = 0;
        unsigned int imports = 0;

        conn = gb2->mConnections;
        while (conn != NULL)
        {
            if (conn->mSelectionState == GridbrainConnection::SS_SELECTED)
            {
                bool success;
                //printf("-> Import connection: ");
                //gb2->printConnection(conn);
                gbNew = importConnection(gbNew, conn, canAddComponent, success, failsOrder, failsComp);

                if (success)
                {
                    conn->mSelectionState = GridbrainConnection::SS_UNSELECTED;
                    imports++;
                }
            }

            conn = (GridbrainConnection*)conn->mNextGlobalConnection;
        }

        unsigned int fails = failsOrder + failsComp;

        bool addComponent = false;
        if (fails > 0)
        {
            if (imports == 0)
            {
                if (failsComp > 0)
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
        }
        else
        {
            // It's done!
            done = true;
        }
    }

    gbNew->generateMemory();
    gbNew->update();

    /*printf(">>> PARENT 1\n");
    printDebug();
    printf(">>> PARENT 2\n");
    gb2->printDebug();
    printf(">>> CHILD\n");
    gbNew->printDebug();*/

    return gbNew;
}

int Gridbrain::compEquivalence(GridbrainComponent* comp1, GridbrainComponent* comp2, CompEquivalenceType eqType)
{
    if (!comp1->isEqual(comp2))
    {
        return 0;
    }

    if (comp1->isUnique())
    {
        return 2;
    }

    switch (eqType)
    {
    case CET_ORIGIN:
        if ((comp1->mInboundConnections == 0)
            && (comp2->mInboundConnections == 0)
            && (comp1->mConnectionsCount == 1)
            && (comp2->mConnectionsCount == 1))
        {
            return 1;
        }
        break;
    case CET_TARGET:
        if ((comp1->mInboundConnections == 1)
            && (comp2->mInboundConnections == 1)
            && (comp1->mConnectionsCount == 0)
            && (comp2->mConnectionsCount == 0))
        {
            return 1;
        }
        break;
    }

    GridbrainConnection* conn1 = (GridbrainConnection*)comp1->mFirstInConnection;
    while (conn1 != NULL)
    {
        GridbrainConnection* conn2 = (GridbrainConnection*)comp2->mFirstInConnection;
        while (conn2 != NULL)
        {
            if (conn1->mGeneTag.isEquivalentOrigin(&(conn2->mGeneTag)))
            {
                return 1;
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
            if (conn1->mGeneTag.isEquivalentTarget(&(conn2->mGeneTag)))
            {
                return 1;
            }

            conn2 = (GridbrainConnection*)conn2->mNextConnection;
        }
        conn1 = (GridbrainConnection*)conn1->mNextConnection;
    }

    return 0;
}

GridbrainGeneTag Gridbrain::findGeneTag(GridbrainConnection* conn)
{
    int bestEq = 1;
    GridbrainGeneTag tag;

    GridbrainConnection* conn2 = mConnections;

    while (conn2 != NULL)
    {
        if (conn != conn2)
        {
            GridbrainComponent* orig1 = (GridbrainComponent*)conn->mOrigComponent;
            GridbrainComponent* targ1 = (GridbrainComponent*)conn->mTargComponent;
            GridbrainComponent* orig2 = (GridbrainComponent*)conn2->mOrigComponent;
            GridbrainComponent* targ2 = (GridbrainComponent*)conn2->mTargComponent;

            int eq = compEquivalence(orig1, orig2, CET_ORIGIN) + compEquivalence(targ1, targ2, CET_TARGET);

            if (eq > bestEq)
            {
                bestEq = eq;
                tag = conn2->mGeneTag;
            }

            if (bestEq == 4)
            {
                return tag;
            }
        }

        conn2 = (GridbrainConnection*)conn2->mNextGlobalConnection;
    }

    return tag;
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
            if (conn->mGeneTag.mGeneID == 0)
            {
                conn->mGeneTag = findGeneTag(conn);

                unsigned int vecPos = 0;
                while ((conn->mGeneTag.mGeneID == 0) && (vecPos < popVec->size()))
                {
                    SimulationObject* obj = (*popVec)[vecPos];
                    Agent* agent = (Agent*)obj;
                    Gridbrain* gb = (Gridbrain*)agent->getBrain();
                    conn->mGeneTag = gb->findGeneTag(conn);
                    vecPos++;
                }
            }

            // If no tag assigned, try to associate with an existing gene
            if (mGeneGrouping && (conn->mGeneTag.mGeneID == 0))
            {
                GridbrainComponent* orig = (GridbrainComponent*)conn->mOrigComponent;
                GridbrainComponent* targ = (GridbrainComponent*)conn->mTargComponent;

                unsigned int count = orig->mInboundConnections + targ->mConnectionsCount;

                if (count > 0)
                {
                    unsigned int pos = mDistRecombine->iuniform(0, count);

                    GridbrainConnection* conn2;

                    if (pos < orig->mInboundConnections)
                    {
                        conn2 = orig->mFirstInConnection;
                        for (unsigned int i = 0; i < pos; i++)
                        {
                            conn2 = (GridbrainConnection*)conn2->mNextInConnection;
                        }
                    }
                    else
                    {
                        pos -= orig->mInboundConnections;

                        conn2 = targ->mFirstConnection;
                        for (unsigned int i = 0; i < pos; i++)
                        {
                            conn2 = (GridbrainConnection*)conn2->mNextConnection;
                        }
                    }

                    conn->mGeneTag.mGeneID = conn2->mGeneTag.mGeneID;
                    conn->mGeneTag.mOrigID = GridbrainGeneTag::generateID();
                    conn->mGeneTag.mTargID = GridbrainGeneTag::generateID();
                }
            }

            // If still no tag assigned, generate new gene
            if (conn->mGeneTag.mGeneID == 0)
            {
                conn->mGeneTag.mGeneID = GridbrainGeneTag::generateID();
                conn->mGeneTag.mOrigID = GridbrainGeneTag::generateID();
                conn->mGeneTag.mTargID = GridbrainGeneTag::generateID();
            }

            conn = (GridbrainConnection*)conn->mNextConnection;
        }
    }
}

