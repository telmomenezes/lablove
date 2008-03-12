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

#include "Agent.h"

#include <math.h>

Agent::Agent(lua_State* luaState) : GraphicalObject()
{
    mType = TYPE_AGENT;
    mBrain = NULL;
}

Agent::Agent(Agent* agent) : GraphicalObject(agent)
{
    mBrain = agent->mBrain->clone();
    mBrain->setOwner(this);

    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;

        if (table->isDynamic())
        {
            int tableID = (*iterTables).first;

            map<unsigned long, Symbol*>::iterator iterSymbol;
            for (iterSymbol = table->getSymbolMap()->begin();
                iterSymbol != table->getSymbolMap()->end();
                iterSymbol++)
            {
                unsigned long symbolID = (*iterSymbol).first;
                Symbol* sym = (*iterSymbol).second;

                if ((!sym->mFixed) && 
                    (!mBrain->symbolUsed(tableID, symbolID)))
                {
                    iterSymbol++;
                    table->getSymbolMap()->erase(symbolID);
                    delete sym;
                }
            }

            table->addRandomSymbol();
        }
    }
}

Agent::~Agent()
{   
    if (mBrain != NULL)
    {
        delete mBrain;
        mBrain = NULL;
    }
}

SimulationObject* Agent::clone()
{
    return new Agent(this);
}

Brain* Agent::setBrain(Brain* brain)
{
    if (mBrain != NULL)
    {
        delete mBrain;
    }
    mBrain = brain;
    mBrain->setOwner(this);
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

void Agent::mutate()
{
    SimulationObject::mutate();

    if (mBrain != NULL)
    {
        mBrain->mutate();
    }
}

SimulationObject* Agent::recombine(SimulationObject* otherParent)
{
    Agent* agent2 = (Agent*)otherParent;
    Agent* newAgent = (Agent*)clone();
    Brain* newBrain = mBrain->recombine(agent2->mBrain);
    newAgent->setBrain(newBrain);
    return newAgent;
}

bool Agent::getFieldValue(string fieldName, float& value)
{
    bool success = GraphicalObject::getFieldValue(fieldName, value);

    if (success)
    {
        return true;
    }
    else
    {
        return mBrain->getFieldValue(fieldName, value);
    }
}

const char Agent::mClassName[] = "Agent";

Orbit<Agent>::MethodType Agent::mMethods[] = {
    {"addSymbolTable", &SimulationObject::addSymbolTable},
    {"addGraphic", &GraphicalObject::addGraphic},
	{"setSymbolName", &SimulationObject::setSymbolName},
	{"setBirthRadius", &SimulationObject::setBirthRadius},
	{"setFitnessMeasure", &SimulationObject::setFitnessMeasure},
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

