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
#include <string>

using namespace std;

class Chemistry
{
public:
	Chemistry(string name, Molecule* ref_molecule);
	virtual ~Chemistry();
	Chemistry* clone();

	Molecule* get_molecule(unsigned int index);
	void add_molecule(Molecule* mol);
	void mutate();

	static const char class_name[];
        static Orbit<Chemistry>::MethodType methods[];
	static Orbit<Chemistry>::NumberGlobalType number_globals[];

        Chemistry(lua_State* L);

protected:
	string _name;
	Molecule* _reference_molecule;
	vector<Molecule*> _molecule_vec;
};
#endif

