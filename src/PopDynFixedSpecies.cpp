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
#include "Lab.h"
#include "SimulationObject.h"
#include "functions.h"
#include "random.h"

unsigned int PopDynFixedSpecies::CURRENT_SPECIES_ID = 0;

PopDynFixedSpecies::PopDynFixedSpecies()
{
}

PopDynFixedSpecies::~PopDynFixedSpecies()
{
}

void PopDynFixedSpecies::addStaticSpecies(SimulationObject* org, long population)
{
	org->setSpeciesID(++CURRENT_SPECIES_ID);
	mStaticSpecies.push_back(org);
	mStaticSpeciesPopulations.push_back(population);
	vector<SimulationObject*>* orgVec = new vector<SimulationObject*>();
	mStaticSpeciesOrganismVecs.push_back(orgVec);
}

void PopDynFixedSpecies::addEvolvingSpecies(SimulationObject* org, long population)
{
	org->setSpeciesID(++CURRENT_SPECIES_ID);
	mEvolvingSpecies.push_back(org);
	mEvolvingSpeciesPopulations.push_back(population);
	vector<SimulationObject*>* orgVec = new vector<SimulationObject*>();
	mEvolvingSpeciesOrganismVecs.push_back(orgVec);
	mEvolvingSpeciesDeathCount.push_back(0);
	mEvolvingSpeciesFitness.push_back(0);
	mEvolvingSpeciesBestFitness.push_back(-9999999.9);
}

void PopDynFixedSpecies::init()
{
	list<SimulationObject*>::iterator iterSpecies = mStaticSpecies.begin();
	list<long>::iterator iterPop = mStaticSpeciesPopulations.begin();
	list<vector<SimulationObject*>*>::iterator iterSpeciesOrgVec = mStaticSpeciesOrganismVecs.begin();

	while (iterSpecies != mStaticSpecies.end())
	{
		(*iterSpecies)->initRandom();

		for (unsigned int i = 0; i < (*iterPop); i++)
		{
			SimulationObject* org = (*iterSpecies)->clone();
			org->initRandom();
			org->placeRandom();
#ifdef __LOVELAB_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getInstance().getSimulation()->addObject(org);
			(*iterSpeciesOrgVec)->push_back(org);
		}

		iterSpecies++;
		iterPop++;
		iterSpeciesOrgVec++;
	}

	iterSpecies = mEvolvingSpecies.begin();
	iterPop = mEvolvingSpeciesPopulations.begin();
	iterSpeciesOrgVec = mEvolvingSpeciesOrganismVecs.begin();
	while (iterSpecies != mEvolvingSpecies.end())
	{
		(*iterSpecies)->initRandom();

		for (unsigned int i = 0; i < (*iterPop); i++)
		{
			SimulationObject* org = (*iterSpecies)->clone(false);
			org->initRandom();
			org->setEnergy(org->getInitialEnergy());
			org->placeRandom();
#ifdef __LOVELAB_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getInstance().getSimulation()->addObject(org);
			(*iterSpeciesOrgVec)->push_back(org);
		}

		iterSpecies++;
		iterPop++;
		iterSpeciesOrgVec++;
	}
}

void PopDynFixedSpecies::onCycle()
{
	if ((Lab::getInstance().getSimulation()->time() % 1000) == 0)
	{
		printf("%d", Lab::getInstance().getSimulation()->time());
		for (unsigned int i = 0; i < mEvolvingSpeciesFitness.size(); i++)
		{
			printf(", %f, %f",
				mEvolvingSpeciesFitness[i] / mEvolvingSpeciesDeathCount[i],
				mEvolvingSpeciesBestFitness[i]);
			mEvolvingSpeciesDeathCount[i] = 0;
			mEvolvingSpeciesFitness[i] = 0;
			mEvolvingSpeciesBestFitness[i] = -9999999.9;
		}
		printf("\n");
	}
}

void PopDynFixedSpecies::onOrganismDeath(SimulationObject* org)
{
	list<SimulationObject*>::iterator iterSpecies = mStaticSpecies.begin();
	while (iterSpecies != mStaticSpecies.end())
	{
		if (org->getSpeciesID() == (*iterSpecies)->getSpeciesID())
		{
			Lab::getInstance().getSimulation()->removeObject(org);

			SimulationObject* org = (*iterSpecies)->clone();
			org->setEnergy(org->getInitialEnergy());
			org->placeRandom();
#ifdef __LOVELAB_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getInstance().getSimulation()->addObject(org);
			return;
		}

		iterSpecies++;
	}

	iterSpecies = mEvolvingSpecies.begin();
	list<long>::iterator iterPop = mEvolvingSpeciesPopulations.begin();
	list<vector<SimulationObject*>*>::iterator iterSpeciesOrgVec = mEvolvingSpeciesOrganismVecs.begin();
	unsigned int speciesPos = 0;
	while (iterSpecies != mEvolvingSpecies.end())
	{
		if (org->getSpeciesID() == (*iterSpecies)->getSpeciesID())
		{
			vector<SimulationObject*>::iterator iterOrg;

			int orgPos = -1;
			int curPos = 0;
			for (iterOrg = (*iterSpeciesOrgVec)->begin();
				(iterOrg != (*iterSpeciesOrgVec)->end()) && orgPos < 0;
				iterOrg++)
			{
				if ((*iterOrg) == org)
				{
					orgPos = curPos;
				}
				curPos++;
			}

			SimulationObject* newOrganism = NULL;

			// Tournment selection of 2
			SimulationObject* bestOrganism = NULL;
			float bestFitness = 0.0f;

			for (unsigned int tournmentStep = 0; tournmentStep < 2; tournmentStep++)
			{
				unsigned int organismNumber = randomUniformInt(0, (*iterPop) - 1);

				iterOrg = (*iterSpeciesOrgVec)->begin();
				for (unsigned int i = 0; i < organismNumber; i++)
				{
					iterOrg++;
				}

				if ((tournmentStep == 0) || ((*iterOrg)->getEnergy() > bestFitness))
				{
					bestOrganism = (*iterOrg);
					bestFitness = bestOrganism->getEnergy();
				}
			}

			// Clone best and add to simulation
			newOrganism = bestOrganism->clone();
			newOrganism->setEnergy(newOrganism->getInitialEnergy());
			newOrganism->placeRandom();
#ifdef __LOVELAB_WITH_GRAPHICS
			newOrganism->createGraphics();
#endif
			Lab::getInstance().getSimulation()->addObject(newOrganism);

			// Mutate
			newOrganism->mutate();

			// Update statistics
			mEvolvingSpeciesDeathCount[speciesPos] += 1;
			float fitness = org->getEnergy();
			mEvolvingSpeciesFitness[speciesPos] += fitness;

			if (fitness > mEvolvingSpeciesBestFitness[speciesPos])
			{
				mEvolvingSpeciesBestFitness[speciesPos] = fitness;
			}
			
			// Remove
			(*(*iterSpeciesOrgVec))[orgPos] = newOrganism;
			Lab::getInstance().getSimulation()->removeObject(org);

			return;
		}

		iterSpecies++;
		iterPop++;
		iterSpeciesOrgVec++;
		speciesPos++;
	}
}

const char PopDynFixedSpecies::mClassName[] = "PopDynFixedSpecies";

Orbit<PopDynFixedSpecies>::MethodType PopDynFixedSpecies::mMethods[] = {
        {"addStaticSpecies", &PopDynFixedSpecies::addStaticSpecies},
        {"addEvolvingSpecies", &PopDynFixedSpecies::addEvolvingSpecies},
        {0,0}
};

Orbit<PopDynFixedSpecies>::NumberGlobalType PopDynFixedSpecies::mNumberGlobals[] = {{0,0}};

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* luaState)
{
}

int PopDynFixedSpecies::addStaticSpecies(lua_State* luaState)
{
        SimulationObject* obj = (SimulationObject*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
        int population = luaL_checkint(luaState, 2);
        addStaticSpecies(obj, population);
        return 0;
}

int PopDynFixedSpecies::addEvolvingSpecies(lua_State* luaState)
{
        SimulationObject* obj = (SimulationObject*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
        int population = luaL_checkint(luaState, 2);
        addEvolvingSpecies(obj, population);
        return 0;
}

