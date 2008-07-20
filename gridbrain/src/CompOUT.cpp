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
    mType = OUT;
}

CompOUT::~CompOUT()
{
}

Component* CompOUT::clone()
{
    CompOUT* comp = new CompOUT();
    comp->mSubType = mSubType;
    comp->mOrigSymTable = mOrigSymTable;
    comp->mOrigSymID = mOrigSymID;
    comp->mTargetSymTable = mTargetSymTable;
    comp->mTableLinkType = mTableLinkType;
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
    return ((mSubType == comp->mSubType)
            && (mOrigSymTable == comp->mOrigSymTable)
            && (mTargetSymTable == comp->mTargetSymTable)
            && (mOrigSymID == comp->mOrigSymID));
}

