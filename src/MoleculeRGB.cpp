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
	_red = 0.0f;
	_green = 0.0f;
	_blue = 0.0f;
}

MoleculeRGB::MoleculeRGB(float r, float g, float b)
{
	_red = r;
	_green = g;
	_blue = b;
}

MoleculeRGB::MoleculeRGB(MoleculeRGB* mol)
{
	_red = mol->_red;
	_green = mol->_green;
	_blue = mol->_blue;
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
	MoleculeRGB* mol_rgb = (MoleculeRGB*)mol;

	double distance = 0.0f;
	distance += fabsf(_red - mol_rgb->_red);
	distance += fabsf(_green - mol_rgb->_green);
	distance += fabsf(_blue - mol_rgb->_blue);

	distance = distance / 3.0f;
	return distance;
}

void MoleculeRGB::init_random()
{
	_red = random_uniform_float(0.0f, 1.0f);
	_green = random_uniform_float(0.0f, 1.0f);
	_blue = random_uniform_float(0.0f, 1.0f);
}

void MoleculeRGB::mutate()
{
	float delta = random_uniform_float(-1.0f, 1.0f);
	unsigned int selector = random() % 3;

	switch (selector)
	{
	case 0:
		_red += delta;
		if (_red > 1.0f)
		{
			_red = 1.0f;
		}
		else if (_red < 0.0f)
		{
			_red = 0.0f;
		}
		break;
	case 1:
		_green += delta;
		if (_green > 1.0f)
		{
			_green = 1.0f;
		}
		else if (_green < 0.0f)
		{
			_green = 0.0f;
		}
		break;
	case 2:
		_blue += delta;
		if (_blue > 1.0f)
		{
			_blue = 1.0f;
		}
		else if (_blue < 0.0f)
		{
			_blue = 0.0f;
		}
		break;
	}
}

const char MoleculeRGB::className[] = "MoleculeRGB";

Luna<MoleculeRGB>::RegType MoleculeRGB::methods[] = {
        {0,0}
};

MoleculeRGB::MoleculeRGB(lua_State* L)
{
        _red = luaL_checknumber(L, 1);
        _green = luaL_checknumber(L, 2);
        _blue = luaL_checknumber(L, 3);
}

