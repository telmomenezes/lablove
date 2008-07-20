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

#include "CompINV.h"

CompINV::CompINV()
{
    mType = INV;
}

CompINV::~CompINV()
{
}

Component* CompINV::clone()
{
    return new CompINV();
}

void CompINV::reset(int pass)
{
    mInput = 0.0f;
}

void CompINV::input(float value, int pin)
{
    mInput += value;
}

float CompINV::output(unsigned int id)
{
    float inVal = mInput;
    if (inVal > 1.0f)
    {
        inVal = 1.0f;
    }
    else if (inVal < -1.0f)
    {
        inVal = -1.0f;
    }
    if (inVal >= 0.0f)
    {
        mOutput = 1.0f - inVal;
    }
    else if (inVal < 0.0f)
    {
        mOutput = -1.0f - inVal;
    }

    return mOutput;
}

