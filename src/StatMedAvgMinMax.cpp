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

#include "StatMedAvgMinMax.h"

StatMedAvgMinMax::StatMedAvgMinMax(lua_State* luaState)
{
}

StatMedAvgMinMax::~StatMedAvgMinMax()
{
}

void StatMedAvgMinMax::init()
{
    fprintf(mFile, "sim_time");

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
        fprintf(mFile, "_min", (*iterField).c_str());
        fprintf(mFile, ",%s", (*iterField).c_str());
        fprintf(mFile, "_max", (*iterField).c_str());
    }

    fprintf(mFile, "\n");
    fflush(mFile);
}

void StatMedAvgMinMax::process(SimulationObject* obj)
{
    list<list<float> >::iterator iterValueList = mValueLists.begin();

    for (list<string>::iterator iterField = mFields.begin();
        iterField != mFields.end();
        iterField++)
    {
        float value = obj->getFieldValue(*iterField);
        (*iterValueList).push_back(value);
        iterValueList++;
    }
}

void StatMedAvgMinMax::dump(unsigned long time, double realTime)
{
    fprintf(mFile, "%d", time);

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

        (*iterValueList).clear();

        float avg = 0.0f;

        if (count != 0)
        {
            avg = sum / ((float)count);
        }

        med = (medianValue1 + medianValue2) / 2.0f;

        fprintf(mFile, ",%f,%f,%f,%f", med, avg, min, max);
    }
    fprintf(mFile, "\n");
    fflush(mFile);
}

const char StatMedAvgMinMax::mClassName[] = "StatMedAvgMinMax";

Orbit<StatMedAvgMinMax>::MethodType StatMedAvgMinMax::mMethods[] = {
    {"addField", &Statistics::addField},
    {"setFile", &Statistics::setFile},
    {0,0}
};

Orbit<StatMedAvgMinMax>::NumberGlobalType StatMedAvgMinMax::mNumberGlobals[] = {{0,0}};

