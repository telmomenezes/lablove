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

#if !defined(__INCLUDE_CHEMISTRY_H)
#define __INCLUDE_CHEMISTRY_H

#include "Molecule.h"
#include "Orbit.h"

#include <vector>

class Chemistry
{
public:
        Chemistry(lua_State* luaState);
	Chemistry(Molecule* refMolecule);
        Chemistry(Chemistry* chem);
	virtual ~Chemistry();

	Molecule* getMolecule(unsigned int index);
	unsigned int addMolecule(Molecule* mol);
	void mutate();

	int addMolecule(lua_State* luaState);

	static const char mClassName[];
        static Orbit<Chemistry>::MethodType mMethods[];
	static Orbit<Chemistry>::NumberGlobalType mNumberGlobals[];


protected:
	Molecule* mReferenceMolecule;
	std::vector<Molecule*> mMolecules;
};
#endif

