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

#include "CompIN.h"

namespace gb
{

CompIN::CompIN()
{
    mType = -3;
}

CompIN::~CompIN()
{
}

Component* CompIN::clone()
{
    CompIN* comp = new CompIN();
    comp->mType = mType;
    comp->mInputType = mInputType;
    return comp;
}

void CompIN::reset(int pass)
{
}

void CompIN::input(float value, int pin)
{
}

float CompIN::output(unsigned int id)
{
    return mOutput;
}

bool CompIN::compare(Component* comp)
{
    CompIN* compIN = (CompIN*)comp;
    return (mInputType == compIN->mInputType);
}

}

