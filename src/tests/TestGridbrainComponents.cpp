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

    ~GridbrainCompFixture(){}

    Gridbrain mGridbrain;
    Grid* mGrid;
};

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NUL;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    comp4->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5f;
    comp4->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.05;
    comp4->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    comp4->mType = GridbrainComponent::AMP;
    comp4->mParam = 0.5;
    comp5->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NOT;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}


TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp4->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    comp5->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::ZERO;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::ZERO;
    comp3->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::ZERO;
    comp3->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::ZERO;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::ZERO;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::ZERO;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}


TEST_FIXTURE(GridbrainCompFixture, GridbrainTestZERO12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::ZERO;
    comp2->mType = GridbrainComponent::ZERO;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::ZERO;
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::AMP;
    comp->mParam = 0.5;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP4)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::AMP;
    comp->mParam = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP5)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::AMP;
    comp->mParam = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.11111f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAMP7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.7;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 2.33333f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NEG;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNEG11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp4->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    comp5->mType = GridbrainComponent::NEG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::MOD;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMOD11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AMP;
    comp4->mType = GridbrainComponent::AMP;
    comp3->mParam = 0.5;
    comp4->mParam = 0.5;
    comp5->mType = GridbrainComponent::MOD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::AND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp6 = mGridbrain.getComponent(2, 1, 0);
    GridbrainComponent* comp7 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AMP;
    comp5->mParam = 0.5;
    comp6->mType = GridbrainComponent::NEG;
    comp7->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::SUM;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::SUM;
    comp3->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::SUM;
    comp4->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.947368, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestSUM12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp6 = mGridbrain.getComponent(2, 1, 0);
    GridbrainComponent* comp7 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AMP;
    comp5->mParam = 0.5;
    comp6->mType = GridbrainComponent::NEG;
    comp7->mType = GridbrainComponent::SUM;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp7->mOutput, -2.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::MUL;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::MUL;
    comp4->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp6 = mGridbrain.getComponent(2, 1, 0);
    GridbrainComponent* comp7 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AMP;
    comp5->mParam = 0.5;
    comp6->mType = GridbrainComponent::NEG;
    comp7->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::EQ;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::EQ;
    comp3->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.05;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::EQ;
    comp4->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestEQ11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp6 = mGridbrain.getComponent(2, 1, 0);
    GridbrainComponent* comp7 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::NOT;
    comp5->mType = GridbrainComponent::AMP;
    comp5->mParam = 0.5;
    comp6->mType = GridbrainComponent::NEG;
    comp7->mType = GridbrainComponent::EQ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 1, 0);
    mGridbrain.addConnection(2, 1, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp7->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::GTZ;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::GTZ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::GTZ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestGTZ4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::GTZ;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
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
        GridbrainComponent* per1 = mGridbrain.getComponent(0, 0, 0);
        GridbrainComponent* per2 = mGridbrain.getComponent(0, 1, 0);
        GridbrainComponent* per3 = mGridbrain.getComponent(0, 2, 0);
        per1->mType = GridbrainComponent::IN;
        per2->mType = GridbrainComponent::IN;
        per3->mType = GridbrainComponent::IN;
        per1->mSubType = 0;
        per2->mSubType = 1;
        per3->mSubType = 2;
        GridbrainComponent* act1 = mGridbrain.getComponent(2, 0, 1);
        GridbrainComponent* act2 = mGridbrain.getComponent(2, 1, 1);
        GridbrainComponent* act3 = mGridbrain.getComponent(2, 2, 1);
        act1->mType = GridbrainComponent::OUT;
        act2->mType = GridbrainComponent::OUT;
        act3->mType = GridbrainComponent::OUT;
        act1->mSubType = 0;
        act2->mSubType = 1;
        act3->mSubType = 2;
        mGridbrain.setAllActive(true);
        mGridbrain.init();
    }

    ~GridbrainCompFixture2(){}

    Gridbrain mGridbrain;
    Grid* mAlphaGrid;
    Grid* mBetaGrid;
};

TEST_FIXTURE(GridbrainCompFixture2, GridbrainTwoLayersNoPerceptions)
{
    GridbrainComponent* per1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* per2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* per3 = mGridbrain.getComponent(0, 2, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(per1->mOutput, 0.0f, 0.0001f);
    CHECK_CLOSE(per2->mOutput, 0.0f, 0.0001f);
    CHECK_CLOSE(per3->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainIN1)
{
    GridbrainComponent* per1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* per2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* per3 = mGridbrain.getComponent(0, 2, 0);
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
    GridbrainComponent* per1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* per2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* per3 = mGridbrain.getComponent(0, 2, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX2)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX3)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG2)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 2.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.5f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG3)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = -1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainAVG4)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::AVG;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
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
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::INV;
    mGridbrain.cycle();
    CHECK(comp1->mOutput > 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1;
    comp3->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.888888f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(3, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1;
    comp3->mType = GridbrainComponent::NEG;
    comp4->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 3, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -0.888888f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp5 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::NEG;
    comp5->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 1, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp5->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestINV8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NEG;
    comp3->mType = GridbrainComponent::NOT;
    comp4->mType = GridbrainComponent::INV;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 2, 0, 0);
    mGridbrain.cycle();
    CHECK(comp4->mOutput > 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestRAND)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    comp1->mType = GridbrainComponent::RAND;
    mGridbrain.cycle();
    CHECK(comp1->mOutput >= -1.0f);
    CHECK(comp1->mOutput <= 1.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK1)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::CLK;
    comp2->mParam = 0.1f;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::CLK;
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::CLK;
    comp1->mParam = 0.1f;
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestCLK4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::CLK;
    comp1->mParam = 0.2f;
    for (unsigned int i = 0; i < 1000; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
    for (unsigned int i = 0; i < 1000; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMW1)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::MEMW;
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMW2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.5f;
    comp3->mType = GridbrainComponent::MEMW;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMW3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1f;
    comp3->mType = GridbrainComponent::MEMW;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.11111, 0.0001f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.11111, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMC1)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 1, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MEMW;
    comp3->mType = GridbrainComponent::MEMC;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0);
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMD1)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 1, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1f;
    comp3->mType = GridbrainComponent::MEMW;
    comp4->mType = GridbrainComponent::MEMD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(0, 0, 0, 2, 1, 0);
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -1.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -0.88888, 0.0001f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -0.88888, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMEMD2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(2, 0, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(2, 1, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AMP;
    comp2->mParam = 0.1f;
    comp3->mType = GridbrainComponent::MEMW;
    comp4->mType = GridbrainComponent::MEMD;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 1, 0);
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0);
    mGridbrain.update();
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, -0.11111f, 0.0001f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.88888, 0.0001f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp4->mOutput, 0.88888, 0.0001f);
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
    CHECK_CLOSE(outBuff[0], 0.6f, 0.0001f);
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
    CHECK_CLOSE(outBuff[0], 0.6f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit4)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(0, 0, 1);
    cmp1->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 0, 0, 1);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1);
 
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(0, 0, 1);
    cmp1->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 0, 0, 1);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1);
 
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* cmp2 = mGridbrain.getComponent(2, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    cmp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1);
 
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
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* cmp2 = mGridbrain.getComponent(2, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    cmp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1);
 
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

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDAND1)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::DAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::AND;
    comp2->mType = GridbrainComponent::DAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::DAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDAND4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::CLK;
    comp2->mParam = 0.1f;
    comp3->mType = GridbrainComponent::DAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestDAND5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(0, 2, 0);
    GridbrainComponent* comp4 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::CLK;
    comp2->mParam = 0.1f;
    comp3->mType = GridbrainComponent::AND;
    comp4->mType = GridbrainComponent::DAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0);
    mGridbrain.addConnection(0, 2, 0, 1, 0, 0);
    for (unsigned int i = 0; i < 500; i++)
    {
        mGridbrain.cycle();
        CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    }
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

