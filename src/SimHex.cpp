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

#include "SimHex.h"

SimHex::SimHex(lua_State* luaState)
{
    mHexSide = 0;
    mHexA = 0;
    mHexB = 0;
    mHexCellWidth = 0;
    mHexCellLength = 0;
    mHexMap = NULL;
}

SimHex::~SimHex()
{
    if (mHexMap != NULL)
    {
        free(mHexMap);
        mHexMap = NULL;
    }
}

void SimHex::setWorldDimensions(float worldWidth,
                                float worldLength,
                                unsigned int cellSide,
                                float hexSide)
{
    SimCont2D::setWorldDimensions(worldWidth, worldLength, cellSide);

    mHexSide = hexSide;
    mHexA = 0.866f * mHexSide;
    mHexB = 0.5f * mHexSide;

    int cellX;
    int cellY;
    getHexCell(worldWidth - 1, worldLength - 1, cellX, cellY);
    mHexCellWidth = cellX + 1;
    mHexCellLength = cellY + 1;

    mHexMap = (HexCell*)malloc(mHexCellWidth * mHexCellLength * sizeof(HexCell));
}

void SimHex::drawTerrain()
{
    SimCont2D::drawTerrain();

    art_setColor(0, 0, 0, 255);
    art_setLineWidth(1.0f);

    float x = 0;
    float y = 0;

    for (int hx = 0; hx < mHexCellWidth; hx++)
    {
        for (int hy = 0; hy < mHexCellLength; hy ++)
        {
            float x = ((float)hx) * 2 * mHexA;
            float y = ((float)hy) * (mHexSide + mHexB);

            if ((hy % 2) == 1)
            {
                x += mHexA;
            }

            float x1 = x;
            float y1 = y - mHexB;
            float x2 = x + mHexA;
            float y2 = y;
            float x3 = x + mHexA;
            float y3 = y + mHexSide;
            float x4 = x;
            float y4 = y + mHexSide + mHexB;
            float x5 = x - mHexA;
            float y5 = y + mHexSide;
            float x6 = x - mHexA;
            float y6 = y;

            art_drawLine(x1, y1, x2, y2);
            art_drawLine(x2, y2, x3, y3);
            art_drawLine(x3, y3, x4, y4);
            art_drawLine(x4, y4, x5, y5);
            art_drawLine(x5, y5, x6, y6);
            art_drawLine(x6, y6, x1, y1);
        }
    }
}

void SimHex::getHexCell(float x, float y, int& hX, int& hY)
{
    float deltaX = mHexA * 2;
    float deltaY = mHexSide + mHexB;

    hY = truncf(y / deltaY);
    float inY = y - (hY * deltaY);
    int even = ((((int)hY) % 2) == 0);

    float adjustedX = x;

    if (even)
    {
        adjustedX += mHexA;
    }

    hX = truncf(adjustedX / deltaX);
    float inX = adjustedX - (hX * deltaX);

    if (inY > mHexSide)
    {
        if (inX < mHexA)
        {
            float lineX = inX;
            float lineY = mHexB - (inY - mHexSide);

            float m = -(mHexB / mHexA);

            if (lineY < ((lineX * m) + mHexB))
            {
                hY++;

                if (even)
                {
                    hX--;
                }
            }
        }
        else
        {
            float lineX = inX - mHexA;
            float lineY = mHexB - (inY - mHexSide);
            float m = mHexB / mHexA;

            if (lineY < (lineX * m))
            {
                hY++;

                if (!even)
                {
                    hX++;
                }
            }
        }
    }
}

const char SimHex::mClassName[] = "SimHex";

Orbit<SimHex>::MethodType SimHex::mMethods[] = {
    {"addObject", &Simulation::addObject},
    {"setSeedIndex", &Simulation::setSeedIndex},
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"initGraphics", &Simulation::initGraphics},
    {"run", &Simulation::run},
    {"setTimeLimit", &Simulation::setTimeLimit},
    {"setWorldDimensions", &SimHex::setWorldDimensions},
    {"setViewRange", &SimCont2D::setViewRange},
    {"setViewAngle", &SimCont2D::setViewAngle},
    {"setGoCost", &SimCont2D::setGoCost},
    {"setRotateCost", &SimCont2D::setRotateCost},
    {"setGoForceScale", &SimCont2D::setGoForceScale},
    {"setRotateForceScale", &SimCont2D::setRotateForceScale},
    {"setPos", &SimCont2D::setPos},
    {"setRot", &SimCont2D::setRot},
    {"setHuman", &SimCont2D::setHuman},
    {"setFeedCenter", &SimCont2D::setFeedCenter},
    {"setSoundRange", &SimCont2D::setSoundRange},
    {"setSpeakInterval", &SimCont2D::setSpeakInterval},
    {0,0}
};

Orbit<SimHex>::NumberGlobalType SimHex::mNumberGlobals[] = {
    {"PERCEPTION_NULL", PERCEPTION_NULL},
    {"PERCEPTION_POSITION", PERCEPTION_POSITION},
    {"PERCEPTION_DISTANCE", PERCEPTION_DISTANCE},
    {"PERCEPTION_TARGET", PERCEPTION_TARGET},
    {"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
    {"PERCEPTION_SYMBOL", PERCEPTION_SYMBOL},
    {"PERCEPTION_ENERGY", PERCEPTION_ENERGY},
    {"PERCEPTION_CAN_SPEAK", PERCEPTION_CAN_SPEAK},
    {"ACTION_NULL", ACTION_NULL},
    {"ACTION_GO", ACTION_GO},
    {"ACTION_ROTATE", ACTION_ROTATE},
    {"ACTION_EAT", ACTION_EAT},
    {"ACTION_EATB", ACTION_EATB},
    {"ACTION_SPEAK", ACTION_SPEAK},
    {"FITNESS_ENERGY", FITNESS_ENERGY},
    {"FITNESS_ENERGY_SUM", FITNESS_ENERGY_SUM},
    {"FITNESS_ENERGY_SUM_ABOVE_INIT", FITNESS_ENERGY_SUM_ABOVE_INIT},
    {"FITNESS_RANDOM", FITNESS_RANDOM},
    {0,0}
};

int SimHex::setWorldDimensions(lua_State* luaState)
{
    float width = luaL_checknumber(luaState, 1);
    float height = luaL_checknumber(luaState, 2);
    int cellSide = luaL_checkint(luaState, 3);
    float hexSide = luaL_checknumber(luaState, 4);
    setWorldDimensions(width, height, cellSide, hexSide);
    return 0;
}
