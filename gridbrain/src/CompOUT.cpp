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

#include "CompOUT.h"

CompOUT::CompOUT()
{
    mType = -4;
}

CompOUT::~CompOUT()
{
}

Component* CompOUT::clone()
{
    CompOUT* comp = new CompOUT();
    comp->mType = mType;
    comp->mOutputType = mOutputType;
    return comp;
}

void CompOUT::reset(int pass)
{
    mInput = 0.0f;
}

void CompOUT::input(float value, int pin)
{
    if (mInput == 0.0f)
    {
        mInput = value;
    }
}

float CompOUT::output(unsigned int id)
{
    mOutput = mInput;
    return mOutput;
}

bool CompOUT::compare(Component* comp)
{
    CompOUT* compOUT = (CompOUT*)comp;
    return (mOutputType == compOUT->mOutputType);
}

