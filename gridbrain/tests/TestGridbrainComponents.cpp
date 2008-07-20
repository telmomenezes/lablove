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
#include "Gridbrain.h"
#include "Grid.h"
#include "comps.h"

#include <stdexcept>

struct GridbrainCompFixture
{
    GridbrainCompFixture()
    {
        mGrid = new Grid();
        mGrid->init(Grid::BETA, 4, 3);
        mGridbrain.addGrid(mGrid, "test");
        mGridbrain.setAllActive(true);
        mGridbrain.init();
    }

    void initAndCycle()
    {
        mGridbrain.init();
        mGridbrain.cycle();
    }

    ~GridbrainCompFixture(){}

    Gridbrain mGridbrain;
    Grid* mGrid;
};

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_NUL);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp3->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp3->mParam = 0.5f;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp3->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp5 = mGridbrain.setComponent(2, 0, 0, COMP_NUL);
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NOT);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NOT);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NOT);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_NOT);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_NOT);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}


TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp5 = mGridbrain.setComponent(2, 0, 0, COMP_NOT);
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_ZERO);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_ZERO);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_ZERO);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_ZERO);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_ZERO);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}


TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_ZERO);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_ZERO);
    Component* comp3 = mGridbrain.setComponent(1, 1, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_ZERO);
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_AMP);
    comp->mParam = 0.5;
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    comp3->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP4)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_AMP);
    comp->mParam = 0.1;
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP5)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_AMP);
    comp->mParam = 1.0;
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    comp2->mParam = 0.1;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.11111f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    comp2->mParam = 0.7;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 2.33333f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_NEG);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_NEG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_NEG);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp5 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_MOD);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_MOD);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_MOD);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_MOD);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_MOD);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_MOD);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_MOD);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_MOD);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_MOD);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_MOD);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp4 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp5 = mGridbrain.setComponent(2, 0, 0, COMP_MOD);
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_AND);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_AND);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_AND);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_AND);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_AND);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_AND);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_AND);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp6 = mGridbrain.setComponent(2, 1, 0, COMP_NEG);
    Component* comp7 = mGridbrain.setComponent(3, 0, 0, COMP_AND);
    comp2->mParam = 0.5;
    comp5->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_SUM);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_SUM);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_SUM);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_SUM);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_SUM);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_SUM);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_SUM);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_SUM);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_SUM);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_SUM);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_SUM);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_SUM);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_SUM);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.947368, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp6 = mGridbrain.setComponent(2, 1, 0, COMP_NEG);
    Component* comp7 = mGridbrain.setComponent(3, 0, 0, COMP_SUM);
    comp2->mParam = 0.5;
    comp5->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp7->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_MUL);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_MUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_MUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_MUL);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_MUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_MUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_MUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_MUL);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_MUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_MUL);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_MUL);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_MUL);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_MUL);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL12)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp6 = mGridbrain.setComponent(2, 1, 0, COMP_NEG);
    Component* comp7 = mGridbrain.setComponent(3, 0, 0, COMP_MUL);
    comp2->mParam = 0.5;
    comp5->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_EQ);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_EQ);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_EQ);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_EQ);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_EQ);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_EQ);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_EQ);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_EQ);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_EQ);
    comp2->mParam = 0.05;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ9)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_EQ);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_EQ);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ10)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(3, 0, 0, COMP_EQ);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ11)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp5 = mGridbrain.setComponent(1, 1, 0, COMP_AMP);
    Component* comp6 = mGridbrain.setComponent(2, 1, 0, COMP_NEG);
    Component* comp7 = mGridbrain.setComponent(3, 0, 0, COMP_EQ);
    comp2->mParam = 0.5;
    comp5->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ1)
{
    Component* comp = mGridbrain.setComponent(0, 0, 0, COMP_GTZ);
    initAndCycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_GTZ);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_GTZ);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_GTZ);
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0001f);
}

struct GridbrainCompFixture2
{
    GridbrainCompFixture2()
    {
        mAlphaGrid = new Grid();
        mAlphaGrid->init(Grid::ALPHA, 3, 3);
        mBetaGrid = new Grid();
        mBetaGrid->init(Grid::BETA, 3, 3);
        mGridbrain.addGrid(mAlphaGrid, "alpha");
        mGridbrain.addGrid(mBetaGrid, "beta");
        mGridbrain.init();
        Component* per1 = mGridbrain.setComponent(0, 0, 0, COMP_IN);
        Component* per2 = mGridbrain.setComponent(0, 1, 0, COMP_IN);
        Component* per3 = mGridbrain.setComponent(0, 2, 0, COMP_IN);
        per1->mSubType = 0;
        per2->mSubType = 1;
        per3->mSubType = 2;
        Component* act1 = mGridbrain.setComponent(2, 0, 1, COMP_OUT);
        Component* act2 = mGridbrain.setComponent(2, 1, 1, COMP_OUT);
        Component* act3 = mGridbrain.setComponent(2, 2, 1, COMP_OUT);
        act1->mSubType = 0;
        act2->mSubType = 1;
        act3->mSubType = 2;
        mGridbrain.setAllActive(true);
        mGridbrain.init();
    }

    void initAndCycle()
    {
        mGridbrain.init();
        mGridbrain.cycle();
    }

    ~GridbrainCompFixture2(){}

    Gridbrain mGridbrain;
    Grid* mAlphaGrid;
    Grid* mBetaGrid;
};

TEST_FIXTURE(GridbrainCompFixture2, GridbrainTwoLayersNoPerceptions)
{
    Component* per1 = mGridbrain.getComponent(0, 0, 0);
    Component* per2 = mGridbrain.getComponent(0, 1, 0);
    Component* per3 = mGridbrain.getComponent(0, 2, 0);
    initAndCycle();
    CHECK_CLOSE(per1->mOutput, 0.0f, 0.0001f);
    CHECK_CLOSE(per2->mOutput, 0.0f, 0.0001f);
    CHECK_CLOSE(per3->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainIN1)
{
    Component* per1 = mGridbrain.getComponent(0, 0, 0);
    Component* per2 = mGridbrain.getComponent(0, 1, 0);
    Component* per3 = mGridbrain.getComponent(0, 2, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff[1] = -0.5;
    buff[2] = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(per1->mOutput, 0.0f, 0.0001f);
    CHECK_CLOSE(per2->mOutput, -0.5f, 0.0001f);
    CHECK_CLOSE(per3->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainIN2)
{
    Component* per1 = mGridbrain.getComponent(0, 0, 0);
    Component* per2 = mGridbrain.getComponent(0, 1, 0);
    Component* per3 = mGridbrain.getComponent(0, 2, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff[1] = -0.5;
    buff[2] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.3;
    buff[1] = -0.04;
    buff[2] = -1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(per1->mOutput, 0.3f, 0.0001f);
    CHECK_CLOSE(per2->mOutput, -0.04f, 0.0001f);
    CHECK_CLOSE(per3->mOutput, -1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX1)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX2)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX3)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.5;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX4)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX5)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff[1] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX6)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff[1] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG1)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG2)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 2.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.5f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG3)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG4)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 2.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 3.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 2.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG5)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 2.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 3.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 2.5f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG6)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG7)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff[1] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.04f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG8)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_AVG);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 2.0;
    buff[1] = 3.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.5;
    buff[1] = 2.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 3.5f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV1)
{
    Component* comp1 = mGridbrain.setComponent(1, 0, 0, COMP_INV);
    initAndCycle();
    CHECK(comp1->mOutput > 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_INV);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_INV);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_INV);
    comp2->mParam = 0.1;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.888888f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_AMP);
    Component* comp3 = mGridbrain.setComponent(2, 0, 0, COMP_NEG);
    Component* comp4 = mGridbrain.setComponent(3, 0, 0, COMP_INV);
    comp2->mParam = 0.1;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp4->mOutput, -0.888888f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV6)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_INV);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV7)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(1, 1, 0, COMP_NEG);
    Component* comp5 = mGridbrain.setComponent(2, 0, 0, COMP_INV);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV8)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_NEG);
    Component* comp3 = mGridbrain.setComponent(0, 1, 0, COMP_NOT);
    Component* comp4 = mGridbrain.setComponent(2, 0, 0, COMP_INV);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    initAndCycle();
    CHECK(comp4->mOutput > 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestRAND)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_RAND);
    initAndCycle();
    CHECK(comp1->mOutput >= -1.0f);
    CHECK(comp1->mOutput <= 1.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK1)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_CLK);
    comp2->mParam = 0.1f;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK2)
{
    Component* comp1 = mGridbrain.setComponent(1, 0, 0, COMP_CLK);
    initAndCycle();
    CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK3)
{
    Component* comp1 = mGridbrain.setComponent(1, 0, 0, COMP_CLK);
    comp1->mParam = 0.1f;
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK4)
{
    Component* comp1 = mGridbrain.setComponent(1, 0, 0, COMP_CLK);
    comp1->mParam = 0.2f;
    for (unsigned int i = 0; i < 1000; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 1000; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit1)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    float* outBuff = mGridbrain.getOutputBuffer();
    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 0.2f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit2)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    float* outBuff = mGridbrain.getOutputBuffer();
    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 0.2f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit3)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -0.3;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -0.1;
    float* outBuff = mGridbrain.getOutputBuffer();

    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit4)
{
    Component* cmp1 = mGridbrain.setComponent(0, 0, 1, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 0, 0, 1);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -0.3;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -0.2;
    float* outBuff = mGridbrain.getOutputBuffer();

    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit5)
{
    Component* cmp1 = mGridbrain.setComponent(0, 0, 1, COMP_AND);

    mGridbrain.addConnection(0, 0, 0, 0, 0, 1);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1);

    mGridbrain.init();

    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -0.2;
    float* outBuff = mGridbrain.getOutputBuffer();

    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit6)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    Component* cmp2 = mGridbrain.setComponent(2, 0, 0, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1);

    mGridbrain.init();
 
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff[1] = 1.0;
    buff[2] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.5;
    buff[1] = 1.0;
    buff[2] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.3;
    buff[1] = 1.0;
    buff[2] = 0.0;
    float* outBuff = mGridbrain.getOutputBuffer();

    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit7)
{
    Component* cmp1 = mGridbrain.setComponent(1, 0, 0, COMP_MAX);
    Component* cmp2 = mGridbrain.setComponent(2, 0, 0, COMP_AND);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1);

    mGridbrain.init();
 
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff[1] = 1.0;
    buff[2] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.5;
    buff[1] = 1.0;
    buff[2] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.3;
    buff[1] = 1.0;
    buff[2] = 1.0;
    float* outBuff = mGridbrain.getOutputBuffer();

    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDMUL1)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_DMUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDMUL2)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_AND);
    Component* comp2 = mGridbrain.setComponent(1, 0, 0, COMP_DMUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDMUL3)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_AND);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_DMUL);
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    initAndCycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDMUL4)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_CLK);
    Component* comp3 = mGridbrain.setComponent(1, 0, 0, COMP_DMUL);
    comp2->mParam = 0.1f;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDMUL5)
{
    Component* comp1 = mGridbrain.setComponent(0, 0, 0, COMP_NOT);
    Component* comp2 = mGridbrain.setComponent(0, 1, 0, COMP_CLK);
    Component* comp3 = mGridbrain.setComponent(0, 2, 0, COMP_AND);
    Component* comp4 = mGridbrain.setComponent(1, 0, 0, COMP_DMUL);
    comp2->mParam = 0.1f;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 1, 0, 0);
    for (unsigned int i = 0; i < 500; i++)
    {
        initAndCycle();
        CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    }
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    initAndCycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

