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
#include "GridCoord.h"

TEST(CreateGridCoord)
{
    GridCoord gc;
    CHECK_EQUAL(gc.getDepth(), 0);
    CHECK_EQUAL(gc.getBranch(), 0);
}

TEST(CreateGridCoord2)
{
    GridCoord gc(1, 2);
    CHECK_EQUAL(gc.getDepth(), 1);
    CHECK_EQUAL(gc.getBranch(), 2);
}

TEST(GridCoordCopyConst)
{
    GridCoord gc(3, 5);
    GridCoord gc2 = gc;
    CHECK_EQUAL(gc2.getDepth(), 3);
    CHECK_EQUAL(gc2.getBranch(), 5);
}

TEST(GridCoordLeftOf)
{
    GridCoord gc(3, 5);
    GridCoord gc2 = gc.leftOf();
    CHECK_EQUAL(gc2.getDepth(), 4);
    CHECK_EQUAL(gc2.getBranch(), 10);
}

TEST(GridCoordRightOf)
{
    GridCoord gc(3, 5);
    GridCoord gc2 = gc.rightOf();
    CHECK_EQUAL(gc2.getDepth(), 4);
    CHECK_EQUAL(gc2.getBranch(), 11);
}

TEST(GridCoordPosition)
{
    GridCoord gc;
    int pos;
    pos = gc.position(GridCoord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(0, 0));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(GridCoord(1, 1));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(GridCoord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 1));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 2));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(GridCoord(2, 3));
    CHECK_EQUAL(pos, -1);
}

TEST(GridCoordPosition2)
{
    GridCoord gc(1, 1);
    int pos;
    pos = gc.position(GridCoord(0, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(1, 1));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(GridCoord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 1));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 2));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 3));
    CHECK_EQUAL(pos, -1);
}

TEST(GridCoordPosition3)
{
    GridCoord gc(2, 1);
    int pos;
    pos = gc.position(GridCoord(0, 0));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(GridCoord(1, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(1, 1));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(GridCoord(2, 0));
    CHECK_EQUAL(pos, 1);
    pos = gc.position(GridCoord(2, 1));
    CHECK_EQUAL(pos, 0);
    pos = gc.position(GridCoord(2, 2));
    CHECK_EQUAL(pos, -1);
    pos = gc.position(GridCoord(2, 3));
    CHECK_EQUAL(pos, -1);
}

