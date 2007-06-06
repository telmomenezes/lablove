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

void OgreApplication::go()
{
	if (!setup())
	{
		return;
	}

	mRoot->startRendering();

	// clean up
	destroyScene();
}

bool OgreApplication::setup()
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
	l->setPosition(20,80,50);

	//createSphere("mySphereMesh", 10, 10, 10);
	//createTriangle("mySphereMesh", 10);

	/*for (unsigned int i = 0; i < 25; i++)
	{
		char meshName[255];
		sprintf(meshName, "tri%d", i);
		Entity* sphereEntity = mSceneMgr->createEntity(meshName, "mySphereMesh");
		SceneNode* sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		//sphereEntity->setMaterialName("Examples/Flare");
		sphereNode->attachObject(sphereEntity);
		sphereNode->setPosition(rand() % 200, rand() % 200, rand() % 200);
	}*/

	createXit();
 
 	/*
	// Also add a nice starship in
	Entity *ent = mSceneMgr->createEntity("razor", "razor.mesh");

	mSceneMgr->getRootSceneNode()->attachObject(ent);

	pThrusters = mSceneMgr->createParticleSystem("ParticleSys1", 200);

	pThrusters ->setMaterialName("Examples/Flare");
	pThrusters ->setDefaultDimensions(25, 25);

	ParticleEmitter *pEmit1 = pThrusters->addEmitter("Point");
	ParticleEmitter *pEmit2 = pThrusters->addEmitter("Point");

	// Thruster 1
	pEmit1->setAngle(Degree(3));
	pEmit1->setTimeToLive(0.2);
	pEmit1->setEmissionRate(70);

	pEmit1->setParticleVelocity(50);

	pEmit1->setDirection(-Vector3::UNIT_Z);
	pEmit1->setColour(ColourValue::White, ColourValue::Red);

	// Thruster 2
	pEmit2->setAngle(Degree(3));
	pEmit2->setTimeToLive(0.2);
	pEmit2->setEmissionRate(70);

	pEmit2->setParticleVelocity(50);

	pEmit2->setDirection(-Vector3::UNIT_Z);
	pEmit2->setColour(ColourValue::White, ColourValue::Red);

	// Set the position of the thrusters
	pEmit1->setPosition(Vector3(5.7f, 0.0f, 0.0f));
	pEmit2->setPosition(Vector3(-18.0f, 0.0f, 0.0f));

	mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0.0f, 6.5f, -67.0f))
		->attachObject(pThrusters);
	*/
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

void OgreApplication::createTriangle(const std::string& strName, const float r)
{
	MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	SubMesh *pSphereVertex = pSphere->createSubMesh();

	pSphere->sharedVertexData = new VertexData();
	VertexData* vertexData = pSphere->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = 3;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	pSphereVertex->indexData->indexCount = 6;
	pSphereVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));

	float deltaAngle = Math::PI / 1.5f;

	for(int i = 0; i < 3; i++)
	{
		float x = r * sinf(i * deltaAngle);
		float y = r * cosf(i * deltaAngle);
		float z = 0;

		// Add one vertex to the strip which makes up the sphere
		*pVertex++ = x;
		*pVertex++ = y;
		*pVertex++ = z;

		Vector3 vNormal = Vector3(x, y, z).normalisedCopy();
		*pVertex++ = vNormal.x;
		*pVertex++ = vNormal.y;
		*pVertex++ = vNormal.z;

		*pVertex++ = 0;
		*pVertex++ = 0;	
	}

	*pIndices++ = 2;
	*pIndices++ = 1;
	*pIndices++ = 0;
	*pIndices++ = 0;
	*pIndices++ = 1;
	*pIndices++ = 2;

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
	// Generate face list
	pSphereVertex->useSharedVertices = true;

	// the original code was missing this line:
	pSphere->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
	pSphere->_setBoundingSphereRadius(r);
	// this line makes clear the mesh is loaded (avoids memory leaks)
        pSphere->load();
}
	
ManualObject* OgreApplication::createXit()
{
	ManualObject* meshDecal = new Ogre::ManualObject("MeshDecal");
	mSceneMgr->getRootSceneNode()->attachObject(meshDecal);

	float r = 10.0f;
	float deltaAngle = Math::PI / 1.5f;

	meshDecal->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
 	for(int i = 0; i < 3; i++)
	{
		float x = r * sinf(i * deltaAngle);
		float y = r * cosf(i * deltaAngle);
		float z = 0;

		meshDecal->position(Ogre::Vector3(x, y, z));
		meshDecal->colour(1.0, 0.0, 0.0, 0.0);
	}

	meshDecal->index(2);
	meshDecal->index(1);
	meshDecal->index(0);

	meshDecal->end();

	return meshDecal;
}

