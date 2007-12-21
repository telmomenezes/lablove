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

#include "Agent.h"

#include <math.h>

Agent::Agent(lua_State* luaState) : GraphicalObject()
{
    mType = TYPE_AGENT;
    mBrain = NULL;
}

Agent::Agent(Agent* agent, bool randomize) : GraphicalObject(agent)
{
    mBrain = agent->mBrain->clone(randomize);
    mBrain->init();
}

Agent::~Agent()
{   
    if (mBrain != NULL)
    {
        delete mBrain;
        mBrain = NULL;
    }
}

SimulationObject* Agent::clone(bool randomize)
{
    return new Agent(this, randomize);
}

void Agent::onAdd()
{
    if (mBrain != NULL)
    {
        mBrain->onAdd();
    }
}

Brain* Agent::setBrain(Brain* brain)
{
    mBrain = brain;
    mBrain->init();
}

void Agent::compute()
{
    if (mBrain == NULL)
    {
        return;
    }

    mBrain->cycle();
}

/*Agent* Agent::crossover(Agent* otherParent)
{
    // TODO: check for incompatibilities, like gridbrain dimensions?

    Agent* child = new Agent();
    child->mMaxInputDepth = mMaxInputDepth;
    child->mGridbrain = mGridbrain->crossover(((Agent*)otherParent)->mGridbrain);

    return child;
}
*/

void Agent::mutate()
{
    SimulationObject::mutate();

    if (mBrain != NULL)
    {
        mBrain->mutate();
    }
}

float Agent::getFieldValue(std::string fieldName)
{
    return mBrain->getFieldValue(fieldName);
}

const char Agent::mClassName[] = "Agent";

Orbit<Agent>::MethodType Agent::mMethods[] = {
    {"addSymbolTable", &SimulationObject::addSymbolTable},
    {"addGraphic", &GraphicalObject::addGraphic},
	{"setSymbolName", &SimulationObject::setSymbolName},
    {"setBrain", &Agent::setBrain},
    {0,0}
};

Orbit<Agent>::NumberGlobalType Agent::mNumberGlobals[] = {{0,0}};

int Agent::setBrain(lua_State* luaState)
{
    Brain* brain = (Brain*)Orbit<Agent>::pointer(luaState, 1);
    setBrain(brain);
    return 0;
}

