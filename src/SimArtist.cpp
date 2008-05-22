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

#include "SimArtist.h"

SimArtist::SimArtist(lua_State* luaState)
{
    mLastSimulationTime = 0;
    mLastRealTime = 0.0f;
    mCPS = 0.0f;

    mSimulationTimeText = "";
    mRealTimeText = "";
    mCPSText = "";

    mLastDrawTime = 0.0f;
    mTargetFPS = 60;

    mFrameDuration = 1.0f / (double)mTargetFPS;
}

SimArtist::~SimArtist()
{
}

void SimArtist::initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics)
{
    art_init(width, height, fullScreen, 0, 0, 1, 0);
    art_setBackgroundColor(0, 50, 0);

    mLogo = art_loadImage("media/lablove_small.png");

    art_setWinTitle("LabLOVE");

    if (strcmp(art_systemName(), "Null") == 0)
    {
        mDrawGraphics = false;
    }

    mFont = art_loadFont("media/vera/Vera.ttf", 8);
}

void SimArtist::run()
{
    mInitialRealTime = art_getTime();

    Simulation::run();
}

void SimArtist::cycle()
{
    mDrawThisCycle = false;

    double realTime = art_getTime();
    if (realTime - mLastDrawTime >= mFrameDuration)
    {
        mLastDrawTime = realTime;
        mDrawThisCycle = true;
    }

    Simulation::cycle();

    art_setColor(255, 255, 255, 180);
    art_drawLayer(mLogo, 0, 0);
    art_fillRectangle(122, 0, 200, 50);
    drawTimes();

    if (mDrawThisCycle && mDrawGraphics)
    {
        art_update();
    }

    // Process events
    while (art_nextEvent())
    {
        switch (art_getEventType())
        {
        case ART_EVENT_KEY_DOWN:
            onKeyDown(art_getKeyCode());
            break;
        case ART_EVENT_KEY_UP:
            onKeyUp(art_getKeyCode());
            break;
        case ART_EVENT_MOUSE_BUTTON_DOWN:
            onMouseButtonDown(art_getMouseButton(),
                                art_getMousePosX(),
                                art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_BUTTON_UP:
            onMouseButtonUp(art_getMouseButton(),
                            art_getMousePosX(),
                            art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_MOTION:
            onMouseMove(art_getMousePosX(),
                        art_getMousePosY());
            break;
        case ART_EVENT_MOUSE_WHEEL_UP:
            onMouseWheel(true);
            break;
        case ART_EVENT_MOUSE_WHEEL_DOWN:
            onMouseWheel(false);
            break;
        default:
            break;
        }
    }
}

void SimArtist::drawTimes()
{
    if (mLastRealTime == 0.0f)
    {
        mLastRealTime = art_getTime();
    }
    else if (mSimulationTime % 100 == 0)
    {
        double realTime = art_getTime();
        double deltaRealTime = realTime - mLastRealTime;
        double deltaSimTime = (double)(mSimulationTime - mLastSimulationTime);
        mLastRealTime = realTime;
        mLastSimulationTime = mSimulationTime;

        mCPS = deltaSimTime / deltaRealTime;

        char text[255];

        sprintf(text, "Sim Time: %d Kcycles", mSimulationTime / 1000);
        mSimulationTimeText = text;

        float totalRealTime = realTime - mInitialRealTime;
        float days = 0;
        float hours = 0;
        float minutes = 0;
        float seconds = 0;

        double remainingTime = totalRealTime;

        if (remainingTime > 60 * 60 * 24)
        {
            days = truncf(remainingTime / (60 * 60 * 24));
            remainingTime -= days * 60 * 60 * 24;
        }
        if (remainingTime > 60 * 60)
        {
            hours = truncf(remainingTime / (60 * 60));
            remainingTime -= hours * 60 * 60;
        }
        if (remainingTime > 60)
        {
            minutes = truncf(remainingTime / 60);
            remainingTime -= minutes * 60;
        }
        seconds = truncf(remainingTime);

        sprintf(text, "Real Time: %.0fd %.0fh %.0fm %.0fs", days, hours, minutes, seconds);
        mRealTimeText = text;

        sprintf(text, "CPS: %.0f", mCPS);
        mCPSText = text;
    }

    art_setFont(mFont);
    art_setColor(30, 30, 30, 255);
    art_drawText(140, 16, (char*)mSimulationTimeText.c_str());
    art_drawText(140, 31, (char*)mRealTimeText.c_str());
    art_drawText(140, 46, (char*)mCPSText.c_str());
}

bool SimArtist::onKeyDown(Art_KeyCode keycode)
{
    switch (keycode)
    {
        case ART_KEY_ESCAPE:
            mStop = true;
            return true;
        default:
            return false;
    }
}

int SimArtist::initGraphics(lua_State* luaState)
{
    int width = luaL_checkint(luaState, 1);
    int height = luaL_checkint(luaState, 2);
    bool fullScreen = luaL_checkbool(luaState, 3);
    bool noGraphics = false;
    if (lua_gettop(luaState) > 3)
    {        
        noGraphics = luaL_checkbool(luaState, 4);
    }

    initGraphics(width, height, fullScreen, noGraphics);

    return 0;
}

