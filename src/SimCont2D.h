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

#if !defined(__INCLUDE_SIM_CONT2D_H)
#define __INCLUDE_SIM_CONT2D_H

#include "Simulation.h"
#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "Agent.h"

#include "pyc.h"

#include "Orbit.h"

class SimCont2D : public Simulation
{
public:

    enum Perception {PERCEPTION_NULL,
                    PERCEPTION_POSITION,
                    PERCEPTION_PROXIMITY,
                    PERCEPTION_IN_CONTACT};

    enum Action {ACTION_NULL,
                ACTION_GO,
                ACTION_ROTATE,
                ACTION_EAT};


    SimCont2D(lua_State* luaState=NULL);
    virtual ~SimCont2D();

    void setWorldDimensions(float worldWidth,
                            float worldLength,
                            unsigned int cellSide);
    float getWorldWidth(){return mWorldWidth;}
    float getWorldLength(){return mWorldLength;}
    float getCellSide(){return mCellSide;}
    unsigned int getWorldCellWidth(){return mWorldCellWidth;}
    unsigned int getWorldCellLength(){return mWorldCellLength;}
    list<SimulationObject*>** getCellGrid(){return mCellGrid;}

    void setPos(SimulationObject* obj, float x, float y);
    void setRot(SimulationObject* obj, float rot);

    virtual void placeRandom(SimulationObject* obj);

    virtual void drawBeforeObjects();
    virtual void drawAfterObjects();

    virtual void removeObject(SimulationObject* obj);

    void setGoCost(float cost){mGoCost = cost;}
    void setRotateCost(float cost){mRotateCost = cost;}
    
    void setShowGrid(bool show){mShowGrid = show;}
    void setShowViewRange(bool show){mShowViewRange = show;}
    bool getShowGrid(){return mShowGrid;}
    bool getShowViewRange(){return mShowViewRange;}

    void moveView(float x, float y);
    float getViewX(){return mViewX;}
    float getViewY(){return mViewY;}

    virtual SimulationObject* getObjectByScreenPos(int x, int y);
    
    virtual bool onKeyDown(pyc::KeyCode keycode);
    virtual bool onKeyUp(pyc::KeyCode keycode);
    virtual bool onMouseButtonDown(pyc::MouseButton button, int x, int y);
    virtual bool onMouseButtonUp(pyc::MouseButton button, int x, int y);
    virtual bool onMouseMove(int x, int y);

    void setViewRange(float range);
    void setViewAngle(float angle);
    float getViewRange(){return mViewRange;}
    float getViewAngle(){return mViewAngle;}

    static const char mClassName[];
    static Orbit<SimCont2D>::MethodType mMethods[];
    static Orbit<SimCont2D>::NumberGlobalType mNumberGlobals[];

    int setWorldDimensions(lua_State* luaState);
    int setViewRange(lua_State* luaState);
    int setViewAngle(lua_State* luaState);
    int setGoCost(lua_State* luaState);
    int setRotateCost(lua_State* luaState);
   
    void startCollisionDetection(float x, float y, float rad);
    SimulationObject* nextCollision(float& distance, float& angle);

protected:
    virtual void perceive(Agent* agent);
    virtual void act(Agent* agent);
    virtual void onScanObject(Agent* orig,
                                SimulationObject* targ,
                                float distance,
                                float angle);
    void goFront(Agent* agent, float distance);
    void rotate(Agent* agent, float angle);
    void eat(Agent* agent);

    float normalizeAngle(float angle);

    float mWorldWidth;
    float mWorldLength;

    int mCellSide;
    int mWorldCellWidth;
    int mWorldCellLength;

    list<SimulationObject*>** mCellGrid;

    float mViewX;
    float mViewY;

    float mViewRange;
    float mViewAngle;
    float mHalfViewAngle;
    float mLowLimitViewAngle;
    float mHighLimitViewAngle;

    bool mShowGrid;
    bool mShowViewRange;

    float mGoCost;
    float mRotateCost;

    SimulationObject* mTargetObject;
    float mDistanceToTargetObject;

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

    list<SimulationObject*>* mCurrentCellList;
    list<SimulationObject*>::iterator mCurrentCellListIterator;

    unsigned long mCollisionDetectionIteration;
};
#endif

