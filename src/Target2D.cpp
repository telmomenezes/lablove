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

#include "Target2D.h"
#include "Sim2D.h"

mt_distribution* Target2D::mDistEnergy = gDistManager.getNewDistribution();
mt_distribution* Target2D::mDistSpeed = gDistManager.getNewDistribution();

Target2D::Target2D(lua_State* luaState) : SimObj2D(luaState)
{
    mMinEnergy = 0.0f;
    mMaxEnergy = 0.0f;
    mEnergySizeFactor = 0.0f;

    initSpeed(); 
}

Target2D::Target2D(Target2D* obj) : SimObj2D(obj)
{
    mMinEnergy = obj->mMinEnergy;
    mMaxEnergy = obj->mMaxEnergy;
    mEnergySizeFactor = obj->mEnergySizeFactor;

    initSpeed(); 
}

Target2D::~Target2D()
{
}

void Target2D::initSpeed()
{
    float speed = 0.01f;
    float angle = mDistSpeed->uniform(0, M_PI);

    mSpeedX = cosf(angle) * speed;
    mSpeedY = sinf(angle) * speed;
}

SimObj* Target2D::clone()
{
    return new Target2D(this);
}

void Target2D::init()
{
    mEnergy = mDistEnergy->uniform(mMinEnergy, mMaxEnergy);
    mSize = mEnergy * mEnergySizeFactor;

    SimObj2D::init();
}

void Target2D::process()
{
    SimObj2D::process();

    float x = mX + mSpeedX;
    float y = mY + mSpeedY;

    if (x < 0.0f)
    {
        x = 0.0f;
        mSpeedX = -mSpeedX;
    }
    else if (x > mSim2D->getWorldWidth())
    {
        x = mSim2D->getWorldWidth();
        mSpeedX = -mSpeedX;
    }
    if (y < 0.0f)
    {
        y = 0.0f;
        mSpeedY = -mSpeedY;
    }
    else if (y > mSim2D->getWorldLength())
    {
        y = mSim2D->getWorldLength();
        mSpeedY = -mSpeedY;
    }

    setPos(x, y);
}

void Target2D::setEnergyLimits(float min, float max)
{
    mMinEnergy = min;
    mMaxEnergy = max;
}

const char Target2D::mClassName[] = "Target2D";

Orbit<Target2D>::MethodType Target2D::mMethods[] = {
    {"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
    {"setBrain", &SimObj::setBrain},
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
    {"setLaserRange", &SimObj2D::setLaserRange},
    {"setLaserStrengthFactor", &SimObj2D::setLaserStrengthFactor},
    {"setLaserCostFactor", &SimObj2D::setLaserCostFactor},
    {"setLaserHitDuration", &SimObj2D::setLaserHitDuration},
    {"setEnergyLimits", &Target2D::setEnergyLimits},
    {"setEnergySizeFactor", &Target2D::setEnergySizeFactor},
    {"setEnergyLimits", &Target2D::setEnergyLimits},
    {"setEnergySizeFactor", &Target2D::setEnergySizeFactor},
    {0,0}
};

Orbit<Target2D>::NumberGlobalType Target2D::mNumberGlobals[] = {{0,0}};

int Target2D::setEnergyLimits(lua_State* luaState)
{
    float min = luaL_checknumber(luaState, 1);
    float max = luaL_checknumber(luaState, 2);
    setEnergyLimits(min, max);
    return 0;
}

int Target2D::setEnergySizeFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setEnergySizeFactor(factor);
    return 0;
}

