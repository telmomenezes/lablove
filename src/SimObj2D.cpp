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

#include "SimObj2D.h"
#include "Sim2D.h"
#include "SymbolRGB.h"

mt_distribution* SimObj2D::mDistFitnessRandom = gDistManager.getNewDistribution();

SimObj2D::SimObj2D(lua_State* luaState) : SimObj(luaState)
{
    mX = 0;
    mY = 0;
    mRot = 0;
    mSize = 0;
    mSizeSquared = 0;
    mSpeedX = 0;
    mSpeedY = 0;
    mSpeedRot = 0;
    mDrag = 0;
    mRotDrag = 0;
    mImpulseX = 0;
    mImpulseY = 0;
    mImpulseRot = 0;
    mInitialEnergy = 0;
    mEnergy = 0;
    mMaxAge = 0;
    mAgeLimit = 0;
    mViewRange = 0;
    mViewAngle = 0;
    mHalfViewAngle = 0;
    mLowLimitViewAngle = 0;
    mHighLimitViewAngle = 0;
    mGoCost = 0;
    mRotateCost = 0;
    mGoForceScale = 0;
    mRotateForceScale = 0;
    mFeedCenter = 0;
    mSoundRange = 250.0f;
    mSpeakInterval = 250;
    mFireInterval = 250;

    mCollisionDetectionIteration = 0;
    mLastSpeakTime = 0;
    mLastFireTime = 0;

    mChannelObjects = -1;
    mChannelSounds = -1;
    mChannelSelf = -1;
    mChannelBeta = -1;

    mHumanControlled = false;
    mHumanGo = false;
    mHumanRotateLeft = false;
    mHumanRotateRight = false;
    mHumanEat = false;
    mHumanSpeak = false;
    mHumanFire = false;

    mTargetObject = NULL;
    mDistanceToTargetObject = 0.0f;
    mCurrentTargetInputBuffer = NULL;
}

SimObj2D::SimObj2D(SimObj2D* obj) : SimObj(obj)
{
    for (list<Graphic2D*>::iterator iterGraph = obj->mGraphics.begin();
            iterGraph != obj->mGraphics.end();
            iterGraph++)
    {
        Graphic2D* graph = (*iterGraph)->clone();
        mGraphics.push_back(graph);
    }
    
    mX = 0;
    mY = 0;
    mRot = 0;
    mSize = obj->mSize;
    mSizeSquared = obj->mSizeSquared;
    mSpeedX = 0;
    mSpeedY = 0;
    mSpeedRot = 0;
    mDrag = obj->mDrag;
    mRotDrag = obj->mRotDrag;
    mImpulseX = 0;
    mImpulseY = 0;
    mImpulseRot = 0;
    mInitialEnergy = obj->mInitialEnergy;
    mEnergy = 0;
    mMaxAge = obj->mMaxAge;
    mAgeLimit = 0;
    mViewRange = obj->mViewRange;
    mViewAngle = obj->mViewAngle;
    mHalfViewAngle = obj->mHalfViewAngle;
    mLowLimitViewAngle = obj->mLowLimitViewAngle;
    mHighLimitViewAngle = obj->mHighLimitViewAngle;
    mGoCost = obj->mGoCost;
    mRotateCost = obj->mRotateCost;
    mGoForceScale = obj->mGoForceScale;
    mRotateForceScale = obj->mRotateForceScale;
    mFeedCenter = obj->mFeedCenter;
    mSoundRange = obj->mSoundRange;
    mSpeakInterval = obj->mSpeakInterval;
    mFireInterval = obj->mFireInterval;

    mCollisionDetectionIteration = 0;
    mLastSpeakTime = 0;
    mLastFireTime = 0;

    mChannelObjects = obj->mChannelObjects;
    mChannelSounds = obj->mChannelSounds;
    mChannelSelf = obj->mChannelSelf;
    mChannelBeta = obj->mChannelBeta;

    mHumanControlled = obj->mHumanControlled;
    mHumanGo = false;
    mHumanRotateLeft = false;
    mHumanRotateRight = false;
    mHumanEat = false;
    mHumanSpeak = false;
    mHumanFire = false;

    mTargetObject = NULL;
    mDistanceToTargetObject = 0.0f;
    mCurrentTargetInputBuffer = NULL;
}

SimObj2D::~SimObj2D()
{
    for (list<Graphic2D*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        delete (*iterGraph);
    }
    mGraphics.clear();
}

SimObj* SimObj2D::clone()
{
    return new SimObj2D(this);
}

void SimObj2D::setPos(float x, float y)
{
    mSim2D->updatePos(this, mX, mY, x, y);
    mX = x;
    mY = y;
}

void SimObj2D::setRot(float rot)
{
    mRot = Sim2D::normalizeAngle(rot);
}

void SimObj2D::draw()
{
    for (list<Graphic2D*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        (*iterGraph)->draw();
    }
}

void SimObj2D::addGraphic(Graphic2D* graph)
{
    mGraphics.push_back(graph);
}

void SimObj2D::setSize(float size)
{
    mSize = size;
    mSizeSquared = size * size;
}

void SimObj2D::deltaEnergy(double delta)
{
    mEnergy += delta;
}

void SimObj2D::setViewAngle(float angle)
{
    mViewAngle = (angle * M_PI) / 180.0f;
    mHalfViewAngle = mViewAngle / 2.0f;
}

void SimObj2D::setViewRange(float range)
{
    mViewRange = range;
}

void SimObj2D::process()
{
    if (mEnergy <= 0)
    {
        mSim2D->killOrganism(this);
    }
    else if (mMaxAge > 0)
    {
        if (mSim2D->getTime() - mCreationTime >= mAgeLimit)
        {
            mSim2D->killOrganism(this);
        }
    }

    mSpeedX += mImpulseX / mSize;
    mSpeedY += mImpulseY / mSize;
    mImpulseX = 0.0f;
    mImpulseY = 0.0f;

    float speed = sqrtf((mSpeedX * mSpeedX) + (mSpeedY * mSpeedY));

    float oneMinusDrag = 1 - mDrag;
    
    mSpeedX *= oneMinusDrag;
    mSpeedY *= oneMinusDrag;

    float newX = mX + mSpeedX;
    float newY = mY + mSpeedY;

    if (newX < 0.0f)
    {
        newX = 0.0f;
        mSpeedX = 0.0f;
    }
    else if (newX > mSim2D->getWorldWidth())
    {
        newX = mSim2D->getWorldWidth();
        mSpeedX = 0.0f;
    }
    if (newY < 0.0f)
    {
        newY = 0.0f;
        mSpeedY = 0.0f;
    }
    else if (newY > mSim2D->getWorldLength())
    {
        newY = mSim2D->getWorldLength();
        mSpeedY = 0.0f;
    }

    mSpeedRot += mImpulseRot / mSize;
    mImpulseRot = 0.0f;

    float absRotSpeed = mSpeedRot;
    if (mSpeedRot < 0.0f)
    {
        absRotSpeed = -absRotSpeed;
    }

    float oneMinusRotDrag = 1.0f - mRotDrag;
    mSpeedRot *= oneMinusRotDrag;

    float newRot = mRot + mSpeedRot;

    setPos(newX, newY);
    setRot(newRot);

    // Update fitness
    switch (mFitnessMeasure)
    {
    case Sim2D::FITNESS_ENERGY:
        mFitness = mEnergy;
        break;
    case Sim2D::FITNESS_ENERGY_SUM:
        if ((mSim2D->getTime() % 10) == 0)
        {
            mFitness += mEnergy;
        }
        break;
    case Sim2D::FITNESS_ENERGY_SUM_ABOVE_INIT:
        if ((mSim2D->getTime() % 10) == 0)
        {
            float energy = mEnergy;
            energy -= mInitialEnergy;
            if (energy < 0.0f)
            {
                energy = 0.0f;
            }
            mFitness += energy;
        }
        break;
    case Sim2D::FITNESS_RANDOM:
        if ((mSim2D->getTime() % 1000) == 0)
        {
            mFitness = mDistFitnessRandom->uniform(0.0f, 1.0f);
        }
    }
}

void SimObj2D::perceive()
{
    // Perceive visible objects
    mTargetObject = NULL;
    mDistanceToTargetObject = 9999999999.9f;
    mCurrentTargetInputBuffer = NULL;

    mLowLimitViewAngle = Sim2D::normalizeAngle(mRot - mHalfViewAngle);
    mHighLimitViewAngle = Sim2D::normalizeAngle(mRot + mHalfViewAngle);

    mSim2D->startCollisionDetection(mX, mY, mViewRange);

    SimObj2D* target;
    float distance;
    float angle;

    while (target = mSim2D->nextCollision(distance, angle))
    {
        if (this != target)
        {
            bool visible = false;
            
            // This is an aproximation
            float extraAngle = atan2f(target->mSize, distance);

            distance -= mSize;
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
                onScanObject(target,
                                distance,
                                Sim2D::normalizeAngle(mRot - angle),
                                Sim2D::normalizeAngle(target->mRot - angle));
            }
        }
    }

    // Perceive sounds
    if (mChannelSounds >= 0)
    {
        for (list<Message*>::iterator iterMessage = mMessageList.begin();
                iterMessage != mMessageList.end();
                iterMessage++)
        {
            Message* msg = (*iterMessage);

            list<InterfaceItem*>* interface = mBrain->getInputInterface(mChannelSounds);
            float* inBuffer = mBrain->getInputBuffer(mChannelSounds);

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
                        case Sim2D::PERCEPTION_POSITION:
                            normalizedValue = msg->mData[1] / M_PI;
                            inBuffer[pos] = normalizedValue;
                            break;

                        case Sim2D::PERCEPTION_DISTANCE:
                            normalizedValue = msg->mData[0] / mSoundRange;
                            inBuffer[pos] = normalizedValue;
                            break;

                        case Sim2D::PERCEPTION_SYMBOL:
                            InterfaceItem* item = (*iterItem);
                            normalizedValue = mSim2D->calcSymbolsBinding(this,
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
    if (mChannelSelf >= 0)
    {
        list<InterfaceItem*>* interface = mBrain->getInputInterface(mChannelSelf);
        float* inBuffer = mBrain->getInputBuffer(mChannelSelf);
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
                case Sim2D::PERCEPTION_ENERGY:
                    ratio = mEnergy / mInitialEnergy;
                    normalizedValue = 1.0f - (1.0f / (ratio + 1.0f));
                    inBuffer[pos] = normalizedValue;
                    break;

                case Sim2D::PERCEPTION_CAN_SPEAK:
                    normalizedValue = 0.0f;

                    if ((mSim2D->getTime() - mLastSpeakTime) > mSpeakInterval)
                    {
                        normalizedValue = 1.0f;
                    }
                    inBuffer[pos] = normalizedValue;
                    break;
                case Sim2D::PERCEPTION_CAN_FIRE:
                    normalizedValue = 0.0f;

                    if ((mSim2D->getTime() - mLastFireTime) > mFireInterval)
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

void SimObj2D::onScanObject(SimObj2D* targ,
                float distance,
                float angle,
                float orientation)
{
    float* inBuffer = mBrain->getInputBuffer(mChannelObjects);
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

    list<InterfaceItem*>* interface = mBrain->getInputInterface(mChannelObjects);
    unsigned int pos = 0;

    for (list<InterfaceItem*>::iterator iterItem = interface->begin();
        iterItem != interface->end();
        iterItem++)
    {
        unsigned int type = (*iterItem)->mType;

        switch (type)
        {
            case Sim2D::PERCEPTION_IN_CONTACT:
                if (distance <= 0.0f)
                {
                    inBuffer[pos] = 1.0f;
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case Sim2D::PERCEPTION_POSITION:
                normalizedValue = angle / M_PI;
                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_ORIENTATION:
                normalizedValue = orientation / M_PI;
                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_DISTANCE:
                normalizedValue = distance / mViewRange;
                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_TARGET:
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

            case Sim2D::PERCEPTION_SYMBOL:
                InterfaceItem* item = (*iterItem);
                normalizedValue = mSim2D->calcSymbolsBinding(this,
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

void SimObj2D::act()
{
    bool actionGo = false;
    bool actionRotate = false;
    bool actionFire = false;
    int actionEat = Sim2D::ACTION_NULL;
    Symbol* actionSpeakSymbol = NULL;
    float actionGoParam = 0.0f;
    float actionRotateParam = 0.0f;
    float actionSpeakParam = -99999999.9f;

    if (mHumanControlled)
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
            actionEat = Sim2D::ACTION_EAT;
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
        list<InterfaceItem*>* interface = mBrain->getOutputInterface();
        float* outBuffer = mBrain->getOutputBuffer();
        unsigned int pos = 0;

        for (list<InterfaceItem*>::iterator iterItem = interface->begin();
            iterItem != interface->end();
            iterItem++)
        {
            float output = outBuffer[pos];

            int actionType = (*iterItem)->mType;

            if (output != 0.0f)
            {
                switch (actionType)
                {
                    case Sim2D::ACTION_GO:
                        actionGo = true;
                        actionGoParam += output;
                        break;
                    case Sim2D::ACTION_ROTATE:
                        actionRotate = true;
                        actionRotateParam += output;
                        break;
                    case Sim2D::ACTION_EAT:
                    case Sim2D::ACTION_EATB:
                        actionEat = actionType;
                        break;
                    case Sim2D::ACTION_SPEAK:
                        if (output > actionSpeakParam)
                        {
                            SymbolTable* table = getSymbolTable((*iterItem)->mOrigSymTable);

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
                    case Sim2D::ACTION_FIRE:
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
        go(actionGoParam * mGoForceScale);
    }
    if (actionRotate)
    {
        rotate(actionRotateParam * mRotateForceScale);
    }
    if (actionEat != Sim2D::ACTION_NULL)
    {
        eat(mTargetObject, actionEat);
    }
    if (actionSpeakSymbol != NULL)
    {
        speak(actionSpeakSymbol);
    }
    if (actionFire)
    {
        fire();
    }
}

void SimObj2D::go(float force)
{
    float delta = -mGoCost * fabsf(force);
    deltaEnergy(delta);

    mImpulseX = cosf(mRot) * force;
    mImpulseY = sinf(mRot) * force;
}

void SimObj2D::rotate(float force)
{
    deltaEnergy(-mRotateCost * fabsf(force));
    mImpulseRot -= force;
}

void SimObj2D::eat(SimObj2D* target, unsigned int actionType)
{
    if (target)
    {
        Symbol* sym1 = getSymbolByName("feed");
        if (sym1 == NULL)
        {
            return;
        }
        Symbol* sym2 = target->getSymbolByName("food");
        if (sym2 == NULL)
        {
            return;
        }

        switch (actionType)
        {
        case Sim2D::ACTION_EAT:
            if (sym1->getBinding(sym2) > mFeedCenter)
            {
                float energy = target->mEnergy;
                deltaEnergy(energy);
                target->deltaEnergy(-energy);
            }
            break;
        case Sim2D::ACTION_EATB:
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

            float energy = target->mEnergy;
            deltaEnergy(energyFactor * energy);
            target->deltaEnergy(-energy);
            break;
        }
    }
}

void SimObj2D::fire()
{
    if (((mSim2D->getTime() - mLastFireTime) <= mFireInterval)
        && (mLastFireTime != 0))
    {
        return;
    }

    mLastFireTime = mSim2D->getTime();

    mSim2D->createLaserShot(mX, mY, mRot, 25, 1.0, mID);
}

void SimObj2D::speak(Symbol* sym)
{
    if (((mSim2D->getTime() - mLastSpeakTime) <= mSpeakInterval)
        && (mLastSpeakTime != 0))
    {
        return;
    }

    mLastSpeakTime = mSim2D->getTime();

    mSim2D->startCollisionDetection(mX, mY, mSoundRange);

    SimObj* target;
    float distance;
    float angle;

    while (target = mSim2D->nextCollision(distance, angle))
    {
        if ((target->mType == SimObj::TYPE_AGENT)
            && (target != this))
        {
            Message* msg = new Message();
            msg->mSymbol = sym->clone();
            msg->mType = 0; // Only one message type for now (sound message)
            float* msgData = (float*)malloc(2 * sizeof(float));
            msgData[0] = distance;
            msgData[1] = Sim2D::normalizeAngle(angle + M_PI);
            msg->mData = msgData;
            target->addMessage(msg);
        }
    }

    Sim2D::VisualEvent* ve = (Sim2D::VisualEvent*)malloc(sizeof(Sim2D::VisualEvent));
    ve->mType = Sim2D::VE_SPEAK;
    ve->mX = mX;
    ve->mY = mY;
    ve->mRadius = mSoundRange;
    ve->mRed = sym->getRed();
    ve->mGreen = sym->getGreen();
    ve->mBlue = sym->getBlue();
    ve->mStartTime = mSim2D->getTime();
    ve->mEndTime = mSim2D->getTime() + mSpeakInterval;
    mSim2D->mVisualEvents.push_back(ve);
}

const char SimObj2D::mClassName[] = "SimObj2D";

Orbit<SimObj2D>::MethodType SimObj2D::mMethods[] = {
    {"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
	{"setFitnessMeasure", &SimObj::setFitnessMeasure},
    {"setBrain", &SimObj::setBrain},
    {"setPos", &SimObj2D::setPos},
    {"setRot", &SimObj2D::setRot},
	{"addGraphic", &SimObj2D::addGraphic},
	{"setSize", &SimObj2D::setSize},
	{"setDrag", &SimObj2D::setDrag},
	{"setRotDrag", &SimObj2D::setRotDrag},
	{"setInitialEnergy", &SimObj2D::setInitialEnergy},
	{"setMaxAge", &SimObj2D::setMaxAge},
    {"setViewRange", &SimObj2D::setViewRange},
    {"setViewAngle", &SimObj2D::setViewAngle},
    {"setGoCost", &SimObj2D::setGoCost},
    {"setRotateCost", &SimObj2D::setRotateCost},
    {"setGoForceScale", &SimObj2D::setGoForceScale},
    {"setRotateForceScale", &SimObj2D::setRotateForceScale},
    {"setFeedCenter", &SimObj2D::setFeedCenter},
    {"setSoundRange", &SimObj2D::setSoundRange},
    {"setSpeakInterval", &SimObj2D::setSpeakInterval},
    {"setFireInterval", &SimObj2D::setFireInterval},
    {0,0}
};

Orbit<SimObj2D>::NumberGlobalType SimObj2D::mNumberGlobals[] = {{0,0}};

int SimObj2D::setPos(lua_State* luaState)
{
    float x = luaL_checknumber(luaState, 1);
    float y = luaL_checknumber(luaState, 2);
    setPos(x, y);
    return 0;
}

int SimObj2D::setRot(lua_State* luaState)
{
    float rot = luaL_checknumber(luaState, 1);
    setRot(rot);
    return 0;
}

int SimObj2D::addGraphic(lua_State* luaState)
{
    Graphic2D* graph = (Graphic2D*)Orbit<SimObj2D>::pointer(luaState, 1);
    addGraphic(graph);
    return 0;
}

int SimObj2D::setSize(lua_State* luaState)
{
    float size = luaL_checknumber(luaState, 1);
    setSize(size);
    return 0;
}

int SimObj2D::setDrag(lua_State* luaState)
{
    float drag = luaL_checknumber(luaState, 1);
    setDrag(drag);
    return 0;
}

int SimObj2D::setRotDrag(lua_State* luaState)
{
    float rotDrag = luaL_checknumber(luaState, 1);
    setRotDrag(rotDrag);
    return 0;
}

int SimObj2D::setInitialEnergy(lua_State* luaState)
{
    float energy = luaL_checknumber(luaState, 1);
    setInitialEnergy(energy);
    return 0;
}

int SimObj2D::setMaxAge(lua_State* luaState)
{
    llULINT maxAge = luaL_checkint(luaState, 1);
    setMaxAge(maxAge);
    return 0;
}

int SimObj2D::setViewRange(lua_State* luaState)
{
    float viewRange = luaL_checknumber(luaState, 1);
    setViewRange(viewRange);
    return 0;
}

int SimObj2D::setViewAngle(lua_State* luaState)
{
    float viewAngle = luaL_checknumber(luaState, 1);
    setViewAngle(viewAngle);
    return 0;
}

int SimObj2D::setGoCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setGoCost(cost);
    return 0;
}

int SimObj2D::setRotateCost(lua_State* luaState)
{
    float cost = luaL_checknumber(luaState, 1);
    setRotateCost(cost);
    return 0;
}

int SimObj2D::setGoForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setGoForceScale(scale);
    return 0;
}

int SimObj2D::setRotateForceScale(lua_State* luaState)
{
    float scale = luaL_checknumber(luaState, 1);
    setRotateForceScale(scale);
    return 0;
}

int SimObj2D::setFeedCenter(lua_State* luaState)
{
    float center = luaL_checknumber(luaState, 1);
    setFeedCenter(center);
    return 0;
}

int SimObj2D::setSoundRange(lua_State* luaState)
{
    float range = luaL_checknumber(luaState, 1);
    setSoundRange(range);
    return 0;
}

int SimObj2D::setSpeakInterval(lua_State* luaState)
{
    unsigned int speakInterval = luaL_checkint(luaState, 1);
    setSpeakInterval(speakInterval);
    return 0;
}

int SimObj2D::setFireInterval(lua_State* luaState)
{
    unsigned int fireInterval = luaL_checkint(luaState, 1);
    setFireInterval(fireInterval);
    return 0;
}

