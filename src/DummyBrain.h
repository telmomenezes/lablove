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

#if !defined(__INCLUDE_DUMMY_BRAIN_H)
#define __INCLUDE_DUMMY_BRAIN_H

#include "Brain.h"
#include "InterfaceItem.h"
#include "Orbit.h"

#include <list>
#include <vector>
#include <string>

using std::list;
using std::vector;
using std::string;

class DummyBrain : public Brain
{
public:
    DummyBrain(unsigned int channelCount);
    DummyBrain(lua_State* luaState);
    virtual ~DummyBrain();

    virtual Brain* clone(bool randomize=false);

    virtual void init();

    virtual float* getInputBuffer(unsigned int channel);
    virtual float* getOutputBuffer(){return NULL;}
    virtual void cycle();
    virtual void mutate(){}
    virtual Brain* recombine(Brain* brain){return this->clone();}
 
    void addPerception(string name,
                        unsigned int channel,
                        unsigned int type,
                        int origSymTable=-1,
                        int originSymIndex=-1,
                        int targetSymTable=-1,
                        int targetSymIndex=-1);

    void setChannelName(int channel, string name){mChannels[name] = channel;}

    static const char mClassName[];
    static Orbit<DummyBrain>::MethodType mMethods[];
    static Orbit<DummyBrain>::NumberGlobalType mNumberGlobals[];

    int addPerception(lua_State* luaState);
    int setChannelName(lua_State* luaState);

    virtual void draw();

protected:
    vector<list<string>*> mInputInterfacesNames;
    vector<unsigned int> mOffsetsVector;
    vector<unsigned int> mInputDepths;
    vector<unsigned int> mLastInputDepths;
    unsigned int mChannelCount;
    float* mInputBuffer;
    
};
#endif

