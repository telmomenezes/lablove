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

#include <string.h>
#include <stdexcept>
#include <math.h>

#define GRID_MARGIN 50
#define COMPONENT_SIDE 40
#define COMPONENT_MARGIN 50
#define GRID_TITLE 10

namespace gb
{

void Gridbrain::initGridWritePositions()
{
    unsigned int currentY = 0;
    unsigned int maxAlphaWidth = 0;
    unsigned int betaHeight = 0;

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        if (grid->getType() == Grid::ALPHA)
        {
            grid->setWritePos(GRID_MARGIN, currentY);
            currentY += (grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN)) + GRID_MARGIN + GRID_TITLE;
            if (grid->getWidth() > maxAlphaWidth)
            {
                maxAlphaWidth = grid->getWidth();
            }
        }
        else if (grid->getType() == Grid::BETA)
        {
            betaHeight = (grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN)) + GRID_TITLE;
        }
    }

    unsigned int alphaHeight = currentY - GRID_MARGIN;

    unsigned int alphaY = GRID_MARGIN;
    unsigned int betaY = GRID_MARGIN;

    if (alphaHeight > betaHeight)
    {
        betaY = (alphaHeight / 2) - (betaHeight / 2) + GRID_MARGIN;
    }
    else if (betaHeight > alphaHeight)
    {
        alphaY = (betaHeight / 2) - (alphaHeight / 2) + GRID_MARGIN;
    }

    currentY = alphaY;

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        if (grid->getType() == Grid::ALPHA)
        {
            grid->setWritePos(GRID_MARGIN, currentY);
            currentY += (grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN)) + GRID_MARGIN + GRID_TITLE;
        }
        else if (grid->getType() == Grid::BETA)
        {
            unsigned int betaX = (maxAlphaWidth * (COMPONENT_SIDE + COMPONENT_MARGIN)) + (COMPONENT_MARGIN * 2);
            grid->setWritePos(betaX, betaY);
        }
    }
}

void Gridbrain::getComponentWritePos(unsigned int& posX,
                                        unsigned int& posY,
                                        unsigned int x,
                                        unsigned int y,
                                        unsigned int grid)
{
    Grid* pGrid = mGridsVec[grid];
    unsigned int gridX = pGrid->getWriteX();
    unsigned int gridY = pGrid->getWriteY();
    posX = gridX + (x * (COMPONENT_SIDE + COMPONENT_MARGIN)) + (COMPONENT_SIDE / 2); 
    posY = gridY + (y * (COMPONENT_SIDE + COMPONENT_MARGIN)) + (COMPONENT_SIDE / 2) + GRID_TITLE;
}

string Gridbrain::write()
{
    string svg;
    char buffer[1000];

    char* black = "black";
    char* gray = "#707070";
    char* color = black;

    svg = "<?xml version=\"1.0\" standalone=\"no\"?>\n";
    svg += "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    svg += "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        Grid* grid = mGridsVec[i];

        string gridName = getChannelName(i);

        if (gridName == "")
        {
            if (grid->getType() == Grid::ALPHA)
            {
                gridName = "alpha";
            }
            else
            {
                gridName = "beta";
            }
        }

        sprintf(buffer,
                "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" text-anchor=\"left\" font-size=\"12\" fill=\"black\">%s</text>\n",
                grid->getWriteX(),
                grid->getWriteY(),
                gridName.c_str());
        svg += buffer;

        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                Component* comp = getComponent(x, y, i);
                if (comp->mActive)
                {
                    color = black;
                }
                else
                {
                    color = gray;
                }

                unsigned int compX; 
                unsigned int compY; 
                getComponentWritePos(compX, compY, x, y, i);
                sprintf(buffer,
                        "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"white\" stroke=\"%s\" stroke-width=\"3\"/>\n",
                        compX,
                        compY,
                        COMPONENT_SIDE / 2,
                        color);
                svg += buffer;

                int labelY = compY + 3;

                string label = comp->getLabel();
                if (comp->getLabel() != "")
                {
                    labelY += 5;
                    sprintf(buffer,
                        "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"7\" fill=\"%s\">%s</text>\n",
                        compX,
                        labelY,
                        color,
                        label.c_str());
                    svg += buffer;
                    labelY -= 10;
                }

                sprintf(buffer,
                        "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"10\" fill=\"%s\">%s</text>\n",
                        compX,
                        labelY,
                        color,
                        comp->getName().c_str());
                svg += buffer;
            }
        }
    }

    Connection* conn = mConnections;

    while (conn != NULL)
    {
        Component* origComp = (Component*)conn->mOrigComponent;
        Component* targComp = (Component*)conn->mTargComponent;
        if (origComp->mActive && targComp->mActive)
        {
            color = black;
        }
        else
        {
            color = gray;
        }

        Grid* gridOrig = mGridsVec[conn->mGridOrig];
        Grid* gridTarg = mGridsVec[conn->mGridTarg];
        unsigned int comp1X = conn->mColumnOrig;
        unsigned int comp1Y = conn->mRowOrig;
        unsigned int comp2X = conn->mColumnTarg;
        unsigned int comp2Y = conn->mRowTarg;
        unsigned int centerX1;
        unsigned int centerX2;
        unsigned int centerY1;
        unsigned int centerY2;

        getComponentWritePos(centerX1, centerY1, comp1X, comp1Y, conn->mGridOrig);
        getComponentWritePos(centerX2, centerY2, comp2X, comp2Y, conn->mGridTarg);

        float deltaX = (float)centerX2 - (float)centerX1;
        float deltaY = (float)centerY2 - (float)centerY1;
        float angle = atan2f(deltaY, deltaX);
        float sinAngle = sinf(angle);
        float cosAngle = cosf(angle);
        float radius = COMPONENT_SIDE / 2.0f;

        float x1 = centerX1 + (cosAngle * radius);
        float y1 = centerY1 + (sinAngle * radius);
        float x2 = centerX2 - (cosAngle * radius * 1.3f);
        float y2 = centerY2 - (sinAngle * radius * 1.3f);

        sprintf(buffer, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"2\"/>\n", x1, y1, x2, y2, color);

        svg += buffer;

        float arrowLength = radius / 2;

        float halfArrowAngle = 0.25;
        float arrowAngle1 = angle - halfArrowAngle;
        float arrowAngle2 = angle + halfArrowAngle;
        
        x2 = centerX2 - (cosAngle * radius);
        y2 = centerY2 - (sinAngle * radius);
        float ax1 = x2 - (cosf(arrowAngle1) * arrowLength);
        float ay1 = y2 - (sinf(arrowAngle1) * arrowLength);
        float ax2 = x2 - (cosf(arrowAngle2) * arrowLength);
        float ay2 = y2 - (sinf(arrowAngle2) * arrowLength);

        sprintf(buffer,
            "<polygon fill=\"%s\" stroke=\"%s\" stroke-width=\"1\" points=\"%f,%f %f,%f %f,%f\" />\n",
            color, color, x2, y2, ax1, ay1, ax2, ay2);
        svg += buffer;
        
        /*float cLabelX = x1 + ((x2 - x1) / 2);
        float cLabelY = y1 + ((y2 - y1) / 2);

        sprintf(buffer, "<text x=\"%f\" y=\"%f\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"8\" fill=\"red\">%f</text>\n", cLabelX, cLabelY, conn->mWeight);
        svg += buffer;*/

        conn = (Connection*)(conn->mNextGlobalConnection);
    }

    svg += "</svg>\n";

    return svg;
}

void Gridbrain::printDebug()
{
    printf("\n\n== GRIDBRAIN ==\n");

    for (unsigned int i = 0; i < mGridsCount; i++)
    {
        printf("Grid: %d\n", i);
        Grid* grid = mGridsVec[i];
        for (unsigned int y = 0; y < grid->getHeight(); y++)
        {
            for (unsigned int x = 0; x < grid->getWidth(); x++)
            {
                Component* comp = getComponent(x, y, i);
                printf("%s(%s)\t", comp->getName().c_str(), comp->getLabel().c_str());
            }
            printf("\n");
        }
    }

    printf("CONNECTIONS:\n");

    Connection* conn = mConnections;
    while (conn != NULL)
    {
        printConnection(conn);

        conn = (Connection*)(conn->mNextGlobalConnection);
    }
}

void Gridbrain::printConnection(Connection* conn)
{
    Component* comp1 = (Component*)conn->mOrigComponent;
    Component* comp2 = (Component*)conn->mTargComponent;

    comp1->print();
    printf(" -> ");
    comp2->print();

    conn->mGene.print();

    switch (conn->mSelectionState)
    {
    case Connection::SS_UNKNOWN:
        printf(" *UNKNOWN*");
        break;
    case Connection::SS_SELECTED:
        printf(" *SELECTED*");
        break;
    case Connection::SS_UNSELECTED:
        printf(" *UNSELECTED*");
        break;
    }

    printf("\n");
}

}

