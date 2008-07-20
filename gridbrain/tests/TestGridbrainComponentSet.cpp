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
#include "ComponentSet.h"

TEST(ComponentSetGetRandomFromEmpty)
{
    ComponentSet set;
    Component* comp = set.getRandom();
    CHECK_EQUAL(comp->mType, Component::NUL);
}

struct ComponentSetFixture
{
    ComponentSetFixture()
    {
        mSet.addComponent(Component::OUT, 33);
    }

    ~ComponentSetFixture(){}

    ComponentSet mSet;
};

/*TEST_FIXTURE(ComponentSetFixture, ComponentSetAddComponent)
{
    Component* comp = mSet.getRandom();
    CHECK_EQUAL(comp->mType, Component::OUT);
}

TEST_FIXTURE(ComponentSetFixture, ComponentSetAddComponentCheckSubType)
{
    Component* comp = mSet.getRandom();
    CHECK_EQUAL(comp->mSubType, 33);
}

TEST_FIXTURE(ComponentSetFixture, ComponentSetAddAnotherComponent)
{
    mSet.addComponent(Component::IN);
    Component* comp = mSet.getRandom();
    CHECK((comp->mType == Component::OUT) || (comp->mType == Component::IN));
}*/

