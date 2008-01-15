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
        mGrid->init(Grid::BETA, 2, 2);
        mGridbrain.addGrid(mGrid, "test");
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNUL12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestNOT12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::THR;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTHR12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestAND12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
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
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestMUL12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::TAND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTAND12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::TNAND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND4)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND5)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND6)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND10)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND11)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND12)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND13)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainCompFixture, GridbrainTestTNAND14)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(0, 1, 0);
    GridbrainComponent* comp3 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    comp3->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.0);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp3->mOutput, 1.0f, 0.0f);
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
        per1->mType = GridbrainComponent::PER;
        per2->mType = GridbrainComponent::PER;
        per3->mType = GridbrainComponent::PER;
        per1->mSubType = 0;
        per2->mSubType = 1;
        per3->mSubType = 2;
        GridbrainComponent* act1 = mGridbrain.getComponent(2, 0, 1);
        GridbrainComponent* act2 = mGridbrain.getComponent(2, 1, 1);
        GridbrainComponent* act3 = mGridbrain.getComponent(2, 2, 1);
        act1->mType = GridbrainComponent::ACT;
        act2->mType = GridbrainComponent::ACT;
        act3->mType = GridbrainComponent::ACT;
        act1->mSubType = 0;
        act2->mSubType = 1;
        act3->mSubType = 2;
        mGridbrain.onAdd();
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainPER1)
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainPER2)
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff[1] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMAX6)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff[1] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX1)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX2)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX3)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.5;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX4)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX5)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.1;
    buff[1] = 0.1;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 1.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainMMAX6)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff[1] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 1.0;
    buff[1] = 0.0;
    mGridbrain.cycle();
    CHECK_CLOSE(cmp1->mOutput, 0.0f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit1)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1, 0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    float* outBuff = mGridbrain.getOutputBuffer();
    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], 0.2f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit2)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1, 0.5);
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
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1, -0.5);
    float* buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    buff = mGridbrain.getInputBuffer(0);
    buff[0] = 0.2;
    float* outBuff = mGridbrain.getOutputBuffer();
    mGridbrain.cycle();
    CHECK_CLOSE(outBuff[0], -0.6f, 0.0001f);
}

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit4)
{
    mGridbrain.addConnection(0, 0, 0, 2, 0, 1, 0.5);
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit5)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(0, 0, 1);
    cmp1->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 0, 0, 1, 0.5);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1, 0.5);
 
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit6)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(0, 0, 1);
    cmp1->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 0, 0, 1, 0.5);
    mGridbrain.addConnection(0, 0, 1, 2, 0, 1, 0.5);
 
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit7)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* cmp2 = mGridbrain.getComponent(2, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    cmp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0, 0.5);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0, 0.5);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1, 0.5);
 
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

TEST_FIXTURE(GridbrainCompFixture2, GridbrainCircuit8)
{
    GridbrainComponent* cmp1 = mGridbrain.getComponent(1, 0, 0);
    GridbrainComponent* cmp2 = mGridbrain.getComponent(2, 0, 0);
    cmp1->mType = GridbrainComponent::MMAX;
    cmp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 1, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 2, 0, 2, 0, 0, 0.5);
    mGridbrain.addConnection(1, 0, 0, 2, 0, 0, 0.5);
    mGridbrain.addConnection(2, 0, 0, 2, 0, 1, 0.5);
 
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

