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

#if !defined(__INCLUDE_SIM_2D_H)
#define __INCLUDE_SIM_2D_H

#include "Simulation.h"
#include "SimObj2D.h"
#include "PopulationDynamics.h"
#include "RandDistManager.h"
#include "Laser.h"

#include "art.h"

#include "Orbit.h"

class Sim2D : public Simulation
{
public:

    enum Perception {PERCEPTION_NULL,
                    PERCEPTION_POSITION,
                    PERCEPTION_ORIENTATION,
                    PERCEPTION_DISTANCE,
                    PERCEPTION_TARGET,
                    PERCEPTION_IN_CONTACT,
                    PERCEPTION_SYMBOL,
                    PERCEPTION_ENERGY,
                    PERCEPTION_CAN_SPEAK,
                    PERCEPTION_CAN_FIRE
                    };

    enum Action {ACTION_NULL,
                ACTION_GO,
                ACTION_ROTATE,
                ACTION_EAT,
                ACTION_EATB,
                ACTION_SPEAK,
                ACTION_FIRE
                };

    enum FitnessMeasure {FITNESS_ENERGY,
                        FITNESS_ENERGY_SUM,
                        FITNESS_ENERGY_SUM_ABOVE_INIT,
                        FITNESS_RANDOM
    };

    enum VisualEventType {VE_SPEAK
    };

    typedef struct
    {
        VisualEventType mType;
        float mX;
        float mY;
        llULINT mStartTime;
        llULINT mEndTime;
        int mRed;
        int mGreen;
        int mBlue;
    } VisualEvent;

    Sim2D(lua_State* luaState=NULL);
    virtual ~Sim2D();

    virtual void initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics=false);

    void setWorldDimensions(float worldWidth,
                            float worldLength,
                            unsigned int cellSide);
    float getWorldWidth(){return mWorldWidth;}
    float getWorldLength(){return mWorldLength;}
    float getCellSide(){return mCellSide;}
    unsigned int getWorldCellWidth(){return mWorldCellWidth;}
    unsigned int getWorldCellLength(){return mWorldCellLength;}
    list<SimObj2D*>** getCellGrid(){return mCellGrid;}

    virtual void addObject(SimObj* obj, bool init=false);
    virtual void removeObject(SimObj* obj, bool deleteObj=true);
    virtual void placeRandom(SimObj* obj);
    virtual void placeNear(SimObj* obj, SimObj* ref);

    virtual void setPos(SimObj2D* obj, float x, float y);
    virtual void setRot(SimObj2D* obj, float rot);

    virtual void drawBeforeObjects();
    virtual void drawAfterObjects();

    void setGoCost(float cost){mGoCost = cost;}
    void setRotateCost(float cost){mRotateCost = cost;}
    void setGoForceScale(float scale){mGoForceScale = scale;}
    void setRotateForceScale(float scale){mRotateForceScale = scale;}

    void setFeedCenter(float center){mFeedCenter = center;}
    
    void moveView(float x, float y);
    float getViewX(){return mViewX;}
    float getViewY(){return mViewY;}

    virtual SimObj* getObjectByScreenPos(int x, int y);

    virtual bool getFieldValue(SimObj* obj, string fieldName, float& value);
    
    virtual bool onKeyDown(Art_KeyCode keycode);
    virtual bool onKeyUp(Art_KeyCode keycode);
    virtual bool onMouseButtonDown(Art_MouseButton button, int x, int y);
    virtual bool onMouseButtonUp(Art_MouseButton button, int x, int y);
    virtual bool onMouseMove(int x, int y);
    virtual bool onMouseWheel(bool up);

    void setViewRange(float range);
    void setViewAngle(float angle);
    float getViewRange(){return mViewRange;}
    float getViewAngle(){return mViewAngle;}

    void setHuman(SimObj2D* agent){mHumanAgent = agent;}

    void startCollisionDetection(float x, float y, float rad);
    SimObj2D* nextCollision(float& distance, float& angle);

    virtual string getInterfaceName(bool input, int type);
    
    void setSoundRange(float range){mSoundRange = range;}
    void setSpeakInterval(unsigned int interval){mSpeakInterval = interval;}
    void setFireInterval(unsigned int interval){mFireInterval = interval;}

    static const char mClassName[];
    static Orbit<Sim2D>::MethodType mMethods[];
    static Orbit<Sim2D>::NumberGlobalType mNumberGlobals[];

    int setWorldDimensions(lua_State* luaState);
    int setViewRange(lua_State* luaState);
    int setViewAngle(lua_State* luaState);
    int setGoCost(lua_State* luaState);
    int setRotateCost(lua_State* luaState);
    int setGoForceScale(lua_State* luaState);
    int setRotateForceScale(lua_State* luaState);
    int setPos(lua_State* luaState);
    int setRot(lua_State* luaState);
    int setHuman(lua_State* luaState);
    int setFeedCenter(lua_State* luaState);
    int setSoundRange(lua_State* luaState);
    int setSpeakInterval(lua_State* luaState);
    int setFireInterval(lua_State* luaState);

protected:
    virtual void onCycle();
    virtual void process(SimObj* object);
    virtual void perceive(SimObj* obj);
    virtual void act(SimObj* agent);
    virtual void onScanObject(SimObj2D* orig,
                                SimObj2D* targ,
                                float distance,
                                float angle,
                                float orientation);
    void go(SimObj2D* agent, float force);
    void rotate(SimObj2D* agent, float force);
    void eat(SimObj2D* agent, Action actionType);
    float normalizeAngle(float angle);

    void speak(SimObj2D* agent, Symbol* sym);
    void fire(SimObj2D* agent);

    virtual void drawTerrain();

    void processLaserShots();
    void drawLaserShots();
    void createLaserShot(float x1, float y1, float ang, float length, float speed, llULINT ownerID);
        
    static mt_distribution* mDistAge;
    static mt_distribution* mDistPosition;
    static mt_distribution* mDistFitnessRandom;

    float mWorldWidth;
    float mWorldLength;

    int mCellSide;
    int mWorldCellWidth;
    int mWorldCellLength;

    list<SimObj2D*>** mCellGrid;

    float mViewX;
    float mViewY;

    float mViewRange;
    float mViewAngle;
    float mHalfViewAngle;
    float mLowLimitViewAngle;
    float mHighLimitViewAngle;

    bool mShowGrid;
    bool mShowViewRange;
    bool mShowBrain;
    bool mShowEnergy;

    float mGoCost;
    float mRotateCost;
    float mGoForceScale;
    float mRotateForceScale;

    SimObj2D* mTargetObject;
    float mDistanceToTargetObject;
    float* mCurrentTargetInputBuffer;

    bool mDragging;
    int mLastMouseX;
    int mLastMouseY;

    int mCellX1;
    int mCellX2;
    int mCellY1;
    int mCellY2;

    int mCurrentCellX;
    int mCurrentCellY;

    float mCollisionX;
    float mCollisionY;
    float mCollisionRadius;

    list<SimObj2D*>* mCurrentCellList;
    list<SimObj2D*>::iterator mCurrentCellListIterator;

    llULINT mCollisionDetectionIteration;

    SimObj2D* mHumanAgent;
    bool mHumanGo;
    bool mHumanRotateLeft;
    bool mHumanRotateRight;
    bool mHumanEat;
    bool mHumanSpeak;
    bool mHumanFire;

    float mZoom;

    float mFeedCenter;

    float mSoundRange;
    unsigned int mSpeakInterval;
    unsigned int mFireInterval;

    list<VisualEvent*> mVisualEvents;

    Art_Layer* mBackgroundTexture;

    list<Laser> mLaserShots;
};
#endif

