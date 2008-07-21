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

#if !defined(__INCLUDE_SIM_OBJ_2D_H)
#define __INCLUDE_SIM_OBJ_2D_H

#include "SimObj.h"
#include "Orbit.h"
#include "Laser2D.h"

#include <list>

using std::list;

class Sim2D;

class SimObj2D : public SimObj
{
public:
    static const int FITNESS_ENERGY = 0;
    static const int FITNESS_ENERGY_SUM = 1;
    static const int FITNESS_ENERGY_SUM_ABOVE_INIT = 2;
    static const int FITNESS_RANDOM = 3;
    static const int FITNESS_MSG_SCORE = 4;
    static const int FITNESS_SYNCH_SCORE = 5;
    static const int FITNESS_LASER_HIT_SCORE = 6;
    static const int FITNESS_LASER_EFF_SCORE = 7;

    static const int SHAPE_TRIANGLE = 0;
    static const int SHAPE_SQUARE = 1;
    static const int SHAPE_CIRCLE = 2;

    static const int COLORING_SYMBOL_SOLID = 0;
    static const int COLORING_SYMBOL_SCALE = 1;

    SimObj2D(lua_State* luaState=NULL);
    SimObj2D(SimObj2D* obj);
    virtual ~SimObj2D();
    virtual SimObj* clone();

    virtual void init();

    virtual void process();
    virtual void perceive();
    virtual void act();

    void setPos(float x, float y);
    void setRot(float rot);

    virtual void updateFitnesses();

    virtual void draw();

    void setSize(float size);
    void setDrag(float drag){mDrag = drag;}
    void setRotDrag(float rotDrag){mRotDrag = rotDrag;}
    void setInitialEnergy(float energy){mInitialEnergy = energy;}
    void setMaxAge(llULINT maxAgeLow, llULINT maxAgeHigh=0);

    void deltaEnergy(double delta);

    void processLaserHit(Laser2D* laser);

    void setGoCost(float cost){mGoCost = cost;}
    void setRotateCost(float cost){mRotateCost = cost;}
    void setGoForceScale(float scale){mGoForceScale = scale;}
    void setRotateForceScale(float scale){mRotateForceScale = scale;}
    void setFeedCenter(float center){mFeedCenter = center;}
    void setViewRange(float range);
    void setViewAngle(float angle);
    float getViewRange(){return mViewRange;}
    float getViewAngle(){return mViewAngle;}
    void setSoundRange(float range){mSoundRange = range;}
    void setSpeakInterval(unsigned int interval){mSpeakInterval = interval;}
    void setShape(int shape){mShape = shape;}
    void setColoring(int coloring){mColoring = coloring;}
    void setColoringSymbolName(string symbolName){mColoringSymbolName = symbolName;}
    void setColoringScale(string symbolName,
                            Symbol* referenceSymbol,
                            float scaleCenter,
                            int r1,
                            int g1,
                            int b1,
                            int r2,
                            int g2,
                            int b2);

    void setFireInterval(unsigned int interval){mFireInterval = interval;}
    void setLaserLength(float length){mLaserLength = length;}
    void setLaserSpeed(float speed){mLaserSpeed = speed;}
    void setLaserRange(float range){mLaserRange = range;}
    void setLaserStrengthFactor(float factor){mLaserStrengthFactor = factor;}
    void setLaserCostFactor(float factor){mLaserCostFactor = factor;}
    void setLaserHitDuration(unsigned int duration){mLaserHitDuration = duration;}

    virtual bool getFieldValue(string fieldName, float& value);

    void addObjectSymbolAcquisition(int table);
    void addMessageSymbolAcquisition(int table);

    virtual void addMessage(Message* msg);

    static const char mClassName[];
    static Orbit<SimObj2D>::MethodType mMethods[];
    static Orbit<SimObj2D>::NumberGlobalType mNumberGlobals[];

    int setPos(lua_State* luaState);
    int setRot(lua_State* luaState);
    int setSize(lua_State* luaState);
    int setDrag(lua_State* luaState);
    int setRotDrag(lua_State* luaState);
    int setInitialEnergy(lua_State* luaState);
    int setMaxAge(lua_State* luaState);
    int setViewRange(lua_State* luaState);
    int setViewAngle(lua_State* luaState);
    int setGoCost(lua_State* luaState);
    int setRotateCost(lua_State* luaState);
    int setGoForceScale(lua_State* luaState);
    int setRotateForceScale(lua_State* luaState);
    int setFeedCenter(lua_State* luaState);
    int setSoundRange(lua_State* luaState);
    int setSpeakInterval(lua_State* luaState);
    int setShape(lua_State* luaState);
    int setColoring(lua_State* luaState);
    int setColoringSymbolName(lua_State* luaState);
    int setColoringScale(lua_State* luaState);
    int setFireInterval(lua_State* luaState);
    int setLaserLength(lua_State* luaState);
    int setLaserSpeed(lua_State* luaState);
    int setLaserRange(lua_State* luaState);
    int setLaserStrengthFactor(lua_State* luaState);
    int setLaserCostFactor(lua_State* luaState);
    int setLaserHitDuration(lua_State* luaState);
    int addObjectSymbolAcquisition(lua_State* luaState);
    int addMessageSymbolAcquisition(lua_State* luaState);

    float mX;
    float mY;
    float mRot;
    float mSize;
    float mSizeSquared;
    float mSpeedX;
    float mSpeedY;
    float mSpeedRot;
    float mDrag;
    float mRotDrag;
    float mImpulseX;
    float mImpulseY;
    float mImpulseRot;
    float mInitialEnergy;
    float mEnergy;
    llULINT mMaxAge;
    llULINT mMaxAgeLow;
    llULINT mMaxAgeHigh;
    llULINT mAgeLimit;
    float mViewRange;
    float mViewAngle;
    float mHalfViewAngle;
    float mLowLimitViewAngle;
    float mHighLimitViewAngle;
    float mGoCost;
    float mRotateCost;
    float mGoForceScale;
    float mRotateForceScale;
    float mFeedCenter;
    float mSoundRange;
    unsigned int mSpeakInterval;

    llULINT mCollisionDetectionIteration;
    llULINT mLastSpeakTime;

    int mChannelObjects;
    int mChannelSounds;
    int mChannelSelf;
    int mChannelBeta;

    Sim2D* mSim2D;

    bool mHumanControlled;
    bool mHumanGo;
    bool mHumanRotateLeft;
    bool mHumanRotateRight;
    bool mHumanEat;
    bool mHumanSpeak;
    bool mHumanFire;

    llULINT mLastFireTime;

    float mEnergySum;
    float mEnergySumAboveInit;
    float mMsgScore;
    float mLaserHitScore;
    float mLaserEffScore;
    float mCurrentSynchScore;
    float mBestSynchScore;

protected:
    virtual void onScanObject(SimObj2D* targ,
                                float distance,
                                float angle,
                                float orientation);
    void go(float force);
    void rotate(float force);
    void eat(SimObj2D* target, unsigned int actionType);
    void speak(Symbol* sym, float param);
    void fire(unsigned int actionType, float strength);

    static mt_distribution* mDistFitnessRandom;

    SimObj2D* mTargetObject;
    float mDistanceToTargetObject;
    float* mCurrentTargetInputBuffer;

    list<Laser2D> mLaserHits;

    int mRed;
    int mGreen;
    int mBlue;

    int mShape;
    int mColoring;
    string mColoringSymbolName;
    Symbol* mColoringReferenceSymbol;
    int mColoringScaleRed1;
    int mColoringScaleGreen1;
    int mColoringScaleBlue1;
    int mColoringScaleRed2;
    int mColoringScaleGreen2;
    int mColoringScaleBlue2;
    float mColoringScaleCenter;
    float mShapeSize;

    unsigned int mFireInterval;
    float mLaserLength;
    float mLaserSpeed;
    float mLaserRange;
    float mLaserStrengthFactor;
    float mLaserCostFactor;
    unsigned int mLaserHitDuration;
    unsigned int mFriendlyFire;

    llULINT mCurrentLaserTargetID;
    llULINT mCurrentLaserLockID;
    unsigned int mTargetLockTime;

    list<SymbolPointer> mObjectSymbolAcquisition;
    list<SymbolPointer> mMessageSymbolAcquisition;

    unsigned int mObjSymAcqCounter;

    bool mBlockedX;
    bool mBlockedY;

    Message* mLastMessageSent;

    unsigned int mLastBodyHit;
};
#endif

