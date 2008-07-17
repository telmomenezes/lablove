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

#include "CompAVG.h"

CompAVG::CompAVG()
{
    mType = AVG;
}

CompAVG::~CompAVG()
{
}

GridbrainComponent* CompAVG::clone()
{
    return new CompAVG();
}

void CompAVG::reset(int pass)
{
    mInput = 0;
    mInputFlag = false;
    mPass = pass;

    if (pass == 0)
    {
        mState = 0;
        mEvalCount = 0;
    }
}

void CompAVG::input(float value, int pin)
{
    if (!mInputFlag)
    {
        mInput = value;
        mInputFlag = true;
    }
    else
    {
        mInput *= value;
    }
}

float CompAVG::output(unsigned int id)
{
    if ((mPass == 0) && (mInput != 0))
    {
        mState += mInput;
        mEvalCount++;
    }
    
    mOutput= 0.0f;
    if (mEvalCount > 0)
    {
        mOutput = mState / ((float)mEvalCount);
    }

    return mOutput;
}

