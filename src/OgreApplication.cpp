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

using namespace Ogre;

OgreApplication::OgreApplication()
{
	mFrameListener = 0;
	mRoot = 0;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
	mResourcePath = "";
#endif
}

OgreApplication::~OgreApplication()
{
	if (mFrameListener)
	{
		delete mFrameListener;
	}
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
		mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

	setupResources();

	bool carryOn = configure();

	if (!carryOn)
	{
		return false;
	}

	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "OgreSMInstance");

	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Vector3(0, 0, 500));
	// Look back along -Z
	mCamera->lookAt(Vector3(0, 0, -300));
	mCamera->setNearClipDistance(5);

	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20, 80, 50);

	createFrameListener();

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
		mWindow = mRoot->initialise(true);
		return true;
	}
	else
	{
		return false;
	}
}

void OgreApplication::createFrameListener()
{
	mFrameListener= new OgreFrameListener(mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
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
				String(macBundlePath() + "/" + archName), typeName, secName);
#else
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
#endif
		}
	}
}

void OgreApplication::loadResources()
{
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

#endif

