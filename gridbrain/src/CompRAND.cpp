/*
 * Gridbrain
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

#include "CompRAND.h"

namespace gb
{

mt_distribution* CompRAND::mDistRand = gDistManager.getNewDistribution();

CompRAND::CompRAND()
{
}

CompRAND::~CompRAND()
{
}

Component* CompRAND::clone()
{
    CompRAND* comp = new CompRAND();
    comp->mType = mType;
    return comp;
}

void CompRAND::reset(int pass)
{
}

void CompRAND::input(float value, int pin)
{
}

float CompRAND::output(unsigned int id)
{

    mOutput = mDistRand->uniform(-1.0f, 1.0f);
    return mOutput;
}

}

