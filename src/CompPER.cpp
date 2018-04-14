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

#include "CompPER.h"
#include "Simulation.h"

namespace gb
{

CompPER::CompPER(lua_State* luaState)
{
    if (luaState)
    {
        int inputType = luaL_optinteger(luaState, 1, -1);
        int origTable = luaL_optinteger(luaState, 2, -1);
        gbULINT origSymID = luaL_optinteger(luaState, 3, 0);
        int targTable = luaL_optinteger(luaState, 4, -1);
        bool dynamic = luaL_opt(luaState, lua_toboolean, 5, false);

        mInputType = inputType;
        mOrigSymTable = origTable;
        mOrigSymID = origSymID;
        mTargetSymTable = targTable;
        mDynamic = dynamic;
    }
    else
    {
        mInputType = -1;
        mOrigSymTable = -1;
        mTargetSymTable = -1;
        mOrigSymID = 0;
        mDynamic = false;
    }
}

CompPER::~CompPER()
{
}

Component* CompPER::clone()
{
    CompPER* comp = new CompPER();
    comp->mType = mType;
    comp->mInputType = mInputType;
    comp->mOrigSymTable = mOrigSymTable;
    comp->mOrigSymID = mOrigSymID;
    comp->mTargetSymTable = mTargetSymTable;
    comp->mDynamic = mDynamic;

    return comp;
}

bool CompPER::compare(Component* comp)
{
    CompPER* per = (CompPER*)comp;
    return ((mInputType == per->mInputType)
            && (mOrigSymTable == per->mOrigSymTable)
            && (mTargetSymTable == per->mTargetSymTable)
            && (mOrigSymID == per->mOrigSymID));
}

string CompPER::getLabel()
{
    string label = Simulation::CURRENT->getPerceptionName(mInputType);

    if (label == "")
    {
        char buffer[255];
        sprintf(buffer, "%d", mOrigSymTable);
        label = buffer;
    }

    return label;
}

void CompPER::print()
{
    printf("%s", getName().c_str());
    printf("(%d, %d, ", mInputType, mOrigSymTable);
    printf("%llu, ", mOrigSymID);
    printf("%d)", mTargetSymTable);
    printf("  [%d, %d, %d]", mColumn, mRow, mGrid);
}

const char CompPER::mClassName[] = "PER";
Orbit<CompPER>::MethodType CompPER::mMethods[] = {{0,0}};
Orbit<CompPER>::NumberGlobalType CompPER::mNumberGlobals[] = {{0,0}};

}

