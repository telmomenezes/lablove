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

#if !defined(__INCLUDE_GRAPHIC_GRADIENT_H)
#define __INCLUDE_GRAPHIC_GRADIENT_H

#include "Graphic.h"
#include "Orbit.h"

#include "pyc.h"

class GraphicGradient : public Graphic
{
public:
    GraphicGradient(lua_State* luaState=NULL);
    virtual ~GraphicGradient();
    
    virtual Graphic* clone();

    virtual void init(SimulationObject* obj, pyc::Pycasso* pycasso);
    virtual void draw(pyc::Layer* layer);

    void setSymbolName(string symbolName){mSymbolName = symbolName;}
    void setReferenceSymbol(Symbol* refSymbol){mReferenceSymbol = refSymbol;}
    void setColor1(int red, int green, int blue){mRed1 = red; mGreen1 = green; mBlue1 = blue;}
    void setColor2(int red, int green, int blue){mRed2 = red; mGreen2 = green; mBlue2 = blue;}

    static const char mClassName[];
    static Orbit<GraphicGradient>::MethodType mMethods[];
    static Orbit<GraphicGradient>::NumberGlobalType mNumberGlobals[];

protected:
    float mSize;
    int mRed;
    int mGreen;
    int mBlue;
    int mXIndex;
    int mYIndex;
    int mRotIndex;

    string mSymbolName;
    Symbol* mReferenceSymbol;

    int mRed1;
    int mGreen1;
    int mBlue1;
    int mRed2;
    int mGreen2;
    int mBlue2;
};
#endif

