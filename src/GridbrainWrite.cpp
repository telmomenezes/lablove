/*
 * LOVE Lab
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

#define GRID_MARGIN 50
#define COMPONENT_SIDE 40
#define COMPONENT_MARGIN 50

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
            currentY += (grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN)) + GRID_MARGIN;
            if (grid->getWidth() > maxAlphaWidth)
            {
                maxAlphaWidth = grid->getWidth();
            }
        }
        else if (grid->getType() == Grid::BETA)
        {
            betaHeight = grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN);
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
            currentY += (grid->getHeight() * (COMPONENT_SIDE + COMPONENT_MARGIN)) + GRID_MARGIN;
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
    posY = gridY + (y * (COMPONENT_SIDE + COMPONENT_MARGIN)) + (COMPONENT_SIDE / 2);
}

string Gridbrain::write(SimulationObject* obj, PopulationManager* pop)
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
        for (unsigned int x = 0; x < grid->getWidth(); x++)
        {
            for (unsigned int y = 0; y < grid->getHeight(); y++)
            {
                GridbrainComponent* comp = getComponent(x, y, i);
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

                if ((comp->mType == GridbrainComponent::PER)
                    || (comp->mType == GridbrainComponent::ACT))
                {
                    labelY += 5;
                    string subName = pop->getInterfaceName(comp->mType == GridbrainComponent::PER, comp->mSubType);
                    if (subName == "?")
                    {
                        subName = obj->getSymbolName(comp->mOrigSymTable, comp->mOrigSymID);

                        if (subName == "?")
                        {
                            subName = obj->getTableName(comp->mOrigSymTable);
                        }
                    }
                    sprintf(buffer, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"7\" fill=\"%s\">%s</text>\n", compX, labelY, color, subName.c_str());
                    svg += buffer;
                    labelY -= 10;
                }

                sprintf(buffer, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"10\" fill=\"%s\">%s</text>\n", compX, labelY, color, comp->getName().c_str());
                svg += buffer;
            }
        }
    }

    GridbrainConnection* conn = mConnections;

    while (conn != NULL)
    {
        GridbrainComponent* origComp = (GridbrainComponent*)conn->mOrigComponent;
        GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
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

        if (fabs(conn->mWeight >= 0.5f))
        {
            sprintf(buffer, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"2\"/>\n", x1, y1, x2, y2, color);
        }
        else
        {
            sprintf(buffer, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-width=\"2\" stroke-dasharray=\"9, 5\"/>\n", x1, y1, x2, y2, color);
        }
        svg += buffer;

        float arrowLength = radius / 2;

        if (conn->mWeight > 0)
        {
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
        }
        else
        {
            x2 = centerX2 - (cosAngle * (radius + (arrowLength / 3.0f)));
            y2 = centerY2 - (sinAngle * (radius + (arrowLength / 3.0f)));

            sprintf(buffer,
                        "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\"/>\n",
                        x2,
                        y2,
                        arrowLength / 3.0f,
                        color,
                        color);
            svg += buffer;
        }
        
        /*float cLabelX = x1 + ((x2 - x1) / 2);
        float cLabelY = y1 + ((y2 - y1) / 2);

        sprintf(buffer, "<text x=\"%f\" y=\"%f\" font-family=\"Arial\" text-anchor=\"middle\" font-size=\"8\" fill=\"red\">%f</text>\n", cLabelX, cLabelY, conn->mWeight);
        svg += buffer;*/

        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
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
                GridbrainComponent* comp = getComponent(x, y, i);
                printf("%s\t", comp->getName().c_str());
            }
            printf("\n");
        }
    }

    printf("CONNECTIONS:\n");

    GridbrainConnection* conn = mConnections;
    while (conn != NULL)
    {
        Grid* gridOrig = mGridsVec[conn->mGridOrig];
        Grid* gridTarg = mGridsVec[conn->mGridTarg];
        unsigned int comp1X = conn->mColumnOrig;
        unsigned int comp1Y = conn->mRowOrig;
        unsigned int comp2X = conn->mColumnTarg;
        unsigned int comp2Y = conn->mRowTarg;

        printf("(%d, %d, %d)->(%d, %d, %d) [%f][%f]\n",
                conn->mColumnOrig,
                conn->mRowOrig,
                conn->mGridOrig,
                conn->mColumnTarg,
                conn->mRowTarg,
                conn->mGridTarg,
                conn->mWeight,
                conn->mRealWeight);

        conn = (GridbrainConnection*)(conn->mNextGlobalConnection);
    }
}
