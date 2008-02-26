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

#if !defined(__INCLUDE_BRAIN_H)
#define __INCLUDE_BRAIN_H

#include "InterfaceItem.h"
#include "SimulationObject.h"
#include "PopulationManager.h"

#include "art.h"

#include <list>
#include <vector>
#include <map>
#include <string>

using std::list;
using std::vector;
using std::map;
using std::string;

class Brain
{
public:
    Brain();
    virtual ~Brain();

    virtual Brain* clone(bool randomize=false)=0;

    virtual void init()=0;

    list<InterfaceItem*>* getInputInterface(unsigned int channel);
    list<InterfaceItem*>* getOutputInterface();
    virtual float* getInputBuffer(unsigned int channel)=0;
    virtual float* getOutputBuffer()=0;
    virtual void cycle()=0;

    virtual void mutate()=0;
    virtual Brain* recombine(Brain* brain)=0;

    virtual void draw()=0;

    void setOwner(SimulationObject* obj){mOwner = obj;}

    virtual bool getFieldValue(string fieldName, float& value);

    int getChannelByName(string name);
    virtual string write(SimulationObject* obj, PopulationManager* pop){return "";}

    virtual bool symbolUsed(int tableID, unsigned long symbolID){return false;}

protected:
    void clearInterfaces();

    vector<list<InterfaceItem*>*> mInputInterfacesVector;
    list<InterfaceItem*> mOutputInterface;
    map<string, int> mChannels;
    SimulationObject* mOwner;
};
#endif

