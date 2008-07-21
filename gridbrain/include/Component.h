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
#include "Orbit.h"

#include <string>
using std::string;

class Component
{
public:
    enum ConnType {CONN_IN, CONN_OUT, CONN_INOUT};

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
    virtual bool isInput(){return false;}
    virtual bool isOutput(){return false;}
    virtual bool isNUL(){return false;}
    
    virtual string getLabel(){return "";}

    bool isUsed();
    bool isEqual(Component* comp, bool sameGrid=true);

    virtual bool isProducer(){return false;}
    virtual bool isConsumer(){return false;}
    void calcProducer(bool prod);
    bool calcConsumer();
    bool calcActive();

    virtual void print();

    static const char mClassName[];
    static Orbit<Component>::MethodType mMethods[];
    static Orbit<Component>::NumberGlobalType mNumberGlobals[];

    int mType;
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

    unsigned int mDepth;
    bool mProducer;
    bool mConsumer;
    bool mActive;

protected:
    virtual bool compare(Component* comp){return true;}
};

#endif

