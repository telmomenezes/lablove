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

#ifndef __INCLUDE_OGRE_FRAME_LISTENER_H__
#define __INCLUDE_OGRE_FRAME_LISTENER_H__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

using namespace Ogre;

class OgreFrameListener: public FrameListener, public WindowEventListener
{
public:
	OgreFrameListener(RenderWindow* win,
				Camera* cam,
				bool bufferedKeys = false,
				bool bufferedMouse = false,
				bool bufferedJoy = false );
	virtual ~OgreFrameListener();

	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt);
	bool processUnbufferedMouseInput(const FrameEvent& evt);
	void moveCamera();
	void showDebugOverlay(bool show);
	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);

protected:
	void updateStats(void);

	Camera* mCamera;

	Vector3 mTranslateVector;
	RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	Degree mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;
	Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
};

#endif

