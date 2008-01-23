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
    Gridbrain* gb1 = this;
    Gridbrain* gb2 = (Gridbrain*)brain;
    Gridbrain* gbNew = gb1->baseClone();

    unsigned int columnCount = 0;
    for (unsigned int i = 0; i < gb1->mGridsCount; i++)
    {
        columnCount += gb1->mGridsVec[i]->getWidth();
    }

    // Determine crossover (xover) grid and column in first parent
    int xoverCol1 = mDistConnections->iuniform(0, columnCount);

    int xoverGrid = -1;
    unsigned int limitCol = 0;

    while (xoverCol1 >= limitCol)
    {
        xoverCol1 -= limitCol;
        xoverGrid++;
        limitCol = gb1->mGridsVec[xoverGrid]->getWidth();
    }

    // Determine equivalent xover point in the second parent
    int xoverCol2 = -1;
    int xoverColIter = xoverCol1;

    while (xoverCol2 == -1)
    {
        if (xoverColIter >= gb1->mGridsVec[xoverGrid]->getWidth())
        {
            xoverCol2 = gb2->mGridsVec[xoverGrid]->getWidth() - 1;
        }
        else
        {
            unsigned long code = gb1->mGridsVec[xoverGrid]->getColumnCode(xoverColIter);
            xoverCol2 = gb2->mGridsVec[xoverGrid]->getColumnByCode(code);
            xoverColIter++;
        }
    }

    // Copy grids from parents to offspring
    for (unsigned int i = 0; i < gb1->mGridsCount; i++)
    {
        Grid* grid = gb1->mGridsVec[i];
        Grid* newGrid = new Grid(*grid);

        if (i == xoverGrid)
        {
            newGrid->crossoverColumn(gb2->mGridsVec[i], xoverCol1, xoverCol2);
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
            unsigned long colCode;
            int x1;
            int x2;
            if (gridIndex == xoverGrid)
            {
                colCode = newGrid->getColumnCode(x);
                x1 = grid1->getColumnByCode(colCode);
                x2 = grid2->getColumnByCode(colCode);
            }

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                gbNew->mComponents[index].clearDefinitions();
                gbNew->mComponents[index].clearConnections();
                gbNew->mComponents[index].clearMetrics();
        
                GridbrainComponent* originComp;
                Gridbrain* origBrain;

                if (gridIndex == xoverGrid)
                {
                    unsigned long rowCode = newGrid->getRowCode(y);
                    int y1 = grid1->getRowByCode(rowCode);
                    int y2 = grid2->getRowByCode(rowCode);

                    if (x >= xoverCol1)
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
                            originComp = newGrid->getRandomComponent();
                            origBrain = gbNew;
                        }
                    }
                    else
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
                            originComp = newGrid->getRandomComponent();
                            origBrain = gbNew;
                        }
                    }
                }
                else
                {
                    originComp = gb1->getComponent(x, y, gridIndex);
                    origBrain = gb1;
                }

                gbNew->mComponents[index].copyDefinitions(originComp);

                gbNew->mComponents[index].mOffset = index;
                gbNew->mComponents[index].mColumn = x;
                gbNew->mComponents[index].mRow = y;
                gbNew->mComponents[index].mGrid = gridIndex;

                index++;
            }
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
            unsigned long colCode;
            int x1;
            int x2;
            if (gridIndex == xoverGrid)
            {
                colCode = newGrid->getColumnCode(x);
                x1 = grid1->getColumnByCode(colCode);
                x2 = grid2->getColumnByCode(colCode);
            }

            for (unsigned int y = 0;
                y < newGrid->getHeight();
                y++)
            {
                GridbrainComponent* originComp;
                Gridbrain* origBrain;

                if (gridIndex == xoverGrid)
                {
                    unsigned long rowCode = newGrid->getRowCode(y);
                    int y1 = grid1->getRowByCode(rowCode);
                    int y2 = grid2->getRowByCode(rowCode);

                    if (x >= xoverCol1)
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
                            originComp = gbNew->getComponent(x, y, gridIndex);
                            origBrain = gbNew;
                        }
                    }
                    else
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
                            originComp = gbNew->getComponent(x, y, gridIndex);
                            origBrain = gbNew;
                        }
                    }
                }
                else
                {
                    originComp = gb1->getComponent(x, y, gridIndex);
                    origBrain = gb1;
                }

                GridbrainConnection* conn = originComp->mFirstConnection;
                while (conn != NULL)
                {
                    int rowOrig = conn->mRowOrig;
                    int rowTarg = conn->mRowTarg;
                    int columnOrig = conn->mColumnOrig;
                    int columnTarg = conn->mColumnTarg;

                    unsigned long codeRowOrig = origBrain->mGridsVec[conn->mGridOrig]->getRowCode(rowOrig);
                    unsigned long codeColumnOrig = origBrain->mGridsVec[conn->mGridOrig]->getColumnCode(columnOrig);
                    unsigned long codeRowTarg = origBrain->mGridsVec[conn->mGridTarg]->getRowCode(rowTarg);
                    unsigned long codeColumnTarg = origBrain->mGridsVec[conn->mGridTarg]->getColumnCode(columnTarg);

                    rowOrig = gbNew->mGridsVec[conn->mGridOrig]->getRowByCode(codeRowOrig);
                    columnOrig = gbNew->mGridsVec[conn->mGridOrig]->getColumnByCode(codeColumnOrig);
                    rowTarg = gbNew->mGridsVec[conn->mGridTarg]->getRowByCode(codeRowTarg);
                    columnTarg = gbNew->mGridsVec[conn->mGridTarg]->getColumnByCode(codeColumnTarg);

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

    return gbNew;
}

