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

#include "CompEQ.h"

CompEQ::CompEQ()
{
    mType = EQ;
}

CompEQ::~CompEQ()
{
}

GridbrainComponent* CompEQ::clone()
{
    return new CompEQ();
}

void CompEQ::reset(int pass)
{
    mInputFlag = false;
}

void CompEQ::input(float value, int pin)
{
    if (!mInputFlag)
    {
        mInput = 1.0f;
        mInputFlag = true;
        mState = value;
    }
    else if (mInput == 1.0f)
    {
        if (mState != value)
        {
            mInput = 0.0f;
        }
    }
}

float CompEQ::output(unsigned int id)
{
    mOutput = mInput;
    return mOutput;
}

