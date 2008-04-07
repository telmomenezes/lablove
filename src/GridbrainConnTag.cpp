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

llULINT GridbrainConnTag::ID_SEQ = 0;

GridbrainConnTag::GridbrainConnTag()
{
    mID = 0;
    mGroupID = 0;
    mPrevID = 0;
    mNextID = 0;
}

GridbrainConnTag::GridbrainConnTag(const GridbrainConnTag& tag)
{
    mID = tag.mID;
    mGroupID = tag.mGroupID;
    mPrevID = tag.mPrevID;
    mNextID = tag.mNextID;
}

GridbrainConnTag::~GridbrainConnTag()
{
}

void GridbrainConnTag::generateID()
{
    ID_SEQ++;
    mID = ID_SEQ;
}

bool GridbrainConnTag::isEquivalentOrigin(GridbrainConnTag* tag)
{
    return ((mGroupID == tag->mGroupID) && (mNextID == tag->mNextID));
}

bool GridbrainConnTag::isEquivalentTarget(GridbrainConnTag* tag)
{
    return ((mGroupID == tag->mGroupID) && (mPrevID == tag->mPrevID));
}

