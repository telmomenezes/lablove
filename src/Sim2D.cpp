/*
 * LabLOVE
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

#include "Sim2D.h"
#include "PopulationDynamics.h"
#include "SymbolFloat.h"
#include "SymbolRGB.h"

#include <math.h>
#include <list>

using std::list;

mt_distribution* Sim2D::mDistAge = gDistManager.getNewDistribution();
mt_distribution* Sim2D::mDistPosition = gDistManager.getNewDistribution();
mt_distribution* Sim2D::mDistFitnessRandom = gDistManager.getNewDistribution();

Sim2D::Sim2D(lua_State* luaState)
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
    mFireInterval = 250;
}

Sim2D::~Sim2D()
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

void Sim2D::initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics)
{
    Simulation::initGraphics(width, height, fullScreen, noGraphics);

    mBackgroundTexture = art_loadImage("media/grass.jpg");
}

void Sim2D::setWorldDimensions(float worldWidth,
                                    float worldLength,
                                    unsigned int cellSide)
{
    mWorldWidth = worldWidth;
    mWorldLength = worldLength;
    mCellSide = cellSide;
    mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / ((float)mCellSide)));
    mWorldCellLength = (unsigned int)(ceilf(mWorldLength / ((float)mCellSide)));

    unsigned int gridSize = mWorldCellWidth * mWorldCellLength;
    mCellGrid = (list<SimObj2D*>**)malloc(sizeof(list<SimObj2D*>*) * gridSize);

    for (unsigned int i = 0; i < gridSize; i++)
    {
        mCellGrid[i] = new list<SimObj2D*>();
    }
}

void Sim2D::setPos(SimObj2D* obj, float x, float y)
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
                    list<SimObj2D*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
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
                list<SimObj2D*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
                cellList->push_back(obj);
            }
        }
    }
}

void Sim2D::setRot(SimObj2D* obj, float rot)
{
    obj->mRot = normalizeAngle(rot);
}

void Sim2D::addObject(SimObj* obj, bool init)
{
    SimObj2D* object = (SimObj2D*)obj;

    object->mEnergy = object->mInitialEnergy;

    if (init)
    {
        object->mAgeLimit = mDistAge->iuniform(1, object->mMaxAge);
    }
    else
    {
        object->mAgeLimit = object->mMaxAge;
    }

    if (object->mType == SimObj::TYPE_AGENT)
    {
        int channelObjects = object->getBrain()->getChannelByName("objects");
        int channelSounds = object->getBrain()->getChannelByName("sounds");
        int channelSelf = object->getBrain()->getChannelByName("self");
        int channelBeta = object->getBrain()->getChannelByName("beta");

        object->mChannelObjects = channelObjects;
        object->mChannelSounds = channelSounds;
        object->mChannelSelf = channelSelf;
        object->mChannelBeta = channelBeta;
    }

    SimObj2D* obj2D = (SimObj2D*)object;
    for (list<Graphic*>::iterator iterGraph = obj2D->mGraphics.begin();
            iterGraph != obj2D->mGraphics.end();
            iterGraph++)
    {
        (*iterGraph)->init(obj2D);
    }

    Simulation::addObject(object);
}

void Sim2D::removeObject(SimObj* obj, bool deleteObj)
{
    SimObj2D* object = (SimObj2D*)obj;

    float origX = object->mX;
    float origY = object->mY;
    float size = object->mSize;
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
            list<SimObj2D*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];
            cellList->remove(object);
        }
    }

    Simulation::removeObject(obj, deleteObj);
}

void Sim2D::placeRandom(SimObj* obj)
{
    setPos((SimObj2D*)obj,
            mDistPosition->uniform(0, mWorldWidth),
            mDistPosition->uniform(0, mWorldLength));
    setRot((SimObj2D*)obj, mDistPosition->uniform(0, M_PI * 2));
}

void Sim2D::placeNear(SimObj* obj, SimObj* ref)
{
    if (obj->getBirthRadius() == 0.0f)
    {
        placeRandom(obj);
        return;
    }

    float origX = ((SimObj2D*)ref)->mX;
    float origY = ((SimObj2D*)ref)->mY;

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

    setPos((SimObj2D*)obj, targX, targY);
    setRot((SimObj2D*)obj, mDistPosition->uniform(0, M_PI * 2));
}

void Sim2D::startCollisionDetection(float x, float y, float rad)
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

SimObj2D* Sim2D::nextCollision(float& distance, float& angle)
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

        SimObj2D* obj = *mCurrentCellListIterator;

        if (obj->mCollisionDetectionIteration != mCollisionDetectionIteration)
        {
            obj->mCollisionDetectionIteration = mCollisionDetectionIteration;

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

void Sim2D::onCycle()
{
    processLaserShots();
}

void Sim2D::process(SimObj* object)
{
    SimObj2D* obj = (SimObj2D*)object;

    if (obj->mEnergy <= 0)
    {
        killOrganism(obj);
    }
    else if (obj->mMaxAge > 0)
    {
        if (mSimulationTime - obj->mCreationTime >= obj->mAgeLimit)
        {
            killOrganism(obj);
        }
    }

    obj->mSpeedX += obj->mImpulseX / obj->mSize;
    obj->mSpeedY += obj->mImpulseY / obj->mSize;
    obj->mImpulseX = 0.0f;
    obj->mImpulseY = 0.0f;

    float speed = sqrtf((obj->mSpeedX
                    * obj->mSpeedX)
                    + (obj->mSpeedY
                    * obj->mSpeedY));


    float oneMinusDrag = 1 - obj->mDrag;
    
    obj->mSpeedX *= oneMinusDrag;
    obj->mSpeedY *= oneMinusDrag;

    float newX = obj->mX + obj->mSpeedX;
    float newY = obj->mY + obj->mSpeedY;

    if (newX < 0.0f)
    {
        newX = 0.0f;
        obj->mSpeedX = 0.0f;
    }
    else if (newX > mWorldWidth)
    {
        newX = mWorldWidth;
        obj->mSpeedX = 0.0f;
    }
    if (newY < 0.0f)
    {
        newY = 0.0f;
        obj->mSpeedY = 0.0f;
    }
    else if (newY > mWorldLength)
    {
        newY = mWorldLength;
        obj->mSpeedY = 0.0f;
    }

    obj->mSpeedRot += obj->mImpulseRot / obj->mSize;
    obj->mImpulseRot = 0.0f;

    float absRotSpeed = obj->mSpeedRot;
    if (obj->mSpeedRot < 0.0f)
    {
        absRotSpeed = -absRotSpeed;
    }

    float oneMinusRotDrag = 1.0f - obj->mRotDrag;
    obj->mSpeedRot *= oneMinusRotDrag;

    float newRot = obj->mRot + obj->mSpeedRot;

    setPos((SimObj2D*)obj, newX, newY);
    setRot((SimObj2D*)obj, newRot);

    // Update fitness
    switch (obj->getFitnessMeasure())
    {
    case FITNESS_ENERGY:
        obj->mFitness = obj->mEnergy;
        break;
    case FITNESS_ENERGY_SUM:
        if ((mSimulationTime % 10) == 0)
        {
            obj->mFitness += obj->mEnergy;
        }
        break;
    case FITNESS_ENERGY_SUM_ABOVE_INIT:
        if ((mSimulationTime % 10) == 0)
        {
            float energy = obj->mEnergy;
            energy -= obj->mInitialEnergy;
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

void Sim2D::perceive(SimObj* obj)
{
    SimObj2D* agent = (SimObj2D*)obj;

    // Perceive visible objects
    mTargetObject = NULL;
    mDistanceToTargetObject = 9999999999.9f;
    mCurrentTargetInputBuffer = NULL;

    mLowLimitViewAngle = normalizeAngle(agent->mRot - mHalfViewAngle);
    mHighLimitViewAngle = normalizeAngle(agent->mRot + mHalfViewAngle);

    startCollisionDetection(agent->mX, agent->mY, mViewRange);

    SimObj2D* target;
    float distance;
    float angle;

    while (target = nextCollision(distance, angle))
    {
        if (target != agent)
        {
            bool visible = false;
            
            // This is an aproximation
            float extraAngle = atan2f(target->mSize, distance);

            distance -= agent->mSize;
            distance -= target->mSize;
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
                onScanObject((SimObj2D*)agent,
                                (SimObj2D*)target,
                                distance,
                                normalizeAngle(agent->mRot - angle),
                                normalizeAngle(target->mRot - angle));
            }
        }
    }

    // Perceive sounds
    list<Message*>* messageList = agent->getMessageList();

    if (agent->mChannelSounds >= 0)
    {
        for (list<Message*>::iterator iterMessage = messageList->begin();
                iterMessage != messageList->end();
                iterMessage++)
        {
            Message* msg = (*iterMessage);

            list<InterfaceItem*>* interface = agent->getBrain()->getInputInterface(agent->mChannelSounds);
            float* inBuffer = agent->getBrain()->getInputBuffer(agent->mChannelSounds);

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
    if (agent->mChannelSelf >= 0)
    {
        list<InterfaceItem*>* interface = agent->getBrain()->getInputInterface(agent->mChannelSelf);
        float* inBuffer = agent->getBrain()->getInputBuffer(agent->mChannelSelf);
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
                    ratio = agent->mEnergy / agent->mInitialEnergy;
                    normalizedValue = 1.0f - (1.0f / (ratio + 1.0f));
                    inBuffer[pos] = normalizedValue;
                    break;

                case PERCEPTION_CAN_SPEAK:
                    normalizedValue = 0.0f;

                    if ((mSimulationTime - agent->mLastSpeakTime) > mSpeakInterval)
                    {
                        normalizedValue = 1.0f;
                    }
                    inBuffer[pos] = normalizedValue;
                    break;
                case PERCEPTION_CAN_FIRE:
                    normalizedValue = 0.0f;

                    if ((mSimulationTime - agent->mLastFireTime) > mFireInterval)
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

void Sim2D::onScanObject(SimObj2D* orig,
                SimObj2D* targ,
                float distance,
                float angle,
                float orientation)
{
    float* inBuffer = orig->getBrain()->getInputBuffer(orig->mChannelObjects);
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

    list<InterfaceItem*>* interface = orig->getBrain()->getInputInterface(orig->mChannelObjects);
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

            case PERCEPTION_ORIENTATION:
                normalizedValue = orientation / M_PI;
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

void Sim2D::act(SimObj* agent)
{
    bool actionGo = false;
    bool actionRotate = false;
    bool actionFire = false;
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
        if (mHumanFire)
        {
            actionFire = true;
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
                    case ACTION_FIRE:
                        actionFire = true;
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
        go((SimObj2D*)agent, actionGoParam * mGoForceScale);
    }
    if (actionRotate)
    {
        rotate((SimObj2D*)agent, actionRotateParam * mRotateForceScale);
    }
    if (actionEat != ACTION_NULL)
    {
        eat((SimObj2D*)agent, actionEat);
    }
    if (actionSpeakSymbol != NULL)
    {
        speak((SimObj2D*)agent, actionSpeakSymbol);
    }
    if (actionFire)
    {
        fire((SimObj2D*)agent);
    }
}

void Sim2D::go(SimObj2D* agent, float force)
{
    float delta = -mGoCost * fabsf(force);
    agent->deltaEnergy(delta);

    agent->mImpulseX = cosf(agent->mRot) * force;
    agent->mImpulseY = sinf(agent->mRot) * force;
}

void Sim2D::rotate(SimObj2D* agent, float force)
{
    agent->deltaEnergy(-mRotateCost * fabsf(force));
    agent->mImpulseRot -= force;
}

void Sim2D::eat(SimObj2D* agent, Action actionType)
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
                float energy = mTargetObject->mEnergy;
                agent->deltaEnergy(energy);
                mTargetObject->deltaEnergy(-energy);
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

            float energy = mTargetObject->mEnergy;
            agent->deltaEnergy(energyFactor * energy);
            mTargetObject->deltaEnergy(-energy);
            break;
        }
    }
}

void Sim2D::fire(SimObj2D* agent)
{
    if (((mSimulationTime - agent->mLastFireTime) <= mFireInterval)
        && (agent->mLastFireTime != 0))
    {
        return;
    }

    agent->mLastFireTime = mSimulationTime;

    createLaserShot(agent->mX,
                    agent->mY,
                    agent->mRot,
                    25,
                    0.02,
                    agent->getID());
}

void Sim2D::drawBeforeObjects()
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
        list<SimObj*>::iterator iterObj;

        for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
        {
            SimObj2D* obj = (SimObj2D*)(*iterObj);

            if (obj->mType == SimObj::TYPE_AGENT)
            {
                float beginAngle = normalizeAngle(obj->mRot - mHalfViewAngle);
                float endAngle = normalizeAngle(obj->mRot + mHalfViewAngle);

                if (beginAngle > endAngle)
                {
                    endAngle += M_PI * 2.0f;
                }

                art_setColor(150, 150, 150, 100);
                art_fillCircleSlice(obj->mX,
                                obj->mY,
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

void Sim2D::drawTerrain()
{
    art_setColor(0, 255, 0, 255);
    art_setTexture(mBackgroundTexture);
    art_fillRectangle(0, 0, mWorldWidth, mWorldLength);
    art_clearTexture();
}

void Sim2D::drawAfterObjects()
{
    drawLaserShots();

    if (mShowEnergy)
    {
        art_setFont(mFont);
        char text[255];
        list<SimObj*>::iterator iterObj;
        for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
        {
            SimObj2D* obj = (SimObj2D*)(*iterObj);

            art_setColor(100, 100, 100, 100);
            sprintf(text, "%f", obj->mEnergy);
            art_drawText(obj->mX,
                            obj->mY,
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

SimObj* Sim2D::getObjectByScreenPos(int x, int y)
{
    /*list<SimObj2D*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimObj* obj = *iterObj;
    }*/

    return NULL;
}

bool Sim2D::getFieldValue(SimObj* obj, string fieldName, float& value)
{
    if (fieldName == "energy")
    {
        value = ((SimObj2D*)obj)->mEnergy;
        return true;
    }
    else
    {
        return Simulation::getFieldValue(obj, fieldName, value);
    }
}

void Sim2D::processLaserShots()
{
    for (list<Laser>::iterator iterLaser = mLaserShots.begin();
            iterLaser != mLaserShots.end();
            iterLaser++)
    {
        Laser* laser = &(*iterLaser);

        float x1 = laser->mX1;
        float y1 = laser->mY1;

        laser->mX1 += cosf(laser->mAng) * laser->mSpeed;
        laser->mY1 += sinf(laser->mAng) * laser->mSpeed;
        laser->mX2 += cosf(laser->mAng) * laser->mSpeed;
        laser->mY2 += sinf(laser->mAng) * laser->mSpeed;

        float x2 = laser->mX2;
        float y2 = laser->mY2;

        float cX = x1;
        float cY = y1;

        int cellX = ((int)(cX)) / mCellSide;
        int cellY = ((int)(cY)) / mCellSide;

        int targCellX = ((int)(x2)) / mCellSide;
        int targCellY = ((int)(y2)) / mCellSide;

        if (targCellX < 0)
        {
            targCellX = 0;
        }
        else if (targCellX >= mWorldCellWidth)
        {
            targCellX = mWorldCellWidth - 1;
        }
        if (targCellY < 0)
        {
            targCellY = 0;
        }
        else if (targCellY >= mWorldCellLength)
        {
            targCellY = mWorldCellLength - 1;
        }

        targCellX *= laser->mDirX;
        targCellY *= laser->mDirY;

        while (((cellX * laser->mDirX) <= targCellX)
            && ((cellY * laser->mDirY) <= targCellY))
        {
            list<SimObj2D*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];

            for (list<SimObj2D*>::iterator iterObj = cellList->begin();
                iterObj != cellList->end();
                iterObj++)
            {
                SimObj2D* obj = (SimObj2D*)(*iterObj);

                if (obj->getID() != laser->mOwnerID)
                {
                    float objX = obj->mX;
                    float objY = obj->mY;
                    float r = obj->mSize;

                    float x_1 = x1 - objX;
                    float x_2 = x2 - objX;
                    float y_1 = y1 - objY;
                    float y_2 = y2 - objY;

                    float dx = x_2 - x_1;
                    float dy = y_2 - y_1;
                    float dr2 = (dx * dx) + (dy * dy);
                    float D = (x_1 * y_2) - (x_2 * y_1);

                    float colides = ((r * r) * dr2) - (D * D);

                    if (colides >= 0.0f)
                    {
                        //printf("colision! %d\n", obj->getID());
                        ((SimObj2D*)obj)->deltaEnergy(-1000.0f);
                    }
                }
            }

            float bX;
            float bY;

            if (laser->mDirX == 1)
            {
                bX = (cellX + 1) * mCellSide;
            }
            else
            {
                bX = cellX * mCellSide;
            }
            if (laser->mDirY == 1)
            {
                bY = (cellY + 1) * mCellSide;
            }
            else
            {
                bY = cellY * mCellSide;
            }

            float bXY = (cX * laser->mM) + laser->mB;
            float bYX = (cY - laser->mB) / laser->mM;

            float deltaX1 = cX - bX;
            float deltaY1 = cY - bXY;
            float deltaX2 = cX - bYX;
            float deltaY2 = cY - bY;

            float dis1 = (deltaX1 * deltaX1) + (deltaY1 * deltaY1);
            float dis2 = (deltaX2 * deltaX2) + (deltaY2 * deltaY2);

            if (dis1 >= dis2)
            {
                cX = bX;
                cY = bXY;
                cellX += laser->mDirX;
            }
            else
            {
                cX = bYX;
                cY = bY;
                cellY += laser->mDirY;
            }
        }

        if ((laser->mX1 > mWorldWidth)
            || (laser->mX2 > mWorldWidth)
            || (laser->mY1 > mWorldLength)
            || (laser->mY2 > mWorldLength)
            || (laser->mX1 < 0)
            || (laser->mX2 < 0)
            || (laser->mY1 < 0)
            || (laser->mY2 < 0))
        {
            mLaserShots.erase(iterLaser);
        }
    }
}

void Sim2D::drawLaserShots()
{
    art_setColor(255, 255, 255, 255);
    art_setLineWidth(1.0f);

    for (list<Laser>::iterator iterLaser = mLaserShots.begin();
            iterLaser != mLaserShots.end();
            iterLaser++)
    {
        Laser* laser = &(*iterLaser);
        art_drawLine(laser->mX1, laser->mY1, laser->mX2, laser->mY2);
    }
}

void Sim2D::createLaserShot(float x1, float y1, float ang, float length, float speed, llULINT ownerID)
{
    mLaserShots.push_back(Laser());
    Laser* laser = &(mLaserShots.back());
    laser->mX1 = x1;
    laser->mY1 = y1;
    laser->mAng = ang;
    laser->mLength = length;
    laser->mSpeed = speed;
    laser->mX2 = x1 + (cosf(ang) * length);
    laser->mY2 = y1 + (sinf(ang) * length);
    laser->mM = tanf(ang);
    laser->mB = y1 - (laser->mM * x1);

    laser->mDirX = 1;
    if (x1 < laser->mX2)
    {
        laser->mDirX = -1;
    }
    laser->mDirY = 1;
    if (y1 < laser->mY2)
    {
        laser->mDirY = -1;
    }

    laser->mOwnerID = ownerID;
}

bool Sim2D::onKeyDown(Art_KeyCode key)
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
    case ART_KEY_SPACE:
        mHumanFire = true;
        return true;
    default:
        return false;
    }
}

bool Sim2D::onKeyUp(Art_KeyCode key)
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
    case ART_KEY_SPACE:
        mHumanFire = false;
        return true;
    default:
        return false;
    }
}

bool Sim2D::onMouseButtonDown(Art_MouseButton button, int x, int y)
{
    mDragging = true;
    mLastMouseX = x;
    mLastMouseY = y;
    return false;
}

bool Sim2D::onMouseButtonUp(Art_MouseButton button, int x, int y)
{
    mDragging = false;
    return false;
}

bool Sim2D::onMouseMove(int x, int y)
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

bool Sim2D::onMouseWheel(bool up)
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

void Sim2D::setViewAngle(float angle)
{
    mViewAngle = (angle * M_PI) / 180.0f;
    mHalfViewAngle = mViewAngle / 2.0f;
}

void Sim2D::setViewRange(float range)
{
    mViewRange = range;
}

float Sim2D::normalizeAngle(float angle)
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

string Sim2D::getInterfaceName(bool input, int type)
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
        case PERCEPTION_ORIENTATION:
            return "orientation";
        case PERCEPTION_DISTANCE:
            return "distance";
        case PERCEPTION_ENERGY:
            return "energy";
        case PERCEPTION_CAN_SPEAK:
            return "canspeak";
        case PERCEPTION_CAN_FIRE:
            return "canfire";
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
        case ACTION_FIRE:
            return "fire";
        default:
            return "?";
        }
    }
}

void Sim2D::speak(SimObj2D* agent, Symbol* sym)
{
    if (((mSimulationTime - agent->mLastSpeakTime) <= mSpeakInterval)
        && (agent->mLastSpeakTime != 0))
    {
        return;
    }

    agent->mLastSpeakTime = mSimulationTime;

    startCollisionDetection(agent->mX, agent->mY, mSoundRange);

    SimObj* target;
    float distance;
    float angle;

    while (target = nextCollision(distance, angle))
    {
        if ((target->mType == SimObj::TYPE_AGENT)
            && (agent != target))
        {
            Message* msg = new Message();
            msg->mSymbol = sym->clone();
            msg->mType = 0; // Only one message type for now (sound message)
            float* msgData = (float*)malloc(2 * sizeof(float));
            msgData[0] = distance;
            msgData[1] = normalizeAngle(angle + M_PI);
            msg->mData = msgData;
            target->addMessage(msg);
        }
    }

    VisualEvent* ve = (VisualEvent*)malloc(sizeof(VisualEvent));
    ve->mType = VE_SPEAK;
    ve->mX = agent->mX;
    ve->mY = agent->mY;
    ve->mRed = sym->getRed();
    ve->mGreen = sym->getGreen();
    ve->mBlue = sym->getBlue();
    ve->mStartTime = mSimulationTime;
    ve->mEndTime = mSimulationTime + mSpeakInterval;
    mVisualEvents.push_back(ve);
}

const char Sim2D::mClassName[] = "Sim2D";

Orbit<Sim2D>::MethodType Sim2D::mMethods[] = {
    {"addObject", &Simulation::addObject},
    {"setSeedIndex", &Simulation::setSeedIndex},
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"initGraphics", &Simulation::initGraphics},
    {"run", &Simulation::run},
    {"setTimeLimit", &Simulation::setTimeLimit},
    {"setWorldDimensions", &Sim2D::setWorldDimensions},
    {"setViewRange", &Sim2D::setViewRange},
    {"setViewAngle", &Sim2D::setViewAngle},
    {"setGoCost", &Sim2D::setGoCost},
    {"setRotateCost", &Sim2D::setRotateCost},
    {"setGoForceScale", &Sim2D::setGoForceScale},
    {"setRotateForceScale", &Sim2D::setRotateForceScale},
    {"setPos", &Sim2D::setPos},
    {"setRot", &Sim2D::setRot},
    {"setHuman", &Sim2D::setHuman},
    {"setFeedCenter", &Sim2D::setFeedCenter},
    {"setSoundRange", &Sim2D::setSoundRange},
    {"setSpeakInterval", &Sim2D::setSpeakInterval},
    {"setFireInterval", &Sim2D::setFireInterval},
    {0,0}
};

Orbit<Sim2D>::NumberGlobalType Sim2D::mNumberGlobals[] = {
    {"PERCEPTION_NULL", PERCEPTION_NULL},
    {"PERCEPTION_POSITION", PERCEPTION_POSITION},
    {"PERCEPTION_ORIENTATION", PERCEPTION_ORIENTATION},
    {"PERCEPTION_DISTANCE", PERCEPTION_DISTANCE},
    {"PERCEPTION_TARGET", PERCEPTION_TARGET},
    {"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
    {"PERCEPTION_SYMBOL", PERCEPTION_SYMBOL},
    {"PERCEPTION_ENERGY", PERCEPTION_ENERGY},
    {"PERCEPTION_CAN_SPEAK", PERCEPTION_CAN_SPEAK},
    {"PERCEPTION_CAN_FIRE", PERCEPTION_CAN_FIRE},
    {"ACTION_NULL", ACTION_NULL},
    {"ACTION_GO", ACTION_GO},
    {"ACTION_ROTATE", ACTION_ROTATE},
    {"ACTION_EAT", ACTION_EAT},
    {"ACTION_EATB", ACTION_EATB},
    {"ACTION_SPEAK", ACTION_SPEAK},
    {"ACTION_FIRE", ACTION_FIRE},
    {"FITNESS_ENERGY", FITNESS_ENERGY},
    {"FITNESS_ENERGY_SUM", FITNESS_ENERGY_SUM},
    {"FITNESS_ENERGY_SUM_ABOVE_INIT", FITNESS_ENERGY_SUM_ABOVE_INIT},
    {"FITNESS_RANDOM", FITNESS_RANDOM},
    {0,0}
};

int Sim2D::setWorldDimensions(lua_State* luaState)
{
    float width = luaL_checknumber(luaState, 1);
    float height = luaL_checknumber(luaState, 2);
    int cellSide = luaL_checkint(luaState, 3);
    setWorldDimensions(width, height, cellSide);
    return 0;
}

int Sim2D::setViewRange(lua_State* luaState)
{
    float viewRange = luaL_checknumber(luaState, 1);
    setViewRange(viewRange);
    return 0;
}

int Sim2D::setViewAngle(lua_State* luaState)
{
    float viewAngle = luaL_checknumber(luaState, 1);
    setViewAngle(viewAngle);
    return 0;
}

int Sim2D::setGoCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setGoCost(cost);
    return 0;
}

int Sim2D::setRotateCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setRotateCost(cost);
    return 0;
}

int Sim2D::setGoForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setGoForceScale(scale);
    return 0;
}

int Sim2D::setRotateForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setRotateForceScale(scale);
    return 0;
}

int Sim2D::setPos(lua_State* luaState)
{
    SimObj2D* simObj = (SimObj2D*)Orbit<Sim2D>::pointer(luaState, 1);
    float x = luaL_checknumber(luaState, 2);
    float y = luaL_checknumber(luaState, 3);
    setPos(simObj, x, y);
    return 0;
}

int Sim2D::setRot(lua_State* luaState)
{
    SimObj2D* simObj = (SimObj2D*)Orbit<Sim2D>::pointer(luaState, 1);
    float rot = luaL_checknumber(luaState, 2);
    setRot(simObj, rot);
    return 0;
}

int Sim2D::setHuman(lua_State* luaState)
{
    SimObj2D* agent = (SimObj2D*)Orbit<Sim2D>::pointer(luaState, 1);
    setHuman(agent);
    return 0;
}

int Sim2D::setFeedCenter(lua_State* luaState)
{
    float center = luaL_checknumber(luaState, 1);
    setFeedCenter(center);
    return 0;
}

int Sim2D::setSoundRange(lua_State* luaState)
{
    float range = luaL_checknumber(luaState, 1);
    setSoundRange(range);
    return 0;
}

int Sim2D::setSpeakInterval(lua_State* luaState)
{
    unsigned int speakInterval = luaL_checkint(luaState, 1);
    setSpeakInterval(speakInterval);
    return 0;
}

int Sim2D::setFireInterval(lua_State* luaState)
{
    unsigned int fireInterval = luaL_checkint(luaState, 1);
    setFireInterval(fireInterval);
    return 0;
}

