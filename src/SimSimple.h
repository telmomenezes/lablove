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

#if !defined(__INCLUDE_SIM_SIMPLE_H)
#define __INCLUDE_SIM_SIMPLE_H

#include "Simulation.h"
#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "Agent.h"

#include "pyc.h"

#include <list>

#include "Orbit.h"

class SimSimple : public Simulation
{
public:

	enum Perception {PERCEPTION_NULL,
				PERCEPTION_POSITION,
				PERCEPTION_PROXIMITY,
				PERCEPTION_IN_CONTACT};

	enum Action {ACTION_NULL,
			ACTION_GO,
			ACTION_ROTATE,
			ACTION_EAT};


        SimSimple(lua_State* luaState=NULL);
	virtual ~SimSimple();


	virtual void processObjects();

	virtual void drawBeforeObjects();

	virtual void removeObject(SimulationObject* object);

	void setGoCost(float cost){mGoCost = cost;}
	void setRotateCost(float cost){mRotateCost = cost;}
	
	void setShowGrid(bool show){mShowGrid = show;}
	void setShowViewRange(bool show){mShowViewRange = show;}
	bool getShowGrid(){return mShowGrid;}
	bool getShowViewRange(){return mShowViewRange;}
	
	SimulationObject* getObjectByScreenPos(int x, int y);

	virtual bool onKeyDown(pyc::KeyCode keycode);
	virtual bool onKeyUp(pyc::KeyCode keycode);
	virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y);
	virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y);

	void setViewRange(float range);
	void setViewAngle(float angle);
	float getViewRange(){return mViewRange;}
	float getViewAngle(){return mViewAngle;}

	static const char mClassName[];
        static Orbit<SimSimple>::MethodType mMethods[];
	static Orbit<SimSimple>::NumberGlobalType mNumberGlobals[];

	int setViewRange(lua_State* luaState);
	int setViewAngle(lua_State* luaState);
	int setGoCost(lua_State* luaState);
	int setRotateCost(lua_State* luaState);

protected:
	void perceive(Agent* agent);
	void act(Agent* agent);
	void scanCell(Agent* agent, int cellX, int cellY);
	virtual void onScanObject(Agent* orig,
					SimulationObject* targ,
					bool contact,
					float angle,
					float distance);
	void goFront(Agent* agent, float distance);
	void rotate(Agent* agent, float angle);
	void eat(Agent* agent);

	float mViewRange;
	float mViewAngle;
	float mHalfViewAngle;
	float mLowLimitViewAngle;
	float mHighLimitViewAngle;

	bool mShowGrid;
	bool mShowViewRange;

	float mGoCost;
	float mRotateCost;

	SimulationObject* mTargetObject;
	float mDistanceToTargetObject;
};
#endif

