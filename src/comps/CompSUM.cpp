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

#include "CompSUM.h"

CompSUM::CompSUM()
{
    mType = SUM;
}

CompSUM::~CompSUM()
{
}

GridbrainComponent* CompSUM::clone()
{
    return new CompSUM();
}

void CompSUM::reset(int pass)
{
    mInput = 0.0f;
}

void CompSUM::input(float value, int pin)
{
    mInput += value;
}

float CompSUM::output(unsigned int id)
{
    mOutput = mInput;
    return mOutput;
}

