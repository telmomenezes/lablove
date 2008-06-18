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

#include "InterfaceItem.h"

InterfaceItem::InterfaceItem(lua_State* luaState)
{
}

InterfaceItem::InterfaceItem(const InterfaceItem& item)
{
    mType = item.mType;
    mOrigSymTable = item.mOrigSymTable;
    mTargetSymTable = item.mTargetSymTable;
    mOrigSymID = item.mOrigSymID;
    mTableLinkType = item.mTableLinkType;
}

InterfaceItem::~InterfaceItem()
{
}

const char InterfaceItem::mClassName[] = "InterfaceItem";

Orbit<InterfaceItem>::MethodType InterfaceItem::mMethods[] = {{0,0}};

Orbit<InterfaceItem>::NumberGlobalType InterfaceItem::mNumberGlobals[] = {
    {"NO_LINK", NO_LINK},
    {"SYM_TO_SYM", SYM_TO_SYM},
    {"TAB_TO_SYM", TAB_TO_SYM},
    {0,0}
};

