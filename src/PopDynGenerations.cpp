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
#include "Lab.h"
#include "SimulationObject.h"
#include "functions.h"
#include "random.h"

unsigned int PopDynGenerations::CURRENT_SPECIES_ID = 0;

PopDynGenerations::PopDynGenerations(lua_State* luaState)
{
	mGenerationTime = 0;
	mGeneration = 0;
}

PopDynGenerations::~PopDynGenerations()
{
	// delete species
	// delete lists
}

void PopDynGenerations::addStaticSpecies(SimulationObject* org, long population)
{
	org->setSpeciesID(++CURRENT_SPECIES_ID);
	mStaticSpecies.push_back(org);
	mStaticSpeciesPopulations.push_back(population);
	std::list<SimulationObject*>* orgList = new std::list<SimulationObject*>();
	mStaticSpeciesOrganismLists.push_back(orgList);
}

void PopDynGenerations::addEvolvingSpecies(SimulationObject* org, long population)
{
	org->setSpeciesID(++CURRENT_SPECIES_ID);
	mEvolvingSpecies.push_back(org);
	mEvolvingSpeciesPopulations.push_back(population);
	std::list<SimulationObject*>* orgList = new std::list<SimulationObject*>();
	mEvolvingSpeciesOrganismLists.push_back(orgList);
}

void PopDynGenerations::init()
{
	PopulationDynamics::init();

	std::list<SimulationObject*>::iterator iterSpecies = mStaticSpecies.begin();
	std::list<long>::iterator iterPop = mStaticSpeciesPopulations.begin();
	std::list<std::list<SimulationObject*>*>::iterator iterSpeciesOrgList = mStaticSpeciesOrganismLists.begin();

	while (iterSpecies != mStaticSpecies.end())
	{
		(*iterSpecies)->initRandom();

		for (unsigned int i = 0; i < (*iterPop); i++)
		{
			SimulationObject* org = (*iterSpecies)->clone();
			org->initRandom();
			org->placeRandom();
			org->setEnergy(org->getInitialEnergy());
#ifdef __LABLOVE_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getSingleton().getSimulation()->addObject(org);
			(*iterSpeciesOrgList)->push_back(org);
		}

		iterSpecies++;
		iterPop++;
		iterSpeciesOrgList++;
	}

	iterSpecies = mEvolvingSpecies.begin();
	iterPop = mEvolvingSpeciesPopulations.begin();
	iterSpeciesOrgList = mEvolvingSpeciesOrganismLists.begin();
	while (iterSpecies != mEvolvingSpecies.end())
	{
		(*iterSpecies)->initRandom();

		for (unsigned int i = 0; i < (*iterPop); i++)
		{
			SimulationObject* org = (*iterSpecies)->clone(false);
			org->initRandom();
			org->placeRandom();
			org->setEnergy(org->getInitialEnergy());
#ifdef __LABLOVE_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getSingleton().getSimulation()->addObject(org);
			(*iterSpeciesOrgList)->push_back(org);
		}

		iterSpecies++;
		iterPop++;
		iterSpeciesOrgList++;
	}
}

void PopDynGenerations::onCycle()
{
	if (((Lab::getSingleton().getSimulation()->time() % mGenerationTime) == 0)
		&& (Lab::getSingleton().getSimulation()->time() != 0))
	{
		std::list<SimulationObject*>::iterator iterSpecies = mEvolvingSpecies.begin();
		std::list<long>::iterator iterPop = mEvolvingSpeciesPopulations.begin();
		std::list<std::list<SimulationObject*>*>::iterator iterSpeciesOrgList
			= mEvolvingSpeciesOrganismLists.begin();
		while (iterSpecies != mEvolvingSpecies.end())
		{
			double bestFitness = -9999999999.0f;
			double totalEnergy = 0.0f;

			std::list<SimulationObject*>::iterator iterOrg;
			for (iterOrg = (*iterSpeciesOrgList)->begin();
				iterOrg != (*iterSpeciesOrgList)->end();
				++iterOrg)
			{
				SimulationObject* org = *iterOrg;
				totalEnergy += org->getEnergy();
				if (org->getEnergy() > bestFitness)
				{
					bestFitness = org->getEnergy();
				}
			}
			double averageFitness = totalEnergy / (*iterPop);
		

			printf("%d, %f, %f\n", mGeneration, averageFitness, bestFitness);

			// Create next generation
			SimulationObject* newOrgList = NULL;
			SimulationObject* newOrganism = NULL;
			SimulationObject* prevNewOrganism = NULL;

			for (unsigned int i = 0; i < (*iterPop); i++)
			{
				// Tournment selection of 2
				SimulationObject* bestOrganism = NULL;
				bestFitness = 0.0f;

				for (unsigned int tournmentStep = 0; tournmentStep < 2; tournmentStep++)
				{
					unsigned int organismNumber = randomUniformInt(0, (*iterPop) - 1);

					iterOrg = (*iterSpeciesOrgList)->begin();
					for (unsigned int i = 0; i < organismNumber; i++)
					{
						iterOrg++;
					}
					SimulationObject* org = *iterOrg;

					if ((tournmentStep == 0) || (org->getEnergy() > bestFitness))
					{
						bestOrganism = org;
						bestFitness = org->getEnergy();
					}
				}

				// Clone best and add to simulation
				newOrganism = bestOrganism->clone();
				newOrganism->setEnergy(newOrganism->getInitialEnergy());
				newOrganism->placeRandom();
#ifdef __LABLOVE_WITH_GRAPHICS
				newOrganism->createGraphics();
#endif
				Lab::getSingleton().getSimulation()->addObject(newOrganism);

				// Mutate
				newOrganism->mutate();

				(*iterSpeciesOrgList)->push_back(newOrganism);
			}

			// Delete old population
			for (unsigned int i = 0; i < (*iterPop); i++)
			{
				SimulationObject* org = (*iterSpeciesOrgList)->front();
				(*iterSpeciesOrgList)->pop_front();
				Lab::getSingleton().getSimulation()->removeObject(org);
			}

			iterSpecies++;
			iterPop++;
			iterSpeciesOrgList++;
			mGeneration++;
		}
	}
}

void PopDynGenerations::onOrganismDeath(SimulationObject* org)
{
	std::list<SimulationObject*>::iterator iterSpecies = mStaticSpecies.begin();
	while (iterSpecies != mStaticSpecies.end())
	{
		if (org->getSpeciesID() == (*iterSpecies)->getSpeciesID())
		{
			Lab::getSingleton().getSimulation()->removeObject(org);

			SimulationObject* org = (*iterSpecies)->clone();
			org->placeRandom();
			org->setEnergy(org->getInitialEnergy());
#ifdef __LABLOVE_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getSingleton().getSimulation()->addObject(org);
		}

		iterSpecies++;
	}
}

const char PopDynGenerations::mClassName[] = "PopDynGenerations";

Orbit<PopDynGenerations>::MethodType PopDynGenerations::mMethods[] = {
	{"setHuman", &PopulationDynamics::setHuman},
        {"addStaticSpecies", &PopDynGenerations::addStaticSpecies},
        {"addEvolvingSpecies", &PopDynGenerations::addEvolvingSpecies},
	{"setGenerationTime", &PopDynGenerations::setGenerationTime},
        {0,0}
};

Orbit<PopDynGenerations>::NumberGlobalType PopDynGenerations::mNumberGlobals[] = {{0,0}};

int PopDynGenerations::addStaticSpecies(lua_State* luaState)
{
        SimulationObject* obj = (SimulationObject*)Orbit<PopDynGenerations>::pointer(luaState, 1);
        int population = luaL_checkint(luaState, 2);
        addStaticSpecies(obj, population);
        return 0;
}

int PopDynGenerations::addEvolvingSpecies(lua_State* luaState)
{
        SimulationObject* obj = (SimulationObject*)Orbit<PopDynGenerations>::pointer(luaState, 1);
        int population = luaL_checkint(luaState, 2);
        addEvolvingSpecies(obj, population);
        return 0;
}

int PopDynGenerations::setGenerationTime(lua_State* luaState)
{
        int time = luaL_checkint(luaState, 1);
        setGenerationTime(time);
        return 0;
}

