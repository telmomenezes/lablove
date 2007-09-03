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

#include "AnimatSimple2D.h"
#include "Lab.h"
#include "SimSimple.h"
#include "math.h"
#include "functions.h"
#include "defines.h"
#include "random.h"

AnimatSimple2D::AnimatSimple2D(lua_State* luaState)
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

AnimatSimple2D::AnimatSimple2D(AnimatSimple2D* anim, bool full) : ObjectSimple2D(anim)
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

AnimatSimple2D::~AnimatSimple2D()
{	
	if (mGridbrain != NULL)
	{
		delete mGridbrain;
		mGridbrain = NULL;
	}
}

void AnimatSimple2D::setAlphaObjectsGrid(Grid* grid)
{
	grid->setType(Grid::ALPHA);
	mGridbrain->addGrid(grid);
}

void AnimatSimple2D::setBetaGrid(Grid* grid)
{
	grid->setType(Grid::BETA);
	mGridbrain->addGrid(grid);
}

void AnimatSimple2D::initRandom()
{
	mGridbrain->init();

	for (unsigned int i = 0; i < mInitialConnections; i++)
	{
		mGridbrain->addRandomConnection();
	}
	//initTest();
}

void AnimatSimple2D::initTest()
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

SimulationObject* AnimatSimple2D::clone(bool full)
{
	return new AnimatSimple2D(this, full);
}

void AnimatSimple2D::draw()
{
	if (((SimSimple*)(Lab::getSingleton().getSimulation()))->getShowViewRange())
	{
		float beginAngle = mLowLimitViewAngle;
		float endAngle = mHighLimitViewAngle;

		if (beginAngle > endAngle)
		{
			endAngle += M_PI * 2.0f;
		}

		Lab::getSingleton().getRootLayer()->setColor(150, 150, 150, 100);
		Lab::getSingleton().getRootLayer()->fillCircle(mX, mY, mViewRange, beginAngle, endAngle);
	}

	float a1 = mRot;
	float a2 = mRot + (M_PI * 0.8f);
	float a3 = mRot + (M_PI * 1.2f);
	float x1 = mX + (cosf(a1) * mSize);
	float y1 = mY + (sinf(a1) * mSize);
	float x2 = mX + (cosf(a2) * mSize);
	float y2 = mY + (sinf(a2) * mSize);
	float x3 = mX + (cosf(a3) * mSize);
	float y3 = mY + (sinf(a3) * mSize);

	Lab::getSingleton().getRootLayer()->setColor(mColor.mRed, mColor.mGreen, mColor.mBlue);
	Lab::getSingleton().getRootLayer()->setLineWidth(2.0f);
	Lab::getSingleton().getRootLayer()->fillTriangle(x1, y1, x2, y2, x3, y3);
	Lab::getSingleton().getRootLayer()->drawTriangle(x1, y1, x2, y2, x3, y3);
}

void AnimatSimple2D::setRot(float rot)
{
	ObjectSimple2D::setRot(rot);
	mLowLimitViewAngle = normalizeAngle(mRot - mHalfViewAngle);
	mHighLimitViewAngle = normalizeAngle(mRot + mHalfViewAngle);
}

void AnimatSimple2D::onCycle()
{
	ObjectSimple2D::onCycle();
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

void AnimatSimple2D::computationStep()
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

void AnimatSimple2D::perceptionStep()
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

void AnimatSimple2D::scanCell(int cellX, int cellY)
{
	ObjectSimple2D* obj = (ObjectSimple2D*)
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

		obj = ((ObjectSimple2D*)obj)->mNextCellList;
	}
}

void AnimatSimple2D::setViewAngle(float angle)
{
	mViewAngle = (angle * M_PI) / 180.0f;
	mHalfViewAngle = mViewAngle / 2.0f;
}

void AnimatSimple2D::goFront(float distance)
{
	// TODO: check if move is possible

	mEnergy -= mGoCost * distance;
	float newX = mX + (cosf(mRot) * distance);
	float newY = mY + (sinf(mRot) * distance);
	setPos(newX, newY);
}

void AnimatSimple2D::rotate(float angle)
{
	mEnergy -= mRotateCost * angle;
	setRot(getRot() - angle);
}

void AnimatSimple2D::setViewRange(float range)
{
	mViewRange = range;
}

void AnimatSimple2D::eat()
{
	if (mNearestFood)
	{
		deltaEnergy(mNearestFood->getEnergy());
		Lab::getSingleton().getSimulation()->killOrganism(mNearestFood);
	}
}

void AnimatSimple2D::actionStep()
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

void AnimatSimple2D::onScanObject(SimulationObject* obj, bool visible, bool contact, float angle, float distance)
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

/*AnimatSimple2D* AnimatSimple2D::crossover(AnimatSimple2D* otherParent)
{
	// TODO: check for incompatibilities, like gridbrain dimensions?

	Animat* child = new Animat();
	child->mMaxInputDepth = mMaxInputDepth;
	child->mGridbrain = mGridbrain->crossover(((Animat*)otherParent)->mGridbrain);

	return child;
}
*/

void AnimatSimple2D::mutate()
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

float AnimatSimple2D::getFieldValue(std::string fieldName)
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
		return ObjectSimple2D::getFieldValue(fieldName);
	}
}

const char AnimatSimple2D::mClassName[] = "AnimatSimple2D";

Orbit<AnimatSimple2D>::MethodType AnimatSimple2D::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setPos", &ObjectSimple2D::setPos},
	{"setRot", &ObjectSimple2D::setRot},
	{"setSize", &ObjectSimple2D::setSize},
	{"setColor", &ObjectSimple2D::setColor},
        {"setViewRange", &AnimatSimple2D::setViewRange},
	{"setViewAngle", &AnimatSimple2D::setViewAngle},
	{"setAlphaObjectsGrid", &AnimatSimple2D::setAlphaObjectsGrid},
	{"setBetaGrid", &AnimatSimple2D::setBetaGrid},
	{"setAgeRange", &ObjectSimple2D::setAgeRange},
	{"setMetabolism", &ObjectSimple2D::setMetabolism},
	{"setGoCost", &AnimatSimple2D::setGoCost},
	{"setRotateCost", &AnimatSimple2D::setRotateCost},
        {0,0}
};

Orbit<AnimatSimple2D>::NumberGlobalType AnimatSimple2D::mNumberGlobals[] = {{0,0}};

int AnimatSimple2D::setViewRange(lua_State* luaState)
{
        int viewRange = luaL_checkint(luaState, 1);
        setViewRange(viewRange);
        return 0;
}

int AnimatSimple2D::setViewAngle(lua_State* luaState)
{
        int viewAngle = luaL_checkint(luaState, 1);
        setViewAngle(viewAngle);
        return 0;
}

int AnimatSimple2D::setAlphaObjectsGrid(lua_State* luaState)
{
	Grid* grid = (Grid*)Orbit<AnimatSimple2D>::pointer(luaState, 1);
	setAlphaObjectsGrid(grid);
	return 0;
}

int AnimatSimple2D::setBetaGrid(lua_State* luaState)
{
	Grid* grid = (Grid*)Orbit<AnimatSimple2D>::pointer(luaState, 1);
	setBetaGrid(grid);
	return 0;
}

int AnimatSimple2D::setGoCost(lua_State* luaState)
{
        float cost = luaL_checknumber(luaState, 1);
	setGoCost(cost);
	return 0;
}

int AnimatSimple2D::setRotateCost(lua_State* luaState)
{
	float cost = luaL_checknumber(luaState, 1);
	setRotateCost(cost);
	return 0;
}

/*void AnimatSimple2D::draw_brain()
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

