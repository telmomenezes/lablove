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

#include "GridbrainGeneTag.h"

#include <stdio.h>

llULINT GridbrainGeneTag::ID_SEQ = 0;

GridbrainGeneTag::GridbrainGeneTag()
{
    mGeneID = 0;
    mOrigID = 0;
    mTargID = 0;
}

GridbrainGeneTag::GridbrainGeneTag(const GridbrainGeneTag& tag)
{
    mGeneID = tag.mGeneID;
    mOrigID = tag.mOrigID;
    mTargID = tag.mTargID;
}

GridbrainGeneTag::~GridbrainGeneTag()
{
}

llULINT GridbrainGeneTag::generateID()
{
    ID_SEQ++;
    return ID_SEQ;
}

bool GridbrainGeneTag::isEquivalentOrigin(GridbrainGeneTag* tag)
{
    return ((mGeneID == tag->mGeneID) && (mTargID == tag->mTargID));
}

bool GridbrainGeneTag::isEquivalentTarget(GridbrainGeneTag* tag)
{
    return ((mGeneID == tag->mGeneID) && (mOrigID == tag->mOrigID));
}

void GridbrainGeneTag::print()
{
    printf(" g: %d", mGeneID);
    printf(" o: %d", mOrigID);
    printf(" t: %d", mTargID);
}

