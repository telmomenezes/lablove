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
#include "types.h"

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

    // Clean and expand symbol tables
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;

        if (table->isDynamic())
        {
            int tableID = (*iterTables).first;

            map<llULINT, Symbol*>::iterator iterSymbol;
            for (iterSymbol = table->getSymbolMap()->begin();
                iterSymbol != table->getSymbolMap()->end();
                iterSymbol++)
            {
                llULINT symbolID = (*iterSymbol).first;

                Symbol* sym = (*iterSymbol).second;

                if ((!sym->mFixed) && 
                    (!mBrain->symbolUsed(tableID, symbolID)))
                {
                    iterSymbol++;
                    table->getSymbolMap()->erase(symbolID);
                    delete sym;
                }
            }

            table->grow();
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

    for (list<Message*>::iterator iterMessage = mMessageList.begin();
            iterMessage != mMessageList.end();
            iterMessage++)
    {
        delete (*iterMessage);
    }

    mMessageList.clear();
}

SimulationObject* Agent::clone()
{
    return new Agent(this);
}

void Agent::setBrain(Brain* brain)
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

void Agent::recombine(SimulationObject* parent1, SimulationObject* parent2)
{
    /*printf("==> START RECOMBINE\n\n");
    printf("\nPARENT 1\n");
    printf("========\n");
    parent1->printDebug();

    printf("\nPARENT 2\n");
    printf("========\n");
    parent2->printDebug();*/

    Agent* agent1 = (Agent*)parent1;
    Agent* agent2 = (Agent*)parent2;

    Brain* newBrain = agent1->mBrain->recombine(agent2->mBrain);

    newBrain->setSelectedSymbols(agent1);
    newBrain->setSelectedSymbols(agent2);

    SimulationObject::recombine(parent1, parent2);

    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;

        if (table->isDynamic())
        {
            table->grow();
        }
    }

    setBrain(newBrain);

    /*printf("\nCHILD\n");
    printf("=====\n");
    printDebug();*/
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

void Agent::popAdjust(vector<SimulationObject*>* popVec)
{
    mBrain->popAdjust(popVec);
}

void Agent::emptyMessageList()
{
    for (list<Message*>::iterator iterMessage = mMessageList.begin();
                iterMessage != mMessageList.end();
                iterMessage++)
    {
        delete (*iterMessage);
    }
    mMessageList.clear();
}

void Agent::printDebug()
{
    SimulationObject::printDebug();
    mBrain->printDebug();
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

