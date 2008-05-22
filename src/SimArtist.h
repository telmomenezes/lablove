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

#if !defined(__INCLUDE_SIM_ARTIST_H)
#define __INCLUDE_SIM_ARTIST_H

#include "Simulation.h"

#include "art.h"

#include <string>

using std::string;

class SimArtist : public Simulation
{
public:
    SimArtist(lua_State* luaState=NULL);
    virtual ~SimArtist();

    virtual void run();
    virtual void cycle();

    virtual void initGraphics(unsigned int width,
                                unsigned int height,
                                bool fullScreen,
                                bool noGraphics=false);
    
    virtual bool onKeyDown(Art_KeyCode keycode);
    virtual bool onKeyUp(Art_KeyCode keycode){return false;}
    virtual bool onMouseButtonDown(Art_MouseButton button, int x, int y){return false;}
    virtual bool onMouseButtonUp(Art_MouseButton button, int x, int y){return false;}
    virtual bool onMouseMove(int x, int y){return false;}
    virtual bool onMouseWheel(bool up){return false;}

    int initGraphics(lua_State* luaState);

protected:
    void drawTimes();
    Art_Layer* mLogo;

    double mInitialRealTime;
    llULINT mLastSimulationTime;
    double mLastRealTime;
    double mCPS;
    string mSimulationTimeText;
    string mRealTimeText;
    string mCPSText;

    Art_Font* mFont;

    double mLastDrawTime;
    unsigned int mTargetFPS;
    double mFrameDuration;
};
#endif

