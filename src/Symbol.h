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

#if !defined(__INCLUDE_SYMBOL_H)
#define __INCLUDE_SYMBOL_H

#include "Orbit.h"
#include "types.h"

#include <string>

using std::string;

class Symbol
{
public:
    Symbol();
    Symbol(Symbol* sym);
    virtual ~Symbol();

    virtual Symbol* clone() = 0;

    virtual float proximity(Symbol* sym) = 0;
    virtual bool equals(Symbol* sym) = 0;

    virtual void initRandom() = 0;
    virtual void mutate() = 0;

    void setAlwaysRandom(){mAlwaysRandom = true;}

    void generateNewID();

    int setAlwaysRandom(lua_State* luaState);
    int getID(lua_State* luaState);
    int setID(lua_State* luaState);

    virtual string toString(){return "?";}

    virtual int getRed(){return 0;}
    virtual int getGreen(){return 0;}
    virtual int getBlue(){return 0;}

    bool mAlwaysRandom;
    bool mFixed;
    bool mProtected;

    // for recombination purposes
    bool mSelected;
    llULINT mID;

    bool mUsed;

protected:
    static llULINT NEXT_SYMBOL_ID;
};
#endif

