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
#include "CompPER.h"
#include "CompACT.h"

#include <stdexcept>

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
    mMaxAgeLow = 0;
    mMaxAgeHigh = 0;
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

    mRed = 0;
    mGreen = 0;
    mBlue = 0;

    mShape = SHAPE_TRIANGLE;
    mColoring = COLORING_SYMBOL_SOLID;
    mColoringSymbolName = "";
    mColoringReferenceSymbol = NULL;
    mColoringScaleRed1 = 0;
    mColoringScaleGreen1 = 0;
    mColoringScaleBlue1 = 0;
    mColoringScaleRed2 = 0;
    mColoringScaleGreen2 = 0;
    mColoringScaleBlue2 = 0;
    mColoringScaleCenter = 0.0f;

    mFireInterval = 0;
    mLaserLength = 0.0f;
    mLaserSpeed = 1.0f;
    mLaserRange = 0.0f;
    mLaserStrengthFactor = 0.0f;
    mLaserCostFactor = 0.0f;
    mLaserHitDuration = 0;

    mFriendlyFire = 0;
    mCurrentLaserTargetID = 0;
    mObjSymAcqCounter = 0;

    mBlockedX = false;
    mBlockedY = false;

    mLastMessageSent = NULL;

    mEnergySum = 0.0f;
    mEnergySumAboveInit = 0.0f;
    mMsgScore = 0.0f;
    mLaserScore = 0.0f;
    mLockScore = 0.0f;
    mCurrentSynchScore = 0.0f;
    mBestSynchScore = 0.0f;

    mLastBodyHit = 0;
    
    mCurrentLaserLockID = 0;
    mTargetLockTime = 0;
}

SimObj2D::SimObj2D(SimObj2D* obj) : SimObj(obj)
{
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
    mMaxAgeLow = obj->mMaxAgeLow;
    mMaxAgeHigh = obj->mMaxAgeHigh;
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

    mRed = 0;
    mGreen = 0;
    mBlue = 0;

    mShape = obj->mShape;
    mColoring = obj->mColoring; 
    mColoringSymbolName = obj->mColoringSymbolName;
    mColoringReferenceSymbol = obj->mColoringReferenceSymbol;
    mColoringScaleRed1 = obj->mColoringScaleRed1;
    mColoringScaleGreen1 = obj->mColoringScaleGreen1;
    mColoringScaleBlue1 = obj->mColoringScaleBlue1;
    mColoringScaleRed2 = obj->mColoringScaleRed2;
    mColoringScaleGreen2 = obj->mColoringScaleGreen2;
    mColoringScaleBlue2 = obj->mColoringScaleBlue2;
    mColoringScaleCenter = obj->mColoringScaleCenter;

    mFireInterval = obj->mFireInterval;
    mLaserLength = obj->mLaserLength;
    mLaserSpeed = obj->mLaserSpeed;
    mLaserRange = obj->mLaserRange;
    mLaserStrengthFactor = obj->mLaserStrengthFactor;
    mLaserCostFactor = obj->mLaserCostFactor;
    mLaserHitDuration = obj->mLaserHitDuration;

    mFriendlyFire = 0;
    mCurrentLaserTargetID = 0;
    mObjSymAcqCounter = 0;

    for (list<SymbolPointer>::iterator iterItem = obj->mObjectSymbolAcquisition.begin();
        iterItem != obj->mObjectSymbolAcquisition.end();
        iterItem++)
    {
        mObjectSymbolAcquisition.push_back(*iterItem);
    }
    for (list<SymbolPointer>::iterator iterItem = obj->mMessageSymbolAcquisition.begin();
        iterItem != obj->mMessageSymbolAcquisition.end();
        iterItem++)
    {
        mMessageSymbolAcquisition.push_back(*iterItem);
    }

    mBlockedX = false;
    mBlockedY = false;

    mLastMessageSent = NULL;

    mEnergySum = 0.0f;
    mEnergySumAboveInit = 0.0f;
    mMsgScore = 0.0f;
    mLaserScore = 0.0f;
    mLockScore = 0.0f;
    mCurrentSynchScore = 0.0f;
    mBestSynchScore = 0.0f;

    mLastBodyHit = 0;

    mCurrentLaserLockID = 0;
    mTargetLockTime = 0;
}

SimObj2D::~SimObj2D()
{
    if (mLastMessageSent != NULL)
    {
        delete mLastMessageSent;
    }
}

SimObj* SimObj2D::clone()
{
    return new SimObj2D(this);
}

void SimObj2D::init()
{
    if (mType == TYPE_AGENT)
    {
        mChannelObjects = mBrain->getChannelByName("objects");
        mChannelSounds = mBrain->getChannelByName("sounds");
        mChannelSelf = mBrain->getChannelByName("self");
        mChannelBeta = mBrain->getChannelByName("beta");
    }

    if (mColoring == COLORING_SYMBOL_SOLID)
    {
        Symbol* color = getSymbolByName(mColoringSymbolName);

        if (color != NULL)
        {
            mRed = color->getRed();
            mGreen = color->getGreen();
            mBlue = color->getBlue();
        }
    }
    else if (mColoring == COLORING_SYMBOL_SCALE)
    {
        Symbol* sym = getSymbolByName(mColoringSymbolName);

        if (sym == NULL)
        {
            throw std::runtime_error("Failed to initialize SimObj2D: object does not define '" + mColoringSymbolName + "' named symbol");
        }

        float binding = mColoringReferenceSymbol->proximity(sym);

        if (binding < mColoringScaleCenter)
        {
            binding /= mColoringScaleCenter * 2.0f;
        }
        else
        {
            binding -= mColoringScaleCenter;
            binding /= (1.0f - mColoringScaleCenter) * 2.0f;
            binding += 0.5f;
        }

        float deltaRed = mColoringScaleRed2 - mColoringScaleRed1;
        float deltaGreen = mColoringScaleGreen2 - mColoringScaleGreen1;
        float deltaBlue = mColoringScaleBlue2 - mColoringScaleBlue1;

        deltaRed *= binding;
        deltaGreen *= binding;
        deltaBlue *= binding;

        mRed = mColoringScaleRed1 + deltaRed;
        mGreen = mColoringScaleGreen1 + deltaGreen;
        mBlue = mColoringScaleBlue1 + deltaBlue;
    }

    if (mShape == SHAPE_SQUARE)
    {
        mShapeSize = sqrtf(2.0 * mSize * mSize);
    }
    else
    {
        mShapeSize = mSize;
    }

    //mLastFireTime = mSim2D->getTime();
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
    art_setColor(mRed, mGreen, mBlue, 255);

    float a1;
    float a2;
    float a3;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;

    switch (mShape)
    {
    case SHAPE_TRIANGLE:
        a1 = mRot;
        a2 = mRot + (M_PI * 0.8f);
        a3 = mRot + (M_PI * 1.2f);
        x1 = mX + (cosf(a1) * mShapeSize);
        y1 = mY + (sinf(a1) * mShapeSize);
        x2 = mX + (cosf(a2) * mShapeSize);
        y2 = mY + (sinf(a2) * mShapeSize);
        x3 = mX + (cosf(a3) * mShapeSize);
        y3 = mY + (sinf(a3) * mShapeSize);

        art_setLineWidth(2.0f);
        art_fillTriangle(x1, y1, x2, y2, x3, y3);
        art_drawTriangle(x1, y1, x2, y2, x3, y3);

        //art_drawLine(mX, mY, mX + (cosf(a1) * 300.0f), mY + (sinf(a1) * 300.0f));

        break;
    case SHAPE_SQUARE:
        art_setRotation(mX, mY, mRot);
        art_fillSquare(mX, mY, mShapeSize);
        art_clearRotation();

        break;
    case SHAPE_CIRCLE:
        art_fillCircle(mX, mY, mShapeSize);
        art_drawCircle(mX, mY, mShapeSize);

        break;
    }
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
    gbULINT simTime = mSim2D->getTime();

    // Process laser hit list
    float totalDamage = 0.0f;

    list<Laser2D>::iterator iterLaser = mLaserHits.begin();
    while (iterLaser != mLaserHits.end())
    {
        unsigned int timeSinceFired = simTime - (*iterLaser).mFireTime;
        if (timeSinceFired > mLaserHitDuration)
        {
            iterLaser = mLaserHits.erase(iterLaser);
        }
        else
        {
            //printf("time since fired %d\n", timeSinceFired);
            totalDamage += (*iterLaser).mEnergy;
            iterLaser++;
        }
    }

    if (totalDamage >= mEnergy)
    {
        for (list<Laser2D>::iterator iterLaser = mLaserHits.begin();
            iterLaser != mLaserHits.end();
            iterLaser++)
        {
            gbULINT id = (*iterLaser).mOwnerID;
            SimObj2D* obj = (SimObj2D*)(mSim2D->getObjectByID(id));

            if (obj != NULL)
            {
                if (obj->mSpeciesID != mSpeciesID)
                {
                    obj->mLaserScore += mEnergy;
                }
            }
        }

        mEnergy = 0.0f;
    }

    // Check for death
    if (mEnergy <= 0)
    {
        mDeathType = DEATH_HARD;
        mSim2D->killOrganism(this);
    }
    else if (mMaxAge > 0)
    {
        if (mSim2D->getTime() - mCreationTime >= mAgeLimit)
        {
            mDeathType = DEATH_EXPIRED;
            mSim2D->killOrganism(this);
        }
    }

    // Update physics
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

    if (fabsf(mSpeedX) > 0)
    {
        mBlockedX = false;
    }
    if (fabsf(mSpeedY) > 0)
    {
        mBlockedY = false;
    }

    if (newX < 0.0f)
    {
        newX = 0.0f;
        mSpeedX = 0.0f;
        mBlockedX = true;
    }
    else if (newX > mSim2D->getWorldWidth())
    {
        newX = mSim2D->getWorldWidth();
        mSpeedX = 0.0f;
        mBlockedX = true;
    }
    if (newY < 0.0f)
    {
        newY = 0.0f;
        mSpeedY = 0.0f;
        mBlockedY = true;
    }
    else if (newY > mSim2D->getWorldLength())
    {
        newY = mSim2D->getWorldLength();
        mSpeedY = 0.0f;
        mBlockedY = true;
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

    // Update scores
    if ((simTime % 10) == 0)
    {
        float energy = mEnergy;
        mEnergySum += energy;
        energy -= mInitialEnergy;
        if (energy < 0.0f)
        {
            energy = 0.0f;
        }
        mEnergySumAboveInit += energy;
    }

    // Update laser data
    if (mType == TYPE_AGENT)
    {
        float distance;
        float x1 = mX;
        float y1 = mY;
        float x2 = mX + (cosf(mRot) * mLaserRange);
        float y2 = mY + (sinf(mRot) * mLaserRange);

        SimObj2D* laserTarget = mSim2D->getLineTarget(x1, y1, x2, y2, mID, distance);

        mCurrentLaserTargetID = 0;
        if (laserTarget != NULL)
        {
            float targRatio = distance / laserTarget->mSize;

            if (targRatio < 0.75f)
            {
                mCurrentLaserTargetID = laserTarget->getID();
            }
        }

        SimObj2D* objLock = (SimObj2D*)mSim2D->getObjectByID(mCurrentLaserLockID);

        if ((objLock != NULL)
            && mSim2D->segmentCollides(objLock, x1, y1, x2, y2))
        {
            mTargetLockTime++;
        }
        else
        {
            mCurrentLaserLockID = mCurrentLaserTargetID;
            mTargetLockTime = 0;
        }
        
        //objLock = (SimObj2D*)mSim2D->getObjectByID(mCurrentLaserLockID);

        //printf("lock: %d\n", mTargetLockTime);
        //printf("laser target: %d\n", mCurrentLaserTargetID);
    }
}

void SimObj2D::updateFitnesses()
{
    for (map<int, Fitness>::iterator iterFit = mFitMap.begin();
            iterFit != mFitMap.end();
            iterFit++)
    {
        int type = (*iterFit).first;
        Fitness* fit = &((*iterFit).second);

        switch (type)
        {
        case FITNESS_ENERGY:
            fit->mFitness = mEnergy;
            break;
        case FITNESS_ENERGY_SUM:
            fit->mFitness = mEnergySum;
            break;
        case FITNESS_ENERGY_SUM_ABOVE_INIT:
            fit->mFitness = mEnergySumAboveInit;
            break;
        case FITNESS_RANDOM:
            fit->mFitness = mDistFitnessRandom->uniform(0.0f, 1.0f);
            break;
        case FITNESS_MSG_SCORE:
            fit->mFitness = mMsgScore;
            break;
        case FITNESS_SYNCH_SCORE:
            fit->mFitness = mBestSynchScore;
            break;
        case FITNESS_LASER_SCORE:
            fit->mFitness = mLaserScore;
            break;
        case FITNESS_LOCK_SCORE:
            fit->mFitness = mLockScore;
            break;
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

            // Message symbol acquisition
            for (list<SymbolPointer>::iterator iterItem = mMessageSymbolAcquisition.begin();
                iterItem != mMessageSymbolAcquisition.end();
                iterItem++)
            {
                SymbolPointer item = *iterItem;
        
                SymbolTable* origTable = getSymbolTable(item.mTable);

                if (origTable != NULL)
                {
                    origTable->acquireSymbol(msg->mSymbol);
                }
            }

            list<Component*>* interface = mBrain->getInputInterface(mChannelSounds);
            float* inBuffer = mBrain->getInputBuffer(mChannelSounds);

            if (inBuffer != NULL)
            {
                unsigned int pos = 0;
                float normalizedValue;

                for (list<Component*>::iterator iterItem = interface->begin();
                    iterItem != interface->end();
                    iterItem++)
                {
                    Interface* intf = (Interface*)(*iterItem);
                    CompPER* per = (CompPER*)(*iterItem);
                    unsigned int type = per->getInputType();

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

                        case Sim2D::PERCEPTION_VALUE:
                            normalizedValue = msg->mData[2];
                            inBuffer[pos] = normalizedValue;
                            break;

                        case Sim2D::PERCEPTION_SYMPRO:
                            normalizedValue = mSim2D->calcSymbolsBinding(this,
                                                    intf->getOrigSymTable(),
                                                    intf->getOrigSymID(),
                                                    msg->mSymbol,
                                                    Simulation::BINDING_PROXIMITY);
                            inBuffer[pos] = normalizedValue;
                            break;

                        case Sim2D::PERCEPTION_SYMEQ:
                            normalizedValue = mSim2D->calcSymbolsBinding(this,
                                                    intf->getOrigSymTable(),
                                                    intf->getOrigSymID(),
                                                    msg->mSymbol,
                                                    Simulation::BINDING_EQUALS);
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
        list<Component*>* interface = mBrain->getInputInterface(mChannelSelf);
        float* inBuffer = mBrain->getInputBuffer(mChannelSelf);
        unsigned int pos = 0;
        float normalizedValue;
        float ratio;

        for (list<Component*>::iterator iterItem = interface->begin();
            iterItem != interface->end();
            iterItem++)
        {
            CompPER* per = (CompPER*)(*iterItem);
            unsigned int type = per->getInputType();

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
                case Sim2D::PERCEPTION_ID:
                    inBuffer[pos] = (float)(mBodyID + 1);
                    break;
                case Sim2D::PERCEPTION_BLOCKED:
                    normalizedValue = 0.0f;

                    if (mBlockedX || mBlockedY)
                    {
                        normalizedValue = 1.0f;
                    }
                    inBuffer[pos] = normalizedValue;
                    break;
                case Sim2D::PERCEPTION_COMPASS:
                    float centerX = mSim2D->getWorldWidth() / 2.0f;
                    float centerY = mSim2D->getWorldLength() / 2.0f;
                    float dX =  centerX - mX;
                    float dY =  centerY - mY;

                    float ltAng = Sim2D::normalizeAngle(mRot - atan2f(dY, dX));
                    normalizedValue = ltAng / M_PI;

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
    // Object symbol acquisition
    mObjSymAcqCounter++;

    if (mObjSymAcqCounter >= 250)
    {
        mObjSymAcqCounter = 0;
        for (list<SymbolPointer>::iterator iterItem = mObjectSymbolAcquisition.begin();
            iterItem != mObjectSymbolAcquisition.end();
            iterItem++)
        {
            SymbolPointer item = *iterItem;

            SymbolTable* origTable = getSymbolTable(item.mTable);
            SymbolTable* targTable = targ->getSymbolTable(item.mTable);

            if ((origTable != NULL)
                && (targTable != NULL))
            {
                Symbol* targetSym = targTable->getReferenceSymbol();
                origTable->acquireSymbol(targetSym);
            }
        }
    }

    //printf("bodyID: %d\n", targ->mBodyID);
    float* inBuffer = mBrain->getInputBuffer(mChannelObjects, targ->mBodyID);
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
    float convX;
    float convY;
    float convAngle;
    float dX;
    float dY;
    float ltAng;

    list<Component*>* interface = mBrain->getInputInterface(mChannelObjects);
    unsigned int pos = 0;

    for (list<Component*>::iterator iterItem = interface->begin();
        iterItem != interface->end();
        iterItem++)
    {
        CompPER* per = (CompPER*)(*iterItem);
        Interface* intf = (Interface*)per;
        unsigned int type = per->getInputType();

        //printf("type: %d\n", type);

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
    
            case Sim2D::PERCEPTION_SIZE:
                normalizedValue = targ->mSize / mSize;
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

            case Sim2D::PERCEPTION_SYMPRO:
                normalizedValue = mSim2D->calcSymbolsBinding(this,
                                                    targ,
                                                    intf->getOrigSymTable(),
                                                    intf->getTargetSymTable(),
                                                    intf->getOrigSymID(),
                                                    Simulation::BINDING_PROXIMITY);
                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_SYMEQ:
                normalizedValue = mSim2D->calcSymbolsBinding(this,
                                                    targ,
                                                    intf->getOrigSymTable(),
                                                    intf->getTargetSymTable(),
                                                    intf->getOrigSymID(),
                                                    Simulation::BINDING_EQUALS);

                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_LTARGET:
                if (mCurrentLaserTargetID == targ->mID)
                {
                    inBuffer[pos] = 1.0f;
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case Sim2D::PERCEPTION_LOF:
                if (targ->mCurrentLaserTargetID == mID)
                {
                    inBuffer[pos] = 1.0f;
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case Sim2D::PERCEPTION_CONV:
                if ((mCurrentLaserTargetID != 0)
                    && (mCurrentLaserTargetID == targ->mCurrentLaserTargetID))
                {
                    inBuffer[pos] = 1.0f;
                }
                else
                {
                    inBuffer[pos] = 0.0f;
                }
                break;

            case Sim2D::PERCEPTION_CONVDIR:
                if (targ->mCurrentLaserTargetID != 0)
                {
                    SimObj2D* lTargObj = (SimObj2D*)mSim2D->getObjectByID(targ->mCurrentLaserTargetID);

                    float dX =  lTargObj->mX - mX;
                    float dY =  lTargObj->mY - mY;

                    float ltAng = Sim2D::normalizeAngle(mRot - atan2f(dY, dX));
                    normalizedValue = ltAng / M_PI;
                }
                else
                {
                    convX = targ->mX + cosf(targ->mRot) * mLaserRange;
                    convY = targ->mY + sinf(targ->mRot) * mLaserRange;
                    convAngle = atan2f(convY - mY, convX - mX);
                    normalizedValue = Sim2D::normalizeAngle(mRot - convAngle) / M_PI;
                }

                inBuffer[pos] = normalizedValue;
                break;

            case Sim2D::PERCEPTION_ID:
                inBuffer[pos] = (float)(targ->mBodyID + 1);
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
    int actionFire = Sim2D::ACTION_NULL;
    int actionEat = Sim2D::ACTION_NULL;
    Symbol* actionSpeakSymbol = NULL;
    float actionGoParam = 0.0f;
    float actionRotateParam = 0.0f;
    float actionSpeakParam = -99999999.9f;
    float actionFireParam = 0.0f;

    if (mHumanControlled)
    {
        if (mHumanGo)
        {
            actionGo = true;
            actionGoParam = 0.1f;
        }
        if (mHumanRotateLeft)
        {
            actionRotate = true;
            actionRotateParam += 0.1f;
        }
        if (mHumanRotateRight)
        {
            actionRotate = true;
            actionRotateParam -= 0.1f;
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
            actionFire = Sim2D::ACTION_FIREB;
            actionFireParam = 1.0f;
        }
    }
    else
    {
        list<Component*>* interface = mBrain->getOutputInterface();
        float* outBuffer = mBrain->getOutputBuffer();
        unsigned int pos = 0;

        for (list<Component*>::iterator iterItem = interface->begin();
            iterItem != interface->end();
            iterItem++)
        {
            float output = outBuffer[pos];

            CompACT* act = (CompACT*)(*iterItem);
            Interface* intf = (Interface*)act;
            int actionType = act->getOutputType();

            if (output != 0.0f)
            {
                SymbolTable* table;

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
                        table = getSymbolTable(intf->getOrigSymTable());

                        if (table != NULL)
                        {
                            Symbol* sym = table->getSymbol(intf->getOrigSymID());
                            //printf("id: %d\n", (*iterItem)->mOrigSymID);

                            if (sym != NULL)
                            {
                                actionSpeakParam = output;
                                actionSpeakSymbol = sym;
                            }
                        }
                        break;
                    case Sim2D::ACTION_FIRE:
                    case Sim2D::ACTION_FIREB:
                        actionFire = actionType;
                        actionFireParam += output;
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

    actionFireParam = fabsf(actionFireParam);
    if (actionFireParam > 1.0f)
    {
        actionFireParam = 1.0f;
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
        speak(actionSpeakSymbol, actionSpeakParam);
    }
    if (actionFire != Sim2D::ACTION_NULL)
    {
        fire(actionFire, actionFireParam);
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
            if (sym1->proximity(sym2) > mFeedCenter)
            {
                float energy = target->mEnergy;
                deltaEnergy(energy);
                target->deltaEnergy(-energy);
            }
            break;
        case Sim2D::ACTION_EATB:
            float distance = sym1->proximity(sym2);
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

void SimObj2D::fire(unsigned int actionType, float strength)
{
    float cost = mLaserCostFactor * strength;
    deltaEnergy(-cost);

    if (((mSim2D->getTime() - mLastFireTime) <= mFireInterval)
        && (mLastFireTime != 0))
    {
        return;
    }

    mLastFireTime = mSim2D->getTime();

    Laser2D laser;

    laser.mFireTime = mLastFireTime;

    laser.mX1 = mX;
    laser.mY1 = mY;
    laser.mAng = mRot;
    laser.mLength = mLaserLength;
    laser.mSpeed = mLaserSpeed;
    laser.mX2 = mX + (cosf(mRot) * mLaserLength);
    laser.mY2 = mY + (sinf(mRot) * mLaserLength);
    laser.mM = tanf(mRot);
    laser.mB = mY - (laser.mM * mX);

    laser.mDirX = 1;
    if (mX < laser.mX2)
    {
        laser.mDirX = -1;
    }
    laser.mDirY = 1;
    if (mY < laser.mY2)
    {
        laser.mDirY = -1;
    }

    laser.mOwnerID = mID;
    laser.mOwnerSpecies = mSpeciesID;

    switch (actionType)
    {
    case Sim2D::ACTION_FIREB:
        laser.mType = Laser2D::LASER_COMULATIVE;
        break;
    case Sim2D::ACTION_FIRE:
    default:
        laser.mType = Laser2D::LASER_ONE_HIT;
        break;
    }

    float maxTime = (float)mMaxAgeHigh;
    float lockTime = (float)mTargetLockTime;

    if (lockTime > maxTime)
    {
        lockTime = maxTime;
    }
    float lockFactor = lockTime / maxTime;

    laser.mEnergy = strength * mLaserStrengthFactor * lockFactor;

    laser.mRange = mLaserRange;
    laser.mDistanceTraveled = 0.0f;

    mSim2D->fireLaser(laser);
}

void SimObj2D::speak(Symbol* sym, float param)
{
    if (((mSim2D->getTime() - mLastSpeakTime) <= mSpeakInterval)
        && (mLastSpeakTime != 0))
    {
        mLastSpeakTime = mSim2D->getTime();
        return;
    }

    mMsgScore += 1.0f;

    mCurrentSynchScore = 1.0f;
    if (mCurrentSynchScore > mBestSynchScore)
    {
        mBestSynchScore = mCurrentSynchScore;
    }
    
    mLastSpeakTime = mSim2D->getTime();

    if (mLastMessageSent != NULL)
    {
        delete mLastMessageSent;
    }
    mLastMessageSent = new Message(3);
    mLastMessageSent->mSymbol = sym->clone();
    mLastMessageSent->mData[0] = 0;
    mLastMessageSent->mData[1] = 0;
    mLastMessageSent->mData[2] = param;
    mLastMessageSent->mTime = mLastSpeakTime;

    mSim2D->startCollisionDetection(mX, mY, mSoundRange);

    SimObj* target;
    float distance;
    float angle;

    while (target = mSim2D->nextCollision(distance, angle))
    {
        if ((target->mType == SimObj::TYPE_AGENT)
            && (target != this))
        {
            Message* msg = new Message(mLastMessageSent);
            msg->mData[0] = distance;
            msg->mData[1] = Sim2D::normalizeAngle(angle + M_PI);
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

void SimObj2D::addMessage(Message* msg)
{
    SimObj::addMessage(msg);

    if ((mLastMessageSent != NULL)
        && ((mSim2D->getTime() - mLastMessageSent->mTime) <= 5)
        && (mLastMessageSent->mSymbol->equals(msg->mSymbol)))
    {
        mMsgScore += 1.0f;

        mCurrentSynchScore += 1.0f;
        if (mCurrentSynchScore > mBestSynchScore)
        {
            mBestSynchScore = mCurrentSynchScore;
        }
    }
}

void SimObj2D::processLaserHit(Laser2D* laser)
{
    gbULINT id = laser->mOwnerID;
    float score;
    SimObj2D* obj = (SimObj2D*)(mSim2D->getObjectByID(id));
    if ((obj != NULL) && (laser->mEnergy > 0))
    {
        score = laser->mEnergy * 0.1f;

        //printf("score: %f\n", score);
        if (obj->mSpeciesID != mSpeciesID)
        {
            obj->mLaserScore += score;
        }
    }

    switch (laser->mType)
    {
    case Laser2D::LASER_COMULATIVE:
        for (list<Laser2D>::iterator iterLaser = mLaserHits.begin();
            iterLaser != mLaserHits.end();
            iterLaser++)
        {
            if (((*iterLaser).mOwnerID) == id)
            {
                return;
            }

            // Track friendly fire
            if (((*iterLaser).mOwnerSpecies) == mSpeciesID)
            {
                mFriendlyFire++;
            }
        }
        mLaserHits.push_back(*laser);
        
        //unsigned int timeSinceFired = mSim2D->getTime() - laser->mFireTime;
        //printf("laser delay: %d\n", timeSinceFired);
        break;
    case Laser2D::LASER_ONE_HIT:
    default:
        score = laser->mEnergy;
        if (score > mEnergy)
        {
            score = mEnergy;
        }

        if (obj != NULL)
        {
            if (obj->mSpeciesID != mSpeciesID)
            {
                obj->mLaserScore += score;
            }
        }

        deltaEnergy(-laser->mEnergy);
        break;
    }

    Sim2D::VisualEvent* ve = (Sim2D::VisualEvent*)malloc(sizeof(Sim2D::VisualEvent));
    ve->mType = Sim2D::VE_LASER;
    ve->mX = mX;
    ve->mY = mY;
    ve->mRadius = 10.0f;
    ve->mRed = 255;
    ve->mGreen = 0;
    ve->mBlue = 0;
    ve->mStartTime = mSim2D->getTime();
    ve->mEndTime = mSim2D->getTime() + 1000;
    mSim2D->mVisualEvents.push_back(ve);
}

void SimObj2D::setColoringScale(string symbolName,
                                Symbol* referenceSymbol,
                                float scaleCenter,
                                int r1,
                                int g1,
                                int b1,
                                int r2,
                                int g2,
                                int b2)
{
    mColoring = COLORING_SYMBOL_SCALE;
    mColoringSymbolName = symbolName;
    mColoringReferenceSymbol = referenceSymbol;
    mColoringScaleCenter = scaleCenter;
    mColoringScaleRed1 = r1;
    mColoringScaleGreen1 = g1;
    mColoringScaleBlue1 = b1;
    mColoringScaleRed2 = r2;
    mColoringScaleGreen2 = g2;
    mColoringScaleBlue2 = b2;
}

bool SimObj2D::getFieldValue(string fieldName, float& value)
{
    if (fieldName == "friendly_fire")
    {
        value = (float)mFriendlyFire;
        return true;
    }
    else
    {
        return SimObj::getFieldValue(fieldName, value);
    }
}

void SimObj2D::addObjectSymbolAcquisition(int table)
{
    SymbolPointer item;
    item.mTable = table;
    mObjectSymbolAcquisition.push_back(item);
}

void SimObj2D::addMessageSymbolAcquisition(int table)
{
    SymbolPointer item;
    item.mTable = table;
    mObjectSymbolAcquisition.push_back(item);
}

void SimObj2D::setMaxAge(gbULINT maxAgeLow, gbULINT maxAgeHigh)
{
    mMaxAgeLow = maxAgeLow;

    if (maxAgeHigh == 0)
    {
        mMaxAgeHigh = maxAgeLow;
    }
    else
    {
        mMaxAgeHigh = maxAgeHigh;
    }
}

const char SimObj2D::mClassName[] = "SimObj2D";

Orbit<SimObj2D>::MethodType SimObj2D::mMethods[] = {
    {"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
    {"setBrain", &SimObj::setBrain},
    {"setKeepBodyOnHardDeath", &SimObj::setKeepBodyOnHardDeath},
    {"setKeepBodyOnExpirationDeath", &SimObj::setKeepBodyOnExpirationDeath},
    {"setPos", &SimObj2D::setPos},
    {"setRot", &SimObj2D::setRot},
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
    {"setShape", &SimObj2D::setShape},
    {"setColoring", &SimObj2D::setColoring},
    {"setColoringSymbolName", &SimObj2D::setColoringSymbolName},
    {"setColoringScale", &SimObj2D::setColoringScale},
    {"setFireInterval", &SimObj2D::setFireInterval},
    {"setLaserLength", &SimObj2D::setLaserLength},
    {"setLaserSpeed", &SimObj2D::setLaserSpeed},
    {"setLaserRange", &SimObj2D::setLaserRange},
    {"setLaserStrengthFactor", &SimObj2D::setLaserStrengthFactor},
    {"setLaserCostFactor", &SimObj2D::setLaserCostFactor},
    {"setLaserHitDuration", &SimObj2D::setLaserHitDuration},
    {"addObjectSymbolAcquisition", &SimObj2D::addObjectSymbolAcquisition},
    {"addMessageSymbolAcquisition", &SimObj2D::addMessageSymbolAcquisition},
    {0,0}
};

Orbit<SimObj2D>::NumberGlobalType SimObj2D::mNumberGlobals[] = {
    {"FITNESS_ENERGY", FITNESS_ENERGY},
    {"FITNESS_ENERGY_SUM", FITNESS_ENERGY_SUM},
    {"FITNESS_ENERGY_SUM_ABOVE_INIT", FITNESS_ENERGY_SUM_ABOVE_INIT},
    {"FITNESS_MSG_SCORE", FITNESS_MSG_SCORE},
    {"FITNESS_SYNCH_SCORE", FITNESS_SYNCH_SCORE},
    {"FITNESS_LASER_SCORE", FITNESS_LASER_SCORE},
    {"FITNESS_LOCK_SCORE", FITNESS_LOCK_SCORE},
    {"FITNESS_RANDOM", FITNESS_RANDOM},
    {"SHAPE_TRIANGLE", SHAPE_TRIANGLE},
    {"SHAPE_SQUARE", SHAPE_SQUARE},
    {"SHAPE_CIRCLE", SHAPE_CIRCLE},
    {"COLORING_SYMBOL_SOLID", COLORING_SYMBOL_SOLID},
    {"COLORING_SYMBOL_SCALE", COLORING_SYMBOL_SCALE},
    {0, 0}};
 

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
    gbULINT maxAgeLow = luaL_checkint(luaState, 1);
    gbULINT maxAgeHigh = luaL_optint(luaState, 2, 0);
    setMaxAge(maxAgeLow, maxAgeHigh);
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

int SimObj2D::setShape(lua_State* luaState)
{
    int shape = luaL_checkint(luaState, 1);
    setShape(shape);
    return 0;
}

int SimObj2D::setColoring(lua_State* luaState)
{
    int coloring = luaL_checkint(luaState, 1);
    setShape(coloring);
    return 0;
}

int SimObj2D::setColoringSymbolName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    setColoringSymbolName(name);
    return 0;
}

int SimObj2D::setColoringScale(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    Symbol* refSym = (Symbol*)Orbit<SimObj2D>::pointer(luaState, 2);
    float scaleCenter = luaL_checknumber(luaState, 3);
    int r1 = luaL_checkint(luaState, 4);
    int g1 = luaL_checkint(luaState, 5);
    int b1 = luaL_checkint(luaState, 6);
    int r2 = luaL_checkint(luaState, 7);
    int g2 = luaL_checkint(luaState, 8);
    int b2 = luaL_checkint(luaState, 9);
    setColoringScale(name, refSym, scaleCenter, r1, g1, b1, r2, g2, b2);
    return 0;
}

int SimObj2D::setFireInterval(lua_State* luaState)
{
    unsigned int interval = luaL_checkint(luaState, 1);
    setFireInterval(interval);
    return 0;
}

int SimObj2D::setLaserLength(lua_State* luaState)
{
    float length = luaL_checknumber(luaState, 1);
    setLaserLength(length);
    return 0;
}

int SimObj2D::setLaserSpeed(lua_State* luaState)
{
    float speed = luaL_checknumber(luaState, 1);
    setLaserSpeed(speed);
    return 0;
}

int SimObj2D::setLaserRange(lua_State* luaState)
{
    float range = luaL_checknumber(luaState, 1);
    setLaserRange(range);
    return 0;
}

int SimObj2D::setLaserStrengthFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setLaserStrengthFactor(factor);
    return 0;
}

int SimObj2D::setLaserCostFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setLaserCostFactor(factor);
    return 0;
}

int SimObj2D::setLaserHitDuration(lua_State* luaState)
{
    unsigned int duration = luaL_checkint(luaState, 1);
    setLaserHitDuration(duration);
    return 0;
}

int SimObj2D::addObjectSymbolAcquisition(lua_State* luaState)
{
    int table = luaL_checkint(luaState, 1);
    addObjectSymbolAcquisition(table);
    return 0;
}

int SimObj2D::addMessageSymbolAcquisition(lua_State* luaState)
{
    int table = luaL_checkint(luaState, 1);
    addMessageSymbolAcquisition(table);
    return 0;
}

