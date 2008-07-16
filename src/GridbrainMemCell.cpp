/*
 * LOVE Lab
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

#include "GridbrainMemCell.h"
#include <stdio.h>

GridbrainMemCell::GridbrainMemCell()
{
    mValue = 0.0f;
    mWrite = 0.0f;
    mClear = false;
    mSelCandidate = 0.0f;
    mTriggered = false;
    mValueFound = false;
    mProducer = false;
    mConsumer = false;
}

GridbrainMemCell::~GridbrainMemCell()
{
}

void GridbrainMemCell::cycle()
{
    if (mClear)
    {
        mValue = 0.0f;
    }

    if (mWrite != 0.0f)
    {
        mValue = mWrite;
    }
    else if (!mValueFound)
    {
        mValue = mSelCandidate;
    }

    mWrite = 0.0f;
    mClear = false;

    mSelCandidate = 0.0f;
    mTriggered = false;
    mValueFound = false;

    //printf("value: %f\n", mValue);
}

