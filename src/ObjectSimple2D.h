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

#if !defined(__INCLUDE_OBJECT_SIMPLE_2D_H)
#define __INCLUDE_OBJECT_SIMPLE_2D_H

#include "SimulationObject.h"
#include "MoleculeRGB.h"

#include "Orbit.h"

#if defined(__LABLOVE_WITH_GRAPHICS)
#include "Ogre.h"
#endif

class ObjectSimple2D : public SimulationObject
{
public:
	ObjectSimple2D();
	ObjectSimple2D(ObjectSimple2D* obj);
	virtual ~ObjectSimple2D();
	
	virtual void onCycle();

	virtual void setPos(float x, float y);
	virtual void setSize(float size);
	virtual void setRot(float rot);
	float getX(){return mX;}
	float getY(){return mY;}
	float getSize(){return mSize;}
	float getRot(){return mRot;}
	//virtual void after_draw();
	int getCellX(){return mCellX;}
	int getCellY(){return mCellY;}
	int getCellPos(){return mCellPos;}

	virtual void placeRandom();

	void setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit);
	void setMetabolism(float metabolism){mMetabolism = metabolism;}

	void setColor(MoleculeRGB* color);
	MoleculeRGB* getColor(){return &mColor;}

        int setPos(lua_State* luaState);
        int setSize(lua_State* luaState);
        int setRot(lua_State* luaState);
	int setAgeRange(lua_State* luaState);
	int setMetabolism(lua_State* luaState);
	int setColor(lua_State* luaState);
	virtual void setShowViewRange(bool show){}

	virtual float getFieldValue(std::string fieldName);

	float mX;
	float mY;
	float mSize;
	float mSizeSquared;
	float mRot;

	ObjectSimple2D* mNextCellList;
	ObjectSimple2D* mPrevCellList;

	int mCellX;
	int mCellY;
	int mCellPos;

protected:
	unsigned long mLowAgeLimit;
	unsigned long mHighAgeLimit;
	unsigned long mMaxAge;
	float mMetabolism;
	MoleculeRGB mColor;
#if defined(__LABLOVE_WITH_GRAPHICS)
	Ogre::SceneNode* mNode;
	Ogre::MaterialPtr mMaterial;
	float mZ;
#endif
};
#endif

