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

#if !defined(__INCLUDE_POPULATION_DYNAMICS_H)
#define __INCLUDE_POPULATION_DYNAMICS_H

#include "SimulationObject.h"

class PopulationDynamics
{
public:
	PopulationDynamics();
	virtual ~PopulationDynamics();

	virtual void init() = 0;
	virtual void onCycle() = 0;
	virtual void onOrganismDeath(SimulationObject* org) = 0;
};
#endif

