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

#ifndef __INCLUDE_OGRE_APPLICATION_H__
#define __INCLUDE_OGRE_APPLICATION_H__

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "InputHandler.h"

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "QuickGUI/QuickGUI.h"

#include <list>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

std::string macBundlePath()
{
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert(mainBundle);

	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	assert(mainBundleURL);

	CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert(cfStringRef);

	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);

	return std::string(path);
}
#endif

class OgreApplication : public Ogre::FrameListener,
			public Ogre::WindowEventListener,
			public OIS::KeyListener,
			public OIS::MouseListener,
			public InputHandler
{
public:
	enum UIMode {GUI, CAMERA, CONTROL};

	OgreApplication();
	virtual ~OgreApplication();

	bool init();
	void start();

	Ogre::SceneManager* getSceneManager(){return mSceneMgr;}
	Ogre::Camera* getCamera(){return mCamera;}

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual bool onKeyDown(int key);
	virtual bool onKeyUp(int key);
	virtual bool onMouseMove(int x, int y);

	void addInputHandler(InputHandler* handler);
	void removeInputHandler();

	UIMode getMode(){return mMode;}

protected:
	bool configure();
	void chooseSceneManager();
	void setupResources();
	void initFrameListener();
	void updateStats();
	void updateCamera();
	void updateMode();

	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcePath;

	bool mStop;

	int mSceneDetailIndex;
	Ogre::Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

	std::list<InputHandler*> mHandlersList;

	// Camera control
	Ogre::Vector3 mTranslateVector;
	unsigned int mNumScreenShots;
	float mMoveScale;
	Ogre::Degree mRotScale;
	Ogre::Radian mRotX;
	Ogre::Radian mRotY;
	Ogre::Real mMoveSpeed;
	Ogre::Degree mRotateSpeed;

	bool mMoveCameraLeft;
	bool mMoveCameraRight;
	bool mMoveCameraUp;
	bool mMoveCameraDown;
	bool mMoveCameraFront;
	bool mMoveCameraBack;

	QuickGUI::GUIManager* mGUIManager;
	QuickGUI::Sheet* mSheet;
	QuickGUI::Label* mModeLabel;

	UIMode mMode;
};

#endif

