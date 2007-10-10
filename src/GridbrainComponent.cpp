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

#include "GridbrainComponent.h"

GridbrainComponent GridbrainComponent::mNullComponent;

GridbrainComponent::GridbrainComponent(lua_State* luaState)
{
    clearDefinitions();
    clearPosition();
    clearConnections();
}

GridbrainComponent::~GridbrainComponent()
{
}

void GridbrainComponent::clearDefinitions()
{
    mType = NUL;
    mSubType = -1;
    mInput = 0;
    mOutput = 0;
    mRecurrentInput = 0;
    mState = 0;
    mForwardFlag = false;
    mRecurrentFlag = false;
    mPerceptionPosition = 0;
    mActionPosition = 0;

    mParameter = 0.0f;
    mAggregator = false;

    mSymTable = -1;
    mOrigSymIndex = -1;
    mTargetSymIndex = -1;
}

void GridbrainComponent::clearPosition()
{
    mOffset = 0;
    mColumn = 0;
    mRow = 0;
    mGrid = 0;
}

void GridbrainComponent::clearConnections()
{
    mConnectionsCount = 0;
    mFirstConnection = NULL;
}

void GridbrainComponent::copyDefinitions(GridbrainComponent* comp)
{
    mType = comp->mType;
    mSubType = comp->mSubType;
    mParameter = comp->mParameter;
    mAggregator = comp->mAggregator;

    mSymTable = comp->mSymTable;
    mOrigSymIndex = comp->mOrigSymIndex;
    mTargetSymIndex = comp->mTargetSymIndex;

}

void GridbrainComponent::copyPosition(GridbrainComponent* comp)
{
    mOffset = comp->mOffset;
    mColumn = comp->mColumn;
    mRow = comp->mRow;
    mGrid = comp->mGrid;
}

bool GridbrainComponent::isAggregator()
{
    return ((mType == GridbrainComponent::AGG )
        || (mType == GridbrainComponent::MAX));
}

float GridbrainComponent::computeBinding(SimulationObject* sourceObj,
                SimulationObject* targetObj)
{
    SymbolTable* sourceTable = sourceObj->getSymbolTable(mSymTable);
    SymbolTable* targetTable = targetObj->getSymbolTable(mSymTable);

    if (!targetTable)
    {
        return 0.0f;
    }

    Symbol* sourceSym = sourceTable->getSymbol(mOrigSymIndex);
    Symbol* targetSym = targetTable->getSymbol(mTargetSymIndex);

    if (!targetSym)
    {
        return 0.0f;
    }

    return (sourceSym->bind(targetSym));
}

const char GridbrainComponent::mClassName[] = "GridbrainComponent";

Orbit<GridbrainComponent>::MethodType GridbrainComponent::mMethods[] = {{0,0}};

Orbit<GridbrainComponent>::NumberGlobalType GridbrainComponent::mNumberGlobals[] = {
    {"NUL", NUL},
    {"PER", PER},
    {"STA", STA},
    {"ACT", ACT},
    {"THR", THR},
    {"AGG", AGG},
    {"MAX", MAX},
    {"MUL", MUL},
    {"NOT", NOT},
    {0,0}
};

