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

#include "PlantSimple2D.h"
#include "Lab.h"
#include "math.h"
#include "defines.h"
#include "SimSimple2D.h"

PlantSimple2D::PlantSimple2D()
{
	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;
}

PlantSimple2D::PlantSimple2D(lua_State* luaState)
{
	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;
}

PlantSimple2D::~PlantSimple2D()
{	
}

SimulationObject* PlantSimple2D::clone(bool full)
{
	return new PlantSimple2D(this);
}

#if defined(__LABLOVE_WITH_GRAPHICS)
void PlantSimple2D::createGraphics()
{
	Ogre::SceneManager* sceneMgr = Lab::getSingleton().getOgreApplication()->getSceneManager();
	char nodeName[255];
	sprintf(nodeName, "loveobj%d", mID);
	Ogre::Entity* plantEntity = sceneMgr->createEntity(nodeName, "plant");
	mNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	mNode->attachObject(plantEntity);
	mNode->setPosition(mX, 0, mY);
	mNode->yaw(Ogre::Radian(mRot));
	mNode->scale(mSize, mSize, mSize);

	char materialName[255];
	sprintf(materialName, "plantmat%d", mID);
	
	Ogre::MaterialPtr mMaterial = Ogre::MaterialManager::getSingleton().create(
		materialName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMaterial->setAmbient(mColor.mRed, mColor.mGreen, mColor.mBlue);
	plantEntity->setMaterialName(materialName);
}
#endif

const char PlantSimple2D::mClassName[] = "PlantSimple2D";

Orbit<PlantSimple2D>::MethodType PlantSimple2D::mMethods[] = {
	{"setPos", &ObjectSimple2D::setPos},
	{"setRot", &ObjectSimple2D::setRot},
	{"setSize", &ObjectSimple2D::setSize},
	{"setColor", &ObjectSimple2D::setColor},
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"setAgeRange", &ObjectSimple2D::setAgeRange},
	{"setMetabolism", &ObjectSimple2D::setMetabolism},
        {0,0}
};

Orbit<PlantSimple2D>::NumberGlobalType PlantSimple2D::mNumberGlobals[] = {{0,0}};

/*	
void PlantSimple2D::draw()
{
	SimSimple2D* sim = (SimSimple2D*)(Lab::getSingleton().getSimulation());

	float x = (int)mX - (int)sim->getViewX();
	float y = (int)mY - (int)sim->getViewY();

	float p1x = x - mSize;
	float p2x = x + mSize;
	float p1y = y - mSize;
	float p2y = y + mSize;

	glColor3f(mColor.mRed,
			mColor.mGreen,
			mColor.mBlue);
	glBegin(GL_POLYGON);
	glVertex2f(p1x, p1y);
	glVertex2f(p2x, p1y);
	glVertex2f(p2x, p2y);
	glVertex2f(p1x, p2y);
	glEnd();
}
*/
