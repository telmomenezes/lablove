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

#ifndef _INCLUDE_GRIDBRAIN_COMPONENT_H
#define _INCLUDE_GRIDBRAIN_COMPONENT_H

#include "GridbrainConnection.h"
#include "InterfaceItem.h"
#include "SimulationObject.h"
#include "Orbit.h"
#include "types.h"
#include "GridbrainMemCell.h"

#include <string>
using std::string;

#define COMPONENT_INPUT_TYPE(compType, inputType) \
switch(compType) \
{ \
case GridbrainComponent::MUL: \
case GridbrainComponent::MMAX: \
case GridbrainComponent::AND: \
    inputType = GridbrainComponent::IN_MUL; \
    break; \
case GridbrainComponent::TAND: \
case GridbrainComponent::TNAND: \
    inputType = GridbrainComponent::IN_TMUL; \
    break; \
default: \
    inputType = GridbrainComponent::IN_SUM; \
    break; \
} \

class GridbrainComponent
{
public:
    enum Type {NUL, PER, ACT, THR, STHR, MAX, MUL, NOT, MMAX, AND, TAND, TNAND, INV, RAND, CLK, MEMA, MEMC, MEMT, MEMW};
    enum InputType {IN_SUM, IN_MUL, IN_TMUL};
    enum ConnType {CONN_IN, CONN_OUT, CONN_INOUT};
    enum CalcActivePass {CAP_NEUTRAL, CAP_NO_MEM_PRODUCER, CAP_NO_MEM_CONSUMER};

    GridbrainComponent(lua_State* luaState=NULL);
    virtual ~GridbrainComponent();

    static GridbrainComponent* getNullComponent(){return &mNullComponent;}

    void clearDefinitions();
    void clearPosition();
    void clearConnections();
    void clearMetrics();
    void copyDefinitions(GridbrainComponent* comp);
    void copyPosition(GridbrainComponent* comp);

    
    string getName();
    ConnType getConnectorType();

    bool isAggregator();
    bool isUnique();
    bool isProducer(CalcActivePass pass);
    bool isConsumer(CalcActivePass pass);
    bool isUsed();
    bool isEqual(GridbrainComponent* comp);
    bool isMemory();

    void calcProducer(bool prod, bool& memStable, CalcActivePass pass);
    bool calcConsumer(bool& memStable, CalcActivePass pass);
    bool calcActive(bool& memStable, CalcActivePass pass=CAP_NEUTRAL);


    static const char mClassName[];
    static Orbit<GridbrainComponent>::MethodType mMethods[];
    static Orbit<GridbrainComponent>::NumberGlobalType mNumberGlobals[];

    static GridbrainComponent mNullComponent;

    Type mType;
    int mSubType;
    float mInput;
    float mOutput;
    unsigned int mConnectionsCount;
    unsigned int mInboundConnections;
    GridbrainConnection* mFirstConnection;
    GridbrainConnection* mFirstInConnection;
    float mState;
    bool mCycleFlag;
    bool mForwardFlag;
    unsigned int mOffset;
    unsigned int mPerceptionPosition;
    unsigned int mActionPosition;

    unsigned int mColumn;
    unsigned int mRow;
    unsigned int mGrid;

    int mOrigSymTable;
    llULINT mOrigSymID;
    int mTargetSymTable;
    InterfaceItem::TableLinkType mTableLinkType;

    unsigned int mPossibleConnections;

    unsigned int mDepth;
    bool mProducer;
    bool mConsumer;
    bool mActive;

    llULINT mTimeToTrigger;
    llULINT mTriggerInterval;
    float mLastInput;

    GridbrainMemCell* mMemCell;
};

#endif

