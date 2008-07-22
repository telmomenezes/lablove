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

#include "CompTMEM.h"

namespace gb
{

CompTMEM::CompTMEM()
{
    mInit = false;
    mState = 0.0f;
}

CompTMEM::~CompTMEM()
{
}

Component* CompTMEM::clone()
{
    CompTMEM* comp = new CompTMEM();
    comp->mType = mType;
    comp->mParam = mParam;
    return comp;
}

void CompTMEM::reset(int pass)
{
    if (!mInit)
    {
        mTriggerInterval = (gbULINT)(5000.0f * mParam);
        mTimeToTrigger = 0;
        mInit = true;
    }

    mInput = 0.0f;
}

void CompTMEM::input(float value, int pin)
{
    mInput += value;
}

float CompTMEM::output(unsigned int id)
{
    if (mInput != 0.0f)
    {
        mState = mInput;
        mTimeToTrigger = mTriggerInterval;
    }

    if (mTimeToTrigger == 0)
    {
        mState = 0.0f;
    }
    else
    {
        mTimeToTrigger--;
    }

    mOutput = mState;

    return mOutput;
}

string CompTMEM::getLabel()
{
    char buffer[255];
    sprintf(buffer, "%f", mParam);
    return buffer;
}

}

