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

#include "Component.h"
#include "comps.h"

Component::Component(lua_State* luaState)
{
    mType = -1;
    mParam = 0;
    mInput = 0;
    mOutput = 0;
    mPerceptionPosition = 0;
    mActionPosition = 0;

    mOffset = 0;
    mColumn = 0;
    mRow = 0;
    mGrid = 0;

    mConnectionsCount = 0;
    mFirstConnection = NULL;
    mInboundConnections = 0;
    mFirstInConnection = NULL;

    clearMetrics();
}

Component::~Component()
{
}

void Component::clearMetrics()
{
    mDepth = 0;
    mProducer = false;
    mConsumer = false;
    mActive = false;
}

void Component::copyPosition(Component* comp)
{
    mOffset = comp->mOffset;
    mColumn = comp->mColumn;
    mRow = comp->mRow;
    mGrid = comp->mGrid;
}

void Component::copyConnections(Component* comp)
{
    mConnectionsCount = comp->mConnectionsCount;
    mInboundConnections = comp->mInboundConnections;
    mFirstConnection = comp->mFirstConnection;
    mFirstInConnection = comp->mFirstInConnection;
}

void Component::calcProducer(bool prod)
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

        Connection* conn = mFirstConnection;

        while (conn != NULL)
        {
            Component* targComp = (Component*)conn->mTargComponent;
            targComp->calcProducer(true);

            conn = (Connection*)conn->mNextConnection;
        }
    }
}

bool Component::calcConsumer()
{
    if (mConsumer)
    {
        return true;
    }

    Connection* conn = mFirstConnection;

    while (conn != NULL)
    {
        Component* targComp = (Component*)conn->mTargComponent;
        if (targComp->calcConsumer())
        {
            mConsumer = true;
            return true;
        }

        conn = (Connection*)conn->mNextConnection;
    }
    
    if (isConsumer())
    {
        return true;
    }

    mConsumer = false;
    return false;
}

bool Component::calcActive()
{
    calcProducer(false);
    calcConsumer();
    mActive = ((mConsumer && mProducer)
                || (mConsumer && isProducer())
                || (isConsumer() && mProducer));
    return mActive;
}

bool Component::isUsed()
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

bool Component::isEqual(Component* comp, bool sameGrid)
{
    return ((mType == comp->mType)
            && ((!sameGrid) || (mGrid == comp->mGrid))
            && compare(comp));
}

void Component::print()
{
    printf("%s", getName().c_str());
    printf("(%s)", getLabel().c_str());
    printf("  [%d, %d, %d]", mColumn, mRow, mGrid);
}

const char Component::mClassName[] = "Component";

Orbit<Component>::MethodType Component::mMethods[] = {{0,0}};

Orbit<Component>::NumberGlobalType Component::mNumberGlobals[] = {{0,0}};

