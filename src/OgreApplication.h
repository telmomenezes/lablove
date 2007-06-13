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

//mem probs without this next one
#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
//regular mem handler
#include <OgreMemoryMacros.h>

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "InputHandler.h"

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include <list>
using namespace std;

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

using namespace Ogre;
using namespace OIS;

class OgreApplication : public FrameListener, public WindowEventListener,
			public KeyListener, public MouseListener, public InputHandler
{
public:
	OgreApplication();
	virtual ~OgreApplication();

	bool init();
	void start();

	SceneManager* getSceneManager(){return mSceneMgr;}
	Camera* getCamera(){return mCamera;}

	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);

	virtual bool keyPressed(const KeyEvent &arg);
	virtual bool keyReleased(const KeyEvent &arg);
	virtual bool mouseMoved(const MouseEvent &arg);
	virtual bool mousePressed(const MouseEvent &arg, MouseButtonID id);
	virtual bool mouseReleased(const MouseEvent &arg, MouseButtonID id);

	virtual bool onKeyDown(int key);
	virtual bool onKeyUp(int key);
	virtual bool onMouseMove(int x, int y);

	void addInputHandler(InputHandler* handler);
	void removeInputHandler();

protected:
	bool configure();
	void chooseSceneManager();
	void destroyScene(){} 
	void setupResources();
	void initFrameListener();
	void updateStats();
	void updateCamera();

	Root *mRoot;
	Camera* mCamera;
	SceneManager* mSceneMgr;
	RenderWindow* mWindow;
	Ogre::String mResourcePath;

	bool mStop;

	int mSceneDetailIndex;
	Overlay* mDebugOverlay;

	//OIS Input devices
	InputManager* mInputManager;
	Mouse* mMouse;
	Keyboard* mKeyboard;
	JoyStick* mJoy;

	list<InputHandler*> mHandlersList;

	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
	CEGUI::System* mGUISystem;
	CEGUI::Window* mEditorGuiSheet;

	// Camera control
	Vector3 mTranslateVector;
	unsigned int mNumScreenShots;
	float mMoveScale;
	Degree mRotScale;
	Radian mRotX;
	Radian mRotY;
	Real mMoveSpeed;
	Degree mRotateSpeed;

	bool mMoveCameraLeft;
	bool mMoveCameraRight;
	bool mMoveCameraUp;
	bool mMoveCameraDown;
	bool mMoveCameraFront;
	bool mMoveCameraBack;
};

#endif

