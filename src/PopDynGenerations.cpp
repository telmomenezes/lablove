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

#include "PopDynGenerations.h"
#include "LoveLab.h"
#include "SimulationObject.h"
#include "functions.h"
#include "random.h"

unsigned int PopDynGenerations::CURRENT_SPECIES_ID = 0;

PopDynGenerations::PopDynGenerations()
{
	_generation_time = 0;
	_generation = 0;
}

PopDynGenerations::~PopDynGenerations()
{
	// delete species
	// delete lists
}

void PopDynGenerations::add_static_species(SimulationObject* org, long population)
{
	org->set_species_id(++CURRENT_SPECIES_ID);
	_static_species.push_back(org);
	_static_species_populations.push_back(population);
	list<SimulationObject*>* org_list = new list<SimulationObject*>();
	_static_species_organism_lists.push_back(org_list);
}

void PopDynGenerations::add_evolving_species(SimulationObject* org, long population)
{
	org->set_species_id(++CURRENT_SPECIES_ID);
	_evolving_species.push_back(org);
	_evolving_species_populations.push_back(population);
	list<SimulationObject*>* org_list = new list<SimulationObject*>();
	_evolving_species_organism_lists.push_back(org_list);
}

void PopDynGenerations::init()
{
	list<SimulationObject*>::iterator iter_species = _static_species.begin();
	list<long>::iterator iter_pop = _static_species_populations.begin();
	list<list<SimulationObject*>*>::iterator iter_species_org_list = _static_species_organism_lists.begin();

	while (iter_species != _static_species.end())
	{
		for (unsigned int i = 0; i < (*iter_pop); i++)
		{
			SimulationObject* org = (*iter_species)->clone();
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
			(*iter_species_org_list)->push_back(org);
		}

		iter_species++;
		iter_pop++;
		iter_species_org_list++;
	}

	iter_species = _evolving_species.begin();
	iter_pop = _evolving_species_populations.begin();
	iter_species_org_list = _evolving_species_organism_lists.begin();
	while (iter_species != _evolving_species.end())
	{
		(*iter_species)->init();

		for (unsigned int i = 0; i < (*iter_pop); i++)
		{
			SimulationObject* org = (*iter_species)->clone(false);
			org->init();
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
			(*iter_species_org_list)->push_back(org);
		}

		iter_species++;
		iter_pop++;
		iter_species_org_list++;
	}
}

void PopDynGenerations::on_cycle()
{
	if (((LoveLab::get_instance().get_simulation()->time() % _generation_time) == 0)
		&& (LoveLab::get_instance().get_simulation()->time() != 0))
	{
		list<SimulationObject*>::iterator iter_species = _evolving_species.begin();
		list<long>::iterator iter_pop = _evolving_species_populations.begin();
		list<list<SimulationObject*>*>::iterator iter_species_org_list = _evolving_species_organism_lists.begin();
		while (iter_species != _evolving_species.end())
		{
			double best_fitness = -9999999999.0f;
			double total_energy = 0.0f;

			list<SimulationObject*>::iterator iter_org;
			for (iter_org = (*iter_species_org_list)->begin();
				iter_org != (*iter_species_org_list)->end();
				++iter_org)
			{
				SimulationObject* org = *iter_org;
				total_energy += org->get_energy();
				if (org->get_energy() > best_fitness)
				{
					best_fitness = org->get_energy();
				}
			}
			double average_fitness = total_energy / (*iter_pop);
		

			printf("%d, %f, %f\n", _generation, average_fitness, best_fitness);

			// Create next generation
			SimulationObject* new_org_list = NULL;
			SimulationObject* new_organism = NULL;
			SimulationObject* prev_new_organism = NULL;

			for (unsigned int i = 0; i < (*iter_pop); i++)
			{
				// Tournment selection of 2
				SimulationObject* best_organism = NULL;
				best_fitness = 0.0f;

				for (unsigned int tournment_step = 0; tournment_step < 2; tournment_step++)
				{
					unsigned int organism_number = random_uniform_int(0, (*iter_pop) - 1);

					iter_org = (*iter_species_org_list)->begin();
					for (unsigned int i = 0; i < organism_number; i++)
					{
						iter_org++;
					}
					SimulationObject* org = *iter_org;

					if ((tournment_step == 0) || (org->get_energy() > best_fitness))
					{
						best_organism = org;
						best_fitness = org->get_energy();
					}
				}

				// Clone best and add to simulation
				new_organism = best_organism->clone();
				new_organism->set_energy(0.0f);
				new_organism->place_random();
				LoveLab::get_instance().get_simulation()->add_object(new_organism);

				// Mutate
				new_organism->mutate();

				(*iter_species_org_list)->push_back(new_organism);
			}

			// Delete old population
			for (unsigned int i = 0; i < (*iter_pop); i++)
			{
				SimulationObject* org = (*iter_species_org_list)->front();
				(*iter_species_org_list)->pop_front();
				LoveLab::get_instance().get_simulation()->remove_object(org);
			}

			iter_species++;
			iter_pop++;
			iter_species_org_list++;
			_generation++;
		}
	}
}

void PopDynGenerations::on_organism_death(SimulationObject* org)
{
	list<SimulationObject*>::iterator iter_species = _static_species.begin();
	while (iter_species != _static_species.end())
	{
		if (org->get_species_id() == (*iter_species)->get_species_id())
		{
			LoveLab::get_instance().get_simulation()->remove_object(org);

			SimulationObject* org = (*iter_species)->clone();
			org->place_random();
			LoveLab::get_instance().get_simulation()->add_object(org);
		}

		iter_species++;
	}
}

const char PopDynGenerations::className[] = "PopDynGenerations";

Lunar<PopDynGenerations>::RegType PopDynGenerations::methods[] = {
        {"add_static_species", &PopDynGenerations::add_static_species},
        {"add_evolving_species", &PopDynGenerations::add_evolving_species},
	{"set_generation_time", &PopDynGenerations::set_generation_time},
        {0,0}
};

PopDynGenerations::PopDynGenerations(lua_State* L)
{
	_generation_time = 0;
	_generation = 0;
}

int PopDynGenerations::add_static_species(lua_State *L)
{
        SimulationObject* obj = (SimulationObject*)Lunar<PopDynGenerations>::pointer(L, 1);
        int population = luaL_checkint(L, 2);
        add_static_species(obj, population);
        return 0;
}

int PopDynGenerations::add_evolving_species(lua_State *L)
{
        SimulationObject* obj = (SimulationObject*)Lunar<PopDynGenerations>::pointer(L, 1);
        int population = luaL_checkint(L, 2);
        add_evolving_species(obj, population);
        return 0;
}

int PopDynGenerations::set_generation_time(lua_State* L)
{
        int time = luaL_checkint(L, 1);
        set_generation_time(time);
        return 0;
}

