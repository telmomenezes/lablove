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

#include "CompDMUL.h"

namespace gb
{

CompDMUL::CompDMUL()
{
}

CompDMUL::~CompDMUL()
{
}

Component* CompDMUL::clone()
{
    CompDMUL* comp = new CompDMUL();
    comp->mType = mType;
    return comp;
}

void CompDMUL::reset(int pass, unsigned int entity)
{
}

void CompDMUL::input(float value, int pin)
{
    if (value != 0.0f)
    {
        mInputVals[pin] = value;
    }
}

float CompDMUL::output()
{
    if (mInputVals.size() == mInboundConnections)
    {
        mOutput = 1.0f;

        for (map<int, float>::iterator iterVal = mInputVals.begin();
                iterVal != mInputVals.end();
                iterVal++)
        {
            mOutput *= (*iterVal).second;
        }

        mInputVals.clear();
    }
    else
    {
        mOutput = 0.0f;
    }

    return mOutput;
}

}

