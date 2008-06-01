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

#include "GridbrainState.h"

GridbrainState::GridbrainState(unsigned int size)
{
    mCount = 0;
    mSize = size / 8;
    if ((size % 8) > 0)
    {
        mSize++;
    }
    mBits = (unsigned char*)malloc(mSize * sizeof(unsigned char));
}

GridbrainState::~GridbrainState()
{
    if (mBits != NULL)
    {
        free(mBits);
        mBits = NULL;
    }
}

bool GridbrainState::operator==(const GridbrainState &state) const
{
    for (unsigned int i = 0; i < mSize; i++)
    {
        if (mBits[i] != state.mBits[i])
        {
            return false;
        }
    }
    return true;
}

