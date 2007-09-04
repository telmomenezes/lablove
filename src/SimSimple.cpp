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

#include "SimSimple.h"
#include "Lab.h"
#include "SimulationObject.h"
#include "functions.h"
#include "PopulationDynamics.h"
#include "defines.h"

// TODO: remove this dependency
#include "AgentGB.h"

#include <math.h>

SimSimple::SimSimple(lua_State* luaState)
{
	mShowGrid = false;
	mShowViewRange = false;
}

SimSimple::~SimSimple()
{
}

void SimSimple::removeObject(SimulationObject* object)
{
	SimulationObject* obj = (SimulationObject*)object;

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

SimulationObject* SimSimple::getObjectByScreenPos(int x, int y)
{
	return NULL;
}

void SimSimple::drawBeforeObjects()
{
	if (mShowGrid)
	{
		unsigned int cellSide = (unsigned int)mCellSide;

		int mViewX = 0;
		int mViewY = 0;

		Lab::getSingleton().getRootLayer()->setColor(200, 200, 200);

		unsigned int division = cellSide - (mViewX % cellSide);
		while (division < Lab::getSingleton().getRootLayer()->getWidth())
		{
			Lab::getSingleton().getRootLayer()->drawLine(division,
									0,
									division,
									Lab::getSingleton().getRootLayer()->getHeight());

			division += cellSide;
		}

		division = cellSide - (mViewY % cellSide);
		while (division < Lab::getSingleton().getRootLayer()->getHeight())
		{
			Lab::getSingleton().getRootLayer()->drawLine(0,
									division,
									Lab::getSingleton().getRootLayer()->getWidth(),
									division);

			division += cellSide;
		}
	}
}

bool SimSimple::onKeyDown(pyc::KeyCode key)
{
	AgentGB* human = (AgentGB*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case pyc::KEY_UP:
		human->mActionGo = true;
		return true;
	case pyc::KEY_RIGHT:
		human->mActionRotate = true;
		human->mActionRotateParam = 1.0f;
		return true;
	case pyc::KEY_LEFT:
		human->mActionRotate = true;
		human->mActionRotateParam = -1.0;
		return true;
	case pyc::KEY_E:
		human->mActionEat = true;
		return true;
	default:
		return false;
	}
}

bool SimSimple::onKeyUp(pyc::KeyCode key)
{
	switch (key)
	{
	case pyc::KEY_G:
		setShowGrid(!getShowGrid());
		return true;
	case pyc::KEY_V:
		setShowViewRange(!getShowViewRange());
		return true;
	default:
		break;
	}

	AgentGB* human = (AgentGB*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case pyc::KEY_UP:
		human->mActionGo = false;
		return true;
	case pyc::KEY_RIGHT:
		human->mActionRotate = false;
		return true;
	case pyc::KEY_LEFT:
		human->mActionRotate = false;
		return true;
	case pyc::KEY_E:
		human->mActionEat = false;
		return true;
	default:
		return false;
	}
}

bool SimSimple::onMouseButtonDown(pyc::MouseButton button, int x, int y)
{
	return false;
}

bool SimSimple::onMouseButtonUp(pyc::MouseButton button, int x, int y)
{
	return false;
}

const char SimSimple::mClassName[] = "SimSimple";

Orbit<SimSimple>::MethodType SimSimple::mMethods[] = {
	{"setPopulationDynamics", &Simulation::setPopulationDynamics},
        {"setWorldDimensions", &Simulation::setWorldDimensions},
        {0,0}
};

Orbit<SimSimple>::NumberGlobalType SimSimple::mNumberGlobals[] = {
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

