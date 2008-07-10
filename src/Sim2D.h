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

#include "SimArtist.h"
#include "SimObj2D.h"
#include "PopulationDynamics.h"
#include "RandDistManager.h"
#include "Laser2D.h"

#include "art.h"

#include "Orbit.h"

class Sim2D : public SimArtist
{
public:

    static const int PERCEPTION_NULL = 0;
    static const int PERCEPTION_POSITION = 1;
    static const int PERCEPTION_ORIENTATION = 2;
    static const int PERCEPTION_DISTANCE = 3;
    static const int PERCEPTION_SIZE = 4;
    static const int PERCEPTION_TARGET = 5;
    static const int PERCEPTION_IN_CONTACT = 6;
    static const int PERCEPTION_SYMPRO = 7;
    static const int PERCEPTION_SYMEQ = 8;
    static const int PERCEPTION_ENERGY = 9;
    static const int PERCEPTION_CAN_SPEAK = 10;
    static const int PERCEPTION_CAN_FIRE = 11;
    static const int PERCEPTION_LTARGET = 12;
    static const int PERCEPTION_LOF = 13;
    static const int PERCEPTION_CONV = 14;
    static const int PERCEPTION_CONVDIR = 15;
    static const int PERCEPTION_ID = 16;
    static const int PERCEPTION_BLOCKED = 17;
    static const int PERCEPTION_COMPASS = 18;
    static const int PERCEPTION_VALUE = 19;

    static const int ACTION_NULL = 0;
    static const int ACTION_GO = 1;
    static const int ACTION_ROTATE= 2;
    static const int ACTION_EAT = 3;
    static const int ACTION_EATB = 4;
    static const int ACTION_SPEAK = 5;
    static const int ACTION_FIRE = 6;
    static const int ACTION_FIREB = 7;

    static const int VE_SPEAK = 0;
    static const int VE_LASER = 1;

    typedef struct
    {
        int mType;
        float mX;
        float mY;
        float mRadius;
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
    virtual void replace(SimObj* obj, SimObj* ref);

    virtual void updatePos(SimObj2D* obj,
                            float origX,
                            float origY,
                            float targX,
                            float targY);

    virtual void drawBeforeObjects();
    virtual void drawAfterObjects();

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

    void setHuman(SimObj2D* agent);

    void startCollisionDetection(float x, float y, float rad);
    SimObj2D* nextCollision(float& distance, float& angle);

    virtual string getInterfaceName(bool input, int type);

    void fireLaser(Laser2D laser);

    SimObj2D* getLineTarget(float x1, float y1, float x2, float y2, llULINT excludeID, float &distance);
    bool segmentCollides(SimObj2D* obj, float x1, float y1, float x2, float y2);

    static float normalizeAngle(float angle);

    list<VisualEvent*> mVisualEvents;
    
    static const char mClassName[];
    static Orbit<Sim2D>::MethodType mMethods[];
    static Orbit<Sim2D>::NumberGlobalType mNumberGlobals[];

    int setWorldDimensions(lua_State* luaState);
    
    int setHuman(lua_State* luaState);

protected:
    virtual void onCycle();

    virtual void drawTerrain();

    void processLaserShots();
    void drawLaserShots();
        
    static mt_distribution* mDistAge;
    static mt_distribution* mDistPosition;

    float mWorldWidth;
    float mWorldLength;

    int mCellSide;
    int mWorldCellWidth;
    int mWorldCellLength;

    list<SimObj2D*>** mCellGrid;

    float mViewX;
    float mViewY;

    bool mShowGrid;
    bool mShowViewRange;
    bool mShowBrain;
    bool mShowEnergy;

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

    float mZoom;

    Art_Layer* mBackgroundTexture;

    list<Laser2D> mLaserShots;
};
#endif

