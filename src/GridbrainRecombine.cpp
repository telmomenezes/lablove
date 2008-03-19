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

GridbrainComponent* Gridbrain::findEquivalent(GridbrainComponent* comp, Gridbrain* gb)
{
    GridbrainComponent* eqComp = NULL;

    Grid* grid = gb->mGridsVec[comp->mGrid];
    unsigned int start = grid->getOffset();
    unsigned int end = start + grid->getSize();

    for (unsigned int i = start; i < end; i++)
    {
        GridbrainComponent* curComp = &(gb->mComponents[i]);

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

void Gridbrain::clearRecombineInfo(bool selected)
{
    for (unsigned int i = 0; i < mNumberOfComponents; i++)
    {
        mComponents[i].mEquivalent = NULL;
        mComponents[i].mSelected = selected;
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

        GridbrainComponent* eqOrig = findEquivalent(orig, gb);
        GridbrainComponent* eqTarg = findEquivalent(targ, gb);

        if (eqTarg == NULL)
        {
            unsigned int targetGrid = targ->mGrid;
            unsigned int targetColumn = eqOrig->mColumn + 1;

            // Grid width needs to be expanded?
            if (brain->mGridsVec[targetGrid]->getWidth() == targetColumn)
            {
                Gridbrain* newBrain = brain->clone(targetGrid, ET_COLUMN);
                delete brain;
                brain = newBrain;
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
                    Gridbrain* newBrain = brain->clone(targetGrid, ET_ROW);
                    delete brain;
                    brain = newBrain;
                }
            }

            targ->mEquivalent = eqTarg;
        }

        unsigned int x1 = eqOrig->mColumn;
        unsigned int y1 = eqOrig->mRow;
        unsigned int g1 = eqOrig->mGrid;
        unsigned int x2 = eqTarg->mColumn;
        unsigned int y2 = eqTarg->mRow;
        unsigned int g2 = eqTarg->mGrid;
        float weight = conn->mWeight;

        if (brain->isConnectionValid(x1, y1, g1, x2, y2, g2))
        {
            addConnection(x1, y1, g1, x2, y2, g2, weight);
        }
        else
        {
            failed++;
        }

        brain = crossoverComp(brain, targ, failed);

        conn = (GridbrainConnection*)(conn->mNextConnection);
    }

    return brain;
}

Brain* Gridbrain::recombine(Brain* brain)
{
    //printf("RECOMBINE\n");

    Gridbrain* gbNew = (Gridbrain*)(this->clone());
    Gridbrain* gb2 = (Gridbrain*)(brain->clone());

    //gbNew->printDebug();
    //gb2->printDebug();

    gbNew->clearRecombineInfo(true);
    gb2->clearRecombineInfo(false);

    // find initial equivalents and pivot candidates
    unsigned int possiblePivots = 0;

    for (unsigned int i = 0; i < gbNew->mNumberOfComponents; i++)
    {
        GridbrainComponent* comp = &(gbNew->mComponents[i]);
        GridbrainComponent* eqComp = gbNew->findEquivalent(comp, gb2);

        if (eqComp != NULL)
        {
            possiblePivots++;
            comp->mEquivalent = eqComp;
            eqComp->mEquivalent = comp;
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

        if (pivot1->mEquivalent != NULL)
        {
            curPos++;
        }
        i++;
    }

    GridbrainComponent* pivot2 = pivot1->mEquivalent;

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

    //gbNew->printDebug();

    delete gb2;

    Brain* child = gbNew->clone();
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

