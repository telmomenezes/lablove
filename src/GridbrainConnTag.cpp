/*
 * LabLOVE
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

#include "GridbrainConnTag.h"

#include <stdio.h>

llULINT GridbrainConnTag::ID_SEQ = 0;

GridbrainConnTag::GridbrainConnTag()
{
    mGroupID = 0;
    mOrigID = 0;
    mTargID = 0;
}

GridbrainConnTag::GridbrainConnTag(const GridbrainConnTag& tag)
{
    mGroupID = tag.mGroupID;
    mOrigID = tag.mOrigID;
    mTargID = tag.mTargID;
}

GridbrainConnTag::~GridbrainConnTag()
{
}

llULINT GridbrainConnTag::generateID()
{
    ID_SEQ++;
    return ID_SEQ;
}

bool GridbrainConnTag::isEquivalentOrigin(GridbrainConnTag* tag)
{
    return ((mGroupID == tag->mGroupID) && (mTargID == tag->mTargID));
}

bool GridbrainConnTag::isEquivalentTarget(GridbrainConnTag* tag)
{
    return ((mGroupID == tag->mGroupID) && (mOrigID == tag->mOrigID));
}

void GridbrainConnTag::print()
{
    printf(" g: %d", mGroupID);
    printf(" o: %d", mOrigID);
    printf(" t: %d", mTargID);
}

