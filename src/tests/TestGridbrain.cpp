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

TEST(CreateGridbrain)
{
    Gridbrain gridbrain;
    CHECK_EQUAL(gridbrain.getConnectionsCount(), 0);
    CHECK(gridbrain.isValid());
}

struct GridbrainFixture
{
    GridbrainFixture()
    {
        mGrid = new Grid();
        mGrid->init(Grid::BETA, 3, 3);
        mGridbrain.addGrid(mGrid, "test");
        mGridbrain.init();
    }

    ~GridbrainFixture(){}

    Gridbrain mGridbrain;
    Grid* mGrid;
};

struct GridbrainFixture2
{
    GridbrainFixture2()
    {
        mAlphaGrid = new Grid();
        mAlphaGrid->init(Grid::ALPHA, 3, 3);
        mGridbrain.addGrid(mAlphaGrid, "alpha");
        mBetaGrid = new Grid();
        mBetaGrid->init(Grid::BETA, 2, 2);
        mGridbrain.addGrid(mBetaGrid, "alpha");
        mGridbrain.init();
    }

    ~GridbrainFixture2(){}

    Gridbrain mGridbrain;
    Grid* mAlphaGrid;
    Grid* mBetaGrid;
};

TEST_FIXTURE(GridbrainFixture, GridbrainAddOneGrid)
{
    Grid* gridOut = mGridbrain.getGrid(0);
    CHECK(gridOut != NULL);
    gridOut = mGridbrain.getGrid(1);
    CHECK(gridOut == NULL);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainZeroInitialConnections)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    CHECK(!mGridbrain.connectionExists(0, 0, 0, 1, 1, 0));
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddConnection)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 1);
    CHECK(mGridbrain.connectionExists(0, 0, 0, 1, 1, 0));
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections1)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(1);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 1);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections2)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections3)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(28);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections4)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 20);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture2, GridbrainAddRandomConnections5)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 20);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture2, GridbrainAddRandomConnections6)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(100);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 67);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    comp1->mType = GridbrainComponent::ACT;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 24);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    comp1->mType = GridbrainComponent::PER;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 24);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 2, 0);
    comp1->mType = GridbrainComponent::PER;
    comp2->mType = GridbrainComponent::ACT;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 21);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponent)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    CHECK_EQUAL(comp->mType, GridbrainComponent::NUL);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidGrid)
{
    CHECK_THROW(mGridbrain.getComponent(0, 0, 99999), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidXPosition)
{
    CHECK_THROW(mGridbrain.getComponent(99999, 0, 0), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidYPosition)
{
    CHECK_THROW(mGridbrain.getComponent(0, 99999, 0), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainCycleOnEmptyGrid)
{
    mGridbrain.cycle();
    CHECK(mGridbrain.isValid());
}

