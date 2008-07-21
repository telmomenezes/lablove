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

Component* Component::createByType(Type type)
{
    switch(type)
    {
    case NUL:
        return new CompNUL();
        break;
    case IN:
        return new CompIN();
        break;
    case OUT:
        return new CompOUT();
        break;
    case SUM:
        return new CompSUM();
        break;
    case MAX:
        return new CompMAX();
        break;
    case MIN:
        return new CompMIN();
        break;
    case AVG:
        return new CompAVG();
        break;
    case MUL:
        return new CompMUL();
        break;
    case NOT:
        return new CompNOT();
        break;
    case AND:
        return new CompAND();
        break;
    case INV:
        return new CompINV();
        break;
    case NEG:
        return new CompNEG();
        break;
    case AMP:
        return new CompAMP();
        break;
    case MOD:
        return new CompMOD();
        break;
    case RAND:
        return new CompRAND();
        break;
    case EQ:
        return new CompEQ();
        break;
    case GTZ:
        return new CompGTZ();
        break;
    case ZERO:
        return new CompZERO();
        break;
    case CLK:
        return new CompCLK();
        break;
    case DMUL:
        return new CompDMUL();
        break;
    case SEL:
        return new CompSEL();
        break;
    default:
        return new CompNUL();
        break;
    }
}

const char Component::mClassName[] = "Component";

Orbit<Component>::MethodType Component::mMethods[] = {{0,0}};

Orbit<Component>::NumberGlobalType Component::mNumberGlobals[] = {
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
    {"DMUL", DMUL},
    {"SEL", SEL},
    {0,0}
};

