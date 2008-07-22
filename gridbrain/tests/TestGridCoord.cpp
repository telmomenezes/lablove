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

#include "UnitTest++.h"
#include "Coord.h"

using namespace gb;

TEST(CreateCoord)
{
    Coord gc;
    CHECK_EQUAL(gc.getDepth(), 0);
    CHECK_EQUAL(gc.getBranch(), 0);
}

TEST(CreateCoord2)
{
    Coord gc(1, 2);
    CHECK_EQUAL(gc.getDepth(), 1);
    CHECK_EQUAL(gc.getBranch(), 2);
}

TEST(CoordCopyConst)
{
    Coord gc(3, 5);
    Coord gc2 = gc;
    CHECK_EQUAL(gc2.getDepth(), 3);
    CHECK_EQUAL(gc2.getBranch(), 5);
}

TEST(CoordLeftOf)
{
    Coord gc(3, 5);
    Coord gc2 = gc.leftOf();
    CHECK_EQUAL(gc2.getDepth(), 4);
    CHECK_EQUAL(gc2.getBranch(), 10);
}

TEST(CoordRightOf)
{
    Coord gc(3, 5);
    Coord gc2 = gc.rightOf();
    CHECK_EQUAL(gc2.getDepth(), 4);
    CHECK_EQUAL(gc2.getBranch(), 11);
}

TEST(CoordPosition)
{
    Coord gc;
    int pos;
    pos = gc.position(Coord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(0, 0));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(Coord(1, 1));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(Coord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 1));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 2));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(Coord(2, 3));
    CHECK_EQUAL(pos, -1);
}

TEST(CoordPosition2)
{
    Coord gc(1, 1);
    int pos;
    pos = gc.position(Coord(0, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(1, 1));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(Coord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 1));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 2));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 3));
    CHECK_EQUAL(pos, -1);
}

TEST(CoordPosition3)
{
    Coord gc(2, 1);
    int pos;
    pos = gc.position(Coord(0, 0));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(Coord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(1, 1));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(Coord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(Coord(2, 1));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(Coord(2, 2));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(Coord(2, 3));
    CHECK_EQUAL(pos, -1);
}

