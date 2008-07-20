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

#include "CompSEL.h"

CompSEL::CompSEL()
{
    mType = SEL;
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
    return new CompSEL();
}

void CompSEL::reset(int pass)
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

float CompSEL::output(unsigned int id)
{
    bool triggered;

    if (mPass == 1)
    {
        if (mID == id)
        {
            mIDFound = true;
        }
        if ((id != 0) && (mInput != 0.0f))
        {
            if (!mTriggered)
            {
                mIDCandidate = id;
                mTriggered = true;
            }
        }
    }

    if (mID == id)
    {
        mOutput = mInput;
    }
    else
    {
        mOutput = 0.0f;
    }

    return mOutput;
}

