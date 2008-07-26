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

#include "CompSEL.h"

namespace gb
{

CompSEL::CompSEL()
{
    mID = 0;
    mIDFound = false;
    mIDCandidate = 0;
    mTriggered = false;
    mIDFound = false;
}

CompSEL::~CompSEL()
{
}

Component* CompSEL::clone()
{
    CompSEL* comp = new CompSEL();
    comp->mType = mType;
    return comp;
}

void CompSEL::reset(int pass, unsigned int entity)
{
    mInput = 0.0f;
    mInputFlag = false;

    if ((mPass == 1) && (pass == 0))
    {
        if (!mIDFound)
        {
            mID = mIDCandidate;
        }

        mIDCandidate = 0;
        mTriggered = false;
        mIDFound = false;
    }

    mPass = pass;
    mEntity = entity;
}

void CompSEL::input(float value, int pin)
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

float CompSEL::output()
{
    bool triggered;

    if (mPass == 1)
    {
        if (mID == mEntity)
        {
            mIDFound = true;
        }
        if ((mEntity != 0) && (mInput != 0.0f))
        {
            if (!mTriggered)
            {
                mIDCandidate = mEntity;
                mTriggered = true;
            }
        }
    }

    if (mID == mEntity)
    {
        mOutput = mInput;
    }
    else
    {
        mOutput = 0.0f;
    }

    return mOutput;
}

}

