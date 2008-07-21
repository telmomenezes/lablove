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

#include "CompAMP.h"

CompAMP::CompAMP()
{
}

CompAMP::~CompAMP()
{
}

Component* CompAMP::clone()
{
    CompAMP* comp = new CompAMP();
    comp->mType = mType;
    comp->mParam = mParam;
    return comp;
}

void CompAMP::reset(int pass)
{
    mInput = 0.0f;
}

void CompAMP::input(float value, int pin)
{
    mInput += value;
}

float CompAMP::output(unsigned int id)
{
    if ((mParam == 1.0f) && (mInput == 0.0f))
    {
        mOutput = 0.0f;
    }
    else
    {
        mOutput = mInput * ((1.0f / (1.0f - mParam)) - 1.0f);
    }

    return mOutput;
}

string CompAMP::getLabel()
{
    char buffer[255];
    sprintf(buffer, "%f", mParam);
    return buffer;
}

