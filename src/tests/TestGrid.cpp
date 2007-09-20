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

#include "UnitTest++.h"
#include "Grid.h"

struct GridFixture
{
    GridFixture()
    {
        mType = Grid::BETA;
        mWidth = 2;
        mHeight = 3;
        mGrid.init(mType, mWidth, mHeight);
        mGrid2 = mGrid;
    }

    ~GridFixture(){}

    Grid mGrid;
    Grid mGrid2;
    Grid::Type mType;
    unsigned int mWidth;
    unsigned int mHeight;
};

TEST_FIXTURE(GridFixture, InitGridCheckType)
{
    CHECK_EQUAL(mGrid.getType(), mType);
}

TEST_FIXTURE(GridFixture, InitGridCheckWidth)
{
    CHECK_EQUAL(mGrid.getWidth(), mWidth);
}

TEST_FIXTURE(GridFixture, InitGridCheckHeight)
{
    CHECK_EQUAL(mGrid.getHeight(), mHeight);
}

TEST_FIXTURE(GridFixture, InitGridCheckSize)
{
    CHECK_EQUAL(mGrid.getSize(), mWidth * mHeight);
}

TEST_FIXTURE(GridFixture, CopyGridCheckType)
{
    CHECK_EQUAL(mGrid2.getType(), mType);
}

TEST_FIXTURE(GridFixture, CopyGridCheckWidth)
{
    CHECK_EQUAL(mGrid2.getWidth(), mWidth);
}

TEST_FIXTURE(GridFixture, CopyGridCheckHeight)
{
    CHECK_EQUAL(mGrid2.getHeight(), mHeight);
}

TEST_FIXTURE(GridFixture, CopyGridCheckSize)
{
    CHECK_EQUAL(mGrid2.getSize(), mWidth * mHeight);
}

