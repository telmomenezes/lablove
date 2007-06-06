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

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

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

void OgreApplication::chooseSceneManager()
{
	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "OgreSMInstance");
}

void OgreApplication::createCamera()
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Vector3(0,0,500));
	// Look back along -Z
	mCamera->lookAt(Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);
}

void OgreApplication::createFrameListener()
{
	mFrameListener= new OgreFrameListener(mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
}

void OgreApplication::createScene()
{
	ParticleSystem *pThrusters;

	// Set ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Create a skybox
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 50);

	// Create a light
	Light* l = mSceneMgr->createLight("MainLight");
	// Accept default settings: point light, white diffuse, just set position
	// NB I could attach the light to a SceneNode if I wanted it to move automatically with
	//  other objects, but I don't
	l->setPosition(20, 80, 50);

	createTriangle("triang", 10);

	for (unsigned int i = 0; i < 25; i++)
	{
		char meshName[255];
		sprintf(meshName, "tri%d", i);
		Entity* sphereEntity = mSceneMgr->createEntity(meshName, "triang");
		SceneNode* sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		//sphereEntity->setMaterialName("Examples/Flare");
		sphereNode->attachObject(sphereEntity);
		sphereNode->setPosition(rand() % 200, rand() % 200, rand() % 200);
	}
}

void OgreApplication::createViewports()
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
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

void OgreApplication::createTriangle(const std::string& name, const float r)
{
	ManualObject* obj = new Ogre::ManualObject("MeshDecal");

	float deltaAngle = Math::PI / 1.5f;
	
	obj->estimateVertexCount(3);
	obj->estimateIndexCount(3);
	obj->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
 	for(int i = 0; i < 3; i++)
	{
		float x = r * sinf(i * deltaAngle);
		float y = r * cosf(i * deltaAngle);
		float z = 0;

		obj->position(Ogre::Vector3(x, y, z));
	}

	obj->index(2);
	obj->index(1);
	obj->index(0);

	obj->end();

	MeshPtr mesh = obj->convertToMesh(name);
	mesh->load();

	delete obj;
}

#endif

