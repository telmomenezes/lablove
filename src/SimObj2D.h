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
#include "Graphic2D.h"
#include "Orbit.h"

#include <list>

using std::list;

class Sim2D;

class SimObj2D : public SimObj
{
public:
    SimObj2D(lua_State* luaState=NULL);
    SimObj2D(SimObj2D* obj);
    virtual ~SimObj2D();
    virtual SimObj* clone();

    virtual void process();
    virtual void perceive();
    virtual void act();

    void setPos(float x, float y);
    void setRot(float rot);

    void draw();

    void addGraphic(Graphic2D* graph);

    void setSize(float size);
    void setDrag(float drag){mDrag = drag;}
    void setRotDrag(float rotDrag){mRotDrag = rotDrag;}
    void setInitialEnergy(float energy){mInitialEnergy = energy;}
    void setMaxAge(llULINT maxAge){mMaxAge = maxAge;}

    void deltaEnergy(double delta);

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
    void setFireInterval(unsigned int interval){mFireInterval = interval;}

    list<Graphic2D*> mGraphics;

    static const char mClassName[];
    static Orbit<SimObj2D>::MethodType mMethods[];
    static Orbit<SimObj2D>::NumberGlobalType mNumberGlobals[];

    int setPos(lua_State* luaState);
    int setRot(lua_State* luaState);
    int addGraphic(lua_State* luaState);
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
    int setFireInterval(lua_State* luaState);

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
    unsigned int mFireInterval;

    llULINT mCollisionDetectionIteration;
    llULINT mLastSpeakTime;
    llULINT mLastFireTime;

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

protected:
    virtual void onScanObject(SimObj2D* targ,
                                float distance,
                                float angle,
                                float orientation);
    void go(float force);
    void rotate(float force);
    void eat(SimObj2D* target, unsigned int actionType);
    void speak(Symbol* sym);
    void fire();

    static mt_distribution* mDistFitnessRandom;

    SimObj2D* mTargetObject;
    float mDistanceToTargetObject;
    float* mCurrentTargetInputBuffer;
};
#endif

