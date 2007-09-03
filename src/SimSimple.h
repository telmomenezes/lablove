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
#include "ObjectSimple2D.h"
#include "PopulationDynamics.h"

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

	virtual void drawBeforeObjects();

	void setWorldDimensions(float worldWidth,
			float worldLength,
			float cellSide);
	virtual void removeObject(SimulationObject* object);
	float getWorldWidth(){return mWorldWidth;}
	float getWorldLength(){return mWorldLength;}
	float getCellSide(){return mCellSide;}
	ObjectSimple2D** getCellGrid(){return mCellGrid;}
	void setShowGrid(bool show){mShowGrid = show;}
	void setShowViewRange(bool show){mShowViewRange = show;}
	bool getShowGrid(){return mShowGrid;}
	bool getShowViewRange(){return mShowViewRange;}
	unsigned int getWorldCellWidth(){return mWorldCellWidth;}
	unsigned int getWorldCellLength(){return mWorldCellLength;}
	
	SimulationObject* getObjectByScreenPos(int x, int y);

	virtual bool onKeyDown(pyc::KeyCode keycode);
	virtual bool onKeyUp(pyc::KeyCode keycode);
	virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y);
	virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y);

	static const char mClassName[];
        static Orbit<SimSimple>::MethodType mMethods[];
	static Orbit<SimSimple>::NumberGlobalType mNumberGlobals[];

        int setWorldDimensions(lua_State* luaState);

private:

	float mWorldWidth;
	float mWorldLength;

	float mCellSide;
	unsigned int mWorldCellWidth;
	unsigned int mWorldCellLength;
	ObjectSimple2D** mCellGrid;
	bool mShowGrid;
	bool mShowViewRange;
};
#endif

