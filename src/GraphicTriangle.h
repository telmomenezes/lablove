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

#if !defined(__INCLUDE_GRAPHIC_TRIANGLE_H)
#define __INCLUDE_GRAPHIC_TRIANGLE_H

#include "Graphic.h"
#include "Orbit.h"
#include "SimObj2D.h"

#include "art.h"

class GraphicTriangle : public Graphic
{
public:
    GraphicTriangle(lua_State* luaState=NULL);
    virtual ~GraphicTriangle();
    
    virtual Graphic* clone();

    virtual void init(SimObj2D* obj);
    virtual void draw();

    static const char mClassName[];
    static Orbit<GraphicTriangle>::MethodType mMethods[];
    static Orbit<GraphicTriangle>::NumberGlobalType mNumberGlobals[];

protected:
    float mSize;
    int mRed;
    int mGreen;
    int mBlue;
    int mXIndex;
    int mYIndex;
    int mRotIndex;
};
#endif

