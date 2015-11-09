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

#include "CompCLKG.h"
#include "Random.h"


namespace gb
{

CompCLKG::CompCLKG()
{
    mInit = false;
}

CompCLKG::~CompCLKG()
{
}

Component* CompCLKG::clone()
{
    CompCLKG* comp = new CompCLKG();
    comp->mType = mType;
    comp->mParam = mParam;
    return comp;
}

void CompCLKG::reset(int pass, unsigned int entity)
{
    if (!mInit)
    {
        mTriggerInterval = (gbULINT)(5000.0f * mParam);
        //mTimeToTrigger = mTriggerInterval;
        mTimeToTrigger = gRandom.iuniform(0, mTriggerInterval);
        mInit = true;
    }

    mInput = 0.0f;
}

void CompCLKG::input(float value, int pin)
{
    mInput += value;
}

float CompCLKG::output()
{
    mOutput = 0.0f;

    if (mTimeToTrigger > 0)
    {
        mTimeToTrigger--;
    }
    else
    {
        if (mInput != 0.0f)
        {
            mOutput = mInput;
            mTimeToTrigger = mTriggerInterval;
        }
    }

    return mOutput;
}

string CompCLKG::getLabel()
{
    char buffer[255];
    sprintf(buffer, "%f", mParam);
    return buffer;
}

void CompCLKG::reset()
{
    mTriggerInterval = (gbULINT)(5000.0f * mParam);
    mTimeToTrigger = mTriggerInterval;
    mInit = true;
}

}

