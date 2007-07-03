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
#include "Lab.h"

StatMedAvgMinMax::StatMedAvgMinMax(lua_State* luaState)
{
}

StatMedAvgMinMax::~StatMedAvgMinMax()
{
}

void StatMedAvgMinMax::init()
{
	for (unsigned int i = 0; i < mFields.size(); i++)
	{
		std::list<float> valueList;
		mValueLists.push_back(valueList);
	}
}

void StatMedAvgMinMax::process(SimulationObject* obj)
{
	std::list<std::list<float> >::iterator iterValueList = mValueLists.begin();

	for (std::list<std::string>::iterator iterField = mFields.begin();
		iterField != mFields.end();
		iterField++)
	{
		float value = obj->getFieldValue(*iterField);
		(*iterValueList).push_back(value);
		iterValueList++;
	}
}

void StatMedAvgMinMax::dump()
{
	fprintf(mFile, "%d", Lab::getSingleton().getSimulation()->time());

	for (std::list<std::list<float> >::iterator iterValueList = mValueLists.begin();
		iterValueList != mValueLists.end();
		iterValueList++)
	{
		float max = 0.0f;
		float min = 0.0f;
		float sum = 0.0f;
		float count = 0.0f;
		bool firstVal = true;

		for (std::list<float>::iterator iterValue = (*iterValueList).begin();
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

				sum += value;
				count += 1.0f;
			}
		}

		float avg = 0.0f;

		if (count != 0.0f)
		{
			avg = sum / count;
		}

		fprintf(mFile, ",%f,%f,%f", avg, min, max);
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

