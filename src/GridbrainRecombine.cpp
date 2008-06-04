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
#include "SymbolTable.h"

void Gridbrain::setSelectedSymbols(SimObj* obj)
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

                if (symbolUsed(tableID, symID))
                {
                    sym->mSelected = true;
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

GridbrainComponent* Gridbrain::findEquivalentComponent(GridbrainComponent* comp)
{
    int bestEq = 0;
    GridbrainComponent* eqComp = NULL;

    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        GridbrainComponent* comp2 = &mComponents[i];

        int eq = compEquivalence(comp, comp2, CET_NEW);

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

Gridbrain* Gridbrain::importComponent(Gridbrain* gbTarg,
                                        Gridbrain* gbOrig,
                                        GridbrainComponent* compOrig,
                                        GridbrainComponent*& compTarg)
{
    Gridbrain* brain = gbTarg;

    compTarg = NULL;

    unsigned int origX = compOrig->mColumn;
    unsigned int origY = compOrig->mRow;
    unsigned int g = compOrig->mGrid;

    Grid* gridOrig = gbOrig->getGrid(g);
    Grid* gridTarg = brain->getGrid(g);

    GridCoord coordCol = gridOrig->getColumnCoord(origX);
    GridCoord coordRow = gridOrig->getRowCoord(origY);

    int targX = gridTarg->getColumnByCoord(coordCol);
    int targY = gridTarg->getRowByCoord(coordRow);

    if (targX == -1)
    {
        Gridbrain* newBrain = brain->clone(false, ET_COLUMN, g, &coordCol);
        delete brain;
        brain = newBrain;
        gridTarg = brain->getGrid(g);
        targX = gridTarg->getColumnByCoord(coordCol);
    }
    if (targY == -1)
    {
        Gridbrain* newBrain = brain->clone(false, ET_ROW, g, &coordRow);
        delete brain;
        brain = newBrain;
        gridTarg = brain->getGrid(g);
        targY = gridTarg->getRowByCoord(coordRow);
    }

    compTarg = brain->getComponent(targX, targY, g);

    while (compTarg->isUsed())
    {
        for (unsigned int y = 0;
                (y < gridTarg->getHeight()) && (compTarg->isUsed());
                y++)
        {
            compTarg = brain->getComponent(targX, y, g);
        }

        if (compTarg->isUsed())
        {
            Gridbrain* newBrain = brain->clone(false, ET_ROW, g);
            delete brain;
            brain = newBrain;
            gridTarg = brain->getGrid(g);
            compTarg = brain->getComponent(targX, 0, g);
        }
    }

    compTarg->copyDefinitions(compOrig);

    return brain;
}

Gridbrain* Gridbrain::importConnection(Gridbrain* gbTarg,
                                        Gridbrain* gbOrig,
                                        GridbrainConnection* conn)
{
    Gridbrain* brain = gbTarg;

    GridbrainComponent* orig = (GridbrainComponent*)(conn->mOrigComponent);
    GridbrainComponent* targ = (GridbrainComponent*)(conn->mTargComponent);

    unsigned int g1 = orig->mGrid;
    unsigned int g2 = targ->mGrid;

    GridCoord cX1;
    GridCoord cY1;
    GridCoord cX2;
    GridCoord cY2;

    GridbrainComponent* eqOrig = brain->findEquivalentComponent(orig);
    GridbrainComponent* eqTarg = brain->findEquivalentComponent(targ);

    Grid* grid1 = brain->getGrid(g1);
    Grid* grid2 = brain->getGrid(g2);

    if (eqOrig != NULL)
    {
        cX1 = grid1->getColumnCoord(eqOrig->mColumn);
        cY1 = grid1->getRowCoord(eqOrig->mRow);
    }
    if (eqTarg != NULL)
    {
        cX2 = grid2->getColumnCoord(eqTarg->mColumn);
        cY2 = grid2->getRowCoord(eqTarg->mRow);
    }

    if (eqOrig == NULL)
    {
        brain = importComponent(brain, gbOrig, orig, eqOrig);
        grid1 = brain->getGrid(g1);
        grid2 = brain->getGrid(g2);
        cX1 = grid1->getColumnCoord(eqOrig->mColumn);
        cY1 = grid1->getRowCoord(eqOrig->mRow);
    }

    if (eqTarg == NULL)
    {
        brain = importComponent(brain, gbOrig, targ, eqTarg);
        grid1 = brain->getGrid(g1);
        grid2 = brain->getGrid(g2);
        cX2 = grid2->getColumnCoord(eqTarg->mColumn);
        cY2 = grid2->getRowCoord(eqTarg->mRow);
    }

    unsigned int x1 = grid1->getColumnByCoord(cX1);
    unsigned int y1 = grid1->getRowByCoord(cY1);
    unsigned int x2 = grid2->getColumnByCoord(cX2);
    unsigned int y2 = grid2->getRowByCoord(cY2);

    brain->addConnection(x1, y1, g1, x2, y2, g2, conn->mGeneTag);

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

bool Gridbrain::isGeneSelected(llULINT geneID)
{
    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
        {
            if (conn->mSelectionState == GridbrainConnection::SS_SELECTED)
            {
                return true;
            }
            else
            {
                return false;
            }
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

void Gridbrain::selectConnUniform(Gridbrain* gb1, Gridbrain* gb2)
{
    // Select connections, parent 1
    GridbrainConnection* conn = gb1->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (gb2->checkGene(conn->mGeneTag.mGeneID))
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb1->selectGene(conn->mGeneTag.mGeneID, true);
                    gb2->selectGene(conn->mGeneTag.mGeneID, false);
                }
                else
                {
                    gb1->selectGene(conn->mGeneTag.mGeneID, false);
                    gb2->selectGene(conn->mGeneTag.mGeneID, true);
                }
            }
            else
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb1->selectGene(conn->mGeneTag.mGeneID, true);
                }
                else
                {
                    gb1->selectGene(conn->mGeneTag.mGeneID, false);
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
}

void Gridbrain::selectPath(GridbrainComponent* comp)
{
    GridbrainConnection* conn = comp->mFirstInConnection;
    
    while (conn != NULL)
    {
        selectGene(conn->mGeneTag.mGeneID, true);
        selectPath((GridbrainComponent*)conn->mOrigComponent);
        conn = (GridbrainConnection*)conn->mNextInConnection;
    }
}

void Gridbrain::selectConnPaths(Gridbrain* gb1, Gridbrain* gb2)
{
    // Select paths
    for (unsigned int i = 0; i < gb1->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp1 = &gb1->mComponents[i];

        if (comp1->mActive
            && (comp1->mInboundConnections > 0)
            && (comp1->mConnectionsCount == 0))
        {
            GridbrainComponent* comp2 = gb2->findEquivalentComponent(comp1);

            if (comp2 == NULL)
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb1->selectPath(comp1);
                }
            }
            else
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb1->selectPath(comp1);
                }
                else
                {
                    gb2->selectPath(comp2);
                }
            }
        }
    }
    for (unsigned int i = 0; i < gb2->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp2 = &gb2->mComponents[i];

        if (comp2->mActive
            && (comp2->mInboundConnections > 0)
            && (comp2->mConnectionsCount == 0))
        {
            GridbrainComponent* comp1 = gb1->findEquivalentComponent(comp2);

            if (comp1 == NULL)
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb2->selectPath(comp2);
                }
            }
        }
    }

    // Decide on overalping genes and unknown genes
    GridbrainConnection* conn = gb1->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_SELECTED)
        {
            if (gb2->isGeneSelected(conn->mGeneTag.mGeneID))
            {
                if (mDistRecombine->iuniform(0, 2) == 0)
                {
                    gb2->selectGene(conn->mGeneTag.mGeneID, false);
                }
                else
                {
                    gb1->selectGene(conn->mGeneTag.mGeneID, false);
                }
            }
        }
        else if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (!conn->mActive)
            {
                if (gb2->checkGene(conn->mGeneTag.mGeneID))
                {
                    if (mDistRecombine->iuniform(0, 2) == 0)
                    {
                        gb1->selectGene(conn->mGeneTag.mGeneID, true);
                        gb2->selectGene(conn->mGeneTag.mGeneID, false);
                    }
                    else
                    {
                        gb1->selectGene(conn->mGeneTag.mGeneID, false);
                        gb2->selectGene(conn->mGeneTag.mGeneID, true);
                    }
                }
                else
                {
                    if (mDistRecombine->iuniform(0, 2) == 0)
                    {
                        gb1->selectGene(conn->mGeneTag.mGeneID, true);
                    }
                    else
                    {
                        gb1->selectGene(conn->mGeneTag.mGeneID, false);
                    }
                }
            }
            else
            {
                gb1->selectGene(conn->mGeneTag.mGeneID, false);
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_UNKNOWN)
        {
            if (!conn->mActive)
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
            else
            {
                gb2->selectGene(conn->mGeneTag.mGeneID, false);
            }
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }
}

Brain* Gridbrain::recombine(Brain* brain)
{
    //printf("\n\n=== START RECOMBINE ===\n");

    Gridbrain* gbNew = (Gridbrain*)(this->clone(false));
    Gridbrain* gb2 = (Gridbrain*)brain;

    gbNew->clearRecombineInfo();
    gb2->clearRecombineInfo();

    switch (mRecombinationType)
    {
    case RT_UNIFORM:
        selectConnUniform(gbNew, gb2);
        break;
    case RT_PATHS:
        selectConnPaths(gbNew, gb2);
        break;
    }

    /*printf(">>> PARENT 1\n");
    gbNew->printDebug();
    printf(">>> PARENT 2\n");
    gb2->printDebug();*/

    // Remove unselected connections from child
    GridbrainConnection* conn = gbNew->mConnections;
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
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == GridbrainConnection::SS_SELECTED)
        {
            //printf("-> Import connection: ");
            //gb2->printConnection(conn);
            gbNew = importConnection(gbNew, gb2, conn);
        }

        conn = (GridbrainConnection*)conn->mNextGlobalConnection;
    }

    gbNew->recombineUnusedComponents(this, gb2);

    gbNew->generateMemory();
    gbNew->update();
    
    /*printf(">>> CHILD\n");
    gbNew->printDebug();*/

    return gbNew;
}

bool Gridbrain::exists(GridbrainComponent* comp, unsigned int ex, unsigned int ey)
{
    Grid* grid = mGridsVec[comp->mGrid];
    unsigned int startPos = grid->getOffset();
    unsigned int endPos = grid->getOffset() + grid->getSize();

    for (unsigned int pos = startPos; pos < endPos; pos++)
    {
        GridbrainComponent* comp2 = &mComponents[pos];

        if ((comp2->mColumn != ex) || (comp2->mRow != ey))
        {
            if (comp->isEqual(comp2))
            {
                return true;
            }
        }
    }

    return false;
}

void Gridbrain::recombineUnusedComponents(Gridbrain* gb1, Gridbrain* gb2)
{
    for (unsigned int g = 0; g < mGridsCount; g++)
    {
        Grid* gridChild = mGridsVec[g];
        Grid* gridP1 = gb1->getGrid(g);
        Grid* gridP2 = gb2->getGrid(g);
        
        for (unsigned int x = 0; x < gridChild->getWidth(); x++)
        {
            GridCoord colCoord = gridChild->getColumnCoord(x);
            int x1 = gridP1->getColumnByCoord(colCoord);
            int x2 = gridP2->getColumnByCoord(colCoord);

            for (unsigned int y = 0; y < gridChild->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, g);

                if (!comp->isUsed())
                {
                    GridCoord rowCoord = gridChild->getRowCoord(y);
                    int y1 = gridP1->getRowByCoord(rowCoord);
                    int y2 = gridP2->getRowByCoord(rowCoord);

                    GridbrainComponent* comp1 = NULL;
                    GridbrainComponent* comp2 = NULL;
                    bool choice1 = false;
                    bool choice2 = false;

                    if ((x1 >= 0) && (y1 >= 0))
                    {
                        comp1 = gb1->getComponent(x1, y1, g);
                        choice1 = true;
                        if ((comp1->isUnique()) && (exists(comp1, x, y)))
                        {
                            choice1 = false;
                        }
                    }
                    if ((x2 >= 0) && (y2 >= 0))
                    {
                        comp2 = gb2->getComponent(x2, y2, g);
                        choice2 = true;
                        if ((comp2->isUnique()) && (exists(comp2, x, y)))
                        {
                            choice2 = false;
                        }
                    }

                    if (choice1 && choice2)
                    {
                        if (mDistRecombine->iuniform(0, 2) == 0)
                        {
                            choice1 = false;
                        }
                        else
                        {
                            choice2 = false;
                        }
                    }

                    if (choice1)
                    {
                        comp->copyDefinitions(comp1);
                    }
                    else if (choice2)
                    {
                        comp->copyDefinitions(comp2);
                    }
                }
            }
        }
    }
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
    int bestEq = 0;
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

            int eq1 = compEquivalence(orig1, orig2, CET_ORIGIN);
            int eq2 = compEquivalence(targ1, targ2, CET_TARGET);

            int eq = 0;
            if ((eq1 > 0) && (eq2 > 0))
            {
                eq = eq1 + eq2;
            }

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

void Gridbrain::popAdjust(vector<SimObj*>* popVec)
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
                    SimObj* obj = (*popVec)[vecPos];
                    Gridbrain* gb = (Gridbrain*)obj->getBrain();
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

