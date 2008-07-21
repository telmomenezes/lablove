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

#include "CompACT.h"
#include "Simulation.h"

CompACT::CompACT(lua_State* luaState)
{
    if (luaState)
    {
        int outputType = luaL_optint(luaState, 1, -1);
        int origTable = luaL_optint(luaState, 2, -1);
        llULINT origSymID = luaL_optint(luaState, 3, 0);
        int targTable = luaL_optint(luaState, 4, -1);
        bool dynamic = luaL_opt(luaState, luaL_checkbool, 5, false);

        mOutputType = outputType;
        mOrigSymTable = origTable;
        mOrigSymID = origSymID;
        mTargetSymTable = targTable;
        mDynamic = dynamic;
    }
    else
    {
        mOutputType = -1;
        mOrigSymTable = -1;
        mTargetSymTable = -1;
        mOrigSymID = 0;
        mDynamic = false;
    }
}

CompACT::~CompACT()
{
}

Component* CompACT::clone()
{
    CompACT* comp = new CompACT();
    comp->mOutputType = mOutputType;
    comp->mOrigSymTable = mOrigSymTable;
    comp->mOrigSymID = mOrigSymID;
    comp->mTargetSymTable = mTargetSymTable;
    comp->mDynamic = mDynamic;
    return comp;
}

bool CompACT::compare(Component* comp)
{
    CompACT* act = (CompACT*)comp;
    return ((mOutputType == act->mOutputType)
            && (mOrigSymTable == act->mOrigSymTable)
            && (mTargetSymTable == act->mTargetSymTable)
            && (mOrigSymID == act->mOrigSymID));
}

string CompACT::getLabel()
{
    string label = Simulation::CURRENT->getActionName(mOutputType);

    if (label == "")
    {
        char buffer[255];
        sprintf(buffer, "%d", mOrigSymTable);
        label = buffer;
    }

    return label; 
}

const char CompACT::mClassName[] = "ACT";
Orbit<CompACT>::MethodType CompACT::mMethods[] = {{0,0}};
Orbit<CompACT>::NumberGlobalType CompACT::mNumberGlobals[] = {{0,0}};

