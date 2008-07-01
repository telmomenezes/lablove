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
    mParam = 0;
    mInit = false;
    mInput = 0;
    mOutput = 0;
    mState = 0;
    mPreState = 0;
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
    mParam = comp->mParam;
    mInit = false;
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
            || (mType == GridbrainComponent::MIN)
            || (mType == GridbrainComponent::AVG));
}

bool GridbrainComponent::isUnique()
{
    return ((mType == GridbrainComponent::IN)
        || (mType == GridbrainComponent::OUT));
}

string GridbrainComponent::getName()
{
    switch(mType)
    {
    case NUL:
        return "NUL";
    case IN:
        return "IN";
    case OUT:
        return "OUT";
    case SUM:
        return "SUM";
    case MAX:
        return "MAX";
    case MIN:
        return "MIN";
    case AVG:
        return "AVG";
    case MUL:
        return "MUL";
    case NOT:
        return "NOT";
    case AND:
        return "AND";
    case INV:
        return "INV";
    case NEG:
        return "NEG";
    case MOD:
        return "MOD";
    case AMP:
        return "AMP";
    case RAND:
        return "RAND";
    case EQ:
        return "EQ";
    case GTZ:
        return "GTZ";
    case ZERO:
        return "ZERO";
    case CLK:
        return "CLK";
    case MEMC:
        return "MEMC";
    case MEMW:
        return "MEMW";
    case MEMD:
        return "MEMD";
    case DAND:
        return "DAND";
    default:
        return "?";
    }
}

GridbrainComponent::ConnType GridbrainComponent::getConnectorType()
{
    switch(mType)
    {
    case IN:
    case RAND:
        return CONN_OUT;
    case OUT:
        return CONN_IN;
    default:
        return CONN_INOUT;
    }
}

bool GridbrainComponent::isProducer()
{
    switch(mType)
    {
    case IN:
    case NOT:
    case RAND:
    case ZERO:
    case CLK:
    case MEMC:
    case MEMW:
    case MEMD:
        return true;
    default:
        return false;
    }
}

bool GridbrainComponent::isConsumer()
{
    switch(mType)
    {
    case OUT:
    case MEMC:
    case MEMW:
    case MEMD:
        return true;
    default:
        return false;
    }
}

bool GridbrainComponent::isMemory()
{
    switch(mType)
    {
    case MEMC:
    case MEMW:
    case MEMD:
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

    if (prod || isProducer())
    {
        if (prod)
        {
            mProducer = true;
        }

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
    if (mConsumer)
    {
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
    
    if (isConsumer())
    {
        return true;
    }

    mConsumer = false;
    return false;
}

bool GridbrainComponent::calcActive()
{
    calcProducer(false);
    calcConsumer();
    mActive = ((mConsumer && mProducer)
                || (mConsumer && isProducer())
                || (isConsumer() && mProducer));
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
    {"IN", IN},
    {"OUT", OUT},
    {"SUM", SUM},
    {"MAX", MAX},
    {"MIN", MIN},
    {"AVG", AVG},
    {"MUL", MUL},
    {"NOT", NOT},
    {"AND", AND},
    {"INV", INV},
    {"NEG", NEG},
    {"AMP", AMP},
    {"MOD", MOD},
    {"RAND", RAND},
    {"EQ", EQ},
    {"GTZ", GTZ},
    {"ZERO", ZERO},
    {"CLK", CLK},
    {"MEMC", MEMC},
    {"MEMW", MEMW},
    {"MEMD", MEMD},
    {"DAND", DAND},
    {0,0}
};

