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

#include "MoleculeRGB.h"
#include "random.h"
#include <math.h>
#include <stdlib.h>

MoleculeRGB::MoleculeRGB()
{
	mRed = 0.0f;
	mGreen = 0.0f;
	mBlue = 0.0f;
}

MoleculeRGB::MoleculeRGB(float r, float g, float b)
{
	mRed = r;
	mGreen = g;
	mBlue = b;
}

MoleculeRGB::MoleculeRGB(MoleculeRGB* mol)
{
	mRed = mol->mRed;
	mGreen = mol->mGreen;
	mBlue = mol->mBlue;
}

MoleculeRGB::~MoleculeRGB()
{	
}

Molecule* MoleculeRGB::clone()
{
	return new MoleculeRGB(this);
}

float MoleculeRGB::bind(Molecule* mol)
{
	// TODO: check type
	MoleculeRGB* molRGB = (MoleculeRGB*)mol;

	double distance = 0.0f;
	distance += fabsf(mRed - molRGB->mRed);
	distance += fabsf(mGreen - molRGB->mGreen);
	distance += fabsf(mBlue - molRGB->mBlue);

	distance = distance / 3.0f;
	return distance;
}

void MoleculeRGB::initRandom()
{
	mRed = randomUniformFloat(0.0f, 1.0f);
	mGreen = randomUniformFloat(0.0f, 1.0f);
	mBlue = randomUniformFloat(0.0f, 1.0f);
}

void MoleculeRGB::mutate()
{
	float delta = randomUniformFloat(-1.0f, 1.0f);
	unsigned int selector = random() % 3;

	switch (selector)
	{
	case 0:
		mRed += delta;
		if (mRed > 1.0f)
		{
			mRed = 1.0f;
		}
		else if (mRed < 0.0f)
		{
			mRed = 0.0f;
		}
		break;
	case 1:
		mGreen += delta;
		if (mGreen > 1.0f)
		{
			mGreen = 1.0f;
		}
		else if (mGreen < 0.0f)
		{
			mGreen = 0.0f;
		}
		break;
	case 2:
		mBlue += delta;
		if (mBlue > 1.0f)
		{
			mBlue = 1.0f;
		}
		else if (mBlue < 0.0f)
		{
			mBlue = 0.0f;
		}
		break;
	}
}

const char MoleculeRGB::mClassName[] = "MoleculeRGB";

Orbit<MoleculeRGB>::MethodType MoleculeRGB::mMethods[] = {
        {0,0}
};

Orbit<MoleculeRGB>::NumberGlobalType MoleculeRGB::mNumberGlobals[] = {{0,0}};

MoleculeRGB::MoleculeRGB(lua_State* luaState)
{
        mRed = luaL_checknumber(luaState, 1);
        mGreen = luaL_checknumber(luaState, 2);
        mBlue = luaL_checknumber(luaState, 3);
}

