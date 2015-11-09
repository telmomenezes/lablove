/*
 * Gridbrain
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

#include "Gene.h"

#include <stdio.h>

namespace gb
{

gbULINT Gene::ID_SEQ = 0;

Gene::Gene()
{
    mGeneID = 0;
    mOrigID = 0;
    mTargID = 0;
}

Gene::Gene(const Gene& tag)
{
    mGeneID = tag.mGeneID;
    mOrigID = tag.mOrigID;
    mTargID = tag.mTargID;
}

Gene::~Gene()
{
}

gbULINT Gene::generateID()
{
    ID_SEQ++;
    return ID_SEQ;
}

bool Gene::isEquivalentOrigin(Gene* tag)
{
    return ((mGeneID == tag->mGeneID) && (mTargID == tag->mTargID));
}

bool Gene::isEquivalentTarget(Gene* tag)
{
    return ((mGeneID == tag->mGeneID) && (mOrigID == tag->mOrigID));
}

void Gene::print()
{
    printf(" g: %llu", mGeneID);
    printf(" o: %llu", mOrigID);
    printf(" t: %llu", mTargID);
}

}

