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
    mState = 0;
    mForwardFlag = false;
    mCycleFlag = false;
    mPerceptionPosition = 0;
    mActionPosition = 0;
    mOrigSymTable = -1;
    mTargetSymTable = -1;
    mOrigSymID = 0;
    mTargetSymID = 0;
    mTableLinkType = InterfaceItem::NO_LINK;
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
    mInboundConnections = 0;
}

void GridbrainComponent::clearMetrics()
{
    mDepth = 0;
    mProducer = false;
    mConsumer = false;
    mActive = false;
}

void GridbrainComponent::copyDefinitions(GridbrainComponent* comp)
{
    mType = comp->mType;
    mSubType = comp->mSubType;
    mOrigSymTable = comp->mOrigSymTable;
    mTargetSymTable = comp->mTargetSymTable;
    mOrigSymID = comp->mOrigSymID;
    mTargetSymID = comp->mTargetSymID;
    mTableLinkType = comp->mTableLinkType;
    mID = comp->mID;
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

GridbrainComponent::ConnType GridbrainComponent::getConnectorType()
{
    switch(mType)
    {
    case PER:
        return CONN_OUT;
    case ACT:
        return CONN_IN;
    default:
        return CONN_INOUT;
    }
}

bool GridbrainComponent::isProducer()
{
    switch(mType)
    {
    case PER:
    case NOT:
    case TNAND:
        return true;
    default:
        return false;
    }
}

bool GridbrainComponent::isConsumer()
{
    switch(mType)
    {
    case ACT:
        return true;
    default:
        return false;
    }
}

void GridbrainComponent::calcProducer(bool prod)
{
    if (mProducer)
    {
        // This branch is already calculated
        return;
    }

    if (prod || (isProducer()))
    {
        mProducer = true;

        GridbrainConnection* conn = mFirstConnection;

        while (conn != NULL)
        {
            GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
            targComp->calcProducer(true);

            conn = (GridbrainConnection*)conn->mNextConnection;
        }
    }
}

bool GridbrainComponent::calcConsumer()
{
    if (mConsumer || isConsumer())
    {
        mConsumer = true;
        return true;
    }

    GridbrainConnection* conn = mFirstConnection;

    while (conn != NULL)
    {
        GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
        if (targComp->calcConsumer())
        {
            mConsumer = true;
            return true;
        }

        conn = (GridbrainConnection*)conn->mNextConnection;
    }

    mConsumer = false;
    return false;
}

bool GridbrainComponent::calcActive()
{
    calcProducer();
    calcConsumer();
    mActive = mConsumer && mProducer;
    return mActive;
}

bool GridbrainComponent::isUsed()
{
    if (mConnectionsCount > 0)
    {
        return true;
    }
    if (mInboundConnections > 0)
    {
        return true;
    }

    return false;
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

