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

#ifdef __LOVELAB_WITH_GRAPHICS

#include "OgreApplication.h"
#include "Lab.h"

using namespace Ogre;

OgreApplication::OgreApplication()
{
	mRoot = 0;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
	mResourcePath = "";
#endif
	mStop = false;
	mTranslateVector = Vector3::ZERO;
	mNumScreenShots = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mMoveSpeed = 100;
	mRotateSpeed = 36;
}

OgreApplication::~OgreApplication()
{
	//Remove ourself as a Window listener
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	if (mRoot)
	{
		delete mRoot;
	}
}

void OgreApplication::start()
{
	mRoot->startRendering();
	destroyScene();
}

bool OgreApplication::init()
{
	String pluginsPath;
	// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
	pluginsPath = mResourcePath + "plugins.cfg";
#endif
		
	mRoot = new Root(pluginsPath, 
				mResourcePath + "ogre.cfg",
				mResourcePath + "Ogre.log");

	setupResources();

	bool carryOn = configure();

	if (!carryOn)
	{
		return false;
	}

	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "OgreSMInstance");

	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Vector3(0, 0, 500));
	mCamera->lookAt(Vector3(0, 0, -300));
	mCamera->setNearClipDistance(5);

	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(1.0, 1.0, 1.0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Load resources
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20, 80, 50);

	initFrameListener();

	addInputHandler(this);

	return true;
}

bool OgreApplication::configure()
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "LabLOVE");
		return true;
	}
	else
	{
		return false;
	}
}

void OgreApplication::setupResources()
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(mResourcePath + "resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			ResourceGroupManager::getSingleton().addResourceLocation(
				String(macBundlePath() + "/" + archName),
				typeName,
				secName);
#else
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName,
				typeName,
				secName);
#endif
		}
	}
}

void OgreApplication::initFrameListener()
{
	mInputManager = 0;
	mMouse = 0;
	mKeyboard = 0;
	mJoy = 0;
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = InputManager::createInputSystem(pl);

	//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject(OISKeyboard, true));
	mKeyboard->setEventCallback(this);
	mMouse = static_cast<Mouse*>(mInputManager->createInputObject(OISMouse, true));
	mMouse->setEventCallback(this);
	try
	{
		mJoy = static_cast<JoyStick*>(mInputManager->createInputObject(OISJoyStick, true));
	}
	catch(...)
	{
		mJoy = 0;
	}

	//Set initial mouse clipping size
	windowResized(mWindow);

	mDebugOverlay->show();

	//Register as a Window listener
	WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
}

void OgreApplication::updateStats()
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";
	static String batches = "Batch Count: ";

	// update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = mWindow->getStatistics();
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));
	}
	catch(...) { /* ignore */ }
}

void OgreApplication::windowResized(RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void OgreApplication::windowClosed(RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if (rw == mWindow)
	{
		if (mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);
			mInputManager->destroyInputObject(mJoy);

			InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool OgreApplication::frameStarted(const FrameEvent& evt)
{
	if (mStop)
	{
		return false;
	}
	if (mWindow->isClosed())
	{
		return false;
	}

	if (evt.timeSinceLastFrame == 0)
	{
		mMoveScale = 1;
		mRotScale = 0.1;
	}
	else
	{
		mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
		mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
	}

	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;

	mKeyboard->capture();
	mMouse->capture();
	if (mJoy)
	{
		mJoy->capture();
	}

	// Update camera position
 	mCamera->yaw(mRotX);
	mCamera->pitch(mRotY);
	mCamera->moveRelative(mTranslateVector);

	Lab::getInstance().cycle();

	return true;
}

bool OgreApplication::frameEnded(const FrameEvent& evt)
{
	updateStats();
	return true;
}

bool OgreApplication::keyPressed(const KeyEvent &arg)
{
	list<InputHandler*>::iterator iterHandler = mHandlersList.begin();
	while (iterHandler != mHandlersList.end())
	{
		if ((*iterHandler)->onKeyDown(arg.key))
		{
			return true;
		}
		iterHandler++;
	}
	return false;
}

bool OgreApplication::keyReleased(const KeyEvent &arg)
{
	list<InputHandler*>::iterator iterHandler = mHandlersList.begin();
	while (iterHandler != mHandlersList.end())
	{
		if ((*iterHandler)->onKeyUp(arg.key))
		{
			return true;
		}
		iterHandler++;
	}
	return false;
}

bool OgreApplication::mouseMoved(const MouseEvent &arg)
{
	list<InputHandler*>::iterator iterHandler = mHandlersList.begin();
	while (iterHandler != mHandlersList.end())
	{
		if ((*iterHandler)->onMouseMove(arg.state.X.rel, arg.state.Y.rel))
		{
			return true;
		}
		iterHandler++;
	}
	return false;
}

bool OgreApplication::mousePressed(const MouseEvent &arg, MouseButtonID id)
{
	return false;
}

bool OgreApplication::mouseReleased(const MouseEvent &arg, MouseButtonID id)
{
	return false;
}

bool OgreApplication::onKeyDown(int key)
{
	switch (key)
	{
	case KC_A:
		mTranslateVector.x = -mMoveScale;	// Move camera left
		return true;
	case KC_D:
		mTranslateVector.x = mMoveScale;	// Move camera RIGHT
		return true;
	case KC_UP:
	case KC_W:
		mTranslateVector.z = -mMoveScale;	// Move camera forward
		return true;
	case KC_DOWN:
	case KC_S:
		mTranslateVector.z = mMoveScale;	// Move camera backward
		return true;
	case KC_PGUP:
		mTranslateVector.y = mMoveScale;	// Move camera up
		return true;
	case KC_PGDOWN:
		mTranslateVector.y = -mMoveScale;	// Move camera down
		return true;
	case KC_RIGHT:
		mCamera->yaw(-mRotScale);
		return true;
	case KC_LEFT:
		mCamera->yaw(mRotScale);
		return true;
	case KC_ESCAPE:
	case KC_Q:
		mStop = true;
		return true;
	}

	return false;
}

bool OgreApplication::onMouseMove(int x, int y)
{
	/*if (button pressed)
	{
		mTranslateVector.x += ms.X.rel * 0.13;
		mTranslateVector.y -= ms.Y.rel * 0.13;

		return true;
	}*/
	
	mRotX = Degree(-x * 0.13);
	mRotY = Degree(-y * 0.13);

	return true;
}

void OgreApplication::addInputHandler(InputHandler* handler)
{
	mHandlersList.push_front(handler);
}

void OgreApplication::removeInputHandler()
{
	mHandlersList.pop_front();
}
#endif

