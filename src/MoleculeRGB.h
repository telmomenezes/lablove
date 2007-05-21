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

#if !defined(__INCLUDE_MOLECULE_RGB_H)
#define __INCLUDE_MOLECULE_RGB_H

#include "Molecule.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "luna.h"

class MoleculeRGB : public Molecule
{
public:
	MoleculeRGB();
	MoleculeRGB(float r, float g, float b);
	MoleculeRGB(MoleculeRGB* mol);
	virtual ~MoleculeRGB();
	virtual Molecule* clone();

	virtual float bind(Molecule* mol);
	virtual void init_random();
	virtual void mutate();

	static const char className[];
        static Luna<MoleculeRGB>::RegType methods[];

        MoleculeRGB(lua_State* L);

	float _red;
	float _green;
	float _blue;
};
#endif

