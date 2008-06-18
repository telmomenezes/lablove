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

#if !defined(__INCLUDE_INTERFACE_ITEM_H)
#define __INCLUDE_INTERFACE_ITEM_H

#include "Orbit.h"
#include "types.h"

class InterfaceItem
{
public:
    enum TableLinkType {NO_LINK, SYM_TO_SYM, TAB_TO_SYM};

    InterfaceItem(lua_State* luaState=NULL);
    InterfaceItem(const InterfaceItem& item);
    virtual ~InterfaceItem();

    int mType;
    int mOrigSymTable;
    int mTargetSymTable;
    llULINT mOrigSymID;
    TableLinkType mTableLinkType;

    static const char mClassName[];
    static Orbit<InterfaceItem>::MethodType mMethods[];
    static Orbit<InterfaceItem>::NumberGlobalType mNumberGlobals[];
};
#endif

