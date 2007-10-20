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

#include "GraphicalObject.h"

GraphicalObject::GraphicalObject(lua_State* luaState) : SimulationObject(luaState)
{
    mType = TYPE_GRAPHICAL_OBJECT;
}

GraphicalObject::GraphicalObject(GraphicalObject* obj) : SimulationObject(obj)
{
    for (list<Graphic*>::iterator iterGraph = obj->mGraphics.begin();
            iterGraph != obj->mGraphics.end();
            iterGraph++)
    {
        Graphic* graph = (*iterGraph)->createSameType();
        mGraphics.push_back(graph);
    }
}

GraphicalObject::~GraphicalObject()
{
    for (list<Graphic*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        delete (*iterGraph);
    }
    mGraphics.clear();
}

SimulationObject* GraphicalObject::clone()
{
    return new GraphicalObject(this);
}

void GraphicalObject::draw(pyc::Layer* layer)
{
    for (list<Graphic*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        (*iterGraph)->draw(layer);
    }
}

void GraphicalObject::addGraphic(Graphic* graph)
{
    mGraphics.push_back(graph);
}

const char GraphicalObject::mClassName[] = "GraphicalObject";

Orbit<GraphicalObject>::MethodType GraphicalObject::mMethods[] = {
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"addGraphic", &GraphicalObject::addGraphic},
	{"setSymbolName", &SimulationObject::setSymbolName},
    {0,0}
};

Orbit<GraphicalObject>::NumberGlobalType GraphicalObject::mNumberGlobals[] = {{0,0}};

int GraphicalObject::addGraphic(lua_State* luaState)
{
    Graphic* graph = (Graphic*)Orbit<GraphicalObject>::pointer(luaState, 1);
    addGraphic(graph);
    return 0;
}

