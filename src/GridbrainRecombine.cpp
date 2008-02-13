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
    //printf("RECOMBINE\n");

    Gridbrain* gb1 = this;
    Gridbrain* gb2 = (Gridbrain*)brain;
    Gridbrain* gbNew = gb1->baseClone();

    //gb1->printDebug();
    //gb2->printDebug();

    // Crossover grids from parents to offspring
    for (unsigned int i = 0; i < gb1->mGridsCount; i++)
    {
        Grid* grid = gb1->mGridsVec[i];
        Grid* newGrid = new Grid(*grid);

        // If crossover is not possible in any grid, give up and return first brain's clone
        if (!newGrid->crossover(gb1->mGridsVec[i], gb2->mGridsVec[i]))
        {
            delete newGrid;
            delete gbNew;
            return gb1->clone();
        }

        gbNew->addGrid(newGrid, "");
    }

    gbNew->calcConnectionCounts();

    // Copy components from parents
    gbNew->mComponents = (GridbrainComponent*)malloc(gbNew->mNumberOfComponents * sizeof(GridbrainComponent));

    unsigned int index = 0;
    for (unsigned int gridIndex = 0; gridIndex < mGridsCount; gridIndex++)
    {
        Grid* grid1 = gb1->mGridsVec[gridIndex];
        Grid* grid2 = gb2->mGridsVec[gridIndex];
        Grid* newGrid = gbNew->mGridsVec[gridIndex];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            GridCoord colCoord = newGrid->getColumnCoord(x);
            int x1 = grid1->getColumnByCoord(colCoord);
            int x2 = grid2->getColumnByCoord(colCoord);

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                gbNew->mComponents[index].clearDefinitions();
                gbNew->mComponents[index].clearConnections();
                gbNew->mComponents[index].clearMetrics();
        
                GridbrainComponent* originComp;

                GridCoord rowCoord = newGrid->getRowCoord(y);
                int y1 = grid1->getRowByCoord(rowCoord);
                int y2 = grid2->getRowByCoord(rowCoord);
                
                int orig = colCoord.mXoverOrigin + rowCoord.mXoverOrigin;

                if (orig == 1)
                {
                    if ((x1 >= 0) && (y1 >= 0))
                    {
                        //printf("1");
                        originComp = gb1->getComponent(x1, y1, gridIndex);
                    }
                    else if ((x2 >= 0) && (y2 >= 0))
                    {
                        //printf("2");
                        originComp = gb2->getComponent(x2, y2, gridIndex);
                    }
                    else
                    {
                        //printf("3");
                        originComp = newGrid->getRandomComponent(mOwner);
                    }
                }
                else
                {
                    if ((x2 >= 0) && (y2 >= 0))
                    {
                        //printf("4");
                        originComp = gb2->getComponent(x2, y2, gridIndex);
                    }
                    else if ((x1 >= 0) && (y1 >= 0))
                    {
                        //printf("5");
                        originComp = gb1->getComponent(x1, y1, gridIndex);
                    }
                    else
                    {
                        //printf("6");
                        originComp = newGrid->getRandomComponent(mOwner);
                    }
                }
                
                gbNew->mComponents[index].copyDefinitions(originComp);

                gbNew->mComponents[index].mOffset = index;
                gbNew->mComponents[index].mColumn = x;
                gbNew->mComponents[index].mRow = y;
                gbNew->mComponents[index].mGrid = gridIndex;

                index++;
            }
            //printf("\n");
        }
    }

    // Copy connections from parents
    for (unsigned int gridIndex = 0; gridIndex < mGridsCount; gridIndex++)
    {
        Grid* grid1 = gb1->mGridsVec[gridIndex];
        Grid* grid2 = gb2->mGridsVec[gridIndex];
        Grid* newGrid = gbNew->mGridsVec[gridIndex];

        for (unsigned int x = 0;
            x < newGrid->getWidth();
            x++)
        {
            GridCoord colCoord = newGrid->getColumnCoord(x);
            int x1 = grid1->getColumnByCoord(colCoord);
            int x2 = grid2->getColumnByCoord(colCoord);

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                GridbrainComponent* originComp;
                Gridbrain* origBrain;

                GridCoord rowCoord = newGrid->getRowCoord(y);
                int y1 = grid1->getRowByCoord(rowCoord);
                int y2 = grid2->getRowByCoord(rowCoord);
                
                char orig = colCoord.mXoverOrigin + rowCoord.mXoverOrigin;

                if (orig == 1)
                {
                    if ((x1 >= 0) && (y1 >= 0))
                    {
                        originComp = gb1->getComponent(x1, y1, gridIndex);
                        origBrain = gb1;
                    }
                    else if ((x2 >= 0) && (y2 >= 0))
                    {
                        originComp = gb2->getComponent(x2, y2, gridIndex);
                        origBrain = gb2;
                    }
                    else
                    {
                        originComp = newGrid->getRandomComponent(mOwner);
                        origBrain = gbNew;
                    }
                }
                else
                {
                    if ((x2 >= 0) && (y2 >= 0))
                    {
                        originComp = gb2->getComponent(x2, y2, gridIndex);
                        origBrain = gb2;
                    }
                    else if ((x1 >= 0) && (y1 >= 0))
                    {
                        originComp = gb1->getComponent(x1, y1, gridIndex);
                        origBrain = gb1;
                    }
                    else
                    {
                        originComp = newGrid->getRandomComponent(mOwner);
                        origBrain = gbNew;
                    }
                }

                GridbrainConnection* conn = originComp->mFirstConnection;
                while (conn != NULL)
                {
                    int rowOrig = conn->mRowOrig;
                    int rowTarg = conn->mRowTarg;
                    int columnOrig = conn->mColumnOrig;
                    int columnTarg = conn->mColumnTarg;

                    GridCoord coordRowOrig = origBrain->mGridsVec[conn->mGridOrig]->getRowCoord(rowOrig);
                    GridCoord coordColumnOrig = origBrain->mGridsVec[conn->mGridOrig]->getColumnCoord(columnOrig);
                    GridCoord coordRowTarg = origBrain->mGridsVec[conn->mGridTarg]->getRowCoord(rowTarg);
                    GridCoord coordColumnTarg = origBrain->mGridsVec[conn->mGridTarg]->getColumnCoord(columnTarg);

                    rowOrig = gbNew->mGridsVec[conn->mGridOrig]->getRowByCoord(coordRowOrig);
                    columnOrig = gbNew->mGridsVec[conn->mGridOrig]->getColumnByCoord(coordColumnOrig);
                    rowTarg = gbNew->mGridsVec[conn->mGridTarg]->getRowByCoord(coordRowTarg);
                    columnTarg = gbNew->mGridsVec[conn->mGridTarg]->getColumnByCoord(coordColumnTarg);

                    if ((rowOrig >= 0)
                        && (columnOrig >= 0)
                        && (rowTarg >= 0)
                        && (columnTarg >= 0))
                    {
                        /*printf("addConnection %d, %d, %d -> %d, %d, %d [%f / %f]\n",
                                                columnOrig,
                                                rowOrig,
                                                conn->mGridOrig,
                                                columnTarg,
                                                rowTarg,
                                                conn->mGridTarg,
                                                conn->mWeight,
                                                conn->mAge + 1.0f);*/
                        gbNew->addConnection(columnOrig,
                                                rowOrig,
                                                conn->mGridOrig,
                                                columnTarg,
                                                rowTarg,
                                                conn->mGridTarg,
                                                conn->mWeight,
                                                conn->mAge + 1.0f);
                    }

                    conn = (GridbrainConnection*)conn->mNextConnection;
                }
            }
        }
    }
    
    //gbNew->printDebug();

    return gbNew;
}

