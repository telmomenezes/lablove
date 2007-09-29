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
#include "Lab.h"
#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "functions.h"
#include "defines.h"
#include "random.h"

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

void SimCont2D::init()
{
    Simulation::init();
    mRootLayer2D = Lab::getSingleton().getRootLayer2D();
}

void SimCont2D::setWorldDimensions(float worldWidth,
                                    float worldLength,
                                    unsigned int cellSide)
{
    mWorldWidth = worldWidth;
    mWorldLength = worldLength;
    mCellSide = cellSide;
    mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / mCellSide));
    mWorldCellLength = (unsigned int)(ceilf(mWorldLength / mCellSide));

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

    int targX1 = ((int)(x - size)) / mCellSide;
    int targX2 = ((int)(x + size)) / mCellSide;
    int targY1 = ((int)(y - size)) / mCellSide;
    int targY2 = ((int)(y + size)) / mCellSide;

    obj->mX = x;
    obj->mY = y;

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

void SimCont2D::removeObject(SimulationObject* obj)
{
    float origX = obj->mX;
    float origY = obj->mY;
    float size = obj->mSize;
    int origX1 = ((int)(origX - size)) / mCellSide;
    int origX2 = ((int)(origX + size)) / mCellSide;
    int origY1 = ((int)(origY - size)) / mCellSide;
    int origY2 = ((int)(origY + size)) / mCellSide;

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
    setRot(obj, randomUniformProbability() * M_PI * 2);
}

void SimCont2D::startCollisionDetection(float x, float y, float rad)
{
    mCollisionX = x;
    mCollisionY = y;
    mCollisionRadius = rad;

    mCellX1 = ((int)(x - rad)) / mCellSide;
    mCellX2 = ((int)(x + rad)) / mCellSide;
    mCellY1 = ((int)(y - rad)) / mCellSide;
    mCellY2 = ((int)(y + rad)) / mCellSide;

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
                    mCurrentCellX == mCellX1;
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

        if (obj->mCollisionDetectionIteration != mCollisionDetectionIteration)
        {
            obj->mCollisionDetectionIteration = mCollisionDetectionIteration;

            float dX = mCollisionX - obj->mX;
            float dY = mCollisionY - obj->mY;
            float dist = sqrtf((dX * dX) + (dY * dY));

            if (dist <= mCollisionRadius)
            {
                distance = dist - obj->mSize;
                angle = atan2f(-dY, -dX);
                mCurrentCellListIterator++;
                return obj;
            }
        }

        mCurrentCellListIterator++;
    }
}

void SimCont2D::processObjects()
{
    list<SimulationObject*>::iterator iterObj;

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
    if (orig->mHuman)
    {
        return;
    }

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

            case PERCEPTION_PROXIMITY:
                normalizedValue = 1.0f - (distance / mViewRange);
                inBuffer[pos] = normalizedValue;
                break;

            default:
                //normalizedValue = perception->computeBinding(orig, targ);
                //inBuffer[pos] = normalizedValue;
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

void SimCont2D::goFront(Agent* agent, float distance)
{
    agent->mEnergy -= mGoCost * distance;
    float newX = agent->mX + (cosf(agent->mRotZ) * distance);
    float newY = agent->mY + (sinf(agent->mRotZ) * distance);

    if ((newX < 0)
        || (newY < 0)
        || (newX >= mWorldWidth)
        || (newY >= mWorldLength))
    {
        return;
    }

    setPos(agent, newX, newY);
}

void SimCont2D::rotate(Agent* agent, float angle)
{
    agent->mEnergy -= mRotateCost * angle;
    setRot(agent, agent->mRotZ - angle);
}

void SimCont2D::eat(Agent* agent)
{
    if ((mTargetObject) && (mTargetObject->isFood()))
    {
        agent->deltaEnergy(mTargetObject->getEnergy());
        Lab::getSingleton().getSimulation()->killOrganism(mTargetObject);
    }
}

void SimCont2D::drawBeforeObjects()
{
    if (mShowGrid)
    {
        unsigned int cellSide = (unsigned int)mCellSide;

        int viewX = (int)mViewX;
        int viewY = (int)mViewY;

        mRootLayer2D->setColor(200, 200, 200);

        unsigned int division = cellSide - ((cellSide - viewX) % cellSide);
        while (division < mRootLayer2D->getWidth())
        {
            mRootLayer2D->drawLine(division,
                                    0,
                                    division,
                                    mRootLayer2D->getHeight());

            division += cellSide;
        }

        division = cellSide - ((cellSide - viewY) % cellSide);
        while (division < mRootLayer2D->getHeight())
        {
            mRootLayer2D->drawLine(0,
                                    division,
                                    mRootLayer2D->getWidth(),
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
                mRootLayer2D->fillCircle(obj->mX + mViewX,
                                            obj->mY + mViewY,
                                            mViewRange,
                                            beginAngle,
                                            endAngle);
            }
        }
    }
    
}

void SimCont2D::moveView(float x, float y)
{
    mViewX += x;
    mViewY += y;
}

SimulationObject* Simulation::getObjectByScreenPos(int x, int y)
{
    return NULL;
}

bool SimCont2D::onKeyDown(pyc::KeyCode key)
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

bool SimCont2D::onKeyUp(pyc::KeyCode key)
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

        moveView(deltaX, deltaY);

        return true;
    }

    return false;
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

const char SimCont2D::mClassName[] = "SimCont2D";

Orbit<SimCont2D>::MethodType SimCont2D::mMethods[] = {
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"setWorldDimensions", &SimCont2D::setWorldDimensions},
    {"setViewRange", &SimCont2D::setViewRange},
    {"setViewAngle", &SimCont2D::setViewAngle},
    {"setGoCost", &SimCont2D::setGoCost},
    {"setRotateCost", &SimCont2D::setRotateCost},
    {0,0}
};

Orbit<SimCont2D>::NumberGlobalType SimCont2D::mNumberGlobals[] = {
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

