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

#ifndef _INCLUDE_GRIDBRAIN_CONNECTION_H
#define _INCLUDE_GRIDBRAIN_CONNECTION_H

typedef struct
{
    unsigned long mColumnOrig;
    unsigned long mRowOrig;
    unsigned int mGridOrig;
    unsigned long mColumnTarg;
    unsigned long mRowTarg;
    unsigned int mGridTarg;
    unsigned int mTarget;
    bool mInterGrid;
    float mWeight;
    float mRealWeight;
    void* mPrevConnection;
    void* mNextConnection;
    void* mPrevGlobalConnection;
    void* mNextGlobalConnection;
    void* mOrigComponent;
    void* mTargComponent;
    double mAge;
} GridbrainConnection;

#endif

