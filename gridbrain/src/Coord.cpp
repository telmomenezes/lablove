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

#include "Coord.h"

namespace gb
{

Coord::Coord()
{
    mDepth = 0;
    mBranch = 0;
}

Coord::Coord(llULINT depth, llULINT branch)
{
    mDepth = depth;
    mBranch = branch;
}

Coord::Coord(const Coord& coord)
{
    mDepth = coord.mDepth;
    mBranch = coord.mBranch;
}

Coord::~Coord()
{
}

Coord Coord::leftOf()
{
    llULINT depth = mDepth + 1;
    llULINT branch = 2 * mBranch;
    Coord gc(depth, branch);
    return gc;
}

Coord Coord::rightOf()
{
    llULINT depth = mDepth + 1;
    llULINT branch = (2 * mBranch) + 1;
    Coord gc(depth, branch);
    return gc;
}

bool Coord::operator==(const Coord &gc) const
{
    if ((mDepth == gc.mDepth) && (mBranch == gc.mBranch))
    {
        return true;
    }

    return false;
}

int Coord::position(Coord gc)
{
    llULINT d1 = mDepth;
    llULINT b1 = mBranch;
    llULINT d2 = gc.mDepth;
    llULINT b2 = gc.mBranch;

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

    llULINT deltaD;
    llULINT downB;
    int inversor;
    Coord firstRight;

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

string Coord::toString()
{
    char buf[255];
    sprintf(buf, "(%d, %d)", mDepth, mBranch);
    return string(buf);
}

bool Coord::isValid()
{
    llULINT maxBranch = 1;
    for (llULINT i = 0; i < mDepth; i++)
    {
        maxBranch *= 2;
    }
    maxBranch--;

    if (mBranch > maxBranch)
    {
        return false;
    }

    return true;
}

}

