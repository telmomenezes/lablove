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

#include "Gridbrain.h"

bool Gridbrain::isRedundant(GridbrainComponent* comp1, GridbrainComponent* comp2)
{
    if (!comp1->isEqual(comp2))
    {
        return false;
    }

    list<int> tags;

    GridbrainConnection* conn = comp2->mFirstInConnection;

    while (conn != NULL)
    {
        GridbrainComponent* origComp = (GridbrainComponent*)conn->mOrigComponent;

        int origTag = origComp->mRedTag;

        if (origTag == -1)
        {
            return false;
        }

        tags.push_back(origTag);

        conn = (GridbrainConnection*)conn->mNextInConnection;
    }

    conn = comp1->mFirstInConnection;

    while (conn != NULL)
    {
        GridbrainComponent* origComp = (GridbrainComponent*)conn->mOrigComponent;

        int origTag = origComp->mRedTag;

        bool found = false;
        for (list<int>::iterator iterTag = tags.begin();
                (iterTag != tags.end()) && (!found);
                iterTag++)
        {
            if ((*iterTag) == origTag)
            {
                found = true;
            }
        }

        if (!found)
        {
            return false;
        }

        tags.remove(origTag);

        conn = (GridbrainConnection*)conn->mNextInConnection;
    }

    if (!tags.empty())
    {
        return false;
    }

    return true;
}

void Gridbrain::calcRedundancyTags()
{
    int currentTag = 0;

    for (unsigned int pos1 = 0; pos1 < mNumberOfComponents; pos1++)
    {
        GridbrainComponent* comp1 = &mComponents[pos1];

        if (comp1->mRedTag == -1)
        {
            comp1->mRedTag = currentTag;

            for (unsigned int pos2 = pos1 + 1; pos2 < mNumberOfComponents; pos2++)
            {
                GridbrainComponent* comp2 = &mComponents[pos2];

                if (comp2->mRedTag == -1)
                {
                    if (isRedundant(comp1, comp2))
                    {
                        comp2->mRedTag = currentTag;
                    }
                }
            }

            currentTag++;
        }
    }
}

void Gridbrain::cleanRedundant()
{
    calcRedundancyTags();

    for (int pos1 = mNumberOfComponents - 1; pos1 >= 0; pos1--)
    {
        GridbrainComponent* comp1 = &mComponents[pos1];

        GridbrainConnection* conn1 = comp1->mFirstInConnection;
        while (conn1 != NULL)
        {
            int origTag1 = ((GridbrainComponent*)conn1->mOrigComponent)->mRedTag;
            int targTag1 = ((GridbrainComponent*)conn1->mTargComponent)->mRedTag;

            GridbrainConnection* nextConn1 = (GridbrainConnection*)conn1->mNextInConnection;

            GridbrainConnection* conn2 = mConnections;
            while (conn2 != NULL)
            {
                GridbrainConnection* nextConn2 = (GridbrainConnection*)conn2->mNextGlobalConnection;
                if (conn1 != conn2)
                {
                    GridbrainComponent* targComp2 = (GridbrainComponent*)conn2->mTargComponent;

                    if (targComp2->mConnectionsCount == 0)
                    {
                        int origTag2 = ((GridbrainComponent*)conn2->mOrigComponent)->mRedTag;
                        int targTag2 = targComp2->mRedTag;

                        if ((origTag1 == origTag2)
                            && (targTag1 == targTag2))
                        {
                            if (nextConn1 == conn2)
                            {
                                nextConn1 = (GridbrainConnection*)nextConn1->mNextInConnection;
                            }

                            if (nextConn2 != NULL)
                            {
                                nextConn2 = (GridbrainConnection*)nextConn2->mNextGlobalConnection;
                            }

                            removeConnection(conn2);
                            //printf("redundant removed!\n");
                        }
                    }
                }

                conn2 = nextConn2;
            }

            conn1 = nextConn1;
        }
    }
}

