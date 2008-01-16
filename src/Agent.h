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

#if !defined(__INCLUDE_AGENT_H)
#define __INCLUDE_AGENT_H

#include "GraphicalObject.h"
#include "Brain.h"
#include "Orbit.h"

class Agent : public GraphicalObject
{
public:
    Agent(lua_State* luaState=NULL);
    Agent(Agent* agent, bool randomize=false);
    virtual ~Agent();
    virtual SimulationObject* clone(bool randomize=false);

    virtual void onAdd();

    Brain* setBrain(Brain* brain);
    Brain* getBrain(){return mBrain;}

    virtual void compute();

    //virtual Agent* crossover(Agent* other_parent);
    virtual void mutate();

    virtual bool getFieldValue(string fieldName, float& value);

    static const char mClassName[];
    static Orbit<Agent>::MethodType mMethods[];
    static Orbit<Agent>::NumberGlobalType mNumberGlobals[];

    int setBrain(lua_State* luaState);

protected:
    Brain* mBrain;
};
#endif

