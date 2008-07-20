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

#include "Connection.h"
#include "InterfaceItem.h"
#include "Orbit.h"
#include "types.h"

#include <string>
using std::string;

class Component
{
public:
    enum Type {NUL, IN, OUT, AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, EQ, GTZ, ZERO, MAX, MIN, AVG, CLK, DMUL, SEL};
    enum ConnType {CONN_IN, CONN_OUT, CONN_INOUT};

    static Component* createByType(Type type);

    Component(lua_State* luaState=NULL);
    virtual ~Component();

    virtual Component* clone(){return NULL;}

    virtual void input(float value, int pin){}
    virtual float output(unsigned int id){return 0.0f;}
    virtual void reset(int pass){}

    void clearMetrics();

    void copyPosition(Component* comp);
    void copyConnections(Component* comp);

    virtual string getName(){return "?";}
    virtual ConnType getConnectorType(){return CONN_INOUT;}

    virtual bool isUnique(){return false;}
    
    bool isUsed();
    bool isEqual(Component* comp, bool sameGrid=true);

    virtual bool isProducer(){return false;}
    virtual bool isConsumer(){return false;}
    void calcProducer(bool prod);
    bool calcConsumer();
    bool calcActive();

    void print();

    static const char mClassName[];
    static Orbit<Component>::MethodType mMethods[];
    static Orbit<Component>::NumberGlobalType mNumberGlobals[];

    Type mType;
    int mSubType;
    float mParam;

    float mInput;
    float mOutput;

    unsigned int mOffset;
    unsigned int mPerceptionPosition;
    unsigned int mActionPosition;

    unsigned int mColumn;
    unsigned int mRow;
    unsigned int mGrid;

    unsigned int mConnectionsCount;
    unsigned int mInboundConnections;
    Connection* mFirstConnection;
    Connection* mFirstInConnection;

    unsigned int mPossibleConnections;

    int mOrigSymTable;
    llULINT mOrigSymID;
    int mTargetSymTable;
    InterfaceItem::TableLinkType mTableLinkType;

    unsigned int mDepth;
    bool mProducer;
    bool mConsumer;
    bool mActive;

protected:
    virtual bool compare(Component* comp){return true;}
};

#endif

