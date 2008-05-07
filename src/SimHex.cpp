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
}

SimHex::~SimHex()
{
}

void SimHex::setWorldDimensions(float worldWidth,
                                float worldLength,
                                unsigned int cellSide,
                                float hexSide)
{
    SimCont2D::setWorldDimensions(worldWidth, worldLength, cellSide);

    mHexSide = hexSide;
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

