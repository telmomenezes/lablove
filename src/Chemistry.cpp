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

#include "Chemistry.h"

Chemistry::Chemistry(Molecule* refMolecule)
{
	mReferenceMolecule = refMolecule;
}

Chemistry::Chemistry(lua_State* luaState)
{
	Molecule* molecule = (Molecule*)Orbit<Chemistry>::pointer(luaState, 1);

	mReferenceMolecule = molecule;
}

Chemistry::Chemistry(Chemistry* chem)
{
	mReferenceMolecule = chem->mReferenceMolecule->clone();

	std::vector<Molecule*>::iterator iterMolecule;

	for (iterMolecule = chem->mMolecules.begin();
		iterMolecule != chem->mMolecules.end();
		iterMolecule++)
	{
		mMolecules.push_back((*iterMolecule)->clone());
	}
}

Chemistry::~Chemistry()
{
	std::vector<Molecule*>::iterator iterMolecule;

	for (iterMolecule = mMolecules.begin();
		iterMolecule != mMolecules.end();
		iterMolecule++)
	{
		delete (*iterMolecule);
	}

	mMolecules.clear();
}

Molecule* Chemistry::getMolecule(unsigned int index)
{
	return mMolecules[index];
}

unsigned int Chemistry::addMolecule(Molecule* mol)
{
	unsigned int pos = mMolecules.size();
	mMolecules.push_back(mol);
	return pos;
}

void Chemistry::mutate()
{
	unsigned int index = random() % mMolecules.size();
	mMolecules[index]->mutate();
}

int Chemistry::addMolecule(lua_State* luaState)
{
	Molecule* mol = (Molecule*)Orbit<Chemistry>::pointer(luaState, 1);
	unsigned int index = addMolecule(mol);
	lua_pushnumber(luaState, index);
	return 1;
}

const char Chemistry::mClassName[] = "Chemistry";

Orbit<Chemistry>::MethodType Chemistry::mMethods[] = {
	{"addMolecule", &Chemistry::addMolecule},
        {0,0}
};

Orbit<Chemistry>::NumberGlobalType Chemistry::mNumberGlobals[] = {{0,0}};

