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

#if !defined(__INCLUDE_SPECIES_DATA_H)
#define __INCLUDE_SPECIES_DATA_H

#include <vector>

using std::vector;

class SpeciesData
{
public:
    SimObj* mBaseOrganism;
    vector<SimObj*> mOrganismVector;
    list<Log*> mDeathLogs;
    list<Log*> mSampleLogs;
    unsigned int mPopulation;
    unsigned int mBufferSize;
    int mTournamentSize;
    bool mRoulette;

    float mGroupFactor;
    bool mQueen;
    unsigned int mCurrentQueen;
    unsigned int mQueenState;
    SimObj* mSuperSister;
};
#endif

