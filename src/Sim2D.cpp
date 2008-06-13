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

#include <math.h>
#include <list>

using std::list;

mt_distribution* Sim2D::mDistAge = gDistManager.getNewDistribution();
mt_distribution* Sim2D::mDistPosition = gDistManager.getNewDistribution();

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

    mZoom = 1.0f;

    mHumanAgent = NULL;
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
    SimArtist::initGraphics(width, height, fullScreen, noGraphics);

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

void Sim2D::updatePos(SimObj2D* obj,
                        float origX,
                        float origY,
                        float targX,
                        float targY)
{
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

    int targX1 = ((int)(targX - size)) / mCellSide;
    int targX2 = ((int)(targX + size)) / mCellSide;
    int targY1 = ((int)(targY - size)) / mCellSide;
    int targY2 = ((int)(targY + size)) / mCellSide;

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

void Sim2D::addObject(SimObj* obj, bool init)
{
    SimObj2D* object = (SimObj2D*)obj;

    object->mSim2D = this;

    object->mEnergy = object->mInitialEnergy;

    if (init)
    {
        object->mAgeLimit = mDistAge->iuniform(1, object->mMaxAge);
    }
    else
    {
        object->mAgeLimit = object->mMaxAge;
    }

    object->init(); 

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
    SimObj2D* obj2D = (SimObj2D*)obj;
    float x = mDistPosition->uniform(0, mWorldWidth);
    float y = mDistPosition->uniform(0, mWorldLength);
    obj2D->setPos(x, y);
    obj2D->setRot(mDistPosition->uniform(0, M_PI * 2));
}

void Sim2D::replace(SimObj* obj, SimObj* ref)
{
    float x = ((SimObj2D*)ref)->mX;
    float y = ((SimObj2D*)ref)->mY;
    float rot = ((SimObj2D*)ref)->mRot;

    SimObj2D* obj2D = (SimObj2D*)obj;

    obj2D->setPos(x, y);
    obj2D->setRot(rot);
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

    if (targX < 0.0f)
    {
        targX = 0.0f;
    }
    if (targY < 0.0f)
    {
        targY = 0.0f;
    }
    if (targX > mWorldWidth)
    {
        targX = mWorldWidth;
    }
    if (targY > mWorldLength)
    {
        targY = mWorldLength;
    }

    SimObj2D* obj2D = (SimObj2D*)obj;

    obj2D->setPos(targX, targY);
    obj2D->setRot(mDistPosition->uniform(0, M_PI * 2));
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
                float beginAngle = normalizeAngle(obj->mRot - obj->mHalfViewAngle);
                float endAngle = normalizeAngle(obj->mRot + obj->mHalfViewAngle);

                if (beginAngle > endAngle)
                {
                    endAngle += M_PI * 2.0f;
                }

                art_setColor(150, 150, 150, 100);
                art_fillCircleSlice(obj->mX,
                                obj->mY,
                                obj->mViewRange,
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
                art_fillCircle(ve->mX, ve->mY, ve->mRadius);
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

SimObj2D* Sim2D::getLineTarget(float x1, float y1, float x2, float y2, llULINT excludeID)
{
    //printf("%f %f %f %f %d\n", x1, y1, x2, y2, excludeID);
    float dx = x2 - x1;
    float dy = y2 - y1;
    float M = dy / dx; //tan(angle)
    float B = y1 - (M * x1);

    float dirX = 1;
    if (x1 > x2)
    {
        dirX = -1;
    }
    float dirY = 1;
    if (y1 > y2)
    {
        dirY = -1;
    }
    float cX = x1;
    float cY = y1;

    int cellX = ((int)(cX)) / mCellSide;
    int cellY = ((int)(cY)) / mCellSide;

    int targCellX = ((int)(x2)) / mCellSide;
    int targCellY = ((int)(y2)) / mCellSide;

    //printf("*** cells: %d %d %d %d\n", cellX, cellY, targCellX, targCellY);

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

    targCellX *= dirX;
    targCellY *= dirY;

    SimObj2D* bestTarget = NULL;

    while (((cellX * dirX) <= targCellX)
        && ((cellY * dirY) <= targCellY)
        && (cellX < mWorldCellWidth)
        && (cellY < mWorldCellLength)
        && (bestTarget == NULL))
    {
        //printf("cell: %d %d\n", cellX, cellY);

        float mindSource = 999999999.9f;
        list<SimObj2D*>* cellList = mCellGrid[(mWorldCellWidth * cellY) + cellX];

        for (list<SimObj2D*>::iterator iterObj = cellList->begin();
            iterObj != cellList->end();
            iterObj++)
        {
            SimObj2D* obj = (SimObj2D*)(*iterObj);

            if (obj->getID() != excludeID)
            {
                //printf("obj: %d\n", obj->getID());
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
                    float segmentColides = false;

                    float colX1 = x1;
                    float colX2 = x2;
                    float colY1 = y1;
                    float colY2 = y2;

                    if (colX1 > colX2)
                    {
                        colX1 = x2;
                        colX2 = x1;
                    }
                    if (colY1 > colY2)
                    {
                        colY1 = y2;
                        colY2 = y1;
                    }

                    if (((objX + r) >= colX1)
                        && ((objX - r) <= colX2)
                        && ((objY + r) >= colY1)
                        && ((objY - r) <= colY2))
                    {
                        segmentColides = true;
                    }
                    else
                    {
                        float dist1 = (x_1 * x_1) + (y_1 * y_1);
                        float dist2 = (x_2 * x_2) + (y_2 * y_2);
                        float dist = (obj->mSize * obj->mSize);

                        if ((dist1 <= dist)
                            || (dist2 <= dist))
                        {
                            segmentColides = true;
                        }
                    }

                    if (segmentColides)
                    {
                        float dsX = obj->mX - x1;
                        float dsY = obj->mY - y1;
                        float dSource = (dsX * dsX) + (dsY * dsY);

                        if (dSource < mindSource)
                        {
                            mindSource = dSource;
                            bestTarget = (SimObj2D*)obj;
                        }
                    }
                }
            }
        }

        if (bestTarget != NULL)
        {
            //printf("colision! %d (%d, %d)\n", bestTarget->getID(), cellX, cellY);
            return bestTarget;
        }

        float bX;
        float bY;

        if (dirX == 1)
        {
            bX = (cellX + 1) * mCellSide;
        }
        else
        {
            bX = cellX * mCellSide;
        }
        if (dirY == 1)
        {
            bY = (cellY + 1) * mCellSide;
        }
        else
        {
            bY = cellY * mCellSide;
        }

        float bXY = (cX * M) + B;
        float bYX = (cY - B) / M;

        //printf("M %f B %f dirX: %f dirY: %f\n", M, B, dirX, dirY);

        float deltaX1 = cX - bX;
        float deltaY1 = cY - bXY;
        float deltaX2 = cX - bYX;
        float deltaY2 = cY - bY;

        //printf("cX %f cY %f bX %f bY %f bXY %f bYX %f\n", cX, cY, bX, bY, bXY, bYX);
        //printf("deltaX1 %f deltaY1 %f deltaX2 %f deltaY2 %f\n", deltaX1, deltaY1, deltaX2, deltaY2);

        float dis1 = (deltaX1 * deltaX1) + (deltaY1 * deltaY1);
        float dis2 = (deltaX2 * deltaX2) + (deltaY2 * deltaY2);

        //printf("dis1 %f dis2 %f\n", dis1, dis2);

        if ((dis1 < dis2) || isnan(dis2))
        {
            cX = bX;
            cY = bXY;
            cellX += dirX;
        }
        else
        {
            cX = bYX;
            cY = bY;
            cellY += dirY;
        }

        //printf("cell after: %d %d\n", cellX, cellY);
    }

    return NULL;
}

void Sim2D::processLaserShots()
{
    list<Laser2D>::iterator iterLaser = mLaserShots.begin();

    while (iterLaser != mLaserShots.end())
    {
        Laser2D* laser = &(*iterLaser);

        float x1 = laser->mX1;
        float y1 = laser->mY1;

        laser->mX1 += cosf(laser->mAng) * laser->mSpeed;
        laser->mY1 += sinf(laser->mAng) * laser->mSpeed;
        laser->mX2 += cosf(laser->mAng) * laser->mSpeed;
        laser->mY2 += sinf(laser->mAng) * laser->mSpeed;
        laser->mDistanceTraveled += laser->mSpeed;

        float x2 = laser->mX2;
        float y2 = laser->mY2;

        SimObj2D* bestTarget = getLineTarget(x1, y1, x2, y2, laser->mOwnerID);
        if (bestTarget != NULL)
        {
            //printf("colision! %d\n", bestTarget->getID());
            bestTarget->processLaserHit(laser);
        }

        if ((bestTarget != NULL)
            || (laser->mDistanceTraveled > laser->mRange)
            || (laser->mX1 > mWorldWidth)
            || (laser->mX2 > mWorldWidth)
            || (laser->mY1 > mWorldLength)
            || (laser->mY2 > mWorldLength)
            || (laser->mX1 < 0)
            || (laser->mX2 < 0)
            || (laser->mY1 < 0)
            || (laser->mY2 < 0))
        {
            iterLaser = mLaserShots.erase(iterLaser);
        }
        else
        {
            iterLaser++;
        }
    }
}

void Sim2D::drawLaserShots()
{
    art_setColor(255, 255, 255, 255);
    art_setLineWidth(1.0f);

    for (list<Laser2D>::iterator iterLaser = mLaserShots.begin();
            iterLaser != mLaserShots.end();
            iterLaser++)
    {
        Laser2D* laser = &(*iterLaser);
        art_drawLine(laser->mX1, laser->mY1, laser->mX2, laser->mY2);
    }
}

void Sim2D::fireLaser(Laser2D laser)
{
    mLaserShots.push_back(laser);
}

bool Sim2D::onKeyDown(Art_KeyCode key)
{
    if (SimArtist::onKeyDown(key))
    {
        return true;
    }

    if (mHumanAgent != NULL)
    {
        switch (key)
        {
        case ART_KEY_UP:
            mHumanAgent->mHumanGo = true;
            return true;
        case ART_KEY_RIGHT:
            mHumanAgent->mHumanRotateRight = true;
            return true;
        case ART_KEY_LEFT:
            mHumanAgent->mHumanRotateLeft = true;
            return true;
        case ART_KEY_E:
            mHumanAgent->mHumanEat = true;
            return true;
        case ART_KEY_S:
            mHumanAgent->mHumanSpeak = true;
            return true;
        case ART_KEY_SPACE:
            mHumanAgent->mHumanFire = true;
            return true;
        default:
            return false;
        }
    }
}

bool Sim2D::onKeyUp(Art_KeyCode key)
{
    if (SimArtist::onKeyUp(key))
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
    }

    if (mHumanAgent != NULL)
    {
        switch (key)
        {
        case ART_KEY_UP:
            mHumanAgent->mHumanGo = false;
            return true;
        case ART_KEY_RIGHT:
            mHumanAgent->mHumanRotateRight = false;
            return true;
        case ART_KEY_LEFT:
            mHumanAgent->mHumanRotateLeft = false;
            return true;
        case ART_KEY_E:
            mHumanAgent->mHumanEat = false;
            return true;
        case ART_KEY_S:
            mHumanAgent->mHumanSpeak = false;
            return true;
        case ART_KEY_SPACE:
            mHumanAgent->mHumanFire = false;
            return true;
        }
    }

    return false;
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
        case PERCEPTION_SIZE:
            return "size";
        case PERCEPTION_ENERGY:
            return "energy";
        case PERCEPTION_CAN_SPEAK:
            return "canspeak";
        case PERCEPTION_CAN_FIRE:
            return "canfire";
        case PERCEPTION_CONVERGENCE:
            return "convergence";
        case PERCEPTION_LTARGET:
            return "ltarg";
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
        case ACTION_FIREB:
            return "fireB";
        default:
            return "?";
        }
    }
}

void Sim2D::setHuman(SimObj2D* agent)
{
    mHumanAgent = agent;
    mHumanAgent->mHumanControlled = true;
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

const char Sim2D::mClassName[] = "Sim2D";

Orbit<Sim2D>::MethodType Sim2D::mMethods[] = {
    {"addObject", &Simulation::addObject},
    {"setSeedIndex", &Simulation::setSeedIndex},
    {"setPopulationDynamics", &Simulation::setPopulationDynamics},
    {"run", &Simulation::run},
    {"setTimeLimit", &Simulation::setTimeLimit},
    {"initGraphics", &SimArtist::initGraphics},
    {"setWorldDimensions", &Sim2D::setWorldDimensions},
    {"setHuman", &Sim2D::setHuman},
    {0,0}
};

Orbit<Sim2D>::NumberGlobalType Sim2D::mNumberGlobals[] = {
    {"PERCEPTION_NULL", PERCEPTION_NULL},
    {"PERCEPTION_POSITION", PERCEPTION_POSITION},
    {"PERCEPTION_ORIENTATION", PERCEPTION_ORIENTATION},
    {"PERCEPTION_DISTANCE", PERCEPTION_DISTANCE},
    {"PERCEPTION_SIZE", PERCEPTION_SIZE},
    {"PERCEPTION_TARGET", PERCEPTION_TARGET},
    {"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
    {"PERCEPTION_SYMBOL", PERCEPTION_SYMBOL},
    {"PERCEPTION_ENERGY", PERCEPTION_ENERGY},
    {"PERCEPTION_CAN_SPEAK", PERCEPTION_CAN_SPEAK},
    {"PERCEPTION_CAN_FIRE", PERCEPTION_CAN_FIRE},
    {"PERCEPTION_CONVERGENCE", PERCEPTION_CONVERGENCE},
    {"PERCEPTION_LTARGET", PERCEPTION_LTARGET},
    {"ACTION_NULL", ACTION_NULL},
    {"ACTION_GO", ACTION_GO},
    {"ACTION_ROTATE", ACTION_ROTATE},
    {"ACTION_EAT", ACTION_EAT},
    {"ACTION_EATB", ACTION_EATB},
    {"ACTION_SPEAK", ACTION_SPEAK},
    {"ACTION_FIRE", ACTION_FIRE},
    {"ACTION_FIREB", ACTION_FIREB},
    {"FITNESS_ENERGY", FITNESS_ENERGY},
    {"FITNESS_ENERGY_SUM", FITNESS_ENERGY_SUM},
    {"FITNESS_ENERGY_SUM_ABOVE_INIT", FITNESS_ENERGY_SUM_ABOVE_INIT},
    {"FITNESS_SCORE_SUM", FITNESS_SCORE_SUM},
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

int Sim2D::setHuman(lua_State* luaState)
{
    SimObj2D* agent = (SimObj2D*)Orbit<Sim2D>::pointer(luaState, 1);
    setHuman(agent);
    return 0;
}

