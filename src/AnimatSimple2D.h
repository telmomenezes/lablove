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

#if !defined(__INCLUDE_ANIMAT_SIMPLE_2D_H)
#define __INCLUDE_ANIMAT_SIMPLE_2D_H

#include "ObjectSimple2D.h"
#include "Gridbrain.h"

#include "Orbit.h"

class AnimatSimple2D : public ObjectSimple2D
{
public:
        AnimatSimple2D(lua_State* luaState=NULL);
	AnimatSimple2D(AnimatSimple2D* anim, bool full=true);
	virtual ~AnimatSimple2D();
	virtual SimulationObject* clone(bool full=true);
	virtual void initRandom();
	void initTest();

	void setAlphaObjectsGrid(Grid* grid);
	void setBetaGrid(Grid* grid);

	virtual void setRot(float rot);
	virtual void perceptionStep();
	virtual void actionStep();
	virtual void onCycle();
	virtual void computationStep();
	virtual void draw();

	//virtual AnimatSimple2D* crossover(Animat* other_parent);
	virtual void mutate();

	void setViewRange(float range);
	void setViewAngle(float angle);
	float getViewRange(){return mViewRange;}
	float getViewAngle(){return mViewAngle;}

	void goFront(float distance);
	void rotate(float angle);
	void eat();

	void setGoCost(float cost){mGoCost = cost;}
	void setRotateCost(float cost){mRotateCost = cost;}

	bool mActionGo;
	float mActionGoParam;
	bool mActionRotate;
	float mActionRotateParam;
	bool mActionEat;
	float mActionEatParam;

	static const char mClassName[];
        static Orbit<AnimatSimple2D>::MethodType mMethods[];
	static Orbit<AnimatSimple2D>::NumberGlobalType mNumberGlobals[];

        int setViewRange(lua_State* luaState);
	int setViewAngle(lua_State* luaState);
	int setAlphaObjectsGrid(lua_State* luaState);
	int setBetaGrid(lua_State* luaState);
	int setGoCost(lua_State* luaState);
	int setRotateCost(lua_State* luaState);

	virtual float getFieldValue(std::string fieldName);

protected:
	virtual void onScanObject(SimulationObject* obj, bool visible, bool contact, float angle, float distance);
	void scanCell(int cellX, int cellY);

	Gridbrain* mGridbrain;

	unsigned int mCurrentInputDepth;
	unsigned int mNumberOfPerceptions;
	unsigned int mMaxInputDepth;
	unsigned int mInitialConnections;

	float mViewRange;
	float mViewAngle;
	float mHalfViewAngle;
	float mLowLimitViewAngle;
	float mHighLimitViewAngle;

	float mGoCost;
	float mRotateCost;

	SimulationObject* mNearestFood;
	float mDistanceToNearestFood;
};
#endif

