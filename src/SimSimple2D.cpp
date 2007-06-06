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

#include "SimSimple2D.h"
#include "LoveLab.h"
#include "ObjectSimple2D.h"
#include <math.h>
#include "functions.h"
#include "PopulationDynamics.h"
#include "defines.h"

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

SimSimple2D::SimSimple2D()
{
	mViewX = 0;
	mViewY = 0;
	mShowGrid = false;
	mShowViewRange = false;
	mCellGrid = NULL;
	mLastMouseX = 0;
	mLastMouseY = 0;
	mMousePressed = false;
}

SimSimple2D::SimSimple2D(lua_State* luaState)
{
	mViewX = 0;
	mViewY = 0;
	mShowGrid = false;
	mShowViewRange = false;
	mCellGrid = NULL;
	mLastMouseX = 0;
	mLastMouseY = 0;
	mMousePressed = false;
}

SimSimple2D::~SimSimple2D()
{
	if (mCellGrid != NULL)
	{
		free(mCellGrid);
		mCellGrid = NULL;
	}
}

void SimSimple2D::setWorldDimensions(float worldWidth,
			float worldLength,
			float cellSide)
{
	mWorldWidth = worldWidth;
	mWorldLength = worldLength;
	mCellSide = cellSide;
	mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / mCellSide));
	mWorldCellLength = (unsigned int)(ceilf(mWorldLength / mCellSide));

	unsigned int gridSize = mWorldCellWidth * mWorldCellLength;
	mCellGrid = (ObjectSimple2D**)malloc(sizeof(ObjectSimple2D*) * gridSize);

	for (unsigned int i = 0; i < gridSize; i++)
	{
		mCellGrid[i] = NULL;
	}

	
}

void SimSimple2D::removeObject(SimulationObject* object)
{
	ObjectSimple2D* obj = (ObjectSimple2D*)object;

	if (obj->mNextCellList != NULL)
	{
		obj->mNextCellList->mPrevCellList = obj->mPrevCellList;
	}

	if (obj->mPrevCellList == NULL)
	{
		int cellPos = obj->getCellPos();

		if(cellPos >= 0)
		{
			mCellGrid[cellPos] = obj->mNextCellList;
		}
	}
	else
	{
		obj->mPrevCellList->mNextCellList = obj->mNextCellList;
	}

	Simulation::removeObject(object);
}

void SimSimple2D::moveView(int deltaX, int deltaY)
{
	mViewX -= deltaX;
	mViewY -= deltaY;

	if (mViewX < 0)
	{
		mViewX = 0;
	}
	
	if (mViewY < 0)
	{
		mViewY = 0;
	}

	if ((mViewX + LoveLab::getInstance().getScreenWidth()) >= (unsigned int)mWorldWidth)
	{
		mViewX = ((unsigned int)mWorldWidth) - LoveLab::getInstance().getScreenWidth();
	}

	if ((mViewY + LoveLab::getInstance().getScreenHeight()) >= (unsigned int)mWorldLength)
	{
		mViewY = ((unsigned int)mWorldLength) - LoveLab::getInstance().getScreenHeight();
	}
}

SimulationObject* SimSimple2D::getObjectByScreenPos(int x, int y)
{
	list<SimulationObject*>::iterator iterObj;
	for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
	{
		ObjectSimple2D* obj = (ObjectSimple2D*)(*iterObj);

		float dx = obj->mX - (float)(mViewX + x);
		float dy = obj->mY - (float)(mViewY + y);
		dx *= dx;
		dy *= dy;

		if ((dx + dy) <= obj->mSizeSquared)
		{
			return obj;
		}
	}

	return NULL;
}

#ifdef __LOVE_GRAPHICS
/*
void SimSimple2D::drawBeforeObjects()
{
	if (mShowGrid)
	{
		unsigned int cellSide = (unsigned int)mCellSide;

		glColor3ub(DEF_S2D_GRID_COLOR_R,
			DEF_S2D_GRID_COLOR_G,
			DEF_S2D_GRID_COLOR_B);
		glBegin(GL_LINES);

		unsigned int division = cellSide - (mViewX % cellSide);
		while (division < LoveLab::getInstance().getScreenWidth())
		{
			glVertex2f(division, 0);
			glVertex2f(division, LoveLab::getInstance().getScreenHeight());

			division += cellSide;
		}

		division = cellSide - (mViewY % cellSide);
		while (division < LoveLab::getInstance().getScreenHeight())
		{
			glVertex2f(0, division);
			glVertex2f(LoveLab::getInstance().getScreenWidth(), division);

			division += cellSide;
		}

		glEnd();
	}
}
*/

bool SimSimple2D::onKeyDown(int key)
{
	switch (key)
	{
	/*case SDLK_LEFT:
		if (m_human)
		{
			m_human->setTurnLeft(true);
		}
		return true;
	case SDLK_RIGHT:
		if (m_human)
		{
			m_human->setTurnRight(true);
		}
		return true;
	case SDLK_UP:
		if (m_human)
		{
			m_human->setGo(true);
		}
		return true;
	case SDLK_e:
		if (m_human)
		{
			m_human->setEat(true);
		}
		return true;*/
	default:
		return false;
	}
}

bool SimSimple2D::onKeyUp(int key)
{
	switch (key)
	{
	/*case SDLK_LEFT:
		if (m_human)
		{
			m_human->setTurnLeft(false);
		}
		return true;
	case SDLK_RIGHT:
		if (m_human)
		{
			m_human->setTurnRight(false);
		}
		return true;
	case SDLK_UP:
		if (m_human)
		{
			m_human->setGo(false);
		}
		return true;
	case SDLK_e:
		if (m_human)
		{
			m_human->setEat(false);
		}
		return true;*/

	/*
	case SDLK_g:
		setShowGrid(!getShowGrid());
		return true;
	case SDLK_v:
		setShowViewRange(!getShowViewRange());
		return true;
	*/
	default:
		return false;
	}
}

bool SimSimple2D::onMouseButtonDown(int button, int x, int y)
{
	if (button == 1)
	{
		mLastMouseX = x;
		mLastMouseY = y;
		mMousePressed = true;

		SimulationObject* object = getObjectByScreenPos(x, y);

		if (object != NULL)
		{
			setSelectedObject(object);
			return true;
		}
	}

	return false;
}

bool SimSimple2D::onMouseButtonUp(int button, int x, int y)
{
	if (button == 1)
	{
		mMousePressed = false;
		return true;
	}

	return false;
}

bool SimSimple2D::onMouseMove(int x, int y)
{
	if (mMousePressed)
	{
		int newX = x;
		int newY = y;

		moveView(newX - mLastMouseX, newY - mLastMouseY);

		mLastMouseX = newX;
		mLastMouseY = newY;
	}
}
#endif

const char SimSimple2D::mClassName[] = "SimSimple2D";

Orbit<SimSimple2D>::MethodType SimSimple2D::mMethods[] = {
	{"setPopulationDynamics", &Simulation::setPopulationDynamics},
        {"setWorldDimensions", &SimSimple2D::setWorldDimensions},
        {0,0}
};

Orbit<SimSimple2D>::NumberGlobalType SimSimple2D::mNumberGlobals[] = {
	{"PERCEPTION_NULL", PERCEPTION_NULL},
	{"PERCEPTION_COLOR", PERCEPTION_COLOR},
	{"PERCEPTION_POSITION", PERCEPTION_POSITION},
	{"PERCEPTION_PROXIMITY", PERCEPTION_PROXIMITY},
	{"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
	{"ACTION_NULL", ACTION_NULL},
	{"ACTION_GO", ACTION_GO},
	{"ACTION_ROTATE", ACTION_ROTATE},
	{"ACTION_EAT", ACTION_EAT},
	{0,0}
};

int SimSimple2D::setWorldDimensions(lua_State* luaState)
{
        int width = luaL_checkint(luaState, 1);
        int height = luaL_checkint(luaState, 2);
        int cellSide = luaL_checkint(luaState, 3);
        setWorldDimensions(width, height, cellSide);
        return 0;
}

