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

#include "StatCommon.h"

StatCommon::StatCommon(lua_State* luaState)
{
    mCount = 0;
}

StatCommon::~StatCommon()
{
}

void StatCommon::init()
{
    fprintf(mFile, "sim_time,count");

    for (list<string>::iterator iterField = mFields.begin();
        iterField != mFields.end();
        iterField++)
    {
        list<float> valueList;
        mValueLists.push_back(valueList);
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_med", (*iterField).c_str());
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_avg", (*iterField).c_str());
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_sd", (*iterField).c_str());
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_min", (*iterField).c_str());
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_max", (*iterField).c_str());
    }

    fprintf(mFile, "\n");
    fflush(mFile);
}

void StatCommon::process(SimObj* obj, Simulation* sim)
{
    list<list<float> >::iterator iterValueList = mValueLists.begin();

    for (list<string>::iterator iterField = mFields.begin();
        iterField != mFields.end();
        iterField++)
    {
        float value;
        sim->getFieldValue(obj, *iterField, value);
        (*iterValueList).push_back(value);
        iterValueList++;
    }

    mCount++;
}

void StatCommon::dump(llULINT time, double realTime)
{
    fprintf(mFile, "%d", time / 1000);
    fprintf(mFile, ",%d", mCount);

    for (list<list<float> >::iterator iterValueList = mValueLists.begin();
        iterValueList != mValueLists.end();
        iterValueList++)
    {
        (*iterValueList).sort();

        float med = 0.0f;
        float max = 0.0f;
        float min = 0.0f;
        float sum = 0.0f;
        int count = (*iterValueList).size();
        bool firstVal = true;

        int medianIndex1 = count / 2;
        int medianIndex2 = medianIndex1;
        if ((count % 2) == 0)
        {
            medianIndex2--;
        }

        float medianValue1 = 0.0f;
        float medianValue2 = 0.0f;

        int index = 0;  

        for (list<float>::iterator iterValue = (*iterValueList).begin();
            iterValue != (*iterValueList).end();
            iterValue++)
        {
            float value = (*iterValue);

            if (firstVal)
            {
                max = value;
                min = value;
                firstVal = false;
            }
            else
            {
                if (value > max)
                {
                    max = value;
                }
                else if (value < min)
                {
                    min = value;
                }

            }

            sum += value;

            if (index == medianIndex1)
            {
                medianValue1 = value;
            }
            if (index == medianIndex2)
            {
                medianValue2 = value;
            }
            index++;
        }

        float avg = 0.0f;

        if (count != 0)
        {
            avg = sum / ((float)count);
        }

        med = (medianValue1 + medianValue2) / 2.0f;

        float var = 0.0f;

        if (count != 0.0f)
        {
            for (list<float>::iterator iterValue = (*iterValueList).begin();
                    iterValue != (*iterValueList).end();
                    iterValue++)
            {
                float value = (*iterValue);
                float dif = value - avg;
                var += dif * dif;
            }

            var /= count;
        }

        float sd = sqrtf(var);

        (*iterValueList).clear();

        fprintf(mFile, ",%f,%f,%f,%f,%f", med, avg, sd, min, max);
    }
    fprintf(mFile, "\n");
    fflush(mFile);

    mCount = 0;
}

const char StatCommon::mClassName[] = "StatCommon";

Orbit<StatCommon>::MethodType StatCommon::mMethods[] = {
    {"addField", &Statistics::addField},
    {"setFile", &Statistics::setFile},
    {0,0}
};

Orbit<StatCommon>::NumberGlobalType StatCommon::mNumberGlobals[] = {{0,0}};

