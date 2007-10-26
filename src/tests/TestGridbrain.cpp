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
}

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

TEST_FIXTURE(GridbrainFixture, GridbrainAddOneGrid)
{
    Grid* gridOut = mGridbrain.getGrid(0);
    CHECK(gridOut != NULL);
    gridOut = mGridbrain.getGrid(1);
    CHECK(gridOut == NULL);
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
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(1);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 1);
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponent)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    CHECK_EQUAL(comp->mType, GridbrainComponent::NUL);
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
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestNOT)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    comp->mType = GridbrainComponent::NOT;
    mGridbrain.cycle();
    CHECK_CLOSE(comp->mOutput, 1.0f, 0.0f);
}

TEST_FIXTURE(GridbrainFixture, GridbrainTestTwoNOTs)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(0, 0, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 0, 0);
    comp1->mType = GridbrainComponent::NOT;
    comp2->mType = GridbrainComponent::NOT;
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 1.0);
    mGridbrain.cycle();
    CHECK_CLOSE(comp1->mOutput, 1.0f, 0.0f);
    CHECK_CLOSE(comp2->mOutput, 0.0f, 0.0f);
}

