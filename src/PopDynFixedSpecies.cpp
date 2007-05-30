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

#include "PopDynFixedSpecies.h"
#include "LoveLab.h"
#include "Object.h"
#include "functions.h"
#include "random.h"

unsigned int PopDynFixedSpecies::CURRENT_SPECIES_ID = 0;

PopDynFixedSpecies::PopDynFixedSpecies()
{
}

PopDynFixedSpecies::~PopDynFixedSpecies()
{
}

void PopDynFixedSpecies::add_static_species(Object* org, long population)
{
	org->set_species_id(++CURRENT_SPECIES_ID);
	_static_species.push_back(org);
	_static_species_populations.push_back(population);
	vector<Object*>* org_vec = new vector<Object*>();
	_static_species_organism_vecs.push_back(org_vec);
}

void PopDynFixedSpecies::add_evolving_species(Object* org, long population)
{
	org->set_species_id(++CURRENT_SPECIES_ID);
	_evolving_species.push_back(org);
	_evolving_species_populations.push_back(population);
	vector<Object*>* org_vec = new vector<Object*>();
	_evolving_species_organism_vecs.push_back(org_vec);
	_evolving_species_death_count.push_back(0);
	_evolving_species_fitness.push_back(0);
	_evolving_species_best_fitness.push_back(-9999999.9);
}

void PopDynFixedSpecies::init()
{
	list<Object*>::iterator iter_species = _static_species.begin();
	list<long>::iterator iter_pop = _static_species_populations.begin();
	list<vector<Object*>*>::iterator iter_species_org_vec = _static_species_organism_vecs.begin();

	while (iter_species != _static_species.end())
	{
		for (unsigned int i = 0; i < (*iter_pop); i++)
		{
			Object* org = (*iter_species)->clone();
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
			(*iter_species_org_vec)->push_back(org);
		}

		iter_species++;
		iter_pop++;
		iter_species_org_vec++;
	}

	iter_species = _evolving_species.begin();
	iter_pop = _evolving_species_populations.begin();
	iter_species_org_vec = _evolving_species_organism_vecs.begin();
	while (iter_species != _evolving_species.end())
	{
		(*iter_species)->init();

		for (unsigned int i = 0; i < (*iter_pop); i++)
		{
			Object* org = (*iter_species)->clone(false);
			org->init();
			org->set_energy(org->get_initial_energy());
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
			(*iter_species_org_vec)->push_back(org);
		}

		iter_species++;
		iter_pop++;
		iter_species_org_vec++;
	}
}

void PopDynFixedSpecies::on_cycle()
{
	if ((LoveLab::get_instance().get_simulation()->time() % 1000) == 0)
	{
		printf("%d", LoveLab::get_instance().get_simulation()->time());
		for (unsigned int i = 0; i < _evolving_species_fitness.size(); i++)
		{
			printf(", %f, %f",
				_evolving_species_fitness[i] / _evolving_species_death_count[i],
				_evolving_species_best_fitness[i]);
			_evolving_species_death_count[i] = 0;
			_evolving_species_fitness[i] = 0;
			_evolving_species_best_fitness[i] = -9999999.9;
		}
		printf("\n");
	}
}

void PopDynFixedSpecies::on_organism_death(Object* org)
{
	list<Object*>::iterator iter_species = _static_species.begin();
	while (iter_species != _static_species.end())
	{
		if (org->get_species_id() == (*iter_species)->get_species_id())
		{
			LoveLab::get_instance().get_simulation()->remove_object(org);

			Object* org = (*iter_species)->clone();
			org->set_energy(org->get_initial_energy());
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
			return;
		}

		iter_species++;
	}

	iter_species = _evolving_species.begin();
	list<long>::iterator iter_pop = _evolving_species_populations.begin();
	list<vector<Object*>*>::iterator iter_species_org_vec = _evolving_species_organism_vecs.begin();
	unsigned int species_pos = 0;
	while (iter_species != _evolving_species.end())
	{
		if (org->get_species_id() == (*iter_species)->get_species_id())
		{
			vector<Object*>::iterator iter_org;

			int org_pos = -1;
			int cur_pos = 0;
			for (iter_org = (*iter_species_org_vec)->begin();
				(iter_org != (*iter_species_org_vec)->end()) && org_pos < 0;
				iter_org++)
			{
				if ((*iter_org) == org)
				{
					org_pos = cur_pos;
				}
				cur_pos++;
			}

			Object* new_organism = NULL;

			// Tournment selection of 2
			Object* best_organism = NULL;
			float best_fitness = 0.0f;

			for (unsigned int tournment_step = 0; tournment_step < 2; tournment_step++)
			{
				unsigned int organism_number = random_uniform_int(0, (*iter_pop) - 1);

				iter_org = (*iter_species_org_vec)->begin();
				for (unsigned int i = 0; i < organism_number; i++)
				{
					iter_org++;
				}

				if ((tournment_step == 0) || ((*iter_org)->get_energy() > best_fitness))
				{
					best_organism = (*iter_org);
					best_fitness = best_organism->get_energy();
				}
			}

			// Clone best and add to simulation
			new_organism = best_organism->clone();
			new_organism->set_energy(new_organism->get_initial_energy());
			new_organism->place_random();
			LoveLab::get_instance().get_simulation()->add_object(new_organism);

			// Mutate
			new_organism->mutate();

			// Update statistics
			_evolving_species_death_count[species_pos] += 1;
			float fitness = org->get_energy();
			_evolving_species_fitness[species_pos] += fitness;

			if (fitness > _evolving_species_best_fitness[species_pos])
			{
				_evolving_species_best_fitness[species_pos] = fitness;
			}
			
			// Remove
			(*(*iter_species_org_vec))[org_pos] = new_organism;
			LoveLab::get_instance().get_simulation()->remove_object(org);

			return;
		}

		iter_species++;
		iter_pop++;
		iter_species_org_vec++;
		species_pos++;
	}
}

const char PopDynFixedSpecies::className[] = "PopDynFixedSpecies";

Luna<PopDynFixedSpecies>::RegType PopDynFixedSpecies::methods[] = {
        {"add_static_species", &PopDynFixedSpecies::add_static_species},
        {"add_evolving_species", &PopDynFixedSpecies::add_evolving_species},
        {0,0}
};

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* L)
{
}

int PopDynFixedSpecies::add_static_species(lua_State *L)
{
        Object* obj = (Object*)Luna<PopDynFixedSpecies>::pointer(L, 1);
        int population = luaL_checkint(L, 2);
        add_static_species(obj, population);
        return 0;
}

int PopDynFixedSpecies::add_evolving_species(lua_State *L)
{
        Object* obj = (Object*)Luna<PopDynFixedSpecies>::pointer(L, 1);
        int population = luaL_checkint(L, 2);
        add_evolving_species(obj, population);
        return 0;
}

