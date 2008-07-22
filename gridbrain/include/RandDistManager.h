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

#if !defined(__INCLUDE_RAND_DIST_MANAGER_H)
#define __INCLUDE_RAND_DIST_MANAGER_H

#include "randistrs.h"
#include <list>

using std::list;

namespace gb
{

class RandDistManager
{
public:
    RandDistManager();
    virtual ~RandDistManager();

    void setSeedIndex(int index=-1);
    void setSeed(unsigned long seed);
    mt_distribution* getNewDistribution();

protected:
    mt_distribution mSeedGenerator;
    list<mt_distribution*> mDistributions;
};

static RandDistManager gDistManager;

}

#endif

