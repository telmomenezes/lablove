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

#include "ObjectSimple2D.h"
#include "Lab.h"
#include "SimSimple2D.h"
#include <stdlib.h>
#include "functions.h"
#include "random.h"
#include <math.h>

ObjectSimple2D::ObjectSimple2D()
{
	mX = -1.0f;
	mY = -1.0f;
	mSize = 1.0f;
	mSizeSquared = 1.0f;
	mRot = 0.0f;

	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;

	mMaxAge = 0;
	mLowAgeLimit = 0;
	mHighAgeLimit = 0;
	mMetabolism = 0.0f;
#if defined(__LABLOVE_WITH_GRAPHICS)
	mNode = NULL;
	mMaterial.setNull();
	mZ = randomUniformProbability() / 100.0f;
#endif
}

ObjectSimple2D::ObjectSimple2D(ObjectSimple2D* obj) : SimulationObject(obj)
{
	mSize = obj->mSize;
	mSizeSquared = obj->mSizeSquared;
	mLowAgeLimit = obj->mLowAgeLimit;
	mHighAgeLimit = obj->mHighAgeLimit;
	
	if (mHighAgeLimit > 0)
	{
		mMaxAge = randomUniformInt(mLowAgeLimit, mHighAgeLimit);
	}
	else
	{
		mMaxAge = 0;
	}
	mMetabolism = obj->mMetabolism;

	mX = -1.0f;
	mY = -1.0f;
	mRot = 0.0f;

	mColor = MoleculeRGB(obj->mColor);
#if defined(__LABLOVE_WITH_GRAPHICS)
	mNode = NULL;
	mMaterial.setNull();
	mZ = randomUniformProbability() / 100.0f;
#endif
}

ObjectSimple2D::~ObjectSimple2D()
{
#if defined(__LABLOVE_WITH_GRAPHICS)
	if (mNode != NULL)
	{
		Ogre::SceneManager* sceneMgr = Lab::getSingleton().getOgreApplication()->getSceneManager();
		char nodeName[255];
		sprintf(nodeName, "loveobj%d", mID);
		Ogre::MovableObject* obj = mNode->detachObject(nodeName);
		sceneMgr->getRootSceneNode()->removeAndDestroyChild(nodeName);
		delete obj;
	}
	if (!mMaterial.isNull())
	{
		Ogre::MaterialManager::getSingleton().remove(mMaterial->getName());
		mMaterial.setNull();
	}
#endif
}

void ObjectSimple2D::setPos(float x, float y)
{
	SimSimple2D* sim = (SimSimple2D*)(Lab::getSingleton().getSimulation());

	if ((x < 0)
		|| (y < 0)
		|| (x >= sim->getWorldWidth())
		|| (y >= sim->getWorldLength()))
	{
		return;
	}

	unsigned int cellSide = (unsigned int)(sim->getCellSide());
	unsigned int targetCellX = ((unsigned int)x) / cellSide;
	unsigned int targetCellY = ((unsigned int)y) / cellSide;
	ObjectSimple2D** objGrid = sim->getCellGrid();
	unsigned int worldCellWidth = sim->getWorldCellWidth();
	unsigned int targetCellPos = (targetCellY * worldCellWidth) + targetCellX;
	ObjectSimple2D* targetCell = objGrid[targetCellPos];

	if (mX >= 0)
	{
		if ((mCellX != targetCellX) || (mCellY != targetCellY))
		{
			// Remove from origin cell
			if (mNextCellList != NULL)
			{
				mNextCellList->mPrevCellList = mPrevCellList;
			}
			if (mPrevCellList == NULL)
			{
				objGrid[mCellPos] = mNextCellList;
			}
			else
			{
				mPrevCellList->mNextCellList = mNextCellList;
			}

			// Insert in new target cell
			mNextCellList = targetCell;
			mPrevCellList = NULL;
			if (targetCell != NULL)
			{
				targetCell->mPrevCellList = this;
			}
			objGrid[targetCellPos] = this;
			mCellX = targetCellX;
			mCellY = targetCellY;
			mCellPos = targetCellPos;
		}
	}
	else
	{
		// Insert in target cell
		mNextCellList = targetCell;
		mPrevCellList = NULL;
		if (targetCell != NULL)
		{
			targetCell->mPrevCellList = this;
		}
		objGrid[targetCellPos] = this;
		mCellX = targetCellX;
		mCellY = targetCellY;
		mCellPos = targetCellPos;
	}

	mX = x;
	mY = y;

#if defined(__LABLOVE_WITH_GRAPHICS)
	if (mNode != NULL)
	{
		mNode->setPosition(mX, mZ, mY);
	}
#endif
}

void ObjectSimple2D::setSize(float size)
{
	mSize = size;
	mSizeSquared = mSize * mSize;
#if defined(__LABLOVE_WITH_GRAPHICS)
	if (mNode != NULL)
	{
		mNode->scale(size, size, size);
	}
#endif
}

void ObjectSimple2D::setRot(float rot)
{
	mRot = normalizeAngle(rot);
#if defined(__LABLOVE_WITH_GRAPHICS)
	if (mNode != NULL)
	{
		mNode->setOrientation(Ogre::Quaternion(Ogre::Radian(mRot),
						Ogre::Vector3(0.0, -1.0, 0.0)));
	}
#endif
}

void ObjectSimple2D::placeRandom()
{
	SimSimple2D* sim = (SimSimple2D*)Lab::getSingleton().getSimulation();

	unsigned int worldWidth = (unsigned int)sim->getWorldWidth();
	unsigned int worldLength = (unsigned int)sim->getWorldLength();

	setPos(rand() % worldWidth, rand() % worldLength);
	setRot(randomUniformProbability() * M_PI * 2);
}

void ObjectSimple2D::onCycle()
{
	mEnergy -= mMetabolism;

	if (mEnergy < 0)
	{
		Lab::getSingleton().getSimulation()->killOrganism(this);
	}

	if (mMaxAge > 0)
	{
		if (Lab::getSingleton().getSimulation()->time() - mCreationTime >= mMaxAge)
		{
			Lab::getSingleton().getSimulation()->killOrganism(this);
		}
	}
}

void ObjectSimple2D::setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit)
{
	mLowAgeLimit = lowAgeLimit;
	mHighAgeLimit = highAgeLimit;
}

void ObjectSimple2D::setColor(MoleculeRGB* color)
{
	mColor = MoleculeRGB(color);
#if defined(__LABLOVE_WITH_GRAPHICS)
	if (!mMaterial.isNull())
	{
		mMaterial->setAmbient(mColor.mRed, mColor.mGreen, mColor.mBlue);
	}
#endif
}

float ObjectSimple2D::getFieldValue(std::string fieldName)
{
	if (fieldName == "energy")
	{
		return mEnergy;
	}
	else
	{
		return SimulationObject::getFieldValue(fieldName);
	}
}

int ObjectSimple2D::setPos(lua_State* luaState)
{
        float x = luaL_checknumber(luaState, 1);
	float y = luaL_checknumber(luaState, 2);
        setPos(x, y);
        return 0;
}

int ObjectSimple2D::setSize(lua_State* luaState)
{
        int size = luaL_checkint(luaState, 1);
        setSize(size);
        return 0;
}

int ObjectSimple2D::setRot(lua_State* luaState)
{
        float rot = luaL_checknumber(luaState, 1);
        setRot(rot);
        return 0;
}

int ObjectSimple2D::setAgeRange(lua_State* luaState)
{
        int lowAgeLimit = luaL_checkint(luaState, 1);
        int highAgeLimit = luaL_checkint(luaState, 2);
        setAgeRange(lowAgeLimit, highAgeLimit);
        return 0;
}

int ObjectSimple2D::setMetabolism(lua_State* luaState)
{
        float metabolism = luaL_checknumber(luaState, 1);
        setMetabolism(metabolism);
        return 0;
}

int ObjectSimple2D::setColor(lua_State* luaState)
{
	MoleculeRGB* color = (MoleculeRGB*)Orbit<Lab>::pointer(luaState, 1);
        setColor(color);
        return 0;
}

