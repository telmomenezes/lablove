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

#ifndef _INCLUDE_GRIDBRAIN_COMPONENT_PER_H
#define _INCLUDE_GRIDBRAIN_COMPONENT_PER_H

#include "CompIN.h"
#include "Orbit.h"
#include "types.h"
#include "Interface.h"

class CompPER : public CompIN, Interface
{
public:
    CompPER(lua_State* luaState=NULL);
    virtual ~CompPER();

    virtual Component* clone();

    virtual string getName(){return "PER";}

    virtual bool compare(Component* comp);

    virtual string getLabel();

    virtual void print();

    static const char mClassName[];
    static Orbit<CompPER>::MethodType mMethods[];
    static Orbit<CompPER>::NumberGlobalType mNumberGlobals[];
};

static CompPER COMP_PER;

#endif

