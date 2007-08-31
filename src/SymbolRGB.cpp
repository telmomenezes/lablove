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

#include "SymbolRGB.h"
#include "random.h"
#include <math.h>
#include <stdlib.h>

SymbolRGB::SymbolRGB(lua_State* luaState)
{
	if (luaState != NULL)
	{
        	mRed = luaL_checknumber(luaState, 1);
        	mGreen = luaL_checknumber(luaState, 2);
        	mBlue = luaL_checknumber(luaState, 3);
	}
	else
	{
		mRed = 0.0f;
		mGreen = 0.0f;
		mBlue = 0.0f;
	}
}

SymbolRGB::SymbolRGB(float r, float g, float b)
{
	mRed = r;
	mGreen = g;
	mBlue = b;
}

SymbolRGB::SymbolRGB(SymbolRGB* mol)
{
	mRed = mol->mRed;
	mGreen = mol->mGreen;
	mBlue = mol->mBlue;
}

SymbolRGB::~SymbolRGB()
{	
}

Symbol* SymbolRGB::clone()
{
	return new SymbolRGB(this);
}

float SymbolRGB::bind(Symbol* sym)
{
	// TODO: check type
	SymbolRGB* symRGB = (SymbolRGB*)sym;

	double distance = 0.0f;
	distance += fabsf(mRed - symRGB->mRed);
	distance += fabsf(mGreen - symRGB->mGreen);
	distance += fabsf(mBlue - symRGB->mBlue);

	distance = distance / 3.0f;
	return distance;
}

void SymbolRGB::initRandom()
{
	mRed = randomUniformFloat(0.0f, 1.0f);
	mGreen = randomUniformFloat(0.0f, 1.0f);
	mBlue = randomUniformFloat(0.0f, 1.0f);
}

void SymbolRGB::mutate()
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

const char SymbolRGB::mClassName[] = "SymbolRGB";

Orbit<SymbolRGB>::MethodType SymbolRGB::mMethods[] = {
        {0,0}
};

Orbit<SymbolRGB>::NumberGlobalType SymbolRGB::mNumberGlobals[] = {{0,0}};

