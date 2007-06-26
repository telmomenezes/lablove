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

void PopDynFixedSpecies::addSpecies(SimulationObject* org, long population)
{
	org->setSpeciesID(++CURRENT_SPECIES_ID);

	SpeciesData species;
	species.mBaseOrganism = org;
	species.mPopulation = population;
	species.mDeathCount = 0;
	species.mTotalFitness = 0;
	species.mBestFitness = -9999999.9;

	mSpecies.push_back(species);
}

void PopDynFixedSpecies::init()
{
	PopulationDynamics::init();

	for (std::list<SpeciesData>::iterator iterSpecies = mSpecies.begin();
		iterSpecies != mSpecies.end();
		iterSpecies++)
	{
		(*iterSpecies).mBaseOrganism->initRandom();

		for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
		{
			SimulationObject* org = (*iterSpecies).mBaseOrganism->clone();
			org->initRandom();
			org->setEnergy(org->getInitialEnergy());
			org->placeRandom();
#ifdef __LABLOVE_WITH_GRAPHICS
			org->createGraphics();
#endif
			Lab::getSingleton().getSimulation()->addObject(org);
			(*iterSpecies).mOrganismVector.push_back(org);
		}
	}
}

void PopDynFixedSpecies::onCycle()
{
	if ((Lab::getSingleton().getSimulation()->time() % 1000) == 0)
	{
		printf("%d", Lab::getSingleton().getSimulation()->time());
		for (std::list<SpeciesData>::iterator iterSpecies = mSpecies.begin();
			iterSpecies != mSpecies.end();
			iterSpecies++)
		{
			printf(", %f, %f",
				(*iterSpecies).mTotalFitness / (*iterSpecies).mDeathCount,
				(*iterSpecies).mBestFitness);
			(*iterSpecies).mDeathCount = 0;

			(*iterSpecies).mDeathCount = 0;
			(*iterSpecies).mTotalFitness = 0;
			(*iterSpecies).mBestFitness = -9999999.9;
		}
		printf("\n");
	}
}

void PopDynFixedSpecies::onOrganismDeath(SimulationObject* org)
{
	unsigned int speciesPos = 0;
	for (std::list<SpeciesData>::iterator iterSpecies = mSpecies.begin();
		iterSpecies != mSpecies.end();
		iterSpecies++)
	{
		if (org->getSpeciesID() == (*iterSpecies).mBaseOrganism->getSpeciesID())
		{
			std::vector<SimulationObject*>::iterator iterOrg;

			int orgPos = -1;
			int curPos = 0;
			for (iterOrg = (*iterSpecies).mOrganismVector.begin();
				(iterOrg != (*iterSpecies).mOrganismVector.end()) && orgPos < 0;
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
				unsigned int organismNumber = randomUniformInt(0, (*iterSpecies).mPopulation - 1);

				iterOrg = (*iterSpecies).mOrganismVector.begin();
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
#ifdef __LABLOVE_WITH_GRAPHICS
			newOrganism->createGraphics();
#endif
			Lab::getSingleton().getSimulation()->addObject(newOrganism);

			// Mutate
			newOrganism->mutate();

			// Update statistics
			(*iterSpecies).mDeathCount += 1;
			float fitness = org->getEnergy();
			(*iterSpecies).mTotalFitness += fitness;

			if (fitness > (*iterSpecies).mBestFitness)
			{
				(*iterSpecies).mBestFitness = fitness;
			}
			
			// Remove
			(*iterSpecies).mOrganismVector[orgPos] = newOrganism;
			Lab::getSingleton().getSimulation()->removeObject(org);

			return;
		}
	}
}

const char PopDynFixedSpecies::mClassName[] = "PopDynFixedSpecies";

Orbit<PopDynFixedSpecies>::MethodType PopDynFixedSpecies::mMethods[] = {
	{"setHuman", &PopulationDynamics::setHuman},
        {"addSpecies", &PopDynFixedSpecies::addSpecies},
        {0,0}
};

Orbit<PopDynFixedSpecies>::NumberGlobalType PopDynFixedSpecies::mNumberGlobals[] = {{0,0}};

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* luaState)
{
}

int PopDynFixedSpecies::addSpecies(lua_State* luaState)
{
        SimulationObject* obj = (SimulationObject*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
        int population = luaL_checkint(luaState, 2);
        addSpecies(obj, population);
        return 0;
}

