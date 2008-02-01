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

#include "GridCoord.h"

GridCoord::GridCoord()
{
    mDepth = 0;
    mBranch = 0;
    mXoverOrigin = 0;
}

GridCoord::GridCoord(unsigned int depth, unsigned int branch)
{
    mDepth = depth;
    mBranch = branch;
    mXoverOrigin = 0;
}

GridCoord::GridCoord(const GridCoord& coord)
{
    mDepth = coord.mDepth;
    mBranch = coord.mBranch;
    mXoverOrigin = coord.mXoverOrigin;
}

GridCoord::~GridCoord()
{
}

GridCoord GridCoord::leftOf()
{
    unsigned int depth = mDepth + 1;
    unsigned int branch = 2 * mBranch;
    GridCoord gc(depth, branch);
    return gc;
}

GridCoord GridCoord::rightOf()
{
    unsigned int depth = mDepth + 1;
    unsigned int branch = (2 * mBranch) + 1;
    GridCoord gc(depth, branch);
    return gc;
}

bool GridCoord::operator==(const GridCoord &gc) const
{
    if ((mDepth == gc.mDepth) && (mBranch == gc.mBranch))
    {
        return true;
    }

    return false;
}

int GridCoord::position(GridCoord gc)
{
    unsigned int d1 = mDepth;
    unsigned int b1 = mBranch;
    unsigned int d2 = gc.mDepth;
    unsigned int b2 = gc.mBranch;

    if (d1 == d2)
    {
        if (b1 == b2)
        {
            return 0;
        }
        else if (b1 > b2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    unsigned int deltaD;
    unsigned int downB;
    int inversor;
    GridCoord firstRight;

    if (d1 > d2)
    {
        deltaD = d1 - d2;
        downB = b1;
        firstRight = gc;
        inversor = 1;
    }
    else
    {
        deltaD = d2 - d1;
        downB = b2;
        firstRight = *this;
        inversor = -1;
    }

    firstRight = firstRight.rightOf();
    deltaD--;

    while (deltaD > 0)
    {
        firstRight = firstRight.leftOf();
        deltaD--;
    }

    int result;

    if (downB < firstRight.mBranch)
    {
        result = -1;
    }
    else
    {
        result = 1;
    }

    result *= inversor;

    return result;
}

string GridCoord::toString()
{
    char buf[255];
    sprintf(buf, "(%d, %d)", mDepth, mBranch);
    return string(buf);
}

