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

#include "Symbol.h"

llULINT Symbol::NEXT_SYMBOL_ID = 1000000;

Symbol::Symbol()
{
    mAlwaysRandom = false;
    mFixed = true;
    mID = 0;
    mSelected = false;
}

Symbol::Symbol(Symbol* sym)
{
    mAlwaysRandom = sym->mAlwaysRandom;
    mFixed = sym->mFixed;
    mID = sym->mID;
    mSelected = false;
}

Symbol::~Symbol()
{   
}

void Symbol::newDynamicID()
{
    mID = NEXT_SYMBOL_ID;
    NEXT_SYMBOL_ID++;
}

int Symbol::setAlwaysRandom(lua_State* luaState)
{
    setAlwaysRandom();
    return 0;
}

int Symbol::getID(lua_State* luaState)
{
    lua_pushinteger(luaState, mID);
    return 1;
}

