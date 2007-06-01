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

Chemistry::Chemistry()
{
}

Chemistry::~Chemistry()
{
	vector<Molecule*>::iterator iter_molecule;

	for (iter_molecule = _molecule_vec.begin();
		iter_molecule != _molecule_vec.end();
		iter_molecule++)
	{
		delete (*iter_molecule);
	}

	_molecule_vec.clear();
}

Chemistry* Chemistry::clone()
{
	Chemistry* mt = new Chemistry();

	vector<Molecule*>::iterator iter_molecule;

	for (iter_molecule = _molecule_vec.begin();
		iter_molecule != _molecule_vec.end();
		iter_molecule++)
	{
		mt->_molecule_vec.push_back((*iter_molecule)->clone());
	}

	return mt;
}

Molecule* Chemistry::get_molecule(unsigned int index)
{
	return _molecule_vec[index];
}

void Chemistry::add_molecule(Molecule* mol)
{
	_molecule_vec.push_back(mol);
}

void Chemistry::mutate()
{
	unsigned int index = random() % _molecule_vec.size();
	_molecule_vec[index]->mutate();
}

