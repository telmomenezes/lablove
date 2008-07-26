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

#include "CompGTZ.h"

namespace gb
{

CompGTZ::CompGTZ()
{
}

CompGTZ::~CompGTZ()
{
}

Component* CompGTZ::clone()
{
    CompGTZ* comp = new CompGTZ();
    comp->mType = mType;
    return comp;
}

void CompGTZ::reset(int pass, unsigned int entity)
{
    mInput = 0.0f;
}

void CompGTZ::input(float value, int pin)
{
    mInput += value;
}

float CompGTZ::output()
{
    if (mInput > 0.0f)
    {
        mOutput = 1.0f;
    }
    else
    {
        mOutput = 0.0f;
    }

    return mOutput;
}

}

