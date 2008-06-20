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

#include "Message.h"

#include <stdlib.h>

Message::Message(unsigned int dataSize)
{
    mSymbol = NULL;
    mType = 0;
    mDataSize = dataSize;
    
    if (mDataSize > 0)
    {
        mData = (float*)malloc(mDataSize * sizeof(float));
    }
    else
    {
        mData = NULL;
    }
    mTime = 0;
}

Message::Message(Message* msg)
{
    mSymbol = msg->mSymbol->clone();
    mType = msg->mType;
    mDataSize = msg->mDataSize;
    mData = (float*)malloc(mDataSize * sizeof(float));
    memcpy(mData, msg->mData, mDataSize * sizeof(float));
    mTime = msg->mTime;
}

Message::~Message()
{
    if (mSymbol != NULL)
    {
        delete mSymbol;
        mSymbol = NULL;
    }
    if (mData != NULL)
    {
        free(mData);
        mData = NULL;
    }
}

