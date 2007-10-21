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

#if !defined(__INCLUDE_GRAPHICAL_OBJECT_H)
#define __INCLUDE_GRAPHICAL_OBJECT_H

#include "SimulationObject.h"
#include "Graphic.h"
#include "Orbit.h"
#include "pyc.h"

#include <list>

using std::list;

class GraphicalObject : public SimulationObject
{
public:
    GraphicalObject(lua_State* luaState=NULL);
    GraphicalObject(GraphicalObject* obj);
    virtual ~GraphicalObject();
    virtual SimulationObject* clone(bool randomize=false);

    void draw(pyc::Layer* layer);

    void addGraphic(Graphic* graph);

    list<Graphic*> mGraphics;

    static const char mClassName[];
    static Orbit<GraphicalObject>::MethodType mMethods[];
    static Orbit<GraphicalObject>::NumberGlobalType mNumberGlobals[];

    int addGraphic(lua_State* luaState);
};
#endif

