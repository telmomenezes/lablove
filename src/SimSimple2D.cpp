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

#include "SimSimple2D.h"
#include "Lab.h"
#include "ObjectSimple2D.h"
#include <math.h>
#include "functions.h"
#include "PopulationDynamics.h"
#include "defines.h"
#include "AnimatSimple2D.h"

SimSimple2D::SimSimple2D(lua_State* luaState)
{
	mShowGrid = false;
	mShowViewRange = false;
	mCellGrid = NULL;
}

SimSimple2D::~SimSimple2D()
{
	if (mCellGrid != NULL)
	{
		free(mCellGrid);
		mCellGrid = NULL;
	}
}

void SimSimple2D::init()
{
#ifdef __LABLOVE_WITH_GRAPHICS
	// Create Animat Mesh
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
		"animatDefaultMaterial",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->setAmbient(1.0, 0.1, 0.1);


	Ogre::ManualObject* obj = new Ogre::ManualObject("MeshDecal");

	float deltaAngle = Ogre::Math::PI / 1.5f;
	
	obj->estimateVertexCount(3);
	obj->estimateIndexCount(3);
	obj->begin("animatDefaultMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float a = 0;
	float x = cosf(a);
	float z = sinf(a);
	obj->position(Ogre::Vector3(x, 0, z));
	a = Ogre::Math::PI + 0.5f;
	x = cosf(a);
	z = sinf(a);
	obj->position(Ogre::Vector3(x, 0, z));
	a = Ogre::Math::PI - 0.5f;
	x = cosf(a);
	z = sinf(a);
	obj->position(Ogre::Vector3(x, 0, z));

	obj->index(0);
	obj->index(1);
	obj->index(2);

	obj->end();

	Ogre::MeshPtr mesh = obj->convertToMesh("animat");
	mesh->load();

	delete obj;

	// Create view range material
	material = Ogre::MaterialManager::getSingleton().create(
		"viewRangeMaterial",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->setAmbient(0.5, 0.5, 0.5);
	material->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

	// Create plant mesh
	material = Ogre::MaterialManager::getSingleton().create(
		"plantDefaultMaterial",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->setAmbient(0.1, 1.0, 0.1);

	obj = new Ogre::ManualObject("MeshDecal");

	obj->estimateVertexCount(4);
	obj->estimateIndexCount(6);
	obj->begin("plantDefaultMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
	obj->position(Ogre::Vector3(-0.5, 0, 0.5));
	obj->position(Ogre::Vector3(-0.5, 0, -0.5));
	obj->position(Ogre::Vector3(0.5, 0, -0.5));
	obj->position(Ogre::Vector3(0.5, 0, 0.5));

	obj->index(0);
	obj->index(3);
	obj->index(2);
	obj->index(2);
	obj->index(1);
	obj->index(0);

	obj->end();

	mesh = obj->convertToMesh("plant");
	mesh->load();

	delete obj;

	// Adjust camera position
	Ogre::Camera* camera = Lab::getSingleton().getOgreApplication()->getCamera();
	camera->setPosition(Ogre::Vector3(mWorldWidth / 2, 200, 0));
	camera->lookAt(Ogre::Vector3(mWorldWidth / 2, 0, mWorldLength / 3));
#endif

	Simulation::init();
}

void SimSimple2D::setWorldDimensions(float worldWidth,
			float worldLength,
			float cellSide)
{
	mWorldWidth = worldWidth;
	mWorldLength = worldLength;
	mCellSide = cellSide;
	mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / mCellSide));
	mWorldCellLength = (unsigned int)(ceilf(mWorldLength / mCellSide));

	unsigned int gridSize = mWorldCellWidth * mWorldCellLength;
	mCellGrid = (ObjectSimple2D**)malloc(sizeof(ObjectSimple2D*) * gridSize);

	for (unsigned int i = 0; i < gridSize; i++)
	{
		mCellGrid[i] = NULL;
	}

	
}

void SimSimple2D::removeObject(SimulationObject* object)
{
	ObjectSimple2D* obj = (ObjectSimple2D*)object;

	if (obj->mNextCellList != NULL)
	{
		obj->mNextCellList->mPrevCellList = obj->mPrevCellList;
	}

	if (obj->mPrevCellList == NULL)
	{
		int cellPos = obj->getCellPos();

		if(cellPos >= 0)
		{
			mCellGrid[cellPos] = obj->mNextCellList;
		}
	}
	else
	{
		obj->mPrevCellList->mNextCellList = obj->mNextCellList;
	}

	Simulation::removeObject(object);
}

SimulationObject* SimSimple2D::getObjectByScreenPos(int x, int y)
{
	return NULL;
}

void SimSimple2D::setShowViewRange(bool show)
{
	mShowViewRange = show;

	std::list<SimulationObject*>::iterator iterObj;
	for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
	{
		ObjectSimple2D* obj = (ObjectSimple2D*)(*iterObj);
		obj->setShowViewRange(mShowViewRange);
	}
}

#ifdef __LABLOVE_WITH_GRAPHICS
/*
void SimSimple2D::drawBeforeObjects()
{
	if (mShowGrid)
	{
		unsigned int cellSide = (unsigned int)mCellSide;

		glColor3ub(DEF_S2D_GRID_COLOR_R,
			DEF_S2D_GRID_COLOR_G,
			DEF_S2D_GRID_COLOR_B);
		glBegin(GL_LINES);

		unsigned int division = cellSide - (mViewX % cellSide);
		while (division < Lab::getSingleton().getScreenWidth())
		{
			glVertex2f(division, 0);
			glVertex2f(division, Lab::getSingleton().getScreenHeight());

			division += cellSide;
		}

		division = cellSide - (mViewY % cellSide);
		while (division < Lab::getSingleton().getScreenHeight())
		{
			glVertex2f(0, division);
			glVertex2f(Lab::getSingleton().getScreenWidth(), division);

			division += cellSide;
		}

		glEnd();
	}
}
*/

bool SimSimple2D::onKeyDown(int key)
{
	if (Lab::getSingleton().getOgreApplication()->getMode() != OgreApplication::CONTROL)
	{
		return false;
	}

	AnimatSimple2D* human = (AnimatSimple2D*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case OIS::KC_UP:
		human->mActionGo = true;
		return true;
	case OIS::KC_RIGHT:
		human->mActionRotate = true;
		human->mActionRotateParam = 1.0f;
		return true;
	case OIS::KC_LEFT:
		human->mActionRotate = true;
		human->mActionRotateParam = -1.0;
		return true;
	case OIS::KC_E:
		human->mActionEat = true;
		return true;
	default:
		return false;
	}
}

bool SimSimple2D::onKeyUp(int key)
{
	switch (key)
	{
	/*case OIS::KC_G:
		setShowGrid(!getShowGrid());
		return true;*/
	case OIS::KC_V:
		setShowViewRange(!getShowViewRange());
		return true;
	default:
		break;
	}

	if (Lab::getSingleton().getOgreApplication()->getMode() != OgreApplication::CONTROL)
	{
		return false;
	}

	AnimatSimple2D* human = (AnimatSimple2D*)(mPopulationDynamics->getHuman());

	if (human == NULL)
	{
		return false;
	}

	switch (key)
	{
	case OIS::KC_UP:
		human->mActionGo = false;
		return true;
	case OIS::KC_RIGHT:
		human->mActionRotate = false;
		return true;
	case OIS::KC_LEFT:
		human->mActionRotate = false;
		return true;
	case OIS::KC_E:
		human->mActionEat = false;
		return true;
	default:
		return false;
	}
}

bool SimSimple2D::onMouseButtonDown(int button, int x, int y)
{
	return false;
}

bool SimSimple2D::onMouseButtonUp(int button, int x, int y)
{
	return false;
}
#endif

const char SimSimple2D::mClassName[] = "SimSimple2D";

Orbit<SimSimple2D>::MethodType SimSimple2D::mMethods[] = {
	{"setPopulationDynamics", &Simulation::setPopulationDynamics},
        {"setWorldDimensions", &SimSimple2D::setWorldDimensions},
        {0,0}
};

Orbit<SimSimple2D>::NumberGlobalType SimSimple2D::mNumberGlobals[] = {
	{"PERCEPTION_NULL", PERCEPTION_NULL},
	{"PERCEPTION_COLOR", PERCEPTION_COLOR},
	{"PERCEPTION_POSITION", PERCEPTION_POSITION},
	{"PERCEPTION_PROXIMITY", PERCEPTION_PROXIMITY},
	{"PERCEPTION_IN_CONTACT", PERCEPTION_IN_CONTACT},
	{"ACTION_NULL", ACTION_NULL},
	{"ACTION_GO", ACTION_GO},
	{"ACTION_ROTATE", ACTION_ROTATE},
	{"ACTION_EAT", ACTION_EAT},
	{0,0}
};

int SimSimple2D::setWorldDimensions(lua_State* luaState)
{
        int width = luaL_checkint(luaState, 1);
        int height = luaL_checkint(luaState, 2);
        int cellSide = luaL_checkint(luaState, 3);
        setWorldDimensions(width, height, cellSide);
        return 0;
}

