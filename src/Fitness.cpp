/*
 * LabLOVE
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

#include "Fitness.h"

Fitness::Fitness()
{
    mFitness = 0.0f;
    mGroupFitness = 0.0f;
    mFinalFitness = 0.0f;
}

Fitness::Fitness(const Fitness& fit)
{
    mFitness = fit.mFitness;
    mGroupFitness = fit.mGroupFitness;
    mFinalFitness = fit.mFinalFitness;
}

Fitness::~Fitness()
{   
}

void Fitness::clear()
{
    mFitness = 0.0f;
    mGroupFitness = 0.0f;
    mFinalFitness = 0.0f;
}

