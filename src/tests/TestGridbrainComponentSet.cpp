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
#include "GridbrainComponentSet.h"

TEST(GridbrainComponentSetGetRandomFromEmpty)
{
    GridbrainComponentSet set;
    GridbrainComponent* comp = set.getRandom();
    CHECK_EQUAL(comp->mType, GridbrainComponent::NUL);
}

struct GridbrainComponentSetFixture
{
    GridbrainComponentSetFixture()
    {
        mSet.addComponent(GridbrainComponent::OUT, 33);
    }

    ~GridbrainComponentSetFixture(){}

    GridbrainComponentSet mSet;
};

/*TEST_FIXTURE(GridbrainComponentSetFixture, GridbrainComponentSetAddComponent)
{
    GridbrainComponent* comp = mSet.getRandom();
    CHECK_EQUAL(comp->mType, GridbrainComponent::OUT);
}

TEST_FIXTURE(GridbrainComponentSetFixture, GridbrainComponentSetAddComponentCheckSubType)
{
    GridbrainComponent* comp = mSet.getRandom();
    CHECK_EQUAL(comp->mSubType, 33);
}

TEST_FIXTURE(GridbrainComponentSetFixture, GridbrainComponentSetAddAnotherComponent)
{
    mSet.addComponent(GridbrainComponent::IN);
    GridbrainComponent* comp = mSet.getRandom();
    CHECK((comp->mType == GridbrainComponent::OUT) || (comp->mType == GridbrainComponent::IN));
}*/

