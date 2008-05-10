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
#include "SymbolFloat.h"
#include "SymbolRGB.h"

#include <math.h>
#include <list>

using std::list;

mt_distribution* SimCont2D::mDistAge = gDistManager.getNewDistribution();
mt_distribution* SimCont2D::mDistPosition = gDistManager.getNewDistribution();
mt_distribution* SimCont2D::mDistFitnessRandom = gDistManager.getNewDistribution();

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
    mShowEnergy = false;
    
    mViewRange = 0.0f;
    mViewAngle = 0.0f;
    mHalfViewAngle = 0.0f;
    mLowLimitViewAngle = 0.0f;
    mHighLimitViewAngle = 0.0f;

    mGoCost = 0.0f;
    mRotateCost = 0.0f;

    mGoForceScale = 0.0f;
    mRotateForceScale = 0.0f;

    mTargetObject = NULL;
    mDistanceToTargetObject = 0.0f;
    mCurrentTargetInputBuffer = NULL;

    mDragging = false;
    mLastMouseX = 0;
    mLastMouseY = 0;

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
    mHumanSpeak = false;

    mZoom = 1.0f;

    mFeedCenter = 0.5f;

    mSoundRange = 250.0f;
    mSpeakInterval = 250;
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

    for (list<VisualEvent*>::iterator iterEvent = mVisualEvents.begin();
            iterEvent != mVisualEvents.end();
            iterEvent++)
    {
        free(*iterEvent);
    }
    mVisualEvents.clear();
}

void SimCont2D::initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics)
{
    Simulation::initGraphics(width, height, fullScreen, noGraphics);

    mBackgroundTexture = art_loadImage("media/grass.jpg");
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
    float origX = obj->mFloatData[FLOAT_X];
    float origY = obj->mFloatData[FLOAT_Y];
    float size = obj->mFloatData[FLOAT_SIZE];

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

    obj->mFloatData[FLOAT_X] = x;
    obj->mFloatData[FLOAT_Y] = y;

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
    obj->mFloatData[FLOAT_ROT] = normalizeAngle(rot);
}

void SimCont2D::initializeData(SimulationObject* obj)
{
    obj->initFloatData(18);
    obj->initULData(5);
    obj->initIntData(4);
}

void SimCont2D::addObject(SimulationObject* object, bool init)
{
    if(!object->mDataInitialized)
    {
        initializeData(object);
        object->mDataInitialized = true;
    }

    object->setNamedFloatDataIndex("x", FLOAT_X);
    object->setNamedFloatDataIndex("y", FLOAT_Y);
    object->setNamedFloatDataIndex("rot", FLOAT_ROT);

    object->mFloatData[FLOAT_SPEED_X] = 0.0f;
    object->mFloatData[FLOAT_SPEED_Y] = 0.0f;
    object->mFloatData[FLOAT_SPEED_ROT] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_X] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_Y] = 0.0f;
    object->mFloatData[FLOAT_IMPULSE_ROT] = 0.0f;

    object->setFloatDataFromSymbol("size", FLOAT_SIZE);
    object->mFloatData[FLOAT_SIZE_SQUARED] = object->mFloatData[FLOAT_SIZE] * object->mFloatData[FLOAT_SIZE];
    object->setFloatDataFromSymbol("initial_energy", FLOAT_INITIAL_ENERGY);
    object->setFloatDataFromSymbol("metabolism", FLOAT_METABOLISM);
    object->setFloatDataFromSymbol("friction", FLOAT_FRICTION);
    object->setFloatDataFromSymbol("drag", FLOAT_DRAG);
    object->setFloatDataFromSymbol("rot_friction", FLOAT_ROT_FRICTION);
    object->setFloatDataFromSymbol("rot_drag", FLOAT_ROT_DRAG);
    object->setULDataFromSymbol("low_age_limit", UL_LOW_AGE_LIMIT);
    object->setULDataFromSymbol("high_age_limit", UL_HIGH_AGE_LIMIT);

    object->mFloatData[FLOAT_ENERGY] = object->mFloatData[FLOAT_INITIAL_ENERGY];

    object->mULData[UL_COLLISION_DETECTION_ITERATION] = 0;

    if (object->mULData[UL_HIGH_AGE_LIMIT] > 0)
    {
        object->mULData[UL_MAX_AGE] =
            mDistAge->iuniform(
                object->mULData[UL_LOW_AGE_LIMIT],
                object->mULData[UL_HIGH_AGE_LIMIT]);
    }
    else
    {
        object->mULData[UL_MAX_AGE] = 0;
    }

    if (init)
    {
        object->mULData[UL_MAX_AGE] = mDistAge->iuniform(1, object->mULData[UL_MAX_AGE]);
    }

    object->mULData[UL_LAST_SPEAK_TIME] = 0;

    if (object->mType == SimulationObject::TYPE_AGENT)
    {
        Agent* agent = (Agent*)object;
        int channelObjects = agent->getBrain()->getChannelByName("objects");
        int channelSounds = agent->getBrain()->getChannelByName("sounds");
        int channelSelf = agent->getBrain()->getChannelByName("self");
        int channelBeta = agent->getBrain()->getChannelByName("beta");

        object->mIntData[INT_CHANNEL_OBJECTS] = channelObjects;
        object->mIntData[INT_CHANNEL_SOUNDS] = channelSounds;
        object->mIntData[INT_CHANNEL_SELF] = channelSelf;
        object->mIntData[INT_CHANNEL_BETA] = channelBeta;
    }

    Simulation::addObject(object);
}

void SimCont2D::removeObject(SimulationObject* obj, bool deleteObj)
{
    float origX = obj->mFloatData[FLOAT_X];
    float origY = obj->mFloatData[FLOAT_Y];
    float size = obj->mFloatData[FLOAT_SIZE];
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

    Simulation::removeObject(obj, deleteObj);
}

void SimCont2D::placeRandom(SimulationObject* obj)
{
    setPos(obj,
            mDistPosition->uniform(0, mWorldWidth),
            mDistPosition->uniform(0, mWorldLength));
    setRot(obj, mDistPosition->uniform(0, M_PI * 2));
}

void SimCont2D::placeNear(SimulationObject* obj, SimulationObject* ref)
{
    if (obj->getBirthRadius() == 0.0f)
    {
        placeRandom(obj);
        return;
    }

    float origX = ref->mFloatData[FLOAT_X];
    float origY = ref->mFloatData[FLOAT_Y];

    float distance = mDistPosition->uniform(0, obj->getBirthRadius());
    float angle = mDistPosition->uniform(0, M_PI * 2);

    float targX = origX + (sinf(angle) * distance);
    float targY = origY + (cosf(angle) * distance);

    // Wrap around if limits are exceeded
    if (targX < 0.0f)
    {
        targX = mWorldWidth + targX;
    }
    if (targY < 0.0f)
    {
        targY = mWorldLength + targY;
    }
    if (targX > mWorldWidth)
    {
        targX = targX - mWorldWidth;
    }
    if (targY > mWorldLength)
    {
        targY = targY - mWorldLength;
    }

    setPos(obj, targX, targY);
    setRot(obj, mDistPosition->uniform(0, M_PI * 2));
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

            float dX =  obj->mFloatData[FLOAT_X] - mCollisionX;
            float dY =  obj->mFloatData[FLOAT_Y] - mCollisionY;
            distance = sqrtf((dX * dX) + (dY * dY));

            if ((distance - obj->mFloatData[FLOAT_SIZE]) <= mCollisionRadius)
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
    else if (obj->mULData[UL_MAX_AGE] > 0)
    {
        if (mSimulationTime - obj->mCreationTime >= obj->mULData[UL_MAX_AGE])
        {
            killOrganism(obj);
        }
    }

    obj->mFloatData[FLOAT_SPEED_X] += obj->mFloatData[FLOAT_IMPULSE_X] / obj->mFloatData[FLOAT_SIZE];
    obj->mFloatData[FLOAT_SPEED_Y] += obj->mFloatData[FLOAT_IMPULSE_Y] / obj->mFloatData[FLOAT_SIZE];
    obj->mFloatData[FLOAT_IMPULSE_X] = 0.0f;
    obj->mFloatData[FLOAT_IMPULSE_Y] = 0.0f;

    float speed = sqrtf((obj->mFloatData[FLOAT_SPEED_X]
                    * obj->mFloatData[FLOAT_SPEED_X])
                    + (obj->mFloatData[FLOAT_SPEED_Y]
                    * obj->mFloatData[FLOAT_SPEED_Y]));

    float friction = obj->mFloatData[FLOAT_FRICTION] * obj->mFloatData[FLOAT_SIZE];
    float newSpeedRatio = 0.0f;

    if (speed > friction)
    {
        newSpeedRatio = (speed - friction) / speed;
    }

    float oneMinusDrag = 1 - obj->mFloatData[FLOAT_DRAG];
    
    obj->mFloatData[FLOAT_SPEED_X] *= newSpeedRatio * oneMinusDrag;
    obj->mFloatData[FLOAT_SPEED_Y] *= newSpeedRatio * oneMinusDrag;

    float newX = obj->mFloatData[FLOAT_X] + obj->mFloatData[FLOAT_SPEED_X];
    float newY = obj->mFloatData[FLOAT_Y] + obj->mFloatData[FLOAT_SPEED_Y];

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

    obj->mFloatData[FLOAT_SPEED_ROT] += obj->mFloatData[FLOAT_IMPULSE_ROT] / obj->mFloatData[FLOAT_SIZE];
    obj->mFloatData[FLOAT_IMPULSE_ROT] = 0.0f;

    float rotFriction = obj->mFloatData[FLOAT_ROT_FRICTION] * obj->mFloatData[FLOAT_SIZE];
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

    float newRot = obj->mFloatData[FLOAT_ROT] + obj->mFloatData[FLOAT_SPEED_ROT];

    setPos(obj, newX, newY);
    setRot(obj, newRot);

    // Update fitness
    switch (obj->getFitnessMeasure())
    {
    case FITNESS_ENERGY:
        obj->mFitness = obj->mFloatData[FLOAT_ENERGY];
        break;
    case FITNESS_ENERGY_SUM:
        if ((mSimulationTime % 10) == 0)
        {
            obj->mFitness += obj->mFloatData[FLOAT_ENERGY];
        }
        break;
    case FITNESS_ENERGY_SUM_ABOVE_INIT:
        if ((mSimulationTime % 10) == 0)
        {
            float energy = obj->mFloatData[FLOAT_ENERGY];
            energy -= obj->mFloatData[FLOAT_INITIAL_ENERGY];
            if (energy < 0.0f)
            {
                energy = 0.0f;
            }
            obj->mFitness += energy;
        }
        break;
    case FITNESS_RANDOM:
        if ((mSimulationTime % 1000) == 0)
        {
            obj->mFitness = mDistFitnessRandom->uniform(0.0f, 1.0f);
        }
    }
}

void SimCont2D::perceive(Agent* agent)
{
    // Perceive visible objects
    mTargetObject = NULL;
    mDistanceToTargetObject = 9999999999.9f;
    mCurrentTargetInputBuffer = NULL;

    mLowLimitViewAngle = normalizeAngle(agent->mFloatData[FLOAT_ROT] - mHalfViewAngle);
    mHighLimitViewAngle = normalizeAngle(agent->mFloatData[FLOAT_ROT] + mHalfViewAngle);

    startCollisionDetection(agent->mFloatData[FLOAT_X], agent->mFloatData[FLOAT_Y], mViewRange);

    SimulationObject* target;
    float distance;
    float angle;

    while (target = nextCollision(distance, angle))
    {
        if (target != agent)
        {
            bool visible = false;
            
            // This is an aproximation
            float extraAngle = atan2f(target->mFloatData[FLOAT_SIZE], distance);

            distance -= agent->mFloatData[FLOAT_SIZE];
            distance -= target->mFloatData[FLOAT_SIZE];
            if (distance < 0.0f)
            {
                distance = 0.0f;
            }

            if (mHighLimitViewAngle > mLowLimitViewAngle)
            {
                if ((angle - extraAngle <= mHighLimitViewAngle) && (angle + extraAngle >= mLowLimitViewAngle))
                {
                    visible = true;
                }
            }
            else
            {
                if ((angle - extraAngle <= mHighLimitViewAngle) || (angle + extraAngle >= mLowLimitViewAngle))
                {
                    visible = true;
                }
            }

            if (visible)
            {
                onScanObject(agent,
                                target,
                                distance,
                                normalizeAngle(agent->mFloatData[FLOAT_ROT] - angle));
            }
        }
    }

    // Perceive sounds
    list<Message*>* messageList = agent->getMessageList();

    if (agent->mIntData[INT_CHANNEL_SOUNDS] >= 0)
    {
        for (list<Message*>::iterator iterMessage = messageList->begin();
                iterMessage != messageList->end();
                iterMessage++)
        {
            Message* msg = (*iterMessage);

            list<InterfaceItem*>* interface = agent->getBrain()->getInputInterface(agent->mIntData[INT_CHANNEL_SOUNDS]);
            float* inBuffer = agent->getBrain()->getInputBuffer(agent->mIntData[INT_CHANNEL_SOUNDS]);

            if (inBuffer != NULL)
            {
                unsigned int pos = 0;
                float normalizedValue;

                for (list<InterfaceItem*>::iterator iterItem = interface->begin();
                    iterItem != interface->end();
                    iterItem++)
                {
                    unsigned int type = (*iterItem)->mType;

                    switch (type)
                    {
                        case PERCEPTION_POSITION:
                            normalizedValue = msg->mData[1] / M_PI;
                            inBuffer[pos] = normalizedValue;
                            break;

                        case PERCEPTION_DISTANCE:
                            normalizedValue = msg->mData[0] / mSoundRange;
                            inBuffer[pos] = normalizedValue;
                            break;

                        case PERCEPTION_SYMBOL:
                            InterfaceItem* item = (*iterItem);
                            normalizedValue = calcSymbolsBinding(agent,
                                                    item->mOrigSymTable,
                                                    item->mOrigSymID,
                                                    msg->mSymbol);
                            inBuffer[pos] = normalizedValue;
                            break;
                    }

                    pos++;
                }
            }
        }
    }

    // Perceive self
    if (agent->mIntData[INT_CHANNEL_SELF] >= 0)
    {
        list<InterfaceItem*>* interface = agent->getBrain()->getInputInterface(agent->mIntData[INT_CHANNEL_SELF]);
        float* inBuffer = agent->getBrain()->getInputBuffer(agent->mIntData[INT_CHANNEL_SELF]);
        unsigned int pos = 0;
        float normalizedValue;
        float ratio;

        for (list<InterfaceItem*>::iterator iterItem = interface->begin();
            iterItem != interface->end();
            iterItem++)
        {
            unsigned int type = (*iterItem)->mType;

            switch (type)
            {
                case PERCEPTION_ENERGY:
                    ratio = agent->mFloatData[FLOAT_ENERGY] / agent->mFloatData[FLOAT_INITIAL_ENERGY];
                    normalizedValue = 1.0f - (1.0f / (ratio + 1.0f));
                    inBuffer[pos] = normalizedValue;
                    break;

                case PERCEPTION_CAN_SPEAK:
                    normalizedValue = 0.0f;

                    if ((mSimulationTime - agent->mULData[UL_LAST_SPEAK_TIME]) > mSpeakInterval)
                    {
                        normalizedValue = 1.0f;
                    }
                    inBuffer[pos] = normalizedValue;
                    break;
            }

            pos++;
        }
    }
}

void SimCont2D::onScanObject(Agent* orig,
                SimulationObject* targ,
                float distance,
                float angle)
{
    float* inBuffer = orig->getBrain()->getInputBuffer(orig->mIntData[INT_CHANNEL_OBJECTS]);
    if (inBuffer == NULL)
    {
        return;
    }

    bool isTarget = false;

    // TODO: use the nearest to angle 0 instead of the closest distance?
    if (distance <= 0)
    {
        if ((mTargetObject == NULL) || (distance < mDistanceToTargetObject))
        {
            mTargetObject = targ;
            mDistanceToTargetObject = distance;
            isTarget = true;
        }
    }

    float normalizedValue;

    list<InterfaceItem*>* interface = orig->getBrain()->getInputInterface(orig->mIntData[INT_CHANNEL_OBJECTS]);
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
                normalizedValue = angle / M_PI;
                inBuffer[pos] = normalizedValue;
                break;

            case PERCEPTION_DISTANCE:
                normalizedValue = distance / mViewRange;
                inBuffer[pos] = normalizedValue;
                break;

            case PERCEPTION_TARGET:
                if (isTarget)
                {
                    inBuffer[pos] = 1.0f;

                    if (mCurrentTargetInputBuffer != NULL)
                    {
                        mCurrentTargetInputBuffer[pos] = 0.0f;
                    }
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case PERCEPTION_SYMBOL:
                InterfaceItem* item = (*iterItem);
                normalizedValue = calcSymbolsBinding(orig,
                                                    targ,
                                                    item->mOrigSymTable,
                                                    item->mTargetSymTable,
                                                    item->mOrigSymID);
                inBuffer[pos] = normalizedValue;
                break;
        }

        pos++;
    }

    if (isTarget)
    {
        mCurrentTargetInputBuffer = inBuffer;
    }
}

void SimCont2D::act(Agent* agent)
{
    bool actionGo = false;
    bool actionRotate = false;
    Action actionEat = ACTION_NULL;
    Symbol* actionSpeakSymbol = NULL;
    float actionGoParam = 0.0f;
    float actionRotateParam = 0.0f;
    float actionSpeakParam = -99999999.9f;

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
            actionRotateParam += 1.0f;
        }
        if (mHumanRotateRight)
        {
            actionRotate = true;
            actionRotateParam -= 1.0f;
        }
        if (mHumanEat)
        {
            actionEat = ACTION_EAT;
        }
        if (mHumanSpeak)
        {
            actionSpeakSymbol = new SymbolRGB(255, 0, 0);
            actionSpeakParam = 1.0f;
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

            Action actionType = (Action)(*iterItem)->mType;

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
                    case ACTION_EATB:
                        actionEat = actionType;
                        break;
                    case ACTION_SPEAK:
                        if (output > actionSpeakParam)
                        {
                            SymbolTable* table = agent->getSymbolTable((*iterItem)->mOrigSymTable);

                            if (table != NULL)
                            {
                                Symbol* sym = table->getSymbol((*iterItem)->mOrigSymID);
                                //printf("id: %d\n", (*iterItem)->mOrigSymID);

                                if (sym != NULL)
                                {
                                    actionSpeakParam = output;
                                    actionSpeakSymbol = sym;
                                }
                            }
                        }
                        break;
                }
            }

            pos++;
        }
    }

    if (actionGoParam > 1.0f)
    {
        actionGoParam = 1.0f;
    }
    else if (actionGoParam < -1.0f)
    {
        actionGoParam = -1.0f;
    }
    if (actionRotateParam > 1.0f)
    {
        actionRotateParam = 1.0f;
    }
    else if (actionRotateParam < -1.0f)
    {
        actionRotateParam = -1.0f;
    }

    if (actionGo)
    {
        go(agent, actionGoParam * mGoForceScale);
    }
    if (actionRotate)
    {
        rotate(agent, actionRotateParam * mRotateForceScale);
    }
    if (actionEat != ACTION_NULL)
    {
        eat(agent, actionEat);
    }
    if (actionSpeakSymbol != NULL)
    {
        speak(agent, actionSpeakSymbol);
    }
}

void SimCont2D::go(Agent* agent, float force)
{
    float delta = -mGoCost * fabsf(force);
    deltaEnergy(agent, delta);

    agent->mFloatData[FLOAT_IMPULSE_X] = cosf(agent->mFloatData[FLOAT_ROT]) * force;
    agent->mFloatData[FLOAT_IMPULSE_Y] = sinf(agent->mFloatData[FLOAT_ROT]) * force;
}

void SimCont2D::rotate(Agent* agent, float force)
{
    deltaEnergy(agent, -mRotateCost * fabsf(force));

    agent->mFloatData[FLOAT_IMPULSE_ROT] -= force;
}

void SimCont2D::eat(Agent* agent, Action actionType)
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

        switch (actionType)
        {
        case ACTION_EAT:
            if (sym1->getBinding(sym2) > mFeedCenter)
            {
                float energy = mTargetObject->mFloatData[FLOAT_ENERGY];
                deltaEnergy(agent, energy);
                deltaEnergy(mTargetObject, -energy);
            }
            break;
        case ACTION_EATB:
            float distance = sym1->getBinding(sym2);
            float energyFactor;

            if (distance < mFeedCenter)
            {
                energyFactor = -(mFeedCenter - distance) / mFeedCenter;
            }
            else
            {
                energyFactor = (distance - mFeedCenter) / (1.0f - mFeedCenter);
            }

            float energy = mTargetObject->mFloatData[FLOAT_ENERGY];
            deltaEnergy(agent, energyFactor * energy);
            deltaEnergy(mTargetObject, -energy);
            break;
        }
    }
}

void SimCont2D::drawBeforeObjects()
{
    art_setScale(mZoom, mZoom);
    art_setTranslation(mViewX, mViewY);

    drawTerrain();

    if (mShowGrid)
    {
        unsigned int cellSide = (unsigned int)mCellSide;

        int viewX = (int)mViewX;
        int viewY = (int)mViewY;

        art_setColor(0, 0, 0, 255);
        art_setLineWidth(0.5f);

        unsigned int division = cellSide;
        while (division < mWorldWidth)
        {
            art_drawLine(division,
                            0,
                            division,
                            mWorldLength);

            division += cellSide;
        }

        division = cellSide;
        while (division < mWorldLength)
        {
            art_drawLine(0,
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
                float beginAngle = normalizeAngle(obj->mFloatData[FLOAT_ROT] - mHalfViewAngle);
                float endAngle = normalizeAngle(obj->mFloatData[FLOAT_ROT] + mHalfViewAngle);

                if (beginAngle > endAngle)
                {
                    endAngle += M_PI * 2.0f;
                }

                art_setColor(150, 150, 150, 100);
                art_fillCircleSlice(obj->mFloatData[FLOAT_X],
                                obj->mFloatData[FLOAT_Y],
                                mViewRange,
                                beginAngle,
                                endAngle);
            }
        }
    }

    list<VisualEvent*>::iterator iterEvent = mVisualEvents.begin();

    while (iterEvent != mVisualEvents.end())
    {
        VisualEvent* ve = (*iterEvent);

        if (ve->mEndTime < mSimulationTime)
        {
            list<VisualEvent*>::iterator iterRemove = iterEvent;
            iterEvent++;
            mVisualEvents.erase(iterRemove);
            free(ve);
        }
        else
        {
            switch (ve->mType)
            {
            case VE_SPEAK:
                unsigned int deltaTime = ve->mEndTime - ve->mStartTime;
                unsigned int curTime = mSimulationTime - ve->mStartTime;
                float alpha = (1.0f - (((float)curTime) / ((float)deltaTime))) * 255.0f;
                art_setColor(ve->mRed, ve->mGreen, ve->mBlue, (unsigned int)alpha);
                art_fillCircle(ve->mX, ve->mY, mSoundRange);
                break;
            }
            iterEvent++;
        }
    }
}

void SimCont2D::drawTerrain()
{
    art_setColor(0, 255, 0, 255);
    art_setTexture(mBackgroundTexture);
    art_fillRectangle(0, 0, mWorldWidth, mWorldLength);
    art_clearTexture();
}

void SimCont2D::drawAfterObjects()
{
    if (mShowEnergy)
    {
        art_setFont(mFont);
        char text[255];
        list<SimulationObject*>::iterator iterObj;
        for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
        {
            SimulationObject* obj = *iterObj;

            art_setColor(100, 100, 100, 100);
            sprintf(text, "%f", obj->mFloatData[FLOAT_ENERGY]);
            art_drawText(obj->mFloatData[FLOAT_X],
                            obj->mFloatData[FLOAT_Y],
                            text);
        }
    }

    art_clearScale();
    art_clearTranslation();


    if (mShowBrain)
    {
        if (mHumanAgent)
        {
            art_setTranslation(0, art_getWinHeight() / 2);
            art_setColor(255, 255, 255, 130);
            art_fillRectangle(0, 0, art_getWinWidth(), art_getWinHeight() / 2);
            art_setColor(50, 50, 50, 255);
            art_setFont(mFont);
            mHumanAgent->getBrain()->draw();
            art_clearTranslation();
        }
    }
}

SimulationObject* SimCont2D::getObjectByScreenPos(int x, int y)
{
    /*list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimulationObject* obj = *iterObj;
    }*/

    return NULL;
}

bool SimCont2D::getFieldValue(SimulationObject* obj, string fieldName, float& value)
{
    if (fieldName == "energy")
    {
        value = obj->mFloatData[FLOAT_ENERGY];
        return true;
    }
    else
    {
        return Simulation::getFieldValue(obj, fieldName, value);
    }
}

bool SimCont2D::onKeyDown(Art_KeyCode key)
{
    if (Simulation::onKeyDown(key))
    {
        return true;
    }

    switch (key)
    {
    case ART_KEY_UP:
        mHumanGo = true;
        return true;
    case ART_KEY_RIGHT:
        mHumanRotateRight = true;
        return true;
    case ART_KEY_LEFT:
        mHumanRotateLeft = true;
        return true;
    case ART_KEY_E:
        mHumanEat = true;
        return true;
    case ART_KEY_S:
        mHumanSpeak = true;
        return true;
    default:
        return false;
    }
}

bool SimCont2D::onKeyUp(Art_KeyCode key)
{
    if (Simulation::onKeyUp(key))
    {
        return true;
    }

    switch (key)
    {
    case ART_KEY_G:
        mShowGrid = !mShowGrid;
        return true;
    case ART_KEY_V:
        mShowViewRange = !mShowViewRange;
        return true;
    case ART_KEY_B:
        mShowBrain = !mShowBrain;
        return true;
    case ART_KEY_N:
        mShowEnergy = !mShowEnergy;
        return true;
    case ART_KEY_UP:
        mHumanGo = false;
        return true;
    case ART_KEY_RIGHT:
        mHumanRotateRight = false;
        return true;
    case ART_KEY_LEFT:
        mHumanRotateLeft = false;
        return true;
    case ART_KEY_E:
        mHumanEat = false;
        return true;
    case ART_KEY_S:
        mHumanSpeak = false;
        return true;
    default:
        return false;
    }
}

bool SimCont2D::onMouseButtonDown(Art_MouseButton button, int x, int y)
{
    mDragging = true;
    mLastMouseX = x;
    mLastMouseY = y;
    return false;
}

bool SimCont2D::onMouseButtonUp(Art_MouseButton button, int x, int y)
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
    float wWidth = (float)art_getWinWidth();
    float wHeight = (float)art_getWinHeight();

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
    obj->mFloatData[FLOAT_SIZE] = size;
    obj->mFloatData[FLOAT_SIZE_SQUARED] = size * size;
}

void SimCont2D::deltaEnergy(SimulationObject* obj, double delta)
{
    obj->mFloatData[FLOAT_ENERGY] += delta;
}

string SimCont2D::getInterfaceName(bool input, int type)
{
    if (input)
    {
        switch (type)
        {
        case PERCEPTION_IN_CONTACT:
            return "contact";
        case PERCEPTION_TARGET:
            return "target";
        case PERCEPTION_POSITION:
            return "position";
        case PERCEPTION_DISTANCE:
            return "distance";
        case PERCEPTION_ENERGY:
            return "energy";
        case PERCEPTION_CAN_SPEAK:
            return "canspeak";
        default:
            return "?";
        }
    }
    else
    {
        switch (type)
        {
        case ACTION_GO:
            return "go";
        case ACTION_ROTATE:
            return "rotate";
        case ACTION_EAT:
            return "eat";
        case ACTION_EATB:
            return "eatB";
        case ACTION_SPEAK:
            return "speak";
        default:
            return "?";
        }
    }
}

void SimCont2D::speak(Agent* agent, Symbol* sym)
{
    if (((mSimulationTime - agent->mULData[UL_LAST_SPEAK_TIME]) <= mSpeakInterval)
        && (agent->mULData[UL_LAST_SPEAK_TIME] != 0))
    {
        return;
    }

    agent->mULData[UL_LAST_SPEAK_TIME] = mSimulationTime;

    startCollisionDetection(agent->mFloatData[FLOAT_X], agent->mFloatData[FLOAT_Y], mSoundRange);

    SimulationObject* target;
    float distance;
    float angle;

    while (target = nextCollision(distance, angle))
    {
        if ((target->mType == SimulationObject::TYPE_AGENT)
            && (agent != target))
        {
            Agent* targAgent = (Agent*)target;
            Message* msg = new Message();
            msg->mSymbol = sym->clone();
            msg->mType = 0; // Only one message type for now (sound message)
            float* msgData = (float*)malloc(2 * sizeof(float));
            msgData[0] = distance;
            msgData[1] = normalizeAngle(angle + M_PI);
            msg->mData = msgData;
            targAgent->addMessage(msg);
        }
    }

    VisualEvent* ve = (VisualEvent*)malloc(sizeof(VisualEvent));
    ve->mType = VE_SPEAK;
    ve->mX = agent->mFloatData[FLOAT_X];
    ve->mY = agent->mFloatData[FLOAT_Y];
    ve->mRed = sym->getRed();
    ve->mGreen = sym->getGreen();
    ve->mBlue = sym->getBlue();
    ve->mStartTime = mSimulationTime;
    ve->mEndTime = mSimulationTime + mSpeakInterval;
    mVisualEvents.push_back(ve);
}

const char SimCont2D::mClassName[] = "SimCont2D";

Orbit<SimCont2D>::MethodType SimCont2D::mMethods[] = {
    {"addObject", &Simulation::addObject},
    {"setSeedIndex", &Simulation::setSeedIndex},
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"initGraphics", &Simulation::initGraphics},
    {"run", &Simulation::run},
    {"setTimeLimit", &Simulation::setTimeLimit},
    {"setWorldDimensions", &SimCont2D::setWorldDimensions},
    {"setViewRange", &SimCont2D::setViewRange},
    {"setViewAngle", &SimCont2D::setViewAngle},
    {"setGoCost", &SimCont2D::setGoCost},
    {"setRotateCost", &SimCont2D::setRotateCost},
    {"setGoForceScale", &SimCont2D::setGoForceScale},
    {"setRotateForceScale", &SimCont2D::setRotateForceScale},
    {"setPos", &SimCont2D::setPos},
    {"setRot", &SimCont2D::setRot},
    {"setHuman", &SimCont2D::setHuman},
    {"setFeedCenter", &SimCont2D::setFeedCenter},
    {"setSoundRange", &SimCont2D::setSoundRange},
    {"setSpeakInterval", &SimCont2D::setSpeakInterval},
    {0,0}
};

Orbit<SimCont2D>::NumberGlobalType SimCont2D::mNumberGlobals[] = {
    {"PERCEPTION_NULL", PERCEPTION_NULL},
    {"PERCEPTION_POSITION", PERCEPTION_POSITION},
    {"PERCEPTION_DISTANCE", PERCEPTION_DISTANCE},
    {"PERCEPTION_TARGET", PERCEPTION_TARGET},
    {"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
    {"PERCEPTION_SYMBOL", PERCEPTION_SYMBOL},
    {"PERCEPTION_ENERGY", PERCEPTION_ENERGY},
    {"PERCEPTION_CAN_SPEAK", PERCEPTION_CAN_SPEAK},
    {"ACTION_NULL", ACTION_NULL},
    {"ACTION_GO", ACTION_GO},
    {"ACTION_ROTATE", ACTION_ROTATE},
    {"ACTION_EAT", ACTION_EAT},
    {"ACTION_EATB", ACTION_EATB},
    {"ACTION_SPEAK", ACTION_SPEAK},
    {"FITNESS_ENERGY", FITNESS_ENERGY},
    {"FITNESS_ENERGY_SUM", FITNESS_ENERGY_SUM},
    {"FITNESS_ENERGY_SUM_ABOVE_INIT", FITNESS_ENERGY_SUM_ABOVE_INIT},
    {"FITNESS_RANDOM", FITNESS_RANDOM},
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

int SimCont2D::setGoForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setGoForceScale(scale);
    return 0;
}

int SimCont2D::setRotateForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setRotateForceScale(scale);
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

int SimCont2D::setFeedCenter(lua_State* luaState)
{
    float center = luaL_checknumber(luaState, 1);
    setFeedCenter(center);
    return 0;
}

int SimCont2D::setSoundRange(lua_State* luaState)
{
    float range = luaL_checknumber(luaState, 1);
    setSoundRange(range);
    return 0;
}

int SimCont2D::setSpeakInterval(lua_State* luaState)
{
    unsigned int speakInterval = luaL_checkint(luaState, 1);
    setSpeakInterval(speakInterval);
    return 0;
}

