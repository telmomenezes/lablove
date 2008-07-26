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

#include "CompMEM.h"

namespace gb
{

CompMEM::CompMEM()
{
    mState = 0.0f;
}

CompMEM::~CompMEM()
{
}

Component* CompMEM::clone()
{
    CompMEM* comp = new CompMEM();
    comp->mType = mType;
    return comp;
}

void CompMEM::reset(int pass, unsigned int entity)
{
    mInput = 0.0f;
}

void CompMEM::input(float value, int pin)
{
    mInput += value;
}

float CompMEM::output()
{
    if (mInput != 0.0f)
    {
        mState = mInput;
    }
    mOutput = mState;
    return mOutput;
}

}

