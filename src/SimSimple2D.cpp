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
#include "Lab.h"
#include "ObjectSimple2D.h"
#include "functions.h"
#include "PopulationDynamics.h"
#include "defines.h"
#include "AnimatSimple2D.h"
#include "SDL.h"

#include <math.h>

SimSimple2D::SimSimple2D(lua_State* luaState)
{
	mShowGrid = false;
	mShowViewRange = false;
	mCellGrid = NULL;
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

SimulationObject* SimSimple2D::getObjectByScreenPos(int x, int y)
{
	return NULL;
}

void SimSimple2D::drawBeforeObjects()
{
	if (mShowGrid)
	{
		unsigned int cellSide = (unsigned int)mCellSide;

		int mViewX = 0;
		int mViewY = 0;

		Lab::getSingleton().getScreen()->setColor(0.7, 0.7, 0.7);

		unsigned int division = cellSide - (mViewX % cellSide);
		while (division < Lab::getSingleton().getScreen()->getWidth())
		{
			Lab::getSingleton().getScreen()->drawLine(division,
									0,
									division,
									Lab::getSingleton().getScreen()->getHeight());

			division += cellSide;
		}

		division = cellSide - (mViewY % cellSide);
		while (division < Lab::getSingleton().getScreen()->getHeight())
		{
			Lab::getSingleton().getScreen()->drawLine(0,
									division,
									Lab::getSingleton().getScreen()->getWidth(),
									division);

			division += cellSide;
		}
	}
}

#ifdef __LABLOVE_WITH_GRAPHICS
bool SimSimple2D::onKeyDown(int key)
{
	AnimatSimple2D* human = (AnimatSimple2D*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case SDLK_UP:
		human->mActionGo = true;
		return true;
	case SDLK_RIGHT:
		human->mActionRotate = true;
		human->mActionRotateParam = 1.0f;
		return true;
	case SDLK_LEFT:
		human->mActionRotate = true;
		human->mActionRotateParam = -1.0;
		return true;
	case SDLK_e:
		human->mActionEat = true;
		return true;
	default:
		return false;
	}
}

bool SimSimple2D::onKeyUp(int key)
{
	switch (key)
	{
	case SDLK_g:
		setShowGrid(!getShowGrid());
		return true;
	case SDLK_v:
		setShowViewRange(!getShowViewRange());
		return true;
	default:
		break;
	}

	AnimatSimple2D* human = (AnimatSimple2D*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case SDLK_UP:
		human->mActionGo = false;
		return true;
	case SDLK_RIGHT:
		human->mActionRotate = false;
		return true;
	case SDLK_LEFT:
		human->mActionRotate = false;
		return true;
	case SDLK_e:
		human->mActionEat = false;
		return true;
	default:
		return false;
	}
}

bool SimSimple2D::onMouseButtonDown(int button, int x, int y)
{
	return false;
}

bool SimSimple2D::onMouseButtonUp(int button, int x, int y)
{
	return false;
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

