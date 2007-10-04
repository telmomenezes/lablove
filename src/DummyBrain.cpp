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

#include "DummyBrain.h"

DummyBrain::DummyBrain(unsigned int channelCount)
{

    mChannelCount = channelCount;
    create();
}

DummyBrain::DummyBrain(lua_State* luaState)
{
    mChannelCount = luaL_checkint(luaState, 1);
    create();
}

DummyBrain::~DummyBrain()
{
    if (mInputBuffer != NULL)
    {
        free(mInputBuffer);
        mInputBuffer = NULL;
    }

    for (unsigned int i = 0; i < mChannelCount; i++)
    {
        delete mInputInterfacesVector[i];
        delete mInputInterfacesNames[i];
    }
}

void DummyBrain::create()
{
    mInputBuffer = NULL;

    for (unsigned int i = 0; i < mChannelCount; i++)
    {
        list<InterfaceItem*>* interfaceList = new list<InterfaceItem*>();
        mInputInterfacesVector.push_back(interfaceList);
        list<string>* nameList = new list<string>();
        mInputInterfacesNames.push_back(nameList);
        mInputDepths.push_back(0);
        mLastInputDepths.push_back(0);
    }
}

void DummyBrain::init()
{
    unsigned int perceptionsCount = 0;
    for (vector<list<InterfaceItem*>*>::iterator iterInterfacesVector = mInputInterfacesVector.begin();
            iterInterfacesVector != mInputInterfacesVector.end();
            iterInterfacesVector++)
    {
        list<InterfaceItem*>* inputInterface = *iterInterfacesVector;
        mOffsetsVector.push_back(inputInterface->size());
        perceptionsCount += inputInterface->size();
    }

    mInputBuffer = (float*)malloc(perceptionsCount * 50 * sizeof(float));
}

float* DummyBrain::getInputBuffer(unsigned int channel)
{
    float* buffer = mInputBuffer + (mOffsetsVector[channel] * 50) + mInputDepths[channel];
    mInputDepths[channel] = mInputDepths[channel] + 1;
    return buffer;
}

void DummyBrain::addPerception(string name, unsigned int channel, unsigned int type)
{
    InterfaceItem* item = new InterfaceItem();
    item->mType = type;
    mInputInterfacesVector[channel]->push_back(item);
    mInputInterfacesNames[channel]->push_back(name);
}

void DummyBrain::cycle()
{
    for (unsigned int chan = 0; chan < mChannelCount; chan++)
    {
        mLastInputDepths[chan] = mInputDepths[chan];
        mInputDepths[chan] = 0;
    }
}

void DummyBrain::draw(pyc::Layer2D* layer)
{
    layer->setColor(50, 50, 50);

    float y = 15.0f;

    for (unsigned int chan = 0; chan < mChannelCount; chan++)
    {

        list<string>::iterator iterName = mInputInterfacesNames[chan]->begin();
        for (list<InterfaceItem*>::iterator iterInterface = mInputInterfacesVector[chan]->begin();
                iterInterface != mInputInterfacesVector[chan]->end();
                iterInterface++)
        {
            layer->drawText(10, y, *iterName);

            float* buffer = mInputBuffer + (mOffsetsVector[channel] * 50);

            y += 15.0f;
            iterName++;
        }
    }
}

const char DummyBrain::mClassName[] = "DummyBrain";

Orbit<DummyBrain>::MethodType DummyBrain::mMethods[] = {
    {"addPerception", &DummyBrain::addPerception},
    {0,0}
};

Orbit<DummyBrain>::NumberGlobalType DummyBrain::mNumberGlobals[] = {{0,0}};

int DummyBrain::addPerception(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    unsigned int channel = luaL_checkint(luaState, 2);
    unsigned int type = luaL_checkint(luaState, 3);
    addPerception(name, channel, type);
    return 0;
}

