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

#if !defined(__INCLUDE_STATISTICS_H)
#define __INCLUDE_STATISTICS_H

#include "SimulationObject.h"
#include "PopulationManager.h"

#include "Orbit.h"

#include <stdio.h>

#include <string>
#include <list>

using std::string;
using std::list;

class Statistics
{
public:
    Statistics();
    virtual ~Statistics();

    virtual void init(){}

    void addField(string fieldName);
    void setFile(string filePath);
    virtual void process(SimulationObject* obj, PopulationManager* popManager)=0;
    virtual void dump(unsigned long time, double realTime)=0;

    int addField(lua_State* luaState);
    int setFile(lua_State* luaState);

protected:
    list<string> mFields;
    FILE* mFile;
};
#endif

