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
    clearMetrics();
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
    mCycleFlag = false;
    mPerceptionPosition = 0;
    mActionPosition = 0;
    mOrigSymTable = -1;
    mTargetSymTable = -1;
    mOrigSymIndex = -1;
    mTargetSymIndex = InterfaceItem::NO_LINK;
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

void GridbrainComponent::clearMetrics()
{
    mConnectionStrength = 0.0f;
}

void GridbrainComponent::copyDefinitions(GridbrainComponent* comp)
{
    mType = comp->mType;
    mSubType = comp->mSubType;
    mOrigSymTable = comp->mOrigSymTable;
    mTargetSymTable = comp->mTargetSymTable;
    mOrigSymIndex = comp->mOrigSymIndex;
    mTargetSymIndex = comp->mTargetSymIndex;
    mTableLinkType = comp->mTableLinkType;
;}

void GridbrainComponent::copyPosition(GridbrainComponent* comp)
{
    mOffset = comp->mOffset;
    mColumn = comp->mColumn;
    mRow = comp->mRow;
    mGrid = comp->mGrid;
}

bool GridbrainComponent::isAggregator()
{
    return ((mType == GridbrainComponent::MAX)
        || (mType == GridbrainComponent::MMAX));
}

string GridbrainComponent::getName()
{
    switch(mType)
    {
    case NUL:
        return "NUL";
    case PER:
        return "PER";
    case ACT:
        return "ACT";
    case THR:
        return "THR";
    case MAX:
        return "MAX";
    case MUL:
        return "MUL";
    case NOT:
        return "NOT";
    case MMAX:
        return "MMAX";
    case AND:
        return "AND";
    case TAND:
        return "TAND";
    case TNAND:
        return "TNAND";
    default:
        return "?";
    }
}

const char GridbrainComponent::mClassName[] = "GridbrainComponent";

Orbit<GridbrainComponent>::MethodType GridbrainComponent::mMethods[] = {{0,0}};

Orbit<GridbrainComponent>::NumberGlobalType GridbrainComponent::mNumberGlobals[] = {
    {"NUL", NUL},
    {"PER", PER},
    {"ACT", ACT},
    {"THR", THR},
    {"MAX", MAX},
    {"MUL", MUL},
    {"NOT", NOT},
    {"MMAX", MMAX},
    {"AND", AND},
    {"TAND", TAND},
    {"TNAND", TNAND},
    {0,0}
};

