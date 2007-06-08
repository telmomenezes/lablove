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
#include "OgreFrameListener.h"

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

class OgreApplication
{
public:
	OgreApplication();
	virtual ~OgreApplication();

	bool init();
	void start();

	SceneManager* getSceneManager(){return mSceneMgr;}
	Camera* getCamera(){return mCamera;}

protected:
	Root *mRoot;
	Camera* mCamera;
	SceneManager* mSceneMgr;
	OgreFrameListener* mFrameListener;
	RenderWindow* mWindow;
	Ogre::String mResourcePath;

	bool configure();
	void chooseSceneManager();
	void createFrameListener();
	void destroyScene(){} 
	void setupResources();
	void createResourceListener(){}
	void loadResources();
};

#endif

