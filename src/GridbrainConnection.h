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

#ifndef _INCLUDE_GRIDBRAIN_CONNECTION_H
#define _INCLUDE_GRIDBRAIN_CONNECTION_H

#include "types.h"
#include "GridbrainGeneTag.h"

class GridbrainConnection
{
public:
    enum SelectionState {SS_UNKNOWN, SS_SELECTED, SS_UNSELECTED};
    
    GridbrainConnection();
    virtual ~GridbrainConnection();

    unsigned int mColumnOrig;
    unsigned int mRowOrig;
    unsigned int mGridOrig;
    unsigned int mColumnTarg;
    unsigned int mRowTarg;
    unsigned int mGridTarg;
    bool mInterGrid;
    float mWeight;
    float mRealWeight;
    void* mPrevConnection;
    void* mNextConnection;
    void* mPrevInConnection;
    void* mNextInConnection;
    void* mPrevGlobalConnection;
    void* mNextGlobalConnection;
    void* mOrigComponent;
    void* mTargComponent;
    bool mActive;

    SelectionState mSelectionState;
    GridbrainGeneTag mGeneTag;
};

#endif

