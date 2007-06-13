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

#if !defined(__INCLUDE_SIMULATION_OBJECT_H)
#define __INCLUDE_SIMULATION_OBJECT_H

#include "Chemistry.h"
#include "Orbit.h"

#include <map>

using namespace std;

class SimulationObject
{
public:
	static unsigned long CURRENT_ID;

	SimulationObject();
	SimulationObject(SimulationObject* obj);
	virtual ~SimulationObject();

	virtual SimulationObject* clone(bool full=true) = 0;
#ifdef __LABLOVE_WITH_GRAPHICS
	virtual void createGraphics(){}
#endif
	virtual void initRandom(){}

	unsigned long getID(){return mID;}

	virtual void onCycle(){}
	virtual bool isFood(){return false;}
	void setSelected(bool selected){mSelected = selected;}
	bool isSelected(){return mSelected;}

        unsigned int getSpeciesID(){return mSpeciesID;}
	void setSpeciesID(unsigned int id){mSpeciesID = id;}
	float getEnergy(){return mEnergy;}
	float getInitialEnergy(){return mInitialEnergy;}
	void deltaEnergy(double delta){mEnergy += delta;}
	void setEnergy(float energy){mEnergy = energy;}
	void setInitialEnergy(float energy){mInitialEnergy = energy;}

	virtual SimulationObject* crossover(SimulationObject* otherParent){return this;}
	virtual void mutate(){}

	virtual void placeRandom()=0;

	int setInitialEnergy(lua_State* luaState);

	bool mDeleted;

protected:
	unsigned long mID;
	bool mSelected;
        unsigned int mSpeciesID;
	float mEnergy;
	float mInitialEnergy;
	unsigned long mCreationTime;
	map<string, Chemistry*> mChemistriesMap;
};
#endif

