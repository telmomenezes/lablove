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
#include "PopulationDynamics.h"
#include "functions.h"
#include "defines.h"

#include <math.h>

SimSimple::SimSimple(lua_State* luaState)
{
	mShowGrid = false;
	mShowViewRange = false;
	
	mViewRange = 0.0f;
	mViewAngle = 0.0f;
	mHalfViewAngle = 0.0f;
	mLowLimitViewAngle = 0.0f;
	mHighLimitViewAngle = 0.0f;

	mGoCost = 0.0f;
	mRotateCost = 0.0f;
}

SimSimple::~SimSimple()
{
}

void SimSimple::processObjects()
{
	std::list<SimulationObject*>::iterator iterObj;

	for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
	{
		SimulationObject* obj = *iterObj;

		obj->onCycle();

		if (obj->mType == SimulationObject::TYPE_AGENT)
		{
			Agent* agent = (Agent*)obj;
			perceive(agent);
			agent->compute();
			act(agent);
		}
	}
}

void SimSimple::perceive(Agent* agent)
{
	mTargetObject = NULL;
	mDistanceToTargetObject = 9999999999.9f;

	mLowLimitViewAngle = normalizeAngle(agent->mRot - mHalfViewAngle);
	mHighLimitViewAngle = normalizeAngle(agent->mRot + mHalfViewAngle);

	// Determine cells to analyse
	unsigned int cellSide = (unsigned int)mCellSide;
	int x = (int)agent->mX;
	int y = (int)agent->mY;
	int cellPosX = x % cellSide;
	int cellPosY = y % cellSide;
	int range = (int)mViewRange;
	int leftLimit = cellPosX - range; 
	int rightLimit = cellPosX + range;
	int topLimit = cellPosY - range;
	int bottomLimit = cellPosY + range;

	unsigned int cellX = ((unsigned int)x) / cellSide;
	unsigned int cellY = ((unsigned int)y) / cellSide;

	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	if ((cellX > 0) && (leftLimit < 0))
	{
		left = true;
	}
	if ((cellX < (mWorldCellWidth - 1))
		&& (rightLimit >= cellSide))
	{
		right = true;
	}
	if ((cellY > 0) && (topLimit < 0))
	{
		top = true;
	}
	if ((cellY < (mWorldCellLength - 1))
		&& (bottomLimit >= cellSide))
	{
		bottom = true;
	}

	// Check all objects in each cell
	scanCell(agent, cellX, cellY);
	if (left)
	{
		scanCell(agent, cellX - 1, cellY);
	}
	if (left && top)
	{
		scanCell(agent, cellX - 1, cellY - 1);
	}
	if (left && bottom)
	{
		scanCell(agent, cellX - 1, cellY + 1);
	}
	if (right)
	{
		scanCell(agent, cellX + 1, cellY);
	}
	if (right && top)
	{
		scanCell(agent, cellX + 1, cellY - 1);
	}
	if (right && bottom)
	{
		scanCell(agent, cellX + 1, cellY + 1);
	}
	if (top)
	{
		scanCell(agent, cellX, cellY - 1);
	}
	if (bottom)
	{
		scanCell(agent, cellX, cellY + 1);
	}
}

void SimSimple::scanCell(Agent* agent, int cellX, int cellY)
{
	SimulationObject* target = mCellGrid[(cellY * mWorldCellWidth) + cellX];

	while (target)
	{
		if (target != agent)
		{
			bool visible = false;
			bool contact = false;

			float dX = agent->mX - target->mX;
			float dY = agent->mY - target->mY;
			float distance = sqrtf((dX * dX) + (dY * dY));
			float angle = 0.0f;

			if (distance <= mViewRange)
			{
				angle = atan2f(-dY, -dX);

				if (mHighLimitViewAngle > mLowLimitViewAngle)
				{
					if ((angle <= mHighLimitViewAngle) && (angle >= mLowLimitViewAngle))
					{
						visible = true;
					}
				}
				else
				{
					if ((angle <= mHighLimitViewAngle) || (angle >= mLowLimitViewAngle))
					{
						visible = true;
					}
				}

				if (distance <= agent->getSize() + target->getSize())
				{
					contact = true;
				}
			}

			if (contact)
			{
				if ((mTargetObject == NULL) || (distance < mDistanceToTargetObject))
				{
					mTargetObject = target;
					mDistanceToTargetObject = distance;
				}
			}

			if (visible)
			{
				onScanObject(agent,
						target,
						contact,
						normalizeAngle(agent->mRot - angle),
						distance);
			}
		}

		target = target->mNextCellList;
	}
}

void SimSimple::onScanObject(Agent* orig,
				SimulationObject* targ,
				bool contact,
				float angle,
				float distance)
{
	if (orig->mHuman)
	{
		return;
	}

	float normalizedValue;

	if (orig->mCurrentInputDepth >= orig->mMaxInputDepth)
	{
		return;
	}

	Grid* grid = orig->mGridbrain->getGrid(0);

	unsigned int perceptionsCount = grid->getPerceptionsCount();
	for (unsigned int perceptionNumber = 0; perceptionNumber < perceptionsCount; perceptionNumber++)
	{
		GridbrainComponent* perception = grid->getPerception(perceptionNumber);

		switch (perception->mSubType)
		{
			case PERCEPTION_IN_CONTACT:
				if (contact)
				{
					grid->setInput(perceptionNumber, orig->mCurrentInputDepth, 1.0f);
				}
				else
				{
					grid->setInput(perceptionNumber, orig->mCurrentInputDepth, 0.0f);
				}
				break;

			case PERCEPTION_POSITION:
				normalizedValue = angle / mHalfViewAngle;
				grid->setInput(perceptionNumber, orig->mCurrentInputDepth, normalizedValue);
				break;

			case PERCEPTION_PROXIMITY:
				normalizedValue = 1.0f - (distance / mViewRange);
				grid->setInput(perceptionNumber, orig->mCurrentInputDepth, normalizedValue);
				break;

			default:
				normalizedValue = perception->computeBinding(orig, targ);
				grid->setInput(perceptionNumber, orig->mCurrentInputDepth, normalizedValue);
				break;
		}
	}

	orig->mCurrentInputDepth++;
}

void SimSimple::act(Agent* agent)
{
	bool actionGo = false;
	bool actionRotate = false;
	bool actionEat = false;
	float actionGoParam = 0.0f;
	float actionRotateParam = 0.0f;
	float actionEatParam = 0.0f;

	Grid* grid = agent->mGridbrain->getGrid(1);
	unsigned int actionsCount = grid->getActionsCount();

	for (unsigned int number = 0; number < actionsCount; number++)
	{
		float output = grid->getOutput(number);
		unsigned int actionType = grid->getAction(number)->mSubType;

		if (output != 0.0f)
		{
			switch (actionType)
			{
				case ACTION_GO:
					actionGo = true;
					actionGoParam += output;
					break;
				case ACTION_ROTATE:
					actionRotate = true;
					actionRotateParam += output;
					break;
				case ACTION_EAT:
					actionEat = true;
					actionEatParam += output;
					break;
			}
		}
	}

	if (actionGo)
	{
		goFront(agent, 1.0f);
	}
	if (actionRotate)
	{
		if (actionRotateParam > 0.0f)
		{
			rotate(agent, -0.01f);
		}
		else if (actionRotateParam < 0.0f)
		{
			rotate(agent, 0.01f);
		}
	}
	if (actionEat)
	{
		eat(agent);
	}
}

void SimSimple::goFront(Agent* agent, float distance)
{
	// TODO: check if move is possible
	agent->mEnergy -= mGoCost * distance;
	float newX = agent->mX + (cosf(agent->mRot) * distance);
	float newY = agent->mY + (sinf(agent->mRot) * distance);
	agent->setPos(newX, newY);
}

void SimSimple::rotate(Agent* agent, float angle)
{
	agent->mEnergy -= mRotateCost * angle;
	agent->setRot(agent->getRot() - angle);
}

void SimSimple::eat(Agent* agent)
{
	if ((mTargetObject) && (mTargetObject->isFood()))
	{
		agent->deltaEnergy(mTargetObject->getEnergy());
		Lab::getSingleton().getSimulation()->killOrganism(mTargetObject);
	}
}

void SimSimple::drawBeforeObjects()
{
	if (mShowGrid)
	{
		unsigned int cellSide = (unsigned int)mCellSide;

		int viewX = (int)mViewX;
		int viewY = (int)mViewY;

		Lab::getSingleton().getRootLayer()->setColor(200, 200, 200);

		unsigned int division = cellSide - ((cellSide - viewX) % cellSide);
		while (division < Lab::getSingleton().getRootLayer()->getWidth())
		{
			Lab::getSingleton().getRootLayer()->drawLine(division,
									0,
									division,
									Lab::getSingleton().getRootLayer()->getHeight());

			division += cellSide;
		}

		division = cellSide - ((cellSide - viewY) % cellSide);
		while (division < Lab::getSingleton().getRootLayer()->getHeight())
		{
			Lab::getSingleton().getRootLayer()->drawLine(0,
									division,
									Lab::getSingleton().getRootLayer()->getWidth(),
									division);

			division += cellSide;
		}
	}

	if (mShowViewRange)
	{
		std::list<SimulationObject*>::iterator iterObj;

		for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
		{
			SimulationObject* obj = *iterObj;

			if (obj->mType == SimulationObject::TYPE_AGENT)
			{
				float beginAngle = normalizeAngle(obj->mRot - mHalfViewAngle);
				float endAngle = normalizeAngle(obj->mRot + mHalfViewAngle);

				if (beginAngle > endAngle)
				{
					endAngle += M_PI * 2.0f;
				}

				Lab::getSingleton().getRootLayer()->setColor(150, 150, 150, 100);
				Lab::getSingleton().getRootLayer()->fillCircle(obj->mX + mViewX,
									obj->mY + mViewY,
									mViewRange,
									beginAngle,
									endAngle);
			}
		}
	}
	
}

bool SimSimple::onKeyDown(pyc::KeyCode key)
{
	/*switch (key)
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
	}*/
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

	/*switch (key)
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
	}*/
}

bool SimSimple::onMouseButtonDown(pyc::MouseButton button, int x, int y)
{
	return false;
}

bool SimSimple::onMouseButtonUp(pyc::MouseButton button, int x, int y)
{
	return false;
}

void SimSimple::setViewAngle(float angle)
{
	mViewAngle = (angle * M_PI) / 180.0f;
	mHalfViewAngle = mViewAngle / 2.0f;
}

void SimSimple::setViewRange(float range)
{
	mViewRange = range;
}

const char SimSimple::mClassName[] = "SimSimple";

Orbit<SimSimple>::MethodType SimSimple::mMethods[] = {
	{"setPopulationDynamics", &Simulation::setPopulationDynamics},
        {"setWorldDimensions", &Simulation::setWorldDimensions},
	{"setViewRange", &SimSimple::setViewRange},
	{"setViewAngle", &SimSimple::setViewAngle},
	{"setGoCost", &SimSimple::setGoCost},
	{"setRotateCost", &SimSimple::setRotateCost},
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

int SimSimple::setViewRange(lua_State* luaState)
{
        int viewRange = luaL_checkint(luaState, 1);
        setViewRange(viewRange);
        return 0;
}

int SimSimple::setViewAngle(lua_State* luaState)
{
        int viewAngle = luaL_checkint(luaState, 1);
        setViewAngle(viewAngle);
        return 0;
}

int SimSimple::setGoCost(lua_State* luaState)
{
        float cost = luaL_checknumber(luaState, 1);
	setGoCost(cost);
	return 0;
}

int SimSimple::setRotateCost(lua_State* luaState)
{
	float cost = luaL_checknumber(luaState, 1);
	setRotateCost(cost);
	return 0;
}

