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

using namespace gb;

struct GridbrainRecFixture
{
    GridbrainRecFixture()
    {
        Grid* alphaGrid = new Grid();
        Grid* betaGrid = new Grid();
        alphaGrid->init(Grid::ALPHA, 10, 10);
        betaGrid->init(Grid::BETA, 10, 10);
        mGridbrain1 = new Gridbrain();
        mGridbrain1->addGrid(alphaGrid, "alpha");
        mGridbrain1->addGrid(betaGrid, "beta");
        mGridbrain1->init();
        mGridbrain1->addRandomConnections(100);

        alphaGrid = new Grid();
        betaGrid = new Grid();
        alphaGrid->init(Grid::ALPHA, 10, 10);
        betaGrid->init(Grid::BETA, 10, 10);
        mGridbrain2 = new Gridbrain();
        mGridbrain2->addGrid(alphaGrid, "alpha");
        mGridbrain2->addGrid(betaGrid, "beta");
        mGridbrain2->init();
        mGridbrain2->addRandomConnections(100);
    }

    ~GridbrainRecFixture()
    {
        delete mGridbrain1;
        delete mGridbrain2;
    }

    Gridbrain* mGridbrain1;
    Gridbrain* mGridbrain2;
};

TEST_FIXTURE(GridbrainRecFixture, GridbrainRecombine)
{
    Gridbrain* gb = (Gridbrain*)mGridbrain1->recombine(mGridbrain2);
    CHECK(gb->isValid());
    delete gb;
}

TEST_FIXTURE(GridbrainRecFixture, GridbrainRecombineSequence)
{
    Gridbrain* gb1 = (Gridbrain*)mGridbrain1->recombine(mGridbrain2);
    Gridbrain* gb2 = (Gridbrain*)mGridbrain2->recombine(mGridbrain1);

    for (unsigned int i = 0; i < 100; i++)
    {
        CHECK(gb1->isValid());
        CHECK(gb2->isValid());

        Gridbrain* gbOld1 = gb1;
        Gridbrain* gbOld2 = gb2;
        gb1 = (Gridbrain*)gbOld1->recombine(gbOld2);
        gb2 = (Gridbrain*)gbOld2->recombine(gbOld1);
        delete gbOld1;
        delete gbOld2;
    }

    delete gb1;
    delete gb2;
}

