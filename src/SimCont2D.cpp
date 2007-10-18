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

#include "SimCont2D.h"
#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "Random.h"
#include "SymbolFloat.h"

#include <math.h>
#include <list>

using std::list;

SimCont2D::SimCont2D(lua_State* luaState)
{
    mWorldWidth = 0.0f;
    mWorldLength = 0.0f;

    mCellSide = 0;
    mWorldCellWidth = 0;
    mWorldCellLength = 0;

    mViewX = 0.0f;
    mViewY = 0.0f;

    mCellGrid = NULL;

    mShowGrid = false;
    mShowViewRange = false;
    mShowBrain = false;
    
    mViewRange = 0.0f;
    mViewAngle = 0.0f;
    mHalfViewAngle = 0.0f;
    mLowLimitViewAngle = 0.0f;
    mHighLimitViewAngle = 0.0f;

    mGoCost = 0.0f;
    mRotateCost = 0.0f;

    mTargetObject = NULL;
    mDistanceToTargetObject = 0.0f;

    mDragging = false;
    mLastMouseX = 0;
    mLastMouseY = 0;

    mRootLayer2D = NULL;

    mCellX1 = 0;
    mCellX2 = 0;
    mCellY1 = 0;
    mCellY2 = 0;

    mCurrentCellX = 0;
    mCurrentCellY = 0;

    mCurrentCellList = NULL;

    mCollisionX = 0.0f;
    mCollisionY = 0.0f;
    mCollisionRadius = 0.0f;

    mCollisionDetectionIteration = 0;

    mHumanAgent = NULL;
    mHumanGo = false;
    mHumanRotateLeft = false;
    mHumanRotateRight = false;
    mHumanEat = false;

    mZoom = 1.0f;
}

SimCont2D::~SimCont2D()
{
    if (mCellGrid != NULL)
    {
        unsigned int gridSize = mWorldCellWidth * mWorldCellLength;

        for (unsigned int i = 0; i < gridSize; i++)
        {
            delete mCellGrid[i];
        }
    
        free(mCellGrid);
        mCellGrid = NULL;
    }
}

void SimCont2D::setWorldDimensions(float worldWidth,
                                    float worldLength,
                                    unsigned int cellSide)
{
    mWorldWidth = worldWidth;
    mWorldLength = worldLength;
    mCellSide = cellSide;
    mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / ((float)mCellSide)));
    mWorldCellLength = (unsigned int)(ceilf(mWorldLength / ((float)mCellSide)));

    unsigned int gridSize = mWorldCellWidth * mWorldCellLength;
    mCellGrid = (list<SimulationObject*>**)malloc(sizeof(list<SimulationObject*>*) * gridSize);

    for (unsigned int i = 0; i < gridSize; i++)
    {
        mCellGrid[i] = new list<SimulationObject*>();
    }
}

void SimCont2D::setPos(SimulationObject* obj, float x, float y)
{
    float origX = obj->mX;
    float origY = obj->mY;
    float size = obj->mSize;

    int origX1 = ((int)(origX - size)) / mCellSide;
    int origX2 = ((int)(origX + size)) / mCellSide;
    int origY1 = ((int)(origY - size)) / mCellSide;
    int origY2 = ((int)(origY + size)) / mCellSide;

    if (origX1 < 0)
    {
        origX1 = 0;
    }
    if (origY1 < 0)
    {
        origY1 = 0;
    }
    if (origX2 >= mWorldCellWidth)
    {
        origX2 = mWorldCellWidth - 1;
    }
    if (origY2 >= mWorldCellLength)
    {
        origY2 = mWorldCellLength - 1;
    }

    int targX1 = ((int)(x - size)) / mCellSide;
    int targX2 = ((int)(x + size)) / mCellSide;
    int targY1 = ((int)(y - size)) / mCellSide;
    int targY2 = ((int)(y + size)) / mCellSide;

    if (targX1 < 0)
    {
        targX1 = 0;
    }
    if (targY1 < 0)
    {
        targY1 = 0;
    }
    if (targX2 >= mWorldCellWidth)
    {
        targX2 = mWorldCellWidth - 1;
    }
    if (targY2 >= mWorldCellLength)
    {
        targY2 = mWorldCellLength - 1;
    }

    obj->mX = x;
    obj->mY = y;

    if (obj->mInitialized)
    {
        // Remove from cells
        for (int cellX = origX1; cellX <= origX2; cellX++)
        {
            for (int cellY = origY1; cellY <= origY2; cellY++)
            {
                if ((cellX < targX1) 
                    || (cellX > targX2)
                    || (cellY < targY1)
                    || (cellY > targY2))
                {
                    list<SimulationObject*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
                    cellList->remove(obj);
                }
            }
        }
    }
    else
    {
        origX1 = -1;
        origX2 = -1;
        origY1 = -1;
        origY2 = -1;
        obj->mInitialized = true;
    }

    // Add to cells
    for (int cellX = targX1; cellX <= targX2; cellX++)
    {
        for (int cellY = targY1; cellY <= targY2; cellY++)
        {
            if ((cellX < origX1) 
                || (cellX > origX2)
                || (cellY < origY1)
                || (cellY > origY2))
            {
                list<SimulationObject*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
                cellList->push_back(obj);
            }
        }
    }
}

void SimCont2D::setRot(SimulationObject* obj, float rot)
{
    obj->mRotZ = normalizeAngle(rot);
}

void SimCont2D::initializeData(SimulationObject* obj)
{
    obj->initFloatData(13);
    obj->initULData(4);
}

void SimCont2D::addObject(SimulationObject* object)
{
    Simulation::addObject(object);
   
    INITIALIZE_OBJECT(object)

    object->mFloatData[FLOAT_SPEED_X] = 0.0f;
    object->mFloatData[FLOAT_SPEED_Y] = 0.0f;
    object->mFloatData[FLOAT_SPEED_ROT] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_X] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_Y] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_ROT] = 0.0f;

    SymbolFloat* symSize = (SymbolFloat*)object->getSymbolByName("size");

    if (symSize != NULL)
    {
        object->mSize = symSize->getFloat();
        object->mSizeSquared = object->mSize * object->mSize;
    }

    object->mFloatData[FLOAT_ENERGY] = object->mFloatData[FLOAT_INITIAL_ENERGY];

    object->mULData[UL_COLLISION_DETECTION_ITERATION] = 0;

    if (object->mULData[UL_HIGH_AGE_LIMIT] > 0)
    {
        object->mULData[UL_MAX_AGE] =
            Random::getUniformInt(
                object->mULData[UL_LOW_AGE_LIMIT],
                object->mULData[UL_HIGH_AGE_LIMIT]);
    }
    else
    {
        object->mULData[UL_MAX_AGE] = 0;
    }
}

void SimCont2D::removeObject(SimulationObject* obj)
{
    float origX = obj->mX;
    float origY = obj->mY;
    float size = obj->mSize;
    int origX1 = ((int)(origX - size)) / mCellSide;
    int origX2 = ((int)(origX + size)) / mCellSide;
    int origY1 = ((int)(origY - size)) / mCellSide;
    int origY2 = ((int)(origY + size)) / mCellSide;

    if (origX1 < 0)
    {
        origX1 = 0;
    }
    if (origY1 < 0)
    {
        origY1 = 0;
    }
    if (origX2 >= mWorldCellWidth)
    {
        origX2 = mWorldCellWidth - 1;
    }
    if (origY2 >= mWorldCellLength)
    {
        origY2 = mWorldCellLength - 1;
    }

    // Remove from cells
    for (int cellX = origX1; cellX <= origX2; cellX++)
    {
        for (int cellY = origY1; cellY <= origY2; cellY++)
        {
            list<SimulationObject*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
            cellList->remove(obj);
        }
    }

    Simulation::removeObject(obj);
}

void SimCont2D::placeRandom(SimulationObject* obj)
{
    unsigned int worldWidth = (unsigned int)mWorldWidth;
    unsigned int worldLength = (unsigned int)mWorldLength;

    setPos(obj, rand() % worldWidth, rand() % worldLength);
    setRot(obj, Random::getUniformProbability() * M_PI * 2);
}

void SimCont2D::startCollisionDetection(float x, float y, float rad)
{
    mCollisionX = x;
    mCollisionY = y;
    mCollisionRadius = rad;

    float x1 = x - rad;
    float x2 = x + rad;
    float y1 = y - rad;
    float y2 = y + rad;

    mCellX1 = ((int)(x1)) / mCellSide;
    mCellX2 = ((int)(x2)) / mCellSide;
    mCellY1 = ((int)(y1)) / mCellSide;
    mCellY2 = ((int)(y2)) / mCellSide;

    if (mCellX1 < 0)
    {
        mCellX1 = 0;
    }
    if (mCellY1 < 0)
    {
        mCellY1 = 0;
    }
    if (mCellX2 >= mWorldCellWidth)
    {
        mCellX2 = mWorldCellWidth - 1;
    }
    if (mCellY2 >= mWorldCellLength)
    {
        mCellY2 = mWorldCellLength - 1;
    }

    mCurrentCellX = mCellX1;
    mCurrentCellY = mCellY1;

    mCurrentCellList = mCellGrid[(mWorldCellWidth * mCurrentCellY) + mCurrentCellX];
    mCurrentCellListIterator = mCurrentCellList->begin();

    mCollisionDetectionIteration++;
}

SimulationObject* SimCont2D::nextCollision(float& distance, float& angle)
{
    distance = 0.0f;
    angle = 0.0f;

    while (true)
    {
        while (mCurrentCellListIterator == mCurrentCellList->end())
        {
            if (mCurrentCellX >= mCellX2)
            {
                if (mCurrentCellY >= mCellY2)
                {
                    // No more objects found
                    return NULL;
                }
                else
                {
                    mCurrentCellX = mCellX1;
                    mCurrentCellY++;
                }
            }
            else
            {
                mCurrentCellX++;
            }

            mCurrentCellList = mCellGrid[(mWorldCellWidth * mCurrentCellY) + mCurrentCellX];
            mCurrentCellListIterator = mCurrentCellList->begin();
        }

        SimulationObject* obj = *mCurrentCellListIterator;

        if (obj->mULData[UL_COLLISION_DETECTION_ITERATION] != mCollisionDetectionIteration)
        {
            obj->mULData[UL_COLLISION_DETECTION_ITERATION] = mCollisionDetectionIteration;

            float dX =  obj->mX - mCollisionX;
            float dY =  obj->mY - mCollisionY;
            distance = sqrtf((dX * dX) + (dY * dY));

            if ((distance - obj->mSize) <= mCollisionRadius)
            {
                if (distance < 0.0f)
                {
                    distance = 0.0f;
                }
                if ((dX != 0.0f) || (dY != 0.0f))
                {
                    angle = atan2f(dY, dX);
                }
                mCurrentCellListIterator++;
                return obj;
            }
        }

        mCurrentCellListIterator++;
    }
}

void SimCont2D::process(SimulationObject* obj)
{
    obj->mFloatData[FLOAT_ENERGY] -= obj->mFloatData[FLOAT_METABOLISM];

    if (obj->mFloatData[FLOAT_ENERGY] <= 0)
    {
        killOrganism(obj);
    }

    if (obj->mULData[UL_MAX_AGE] > 0)
    {
        if (mSimulationTime - obj->mCreationTime >= obj->mULData[UL_MAX_AGE])
        {
            killOrganism(obj);
        }
    }

    obj->mFloatData[FLOAT_SPEED_X] += obj->mFloatData[FLOAT_IMPULSE_X];
    obj->mFloatData[FLOAT_SPEED_Y] += obj->mFloatData[FLOAT_IMPULSE_Y];
    obj->mFloatData[FLOAT_IMPULSE_X] = 0.0f;
    obj->mFloatData[FLOAT_IMPULSE_Y] = 0.0f;

    float speed = sqrtf((obj->mFloatData[FLOAT_SPEED_X]
                    * obj->mFloatData[FLOAT_SPEED_X])
                    + (obj->mFloatData[FLOAT_SPEED_Y]
                    * obj->mFloatData[FLOAT_SPEED_Y]));

    float friction = obj->mFloatData[FLOAT_FRICTION];
    float newSpeedRatio = 0.0f;

    if (speed > friction)
    {
        newSpeedRatio = (speed - friction) / speed;
    }

    float oneMinusDrag = 1 - obj->mFloatData[FLOAT_DRAG];
    
    obj->mFloatData[FLOAT_SPEED_X] *= newSpeedRatio * oneMinusDrag;
    obj->mFloatData[FLOAT_SPEED_Y] *= newSpeedRatio * oneMinusDrag;

    float newX = obj->mX + obj->mFloatData[FLOAT_SPEED_X];
    float newY = obj->mY + obj->mFloatData[FLOAT_SPEED_Y];

    if (newX < 0.0f)
    {
        newX = 0.0f;
        obj->mFloatData[FLOAT_SPEED_X] = 0.0f;
    }
    else if (newX > mWorldWidth)
    {
        newX = mWorldWidth;
        obj->mFloatData[FLOAT_SPEED_X] = 0.0f;
    }
    if (newY < 0.0f)
    {
        newY = 0.0f;
        obj->mFloatData[FLOAT_SPEED_Y] = 0.0f;
    }
    else if (newY > mWorldLength)
    {
        newY = mWorldLength;
        obj->mFloatData[FLOAT_SPEED_Y] = 0.0f;
    }

    obj->mFloatData[FLOAT_SPEED_ROT] += obj->mFloatData[FLOAT_IMPULSE_ROT];
    obj->mFloatData[FLOAT_IMPULSE_ROT] = 0.0f;

    float rotFriction = obj->mFloatData[FLOAT_ROT_FRICTION];
    float absRotFriction = rotFriction;
    float absRotSpeed = obj->mFloatData[FLOAT_SPEED_ROT];

    if (obj->mFloatData[FLOAT_SPEED_ROT] < 0.0f)
    {
        absRotSpeed = -absRotSpeed;
        rotFriction = -rotFriction;
    }

    if (absRotFriction > absRotSpeed)
    {
        obj->mFloatData[FLOAT_SPEED_ROT] = 0.0f;
    }
    else
    {
        obj->mFloatData[FLOAT_SPEED_ROT] -= rotFriction;
    }

    float oneMinusRotDrag = 1.0f - obj->mFloatData[FLOAT_ROT_DRAG];
    obj->mFloatData[FLOAT_SPEED_ROT] *= oneMinusRotDrag;

    float newRot = obj->mRotZ + obj->mFloatData[FLOAT_SPEED_ROT];

    setPos(obj, newX, newY);
    setRot(obj, newRot);

    // Update fitness
    obj->mFitness = obj->mFloatData[FLOAT_ENERGY];
}

void SimCont2D::perceive(Agent* agent)
{
    mTargetObject = NULL;
    mDistanceToTargetObject = 9999999999.9f;

    mLowLimitViewAngle = normalizeAngle(agent->mRotZ - mHalfViewAngle);
    mHighLimitViewAngle = normalizeAngle(agent->mRotZ + mHalfViewAngle);

    startCollisionDetection(agent->mX, agent->mY, mViewRange);

    SimulationObject* target;
    float distance;
    float angle;

    while (target = nextCollision(distance, angle))
    {
        if (target != agent)
        {
            bool visible = false;
            distance -= agent->mSize;
            distance -= target->mSize;
            if (distance < 0.0f)
            {
                distance = 0.0f;
            }

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

            if (visible)
            {
                // Is in contact?
                // TODO: use the nearest to angle 0 instead of the closest distance
                if (distance <= 0)
                {
                    if ((mTargetObject == NULL) || (distance < mDistanceToTargetObject))
                    {
                        mTargetObject = target;
                        mDistanceToTargetObject = distance;
                    }
                }

                onScanObject(agent,
                                target,
                                distance,
                                normalizeAngle(agent->mRotZ - angle));
            }
        }
    }
}

void SimCont2D::onScanObject(Agent* orig,
                SimulationObject* targ,
                float distance,
                float angle)
{
    float normalizedValue;

    list<InterfaceItem*>* interface = orig->getBrain()->getInputInterface(0);
    float* inBuffer = orig->getBrain()->getInputBuffer(0);
    unsigned int pos = 0;

    for (list<InterfaceItem*>::iterator iterItem = interface->begin();
        iterItem != interface->end();
        iterItem++)
    {
        unsigned int type = (*iterItem)->mType;

        switch (type)
        {
            case PERCEPTION_IN_CONTACT:
                if (distance <= 0.0f)
                {
                    inBuffer[pos] = 1.0f;
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case PERCEPTION_POSITION:
                normalizedValue = angle / mHalfViewAngle;
                inBuffer[pos] = normalizedValue;
                break;

            case PERCEPTION_DISTANCE:
                normalizedValue = distance / mViewRange;
                inBuffer[pos] = normalizedValue;
                break;

            case PERCEPTION_OBJECT_FEATURE:
                InterfaceItem* item = (*iterItem);
                normalizedValue = computeBinding(orig,
                                                    targ,
                                                    item->mSymTable,
                                                    item->mOrigSymIndex,
                                                    item->mTargetSymIndex);
                inBuffer[pos] = normalizedValue;
                break;
        }

        pos++;
    }
}

void SimCont2D::act(Agent* agent)
{
    bool actionGo = false;
    bool actionRotate = false;
    bool actionEat = false;
    float actionGoParam = 0.0f;
    float actionRotateParam = 0.0f;
    float actionEatParam = 0.0f;

    if (agent == mHumanAgent)
    {
        if (mHumanGo)
        {
            actionGo = true;
            actionGoParam = 1.0f;
        }
        if (mHumanRotateLeft)
        {
            actionRotate = true;
            actionRotateParam -= 1.0f;
        }
        if (mHumanRotateRight)
        {
            actionRotate = true;
            actionRotateParam += 1.0f;
        }
        if (mHumanEat)
        {
            actionEat = true;
            actionEatParam = 1.0f;
        }
    }
    else
    {
        list<InterfaceItem*>* interface = agent->getBrain()->getOutputInterface();
        float* outBuffer = agent->getBrain()->getOutputBuffer();
        unsigned int pos = 0;

        for (list<InterfaceItem*>::iterator iterItem = interface->begin();
            iterItem != interface->end();
            iterItem++)
        {
            float output = outBuffer[pos];
            unsigned int actionType = (*iterItem)->mType;

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

            pos++;
        }
    }

    if (actionGo)
    {
        goFront(agent, actionGoParam * 0.01f);
    }
    if (actionRotate)
    {
        rotate(agent, actionRotateParam * 0.0001f);
    }
    if (actionEat)
    {
        eat(agent);
    }
}

void SimCont2D::goFront(Agent* agent, float distance)
{
    /*agent->mEnergy -= mGoCost * distance;
    float newX = agent->mX + (cosf(agent->mRotZ) * distance);
    float newY = agent->mY + (sinf(agent->mRotZ) * distance);

    if ((newX < 0)
        || (newY < 0)
        || (newX >= mWorldWidth)
        || (newY >= mWorldLength))
    {
        return;
    }

    setPos(agent, newX, newY);*/

    agent->mFloatData[FLOAT_IMPULSE_X] = cosf(agent->mRotZ) * distance;
    agent->mFloatData[FLOAT_IMPULSE_Y] = sinf(agent->mRotZ) * distance;
}

void SimCont2D::rotate(Agent* agent, float angle)
{
    /*agent->mEnergy -= mRotateCost * angle;
    setRot(agent, agent->mRotZ - angle);*/

    agent->mFloatData[FLOAT_IMPULSE_ROT] += angle;
}

void SimCont2D::eat(Agent* agent)
{
    if (mTargetObject)
    {
        Symbol* sym1 = agent->getSymbolByName("feed");
        if (sym1 == NULL)
        {
            return;
        }
        Symbol* sym2 = mTargetObject->getSymbolByName("food");
        if (sym2 == NULL)
        {
            return;
        }

        if (sym1->bind(sym2) > 0.5f)
        {
            float energy = mTargetObject->mFloatData[FLOAT_ENERGY];
            deltaEnergy(agent, energy);
            deltaEnergy(mTargetObject, -energy);
        }
    }
}

void SimCont2D::drawBeforeObjects()
{
    mRootLayer2D->setScale(mZoom, mZoom);
    mRootLayer2D->setTranslation(mViewX, mViewY);

    if (mShowGrid)
    {
        unsigned int cellSide = (unsigned int)mCellSide;

        int viewX = (int)mViewX;
        int viewY = (int)mViewY;

        mRootLayer2D->setColor(200, 200, 200);

        unsigned int division = cellSide;
        while (division < mWorldWidth)
        {
            mRootLayer2D->drawLine(division,
                                    0,
                                    division,
                                    mWorldLength);

            division += cellSide;
        }

        division = cellSide;
        while (division < mWorldLength)
        {
            mRootLayer2D->drawLine(0,
                                    division,
                                    mWorldWidth,
                                    division);

            division += cellSide;
        }
    }

    if (mShowViewRange)
    {
        list<SimulationObject*>::iterator iterObj;

        for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
        {
            SimulationObject* obj = *iterObj;

            if (obj->mType == SimulationObject::TYPE_AGENT)
            {
                float beginAngle = normalizeAngle(obj->mRotZ - mHalfViewAngle);
                float endAngle = normalizeAngle(obj->mRotZ + mHalfViewAngle);

                if (beginAngle > endAngle)
                {
                    endAngle += M_PI * 2.0f;
                }

                mRootLayer2D->setColor(150, 150, 150, 100);
                mRootLayer2D->fillCircle(obj->mX,
                                            obj->mY,
                                            mViewRange,
                                            beginAngle,
                                            endAngle);
            }
        }
    }
}

void SimCont2D::drawAfterObjects()
{
    mRootLayer2D->clearScale();
    mRootLayer2D->clearTranslation();

    if (mShowBrain)
    {
        if (mHumanAgent)
        {
            mRootLayer2D->setTranslation(0, mWindow->getHeight() / 2);
            mRootLayer2D->setColor(255, 255, 255, 130);
            mRootLayer2D->fillRectangle(0, 0, mWindow->getWidth(), mWindow->getHeight() / 2);
            mRootLayer2D->setColor(50, 50, 50, 255);
            mRootLayer2D->setFont(mFont);
            mHumanAgent->getBrain()->draw(mRootLayer2D);
            mRootLayer2D->clearTranslation();
        }
    }
}

SimulationObject* SimCont2D::getObjectByScreenPos(int x, int y)
{
    return NULL;
}

float SimCont2D::getFieldValue(SimulationObject* obj, string fieldName)
{
    if (fieldName == "energy")
    {
        return obj->mFloatData[FLOAT_ENERGY];
    }
    else
    {
        return Simulation::getFieldValue(obj, fieldName);
    }
}

bool SimCont2D::onKeyDown(pyc::KeyCode key)
{
    if (Simulation::onKeyDown(key))
    {
        return true;
    }

    switch (key)
    {
    case pyc::KEY_UP:
        mHumanGo = true;
        return true;
    case pyc::KEY_RIGHT:
        mHumanRotateRight = true;
        return true;
    case pyc::KEY_LEFT:
        mHumanRotateLeft = true;
        return true;
    case pyc::KEY_E:
        mHumanEat = true;
        return true;
    default:
        return false;
    }
}

bool SimCont2D::onKeyUp(pyc::KeyCode key)
{
    if (Simulation::onKeyUp(key))
    {
        return true;
    }

    switch (key)
    {
    case pyc::KEY_G:
        mShowGrid = !mShowGrid;
        return true;
    case pyc::KEY_V:
        mShowViewRange = !mShowViewRange;
        return true;
    case pyc::KEY_B:
        mShowBrain = !mShowBrain;
        return true;
    case pyc::KEY_UP:
        mHumanGo = false;
        return true;
    case pyc::KEY_RIGHT:
        mHumanRotateRight = false;
        return true;
    case pyc::KEY_LEFT:
        mHumanRotateLeft = false;
        return true;
    case pyc::KEY_E:
        mHumanEat = false;
        return true;
    default:
        return false;
    }
}

bool SimCont2D::onMouseButtonDown(pyc::MouseButton button, int x, int y)
{
    mDragging = true;
    mLastMouseX = x;
    mLastMouseY = y;
    return false;
}

bool SimCont2D::onMouseButtonUp(pyc::MouseButton button, int x, int y)
{
    mDragging = false;
    return false;
}

bool SimCont2D::onMouseMove(int x, int y)
{
    if (mDragging)
    {
        float deltaX = (float)(x - mLastMouseX);
        float deltaY = (float)(y - mLastMouseY);
        mLastMouseX = x;
        mLastMouseY = y;

        mViewX += deltaX / mZoom;
        mViewY += deltaY / mZoom;

        return true;
    }

    return false;
}

bool SimCont2D::onMouseWheel(bool up)
{
    float wWidth = (float)mWindow->getWidth();
    float wHeight = (float)mWindow->getHeight();

    float centerX = -mViewX + (wWidth / (mZoom * 2.0f));
    float centerY = -mViewY + (wHeight / (mZoom * 2.0f));

    if (up)
    {
        mZoom *= 1.1f;
    }
    else
    {
        mZoom *= 0.9f;
    }
    
    mViewX = -centerX + (wWidth / (mZoom * 2.0f));
    mViewY = -centerY + (wHeight / (mZoom * 2.0f));

    return true;
}

void SimCont2D::setViewAngle(float angle)
{
    mViewAngle = (angle * M_PI) / 180.0f;
    mHalfViewAngle = mViewAngle / 2.0f;
}

void SimCont2D::setViewRange(float range)
{
    mViewRange = range;
}

float SimCont2D::normalizeAngle(float angle)
{
    float PI2 = M_PI * 2.0f;
    while(angle > M_PI)
    {
        angle -= PI2;
    }
    while(angle <= -M_PI)
    {
        angle += PI2;
    }

    return angle;
}

void SimCont2D::setSize(SimulationObject* obj, float size)
{
    obj->mSize = size;
    obj->mSizeSquared = size * size;
}

void SimCont2D::deltaEnergy(SimulationObject* obj, double delta)
{
    obj->mFloatData[FLOAT_ENERGY] += delta;
}

void SimCont2D::setEnergy(SimulationObject* obj, float energy)
{
    obj->mFloatData[FLOAT_ENERGY] = energy;
}

void SimCont2D::setInitialEnergy(SimulationObject* obj, float energy)
{

    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_INITIAL_ENERGY] = energy;
}

void SimCont2D::setAgeRange(SimulationObject* obj, unsigned long lowAgeLimit, unsigned long highAgeLimit)
{
    INITIALIZE_OBJECT(obj)
    obj->mULData[UL_LOW_AGE_LIMIT] = lowAgeLimit;
    obj->mULData[UL_HIGH_AGE_LIMIT] = highAgeLimit;
}

void SimCont2D::setMetabolism(SimulationObject* obj, float metabolism)
{
    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_METABOLISM] = metabolism;
}

void SimCont2D::setFriction(SimulationObject* obj, float friction)
{
    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_FRICTION] = friction;
}

void SimCont2D::setDrag(SimulationObject* obj, float drag)
{
    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_DRAG] = drag;
}

void SimCont2D::setRotFriction(SimulationObject* obj, float friction)
{
    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_ROT_FRICTION] = friction;
}

void SimCont2D::setRotDrag(SimulationObject* obj, float drag)
{
    INITIALIZE_OBJECT(obj)
    obj->mFloatData[FLOAT_ROT_DRAG] = drag;
}

const char SimCont2D::mClassName[] = "SimCont2D";

Orbit<SimCont2D>::MethodType SimCont2D::mMethods[] = {
    {"addObject", &Simulation::addObject},
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"initGraphics", &Simulation::initGraphics},
    {"run", &Simulation::run},
    {"setWorldDimensions", &SimCont2D::setWorldDimensions},
    {"setViewRange", &SimCont2D::setViewRange},
    {"setViewAngle", &SimCont2D::setViewAngle},
    {"setGoCost", &SimCont2D::setGoCost},
    {"setRotateCost", &SimCont2D::setRotateCost},
    {"setPos", &SimCont2D::setPos},
    {"setRot", &SimCont2D::setRot},
    {"setHuman", &SimCont2D::setHuman},
    {"setSize", &SimCont2D::setSize},
    {"setAgeRange", &SimCont2D::setAgeRange},
	{"setMetabolism", &SimCont2D::setMetabolism},
	{"setInitialEnergy", &SimCont2D::setInitialEnergy},
	{"setFriction", &SimCont2D::setFriction},
	{"setDrag", &SimCont2D::setDrag},
	{"setRotFriction", &SimCont2D::setRotFriction},
	{"setRotDrag", &SimCont2D::setRotDrag},
    {0,0}
};

Orbit<SimCont2D>::NumberGlobalType SimCont2D::mNumberGlobals[] = {
    {"PERCEPTION_NULL", PERCEPTION_NULL},
    {"PERCEPTION_POSITION", PERCEPTION_POSITION},
    {"PERCEPTION_DISTANCE", PERCEPTION_DISTANCE},
    {"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
    {"PERCEPTION_OBJECT_FEATURE", PERCEPTION_OBJECT_FEATURE},
    {"ACTION_NULL", ACTION_NULL},
    {"ACTION_GO", ACTION_GO},
    {"ACTION_ROTATE", ACTION_ROTATE},
    {"ACTION_EAT", ACTION_EAT},
    {0,0}
};

int SimCont2D::setWorldDimensions(lua_State* luaState)
{
    float width = luaL_checknumber(luaState, 1);
    float height = luaL_checknumber(luaState, 2);
    int cellSide = luaL_checkint(luaState, 3);
    setWorldDimensions(width, height, cellSide);
    return 0;
}

int SimCont2D::setViewRange(lua_State* luaState)
{
    float viewRange = luaL_checknumber(luaState, 1);
    setViewRange(viewRange);
    return 0;
}

int SimCont2D::setViewAngle(lua_State* luaState)
{
    float viewAngle = luaL_checknumber(luaState, 1);
    setViewAngle(viewAngle);
    return 0;
}

int SimCont2D::setGoCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setGoCost(cost);
    return 0;
}

int SimCont2D::setRotateCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setRotateCost(cost);
    return 0;
}

int SimCont2D::setPos(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float x = luaL_checknumber(luaState, 2);
    float y = luaL_checknumber(luaState, 3);
    setPos(simObj, x, y);
    return 0;
}

int SimCont2D::setRot(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float rot = luaL_checknumber(luaState, 2);
    setRot(simObj, rot);
    return 0;
}

int SimCont2D::setHuman(lua_State* luaState)
{
    Agent* agent = (Agent*)Orbit<SimCont2D>::pointer(luaState, 1);
    setHuman(agent);
    return 0;
}

int SimCont2D::setSize(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float size = luaL_checknumber(luaState, 2);
    setSize(simObj, size);
    return 0;
}

int SimCont2D::setInitialEnergy(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float energy = luaL_checknumber(luaState, 2);
    setInitialEnergy(simObj, energy);
    return 0;
}

int SimCont2D::setAgeRange(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    int lowAgeLimit = luaL_checkint(luaState, 2);
    int highAgeLimit = luaL_checkint(luaState, 3);
    setAgeRange(simObj, lowAgeLimit, highAgeLimit);
    return 0;
}

int SimCont2D::setMetabolism(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float metabolism = luaL_checknumber(luaState, 2);
    setMetabolism(simObj, metabolism);
    return 0;
}

int SimCont2D::setFriction(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float friction = luaL_checknumber(luaState, 2);
    setFriction(simObj, friction);
    return 0;
}

int SimCont2D::setDrag(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float drag = luaL_checknumber(luaState, 2);
    setDrag(simObj, drag);
    return 0;
}

int SimCont2D::setRotFriction(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float friction = luaL_checknumber(luaState, 2);
    setRotFriction(simObj, friction);
    return 0;
}

int SimCont2D::setRotDrag(lua_State* luaState)
{
    SimulationObject* simObj = (SimulationObject*)Orbit<SimCont2D>::pointer(luaState, 1);
    float rotDrag = luaL_checknumber(luaState, 2);
    setRotDrag(simObj, rotDrag);
    return 0;
}

