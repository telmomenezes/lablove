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

#include "AgentGB.h"
#include "Lab.h"
#include "SimSimple.h"
#include "math.h"
#include "functions.h"
#include "defines.h"
#include "random.h"

AgentGB::AgentGB(lua_State* luaState) : SimulationObject()
{
	mViewRange = 0.0f;
	mViewAngle = 0.0f;
	mHalfViewAngle = 0.0f;
	mLowLimitViewAngle = 0.0f;
	mHighLimitViewAngle = 0.0f;
	mNearestFood = NULL;
	mDistanceToNearestFood = 0.0f;

	mActionGo = false;
	mActionRotate = false;
	mActionEat = false;
	
	mCurrentInputDepth = 0;

	mGoCost = 0.0f;
	mRotateCost = 0.0f;

	if (luaState != NULL)
	{
		mInitialConnections = luaL_checkint(luaState, 1);
		mMaxInputDepth = luaL_checkint(luaState, 2);
		mGridbrain = new Gridbrain();
		mGridbrain->setMaxInputDepth(mMaxInputDepth);
	}
	else
	{
		mInitialConnections = 0;
		mMaxInputDepth = 0;
		mGridbrain = NULL;
	}
}

AgentGB::AgentGB(AgentGB* anim, bool full) : SimulationObject(anim)
{
	mMaxInputDepth = anim->mMaxInputDepth;
	mInitialConnections = anim->mInitialConnections;
	mGridbrain = anim->mGridbrain->clone(full);
	mCurrentInputDepth = 0;

	mViewRange = anim->mViewRange;
	mViewAngle = anim->mViewAngle;
	mHalfViewAngle = anim->mHalfViewAngle;
	mLowLimitViewAngle = anim->mLowLimitViewAngle;
	mHighLimitViewAngle = anim->mHighLimitViewAngle;
	mNearestFood = NULL;
	mDistanceToNearestFood = 0;

	mActionGo = false;
	mActionRotate = false;
	mActionEat = false;

	mGoCost = anim->mGoCost;
	mRotateCost = anim->mRotateCost;
}

AgentGB::~AgentGB()
{	
	if (mGridbrain != NULL)
	{
		delete mGridbrain;
		mGridbrain = NULL;
	}
}

void AgentGB::setAlphaObjectsGrid(Grid* grid)
{
	grid->setType(Grid::ALPHA);
	mGridbrain->addGrid(grid);
}

void AgentGB::setBetaGrid(Grid* grid)
{
	grid->setType(Grid::BETA);
	mGridbrain->addGrid(grid);
}

void AgentGB::initRandom()
{
	mGridbrain->init();

	for (unsigned int i = 0; i < mInitialConnections; i++)
	{
		mGridbrain->addRandomConnection();
	}
	//initTest();
}

void AgentGB::initTest()
{
	mGridbrain->setComponent(0, 0, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_POSITION);
	mGridbrain->setComponent(0, 1, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_PROXIMITY);
	//mGridbrain->setComponent(0, 2, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_COLOR);
	mGridbrain->setComponent(1, 0, 0, GridbrainComponent::THR, 0);
	mGridbrain->setComponent(1, 1, 0, GridbrainComponent::MAX, 0);
	mGridbrain->setComponent(2, 0, 0, GridbrainComponent::MUL, 0);
	mGridbrain->setComponent(2, 1, 0, GridbrainComponent::NOT, 0);
	mGridbrain->setComponent(3, 1, 0, GridbrainComponent::MUL, 0);

	mGridbrain->setComponent(0, 0, 1, GridbrainComponent::NOT, 0);
	mGridbrain->setComponent(0, 2, 1, GridbrainComponent::NOT, 0);
	mGridbrain->setComponent(1, 0, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_ROTATE);
	mGridbrain->setComponent(1, 1, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_GO);
	mGridbrain->setComponent(1, 2, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_EAT);

	mGridbrain->addConnection(0, 0, 0, 1, 0, 0, 1.0f);
	mGridbrain->addConnection(0, 1, 0, 1, 1, 0, 0.1f);
	mGridbrain->addConnection(0, 2, 0, 1, 1, 0, 0.5f);
	mGridbrain->addConnection(0, 2, 0, 2, 0, 0, 1.0f);
	mGridbrain->addConnection(0, 2, 0, 3, 1, 0, 1.0f);
	mGridbrain->addConnection(0, 2, 0, 0, 0, 1, 1.0f);
	mGridbrain->addConnection(1, 0, 0, 2, 0, 0, 1.0f);
	mGridbrain->addConnection(1, 0, 0, 2, 1, 0, 1.0f);
	mGridbrain->addConnection(1, 1, 0, 2, 0, 0, 1.0f);
	mGridbrain->addConnection(1, 1, 0, 3, 1, 0, 1.0f);
	mGridbrain->addConnection(2, 0, 0, 1, 0, 1, -1.0f);
	mGridbrain->addConnection(2, 1, 0, 3, 1, 0, 1.0f);
	mGridbrain->addConnection(3, 1, 0, 1, 1, 1, 1.0f);

	mGridbrain->addConnection(0, 0, 1, 1, 0, 1, 1.0f);
	mGridbrain->addConnection(0, 2, 1, 1, 2, 1, 1.0f);

	mGridbrain->initGridsInputOutput();
}

SimulationObject* AgentGB::clone(bool full)
{
	return new AgentGB(this, full);
}

void AgentGB::setRot(float rot)
{
	SimulationObject::setRot(rot);
	mLowLimitViewAngle = normalizeAngle(mRot - mHalfViewAngle);
	mHighLimitViewAngle = normalizeAngle(mRot + mHalfViewAngle);
}

void AgentGB::onCycle()
{
	SimulationObject::onCycle();
	perceptionStep();
	if (!mHuman)
	{
		computationStep();
	}
	actionStep();

	mNearestFood = NULL;
	if (!mHuman)
	{
		mActionGo = false;
		mActionRotate = false;
		mActionEat = false;
	}
}

void AgentGB::computationStep()
{
	Grid* grid = mGridbrain->getGrid(0);
	grid->setInputDepth(mCurrentInputDepth);
	mCurrentInputDepth = 0;

	// TODO: update states

	// Eval gridbrain
	mGridbrain->eval();

	// Apply actions
	mActionGo = false;
	mActionRotate = false;
	mActionEat = false;

	mActionGoParam = 0.0f;
	mActionRotateParam = 0.0f;
	mActionEatParam = 0.0f;

	grid = mGridbrain->getGrid(1);
	unsigned int actionsCount = grid->getActionsCount();

	for (unsigned int number = 0; number < actionsCount; number++)
	{
		float output = grid->getOutput(number);
		unsigned int actionType = grid->getAction(number)->mSubType;

		if (output != 0.0f)
		{
			switch (actionType)
			{
				case SimSimple::ACTION_GO:
					mActionGo = true;
					mActionGoParam += output;
					break;
				case SimSimple::ACTION_ROTATE:
					mActionRotate = true;
					mActionRotateParam += output;
					break;
				case SimSimple::ACTION_EAT:
					mActionEat = true;
					mActionEatParam += output;
					break;
			}
		}
	}
}

void AgentGB::perceptionStep()
{
	// Determine cells to analyse
	unsigned int cellSide = (unsigned int)(((SimSimple*)(Lab::getSingleton().getSimulation()))->getCellSide());
	int x = (int)mX;
	int y = (int)mY;
	int cellPosX = x % cellSide;
	int cellPosY = y % cellSide;
	int range = (int)mViewRange;
	int leftLimit = cellPosX - range; 
	int rightLimit = cellPosX + range;
	int topLimit = cellPosY - range;
	int bottomLimit = cellPosY + range;

	unsigned int cellX = ((unsigned int)mX) / cellSide;
	unsigned int cellY = ((unsigned int)mY) / cellSide;

	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	if ((cellX > 0) && (leftLimit < 0))
	{
		left = true;
	}
	if ((cellX < (((SimSimple*)(Lab::getSingleton().getSimulation()))->getWorldCellWidth() - 1))
		&& (rightLimit >= cellSide))
	{
		right = true;
	}
	if ((cellY > 0) && (topLimit < 0))
	{
		top = true;
	}
	if ((cellY < (((SimSimple*)(Lab::getSingleton().getSimulation()))->getWorldCellLength() - 1))
		&& (bottomLimit >= cellSide))
	{
		bottom = true;
	}

	// Check all objects in each cell
	scanCell(cellX, cellY);
	if (left)
	{
		scanCell(cellX - 1, cellY);
	}
	if (left && top)
	{
		scanCell(cellX - 1, cellY - 1);
	}
	if (left && bottom)
	{
		scanCell(cellX - 1, cellY + 1);
	}
	if (right)
	{
		scanCell(cellX + 1, cellY);
	}
	if (right && top)
	{
		scanCell(cellX + 1, cellY - 1);
	}
	if (right && bottom)
	{
		scanCell(cellX + 1, cellY + 1);
	}
	if (top)
	{
		scanCell(cellX, cellY - 1);
	}
	if (bottom)
	{
		scanCell(cellX, cellY + 1);
	}
}

void AgentGB::scanCell(int cellX, int cellY)
{
	SimulationObject* obj = (SimulationObject*)
		((SimSimple*)(Lab::getSingleton().getSimulation()))->getCellGrid()[
		(cellY * ((SimSimple*)(Lab::getSingleton().getSimulation()))->getWorldCellWidth()) + cellX];

	while (obj)
	{
		if (obj != this)
		{
			bool visible = false;
			bool contact = false;

			float dX = mX - obj->mX;
			float dY = mY - obj->mY;
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

				if (distance <= mSize + obj->getSize())
				{
					contact = true;
				}
			}

			if (contact && obj->isFood())
			{
				if ((mNearestFood == NULL) || (distance < mDistanceToNearestFood))
				{
					mNearestFood = obj;
					mDistanceToNearestFood = distance;
				}
			}

			if (visible)
			{
				onScanObject(obj,
						visible,
						contact,
						normalizeAngle(mRot - angle),
						distance);
			}
		}

		obj = ((SimulationObject*)obj)->mNextCellList;
	}
}

void AgentGB::setViewAngle(float angle)
{
	mViewAngle = (angle * M_PI) / 180.0f;
	mHalfViewAngle = mViewAngle / 2.0f;
}

void AgentGB::goFront(float distance)
{
	// TODO: check if move is possible

	mEnergy -= mGoCost * distance;
	float newX = mX + (cosf(mRot) * distance);
	float newY = mY + (sinf(mRot) * distance);
	setPos(newX, newY);
}

void AgentGB::rotate(float angle)
{
	mEnergy -= mRotateCost * angle;
	setRot(getRot() - angle);
}

void AgentGB::setViewRange(float range)
{
	mViewRange = range;
}

void AgentGB::eat()
{
	if (mNearestFood)
	{
		deltaEnergy(mNearestFood->getEnergy());
		Lab::getSingleton().getSimulation()->killOrganism(mNearestFood);
	}
}

void AgentGB::actionStep()
{
	if (mActionGo)
	{
		goFront(1.0f);
	}
	if (mActionRotate)
	{
		if (mActionRotateParam > 0.0f)
		{
			rotate(-0.01f);
		}
		else if (mActionRotateParam < 0.0f)
		{
			rotate(0.01f);
		}
	}
	if (mActionEat)
	{
		eat();
	}
}

void AgentGB::onScanObject(SimulationObject* obj, bool visible, bool contact, float angle, float distance)
{
	if (mHuman)
	{
		return;
	}

	float normalizedValue;

	if (mCurrentInputDepth >= mMaxInputDepth)
	{
		return;
	}

	Grid* grid = mGridbrain->getGrid(0);

	unsigned int perceptionsCount = grid->getPerceptionsCount();
	for (unsigned int perceptionNumber = 0; perceptionNumber < perceptionsCount; perceptionNumber++)
	{
		GridbrainComponent* perception = grid->getPerception(perceptionNumber);

		switch (perception->mSubType)
		{
			case SimSimple::PERCEPTION_IN_CONTACT:
				if (contact)
				{
					grid->setInput(perceptionNumber, mCurrentInputDepth, 1.0f);
				}
				else
				{
					grid->setInput(perceptionNumber, mCurrentInputDepth, 0.0f);
				}
				break;

			case SimSimple::PERCEPTION_POSITION:
				normalizedValue = 0.0f;
				if (visible)
				{
					normalizedValue = angle / mHalfViewAngle;
				}
				grid->setInput(perceptionNumber, mCurrentInputDepth, normalizedValue);
				break;

			case SimSimple::PERCEPTION_PROXIMITY:
				normalizedValue = 0.0f;
				if (visible)
				{
					normalizedValue = 1.0f - (distance / mViewRange);
				}
				grid->setInput(perceptionNumber, mCurrentInputDepth, normalizedValue);
				break;
			default:
				normalizedValue = 0.0f;
				if (visible)
				{
					normalizedValue = perception->computeBinding(this, obj);
				}
				grid->setInput(perceptionNumber, mCurrentInputDepth, normalizedValue);
				break;
		}
	}

	mCurrentInputDepth++;
}

/*AgentGB* AgentGB::crossover(AgentGB* otherParent)
{
	// TODO: check for incompatibilities, like gridbrain dimensions?

	Animat* child = new Animat();
	child->mMaxInputDepth = mMaxInputDepth;
	child->mGridbrain = mGridbrain->crossover(((Animat*)otherParent)->mGridbrain);

	return child;
}
*/

void AgentGB::mutate()
{
	unsigned type = rand() % 4;

	switch (type)
	{
	case 0:
		mGridbrain->mutateAddConnection();
		break;
	case 1:
		mGridbrain->mutateRemoveConnection();
		break;
	case 2:
		mGridbrain->mutateChangeConnectionWeight();
		break;
	case 3:
		mGridbrain->mutateChangeComponent();
		break;
	deafult:
		// ASSERT ERROR?
		break;
	}
}

float AgentGB::getFieldValue(std::string fieldName)
{
	if (fieldName == "connections")
	{
		if (mGridbrain)
		{
			return ((float)(mGridbrain->getConnectionsCount()));
		}
		else
		{
			return 0.0f;
		}
	}
	else
	{
		return SimulationObject::getFieldValue(fieldName);
	}
}

const char AgentGB::mClassName[] = "AgentGB";

Orbit<AgentGB>::MethodType AgentGB::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setPos", &SimulationObject::setPos},
	{"setRot", &SimulationObject::setRot},
	{"setSize", &SimulationObject::setSize},
	{"setColor", &SimulationObject::setColor},
	{"setGraphic", &SimulationObject::setGraphic},
        {"setViewRange", &AgentGB::setViewRange},
	{"setViewAngle", &AgentGB::setViewAngle},
	{"setAlphaObjectsGrid", &AgentGB::setAlphaObjectsGrid},
	{"setBetaGrid", &AgentGB::setBetaGrid},
	{"setAgeRange", &SimulationObject::setAgeRange},
	{"setMetabolism", &SimulationObject::setMetabolism},
	{"setGoCost", &AgentGB::setGoCost},
	{"setRotateCost", &AgentGB::setRotateCost},
        {0,0}
};

Orbit<AgentGB>::NumberGlobalType AgentGB::mNumberGlobals[] = {{0,0}};

int AgentGB::setViewRange(lua_State* luaState)
{
        int viewRange = luaL_checkint(luaState, 1);
        setViewRange(viewRange);
        return 0;
}

int AgentGB::setViewAngle(lua_State* luaState)
{
        int viewAngle = luaL_checkint(luaState, 1);
        setViewAngle(viewAngle);
        return 0;
}

int AgentGB::setAlphaObjectsGrid(lua_State* luaState)
{
	Grid* grid = (Grid*)Orbit<AgentGB>::pointer(luaState, 1);
	setAlphaObjectsGrid(grid);
	return 0;
}

int AgentGB::setBetaGrid(lua_State* luaState)
{
	Grid* grid = (Grid*)Orbit<AgentGB>::pointer(luaState, 1);
	setBetaGrid(grid);
	return 0;
}

int AgentGB::setGoCost(lua_State* luaState)
{
        float cost = luaL_checknumber(luaState, 1);
	setGoCost(cost);
	return 0;
}

int AgentGB::setRotateCost(lua_State* luaState)
{
	float cost = luaL_checknumber(luaState, 1);
	setRotateCost(cost);
	return 0;
}

/*void AgentGB::draw_brain()
{
	unsigned int height = _gridbrain->get_height();
	unsigned int alpha = _gridbrain->get_alpha();
	unsigned int beta = _gridbrain->get_beta();
	unsigned int gamma = _gridbrain->get_gamma();
	unsigned int width = alpha + beta + gamma;
	unsigned int screen_width = Love::get_instance().get_screen_width();
	unsigned int screen_height = Love::get_instance().get_screen_height();
	unsigned int component_width = screen_width / ((width * 2) + 1);
	unsigned int component_height = screen_height / ((height * 2) + 1);

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			Layer* root = Love::getInstance().getRootLayer();
			int start_x = (x * component_width * 2) + component_width;
			int start_y = (y * component_height * 2) + component_height;
			int end_x = start_x + component_width;
			int end_y = start_y + component_height;

			GridbrainComponent* comp = _gridbrain->get_component(x, y);

			char* label = "?";

			unsigned int perception;
			unsigned int action;
			unsigned int color = COLOR_GBCOMPONENT_DEFAULT;

			switch (comp->_type)
			{
				case NUL:
					label = "NULL";
					break;
				case PER:
					color = COLOR_GBCOMPONENT_PER;
					perception = (unsigned int)comp->_parameter;
					switch (perception)
					{
						case PERCEPTION_NULL:
							label = "No Perception";
							break;
						case PERCEPTION_VISIBLE:
							label = "Visible";
							break;
						case PERCEPTION_POSITION:
							label = "Position";
							break;
						case PERCEPTION_PROXIMITY:
							label = "Proximity";
							break;
						case PERCEPTION_IN_CONTACT:
							label = "Contact";
							break;
						case PERCEPTION_IS_FOOD:
							label = "Food";
							break;
						default:
							break;
					};
					break;
				case STA:
					color = COLOR_GBCOMPONENT_STA;
					label = "STA";
					break;
				case ACT:
					color = COLOR_GBCOMPONENT_ACT;
					action = (unsigned int)comp->_parameter;
					switch (action)
					{
						case ACTION_NULL:
							label = "No Action";
							break;
						case ACTION_GO:
							label = "Go";
							break;
						case ACTION_ROTATE:
							label = "Rotate";
							break;
						case ACTION_EAT:
							label = "Eat";
							break;
						default:
							break;
					}
					break;
				case THR:
					color = COLOR_GBCOMPONENT_THR;
					label = "THR";
					break;
				case AGG:
					color = COLOR_GBCOMPONENT_AGG;
					label = "AGG";
					break;
				case MAX:
					color = COLOR_GBCOMPONENT_MAX;
					label = "MAX";
					break;
				case MUL:
					color = COLOR_GBCOMPONENT_MUL;
					label = "MUL";
					break;
				case NOT:
					color = COLOR_GBCOMPONENT_NOT;
					label = "NOT";
					break;
				default:
					break;
			}

			boxColor(screen,
				start_x,
				start_y,
				end_x,
				end_y,
				color);

			int label_length = strlen(label);
			int label_x = start_x + ((component_width - (label_length * 8)) / 2);
			int label_y = start_y + ((component_height - 8) / 2);
			stringColor(screen, label_x, label_y, label, 0x000000FF);

			char string_buffer[255];
			sprintf(string_buffer, "%f", comp->_input);
			stringColor(screen, start_x, start_y, string_buffer, 0x00FF00FF);
			sprintf(string_buffer, "%f", comp->_output);
			stringColor(screen, start_x, end_y, string_buffer, 0xFF0000FF);

			start_x += component_width;
			GridbrainConnection* conn = comp->_first_connection;
			for (unsigned int i = 0; i < comp->_connections_count; i++)
			{
				start_y += 3;
				int target_x = conn->_x_targ;
				int target_y = conn->_target - (conn->_x_targ * _gridbrain->_height);
				end_x = (target_x * component_width * 2) + component_width;
				end_y = (target_y * component_height * 2) + component_height;
				end_y += component_height / 2;
				lineColor(screen, start_x, start_y, end_x, end_y, color);

				conn = (GridbrainConnection*)conn->_next_connection;
			}
		}
	}
}*/

