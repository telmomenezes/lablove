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

struct GridbrainFixture
{
    GridbrainFixture()
    {
        mGrid = new Grid();
        mGrid->init(Grid::BETA, 2, 2);
        mGridbrain.addGrid(mGrid, "test");
        mGridbrain.init();
    }

    ~GridbrainFixture(){}

    Gridbrain mGridbrain;
    Grid* mGrid;
};

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NUL;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNUL12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NOT;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::THR;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::THR;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR11)
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
    CHECK_CLOSE(comp3->mOutput, 0.947368f, 0.0001f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTHR12)
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
TEST_FIXTURE(GridbrainFixture, GridbrainTestAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::AND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestAND12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::MUL;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::AND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::MUL;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -0.0526316f, 0.0001f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestMUL12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::TAND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, -1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTAND12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND1)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::TNAND;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND2)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND3)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND4)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND5)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND6)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.5);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::TNAND;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, -0.05);
    mGridbrain.cycle();
    CHECK_CLOSE(comp2->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND9)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND10)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND11)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND12)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND13)
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

TEST_FIXTURE(GridbrainFixture, GridbrainTestTNAND14)
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

