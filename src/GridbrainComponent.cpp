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
    mTableLinkType = InterfaceItem::NO_LINK;
    mTimeToTrigger = 0;
    mTriggerInterval = 0;
    mLastInput = 0.0f;
    mMemCell = NULL;
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
    mFirstInConnection = NULL;
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
    mTableLinkType = comp->mTableLinkType;
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

bool GridbrainComponent::isUnique()
{
    return ((mType == GridbrainComponent::PER)
        || (mType == GridbrainComponent::ACT));
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
    case STHR:
        return "STHR";
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
    case INV:
        return "INV";
    case RAND:
        return "RAND";
    case CLK:
        return "CLK";
    case MEMA:
        return "MEMA";
    case MEMC:
        return "MEMC";
    case MEMT:
        return "MEMT";
    case MEMW:
        return "MEMW";
    default:
        return "?";
    }
}

GridbrainComponent::ConnType GridbrainComponent::getConnectorType()
{
    switch(mType)
    {
    case PER:
    case RAND:
        return CONN_OUT;
    case ACT:
        return CONN_IN;
    default:
        return CONN_INOUT;
    }
}

bool GridbrainComponent::isProducer(CalcActivePass pass)
{
    switch(mType)
    {
    case PER:
    case NOT:
    case TNAND:
    case RAND:
        return true;
    case MEMA:
    case MEMC:
    case MEMT:
    case MEMW:
        if (pass == CAP_NO_MEM_PRODUCER)
        {
            return false;
        }
        return mMemCell->mProducer;
    default:
        return false;
    }
}

bool GridbrainComponent::isConsumer(CalcActivePass pass)
{
    switch(mType)
    {
    case ACT:
        return true;
    case MEMA:
    case MEMC:
    case MEMT:
    case MEMW:
        if (pass == CAP_NO_MEM_CONSUMER)
        {
            return false;
        }
        return mMemCell->mConsumer;
    default:
        return false;
    }
}

bool GridbrainComponent::isMemory()
{
    switch(mType)
    {
    case MEMA:
    case MEMC:
    case MEMT:
    case MEMW:
        return true;
    default:
        return false;
    }
}

void GridbrainComponent::calcProducer(bool prod, bool& memStable, CalcActivePass pass)
{
    if (mProducer)
    {
        // This branch is already calculated
        return;
    }

    if (prod || (isProducer(pass)))
    {
        mProducer = true;

        if (isMemory())
        {
            mMemCell->mProducer = true;
            memStable = false;
        }

        GridbrainConnection* conn = mFirstConnection;

        while (conn != NULL)
        {
            GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
            targComp->calcProducer(true, memStable, pass);

            conn = (GridbrainConnection*)conn->mNextConnection;
        }
    }
}

bool GridbrainComponent::calcConsumer(bool& memStable, CalcActivePass pass)
{
    if (mConsumer || isConsumer(pass))
    {
        mConsumer = true;
        return true;
    }

    GridbrainConnection* conn = mFirstConnection;

    while (conn != NULL)
    {
        GridbrainComponent* targComp = (GridbrainComponent*)conn->mTargComponent;
        if (targComp->calcConsumer(memStable, pass))
        {
            if (isMemory())
            {
                mMemCell->mConsumer = true;
                memStable = false;
            }
            mConsumer = true;
            return true;
        }

        conn = (GridbrainConnection*)conn->mNextConnection;
    }

    mConsumer = false;
    return false;
}

bool GridbrainComponent::calcActive(bool& memStable, CalcActivePass pass)
{
    memStable = true;
    calcProducer(false, memStable, pass);
    calcConsumer(memStable, pass);
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

bool GridbrainComponent::isEqual(GridbrainComponent* comp, bool sameGrid)
{
    return ((mType == comp->mType)
            && (mSubType == comp->mSubType)
            && ((!sameGrid) || (mGrid == comp->mGrid))
            && (mOrigSymTable == comp->mOrigSymTable)
            && (mTargetSymTable == comp->mTargetSymTable)
            && (mOrigSymID == comp->mOrigSymID));
}

void GridbrainComponent::print()
{
    printf("%s", getName().c_str());
    printf("(%d)", mSubType);
    printf("  [%d, %d, %d]", mColumn, mRow, mGrid);
}

const char GridbrainComponent::mClassName[] = "GridbrainComponent";

Orbit<GridbrainComponent>::MethodType GridbrainComponent::mMethods[] = {{0,0}};

Orbit<GridbrainComponent>::NumberGlobalType GridbrainComponent::mNumberGlobals[] = {
    {"NUL", NUL},
    {"PER", PER},
    {"ACT", ACT},
    {"THR", THR},
    {"STHR", STHR},
    {"MAX", MAX},
    {"MUL", MUL},
    {"NOT", NOT},
    {"MMAX", MMAX},
    {"AND", AND},
    {"TAND", TAND},
    {"TNAND", TNAND},
    {"INV", INV},
    {"RAND", RAND},
    {"CLK", CLK},
    {"MEMA", MEMA},
    {"MEMC", MEMC},
    {"MEMT", MEMT},
    {"MEMW", MEMW},
    {0,0}
};

