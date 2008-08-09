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

#include "CompCLK.h"

namespace gb
{

mt_distribution* CompCLK::mDistRand = gDistManager.getNewDistribution();

CompCLK::CompCLK()
{
    mInit = false;
    mLastInput = 0.0f;
}

CompCLK::~CompCLK()
{
}

Component* CompCLK::clone()
{
    CompCLK* comp = new CompCLK();
    comp->mType = mType;
    comp->mParam = mParam;
    return comp;
}

void CompCLK::reset(int pass, unsigned int entity)
{
    if (!mInit)
    {
        mTriggerInterval = (gbULINT)(5000.0f * mParam);
        //mTimeToTrigger = mTriggerInterval;
        mTimeToTrigger = mDistRand->iuniform(0, mTriggerInterval);
        mInit = true;
    }

    mInput = 0.0f;
}

void CompCLK::input(float value, int pin)
{
    mInput += value;
}

float CompCLK::output()
{
    mOutput = 0.0f;

    if (mTriggerInterval > 0)
    {
        // Check for synch signal
        if ((mLastInput == 0.0f) && (mInput != 0.0f))
        {
            mTimeToTrigger = 0;
        }
                        
        if (mTimeToTrigger == 0)
        {
            mOutput = 1.0f;
            mTimeToTrigger = mTriggerInterval;
        }
        else
        {
            mTimeToTrigger--;
        }
    }

    mLastInput = mInput;

    return mOutput;
}

string CompCLK::getLabel()
{
    char buffer[255];
    sprintf(buffer, "%f", mParam);
    return buffer;
}

void CompCLK::reset()
{
    mTriggerInterval = (gbULINT)(5000.0f * mParam);
    mTimeToTrigger = mTriggerInterval;
    mInit = true;
}

}

