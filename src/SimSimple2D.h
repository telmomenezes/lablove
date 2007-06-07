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

#if !defined(__INCLUDE_SIM_SIMPLE_2D_H)
#define __INCLUDE_SIM_SIMPLE_2D_H

#include "Simulation.h"
#include "ObjectSimple2D.h"
#include "PopulationDynamics.h"

#include <list>
using namespace std;

#include "Orbit.h"

class SimSimple2D : public Simulation
{
public:

	enum Perception {PERCEPTION_NULL,
				PERCEPTION_COLOR,
				PERCEPTION_POSITION,
				PERCEPTION_PROXIMITY,
				PERCEPTION_IN_CONTACT};

	enum Action {ACTION_NULL,
			ACTION_GO,
			ACTION_ROTATE,
			ACTION_EAT};

	SimSimple2D();
	virtual ~SimSimple2D();

	virtual void init();

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

#ifdef __LOVELAB_WITH_GRAPHICS
	virtual bool onKeyDown(int keycode);
	virtual bool onKeyUp(int keycode);
	virtual bool onMouseButtonDown(int button, int x, int y);
	virtual bool onMouseButtonUp(int button, int x, int y);
	virtual bool onMouseMove(int x, int y);
#endif

	static const char mClassName[];
        static Orbit<SimSimple2D>::MethodType mMethods[];
	static Orbit<SimSimple2D>::NumberGlobalType mNumberGlobals[];

        SimSimple2D(lua_State* luaState);
        int setWorldDimensions(lua_State* luaState);

private:
#ifdef __LOVELAB_WITH_GRAPHICS
	//virtual void drawBeforeObjects();
#endif

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

