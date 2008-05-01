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

#if !defined(__INCLUDE_AGENT_H)
#define __INCLUDE_AGENT_H

#include "GraphicalObject.h"
#include "Brain.h"
#include "Orbit.h"
#include "Message.h"

#include <list>

using std::list;

class Agent : public GraphicalObject
{
public:
    Agent(lua_State* luaState=NULL);
    Agent(Agent* agent);
    virtual ~Agent();
    virtual SimulationObject* clone();

    void setBrain(Brain* brain);
    Brain* getBrain(){return mBrain;}

    virtual void compute();

    virtual void mutate();

    virtual bool getFieldValue(string fieldName, float& value);

    void addMessage(Message* msg){mMessageList.push_back(msg);}
    list<Message*>* getMessageList(){return &mMessageList;}
    void emptyMessageList();

    virtual void popAdjust(vector<SimulationObject*>* popVec);

    static const char mClassName[];
    static Orbit<Agent>::MethodType mMethods[];
    static Orbit<Agent>::NumberGlobalType mNumberGlobals[];

    int setBrain(lua_State* luaState);

    virtual void printDebug();

protected:
    virtual void recombine(SimulationObject* parent1, SimulationObject* parent2);

    Brain* mBrain;
    list<Message*> mMessageList;
};
#endif

