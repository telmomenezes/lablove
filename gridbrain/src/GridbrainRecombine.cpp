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

void Gridbrain::clearRecombineInfo()
{
    Connection* conn = mConnections;

    while (conn != NULL)
    {
        conn->mSelectionState = Connection::SS_UNKNOWN;
        conn = (Connection*)conn->mNextGlobalConnection;
    }
}

Component* Gridbrain::findEquivalentComponent(Component* comp)
{
    int bestEq = 0;
    Component* eqComp = NULL;

    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        Component* comp2 = mComponents[i];

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
                                        Component* compOrig,
                                        Component*& compTarg)
{
    Gridbrain* brain = gbTarg;

    compTarg = NULL;

    unsigned int origX = compOrig->mColumn;
    unsigned int origY = compOrig->mRow;
    unsigned int g = compOrig->mGrid;

    Grid* gridOrig = gbOrig->getGrid(g);
    Grid* gridTarg = brain->getGrid(g);

    Coord coordCol = gridOrig->getColumnCoord(origX);
    Coord coordRow = gridOrig->getRowCoord(origY);

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

    // Check for replacing component and clean
    if (compOrig->isUnique())
    {
        unsigned int start = gridTarg->getOffset();
        unsigned int end = start + gridTarg->getSize();
        for (unsigned int i = start; i < end; i++)
        {
            Component* checkComp = brain->mComponents[i];
            if (checkComp->isEqual(compOrig))
            {
                Component* newComp = gridTarg->getRandomComponent();
                brain->replaceComponent(i, newComp);
            }
        }
    }

    brain->replaceComponent(compTarg->mOffset, compOrig);

    return brain;
}

Gridbrain* Gridbrain::importConnection(Gridbrain* gbTarg,
                                        Gridbrain* gbOrig,
                                        Connection* conn)
{
    Gridbrain* brain = gbTarg;

    Component* orig = (Component*)(conn->mOrigComponent);
    Component* targ = (Component*)(conn->mTargComponent);

    unsigned int g1 = orig->mGrid;
    unsigned int g2 = targ->mGrid;

    Coord cX1;
    Coord cY1;
    Coord cX2;
    Coord cY2;

    Component* eqOrig = brain->findEquivalentComponent(orig);
    Component* eqTarg = brain->findEquivalentComponent(targ);

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
    Connection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
        {
            return true;
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }

    return false;
}

bool Gridbrain::isGeneSelected(llULINT geneID)
{
    Connection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
        {
            if (conn->mSelectionState == Connection::SS_SELECTED)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }

    return false;
}

bool Gridbrain::selectGene(llULINT geneID, bool select)
{
    Connection* conn = mConnections;

    while (conn != NULL)
    {
        if (conn->mGeneTag.mGeneID == geneID)
        {
            if (select)
            {
                conn->mSelectionState = Connection::SS_SELECTED;
            }
            else
            {
                conn->mSelectionState = Connection::SS_UNSELECTED;
            }
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }
}

void Gridbrain::selectConnUniform(Gridbrain* gb1, Gridbrain* gb2)
{
    // Select connections, parent 1
    Connection* conn = gb1->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == Connection::SS_UNKNOWN)
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

        conn = (Connection*)conn->mNextGlobalConnection;
    }

    // Select connections, parent 2
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == Connection::SS_UNKNOWN)
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

        conn = (Connection*)conn->mNextGlobalConnection;
    }
}

void Gridbrain::selectPath(Component* comp)
{
    Connection* conn = comp->mFirstInConnection;
    
    while (conn != NULL)
    {
        selectGene(conn->mGeneTag.mGeneID, true);
        selectPath((Component*)conn->mOrigComponent);
        conn = (Connection*)conn->mNextInConnection;
    }
}

void Gridbrain::selectConnPaths(Gridbrain* gb1, Gridbrain* gb2)
{
    // Select paths
    for (unsigned int i = 0; i < gb1->mNumberOfComponents; i++)
    {
        Component* comp1 = gb1->mComponents[i];

        if (comp1->mActive
            && (comp1->mInboundConnections > 0)
            && (comp1->mConnectionsCount == 0))
        {
            Component* comp2 = gb2->findEquivalentComponent(comp1);

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
        Component* comp2 = gb2->mComponents[i];

        if (comp2->mActive
            && (comp2->mInboundConnections > 0)
            && (comp2->mConnectionsCount == 0))
        {
            Component* comp1 = gb1->findEquivalentComponent(comp2);

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
    Connection* conn = gb1->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == Connection::SS_SELECTED)
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
        else if (conn->mSelectionState == Connection::SS_UNKNOWN)
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

        conn = (Connection*)conn->mNextGlobalConnection;
    }
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == Connection::SS_UNKNOWN)
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

        conn = (Connection*)conn->mNextGlobalConnection;
    }
}

Gridbrain* Gridbrain::recombine(Gridbrain* brain)
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
    Connection* conn = gbNew->mConnections;
    while (conn != NULL)
    {
        Connection* nextConn = (Connection*)conn->mNextGlobalConnection;

        if (conn->mSelectionState == Connection::SS_UNSELECTED)
        {
            gbNew->removeConnection(conn);
        }

        conn = nextConn;
    }

    // Import selected connections from parent 2
    conn = gb2->mConnections;
    while (conn != NULL)
    {
        if (conn->mSelectionState == Connection::SS_SELECTED)
        {
            //printf("-> Import connection: ");
            //gb2->printConnection(conn);
            gbNew = importConnection(gbNew, gb2, conn);
        }

        conn = (Connection*)conn->mNextGlobalConnection;
    }

    gbNew->recombineUnusedComponents(this, gb2);

    gbNew->update();
    
    /*printf(">>> CHILD\n");
    gbNew->printDebug();*/

    return gbNew;
}

bool Gridbrain::exists(Component* comp, unsigned int ex, unsigned int ey)
{
    Grid* grid = mGridsVec[comp->mGrid];
    unsigned int startPos = grid->getOffset();
    unsigned int endPos = grid->getOffset() + grid->getSize();

    for (unsigned int pos = startPos; pos < endPos; pos++)
    {
        Component* comp2 = mComponents[pos];

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
            Coord colCoord = gridChild->getColumnCoord(x);
            int x1 = gridP1->getColumnByCoord(colCoord);
            int x2 = gridP2->getColumnByCoord(colCoord);

            for (unsigned int y = 0; y < gridChild->getHeight(); y++)
            {
                Component* comp = getComponent(x, y, g);

                if (!comp->isUsed())
                {
                    Coord rowCoord = gridChild->getRowCoord(y);
                    int y1 = gridP1->getRowByCoord(rowCoord);
                    int y2 = gridP2->getRowByCoord(rowCoord);

                    Component* comp1 = NULL;
                    Component* comp2 = NULL;
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
                        replaceComponent(comp->mOffset, comp1);
                    }
                    else if (choice2)
                    {
                        replaceComponent(comp->mOffset, comp2);
                    }
                }
            }
        }
    }
}

int Gridbrain::compEquivalence(Component* comp1, Component* comp2, CompEquivalenceType eqType)
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

    Connection* conn1 = (Connection*)comp1->mFirstInConnection;
    while (conn1 != NULL)
    {
        Connection* conn2 = (Connection*)comp2->mFirstInConnection;
        while (conn2 != NULL)
        {
            if (conn1->mGeneTag.isEquivalentOrigin(&(conn2->mGeneTag)))
            {
                return 1;
            }

            conn2 = (Connection*)conn2->mNextInConnection;
        }
        conn1 = (Connection*)conn1->mNextInConnection;
    }

    conn1 = (Connection*)comp1->mFirstConnection;
    while (conn1 != NULL)
    {
        Connection* conn2 = (Connection*)comp2->mFirstConnection;
        while (conn2 != NULL)
        {
            if (conn1->mGeneTag.isEquivalentTarget(&(conn2->mGeneTag)))
            {
                return 1;
            }

            conn2 = (Connection*)conn2->mNextConnection;
        }
        conn1 = (Connection*)conn1->mNextConnection;
    }

    return 0;
}

GeneTag Gridbrain::findGeneTag(Connection* conn)
{
    int bestEq = 0;
    GeneTag tag;

    Connection* conn2 = mConnections;

    while (conn2 != NULL)
    {
        if (conn != conn2)
        {
            Component* orig1 = (Component*)conn->mOrigComponent;
            Component* targ1 = (Component*)conn->mTargComponent;
            Component* orig2 = (Component*)conn2->mOrigComponent;
            Component* targ2 = (Component*)conn2->mTargComponent;

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

        conn2 = (Connection*)conn2->mNextGlobalConnection;
    }

    return tag;
}

void Gridbrain::generateGenes(vector<Gridbrain*>* brainVec)
{
    for (unsigned int pos = 0; pos < mNumberOfComponents; pos++)
    {
        Component* comp = mComponents[pos];

        Connection* conn = comp->mFirstConnection;
        while (conn != NULL)
        {
            // If no tag assigned, look for equivalent connection
            if (conn->mGeneTag.mGeneID == 0)
            {
                conn->mGeneTag = findGeneTag(conn);

                unsigned int vecPos = 0;
                while ((conn->mGeneTag.mGeneID == 0) && (vecPos < brainVec->size()))
                {
                    Gridbrain* gb = (*brainVec)[vecPos];
                    conn->mGeneTag = gb->findGeneTag(conn);
                    vecPos++;
                }
            }

            // If no tag assigned, generate new gene
            if (conn->mGeneTag.mGeneID == 0)
            {
                conn->mGeneTag.mGeneID = GeneTag::generateID();
                conn->mGeneTag.mOrigID = GeneTag::generateID();
                conn->mGeneTag.mTargID = GeneTag::generateID();
            }

            conn = (Connection*)conn->mNextConnection;
        }
    }
}

