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

Chemistry::Chemistry(std::string name, Molecule* refMolecule)
{
	mName = name;
	mReferenceMolecule = refMolecule;
}

Chemistry::Chemistry(lua_State* luaState)
{
	const char* name = luaL_checkstring(luaState, 1);
	Molecule* molecule = (Molecule*)Orbit<Chemistry>::pointer(luaState, 2);

	mName = std::string(name);
	mReferenceMolecule = molecule;
}

Chemistry::~Chemistry()
{
	std::vector<Molecule*>::iterator iterMolecule;

	for (iterMolecule = mMoleculeVec.begin();
		iterMolecule != mMoleculeVec.end();
		iterMolecule++)
	{
		delete (*iterMolecule);
	}

	mMoleculeVec.clear();
}

Chemistry* Chemistry::clone()
{
	Chemistry* mt = new Chemistry(mName, mReferenceMolecule->clone());

	std::vector<Molecule*>::iterator iterMolecule;

	for (iterMolecule = mMoleculeVec.begin();
		iterMolecule != mMoleculeVec.end();
		iterMolecule++)
	{
		mt->mMoleculeVec.push_back((*iterMolecule)->clone());
	}

	return mt;
}

Molecule* Chemistry::getMolecule(unsigned int index)
{
	return mMoleculeVec[index];
}

void Chemistry::addMolecule(Molecule* mol)
{
	mMoleculeVec.push_back(mol);
}

void Chemistry::mutate()
{
	unsigned int index = random() % mMoleculeVec.size();
	mMoleculeVec[index]->mutate();
}

const char Chemistry::mClassName[] = "Chemistry";

Orbit<Chemistry>::MethodType Chemistry::mMethods[] = {
        {0,0}
};

Orbit<Chemistry>::NumberGlobalType Chemistry::mNumberGlobals[] = {{0,0}};

