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

#ifndef _INCLUDE_INTERFACE_H
#define _INCLUDE_INTERFACE_H

#include "types.h"

class Interface
{
public:
    Interface(){}
    virtual ~Interface(){}

    void setSymbolLink(int origTable, gbULINT origID, int targTable, bool dynamic);

    int getOrigSymTable(){return mOrigSymTable;}
    gbULINT getOrigSymID(){return mOrigSymID;}
    int getTargetSymTable(){return mTargetSymTable;}
    bool isDynamic(){return mDynamic;}

    void setOrigSymTable(int table){mOrigSymTable = table;}
    void setOrigSymID(gbULINT id){mOrigSymID = id;}
    void setTargetSymTable(int table){mTargetSymTable = table;}

protected:
    int mOrigSymTable;
    gbULINT mOrigSymID;
    int mTargetSymTable;
    bool mDynamic;
};

#endif

