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

#if !defined(__INCLUDE_LOG_BEST_BRAIN_H)
#define __INCLUDE_LOG_BEST_BRAIN_H

#include "Log.h"
#include "SimObj.h"

#include "Orbit.h"

#include <string>
using std::string;

class LogBestBrain : public Log
{
public:
    LogBestBrain(lua_State* luaState=NULL);
    virtual ~LogBestBrain();

    virtual void init(){}

    virtual void process(SimObj* obj, Simulation* sim);
    virtual void dump(gbULINT time, double realTime);

    void setFileNamePrefix(string prefix){mFileNamePrefix = prefix;}
    void setFileNameSuffix(string suffix){mFileNameSuffix = suffix;}
    void setLogOnlyLast(bool onlyLast){mLogOnlyLast = onlyLast;}

    static const char mClassName[];
    static Orbit<LogBestBrain>::MethodType mMethods[];
    static Orbit<LogBestBrain>::NumberGlobalType mNumberGlobals[];

    int setFileNamePrefix(lua_State* luaState);
    int setFileNameSuffix(lua_State* luaState);
    int setLogOnlyLast(lua_State* luaState);

protected:
    unsigned int mFitIndex;
    float mBestFitness;
    bool mFirstObject;
    string mBrainString;
    string mFileNamePrefix;
    string mFileNameSuffix;
    bool mLogOnlyLast;
};
#endif

