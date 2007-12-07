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

#include "LogBestBrain.h"
#include "Agent.h"
#include "Simulation.h"

#include <stdio.h>

LogBestBrain::LogBestBrain(lua_State* luaState)
{
    mBestFitness = 0.0f;
    mFirstObject = true;
    mFileNamePrefix = "";
    mFileNameSuffix = "";
    mBrainString = "";
    mLogOnlyLast = false;
}

LogBestBrain::~LogBestBrain()
{
}

void LogBestBrain::process(SimulationObject* obj, PopulationManager* popManager)
{
    if (mFirstObject || (obj->mFitness > mBestFitness))
    {
        mBestFitness = obj->mFitness;
        mFirstObject = false;
       //TODO: check if object is agent
       Agent* agent = (Agent*)obj;
       mBrainString = agent->getBrain()->write(agent, (Simulation*)popManager);
    }
}

void LogBestBrain::dump(unsigned long time, double realTime)
{
    char fileName[255];
    if (mLogOnlyLast)
    {
        sprintf(fileName, "%s%s", mFileNamePrefix.c_str(), mFileNameSuffix.c_str());
    }
    else
    {
        sprintf(fileName, "%s%d%s", mFileNamePrefix.c_str(), time / 1000, mFileNameSuffix.c_str());
    }    
    FILE* file = fopen(fileName, "w");
    fprintf(file, mBrainString.c_str());
    fflush(file);
    fclose(file);

    mFirstObject = true;
    mBrainString = "";
}

const char LogBestBrain::mClassName[] = "LogBestBrain";

Orbit<LogBestBrain>::MethodType LogBestBrain::mMethods[] = {
    {"setFileNamePrefix", &LogBestBrain::setFileNamePrefix},
    {"setFileNameSuffix", &LogBestBrain::setFileNameSuffix},
    {"setLogOnlyLast", &LogBestBrain::setLogOnlyLast},
    {0,0}
};

Orbit<LogBestBrain>::NumberGlobalType LogBestBrain::mNumberGlobals[] = {{0,0}};

int LogBestBrain::setFileNamePrefix(lua_State* luaState)
{
    string prefix = luaL_checkstring(luaState, 1);
    setFileNamePrefix(prefix);

    return 0;
}

int LogBestBrain::setFileNameSuffix(lua_State* luaState)
{
    string suffix = luaL_checkstring(luaState, 1);
    setFileNameSuffix(suffix);

    return 0;
}

int LogBestBrain::setLogOnlyLast(lua_State* luaState)
{
    bool onlyLast = luaL_checkbool(luaState, 1);
    setLogOnlyLast(onlyLast);

    return 0;
}

