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
        mOffsetsVector.push_back(perceptionsCount);
        perceptionsCount += inputInterface->size();
    }

    mInputBuffer = (float*)malloc(perceptionsCount * 50 * sizeof(float));
}

float* DummyBrain::getInputBuffer(unsigned int channel)
{
    unsigned int perceptionCount = mInputInterfacesVector[channel]->size();
    unsigned int offset = (mOffsetsVector[channel] * 50) + (mInputDepths[channel] * perceptionCount);
    float* buffer = mInputBuffer + offset;
    if (mInputDepths[channel] < 49)
    {
        mInputDepths[channel] = mInputDepths[channel] + 1;
    }
    return buffer;
}

void DummyBrain::addPerception(string name,
                                unsigned int channel,
                                unsigned int type,
                                int symTable,
                                int origSymIndex,
                                int targetSymIndex)
{
    InterfaceItem* item = new InterfaceItem();
    item->mType = type;
    item->mSymTable = symTable;
    item->mOrigSymIndex = origSymIndex;
    item->mTargetSymIndex = targetSymIndex;
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
    if (!mInputBuffer)
    {
        return;
    }

    layer->setColor(50, 50, 50);

    char text[255];
    float y = 15.0f;

    for (unsigned int chan = 0; chan < mChannelCount; chan++)
    {
        unsigned int perception = 0;
        unsigned int perceptionCount = mInputInterfacesVector[chan]->size();

        list<string>::iterator iterName = mInputInterfacesNames[chan]->begin();
        for (list<InterfaceItem*>::iterator iterInterface = mInputInterfacesVector[chan]->begin();
                iterInterface != mInputInterfacesVector[chan]->end();
                iterInterface++)
        {
            layer->drawText(10, y, *iterName);

            float x = 100.0f;
            for (unsigned int depth = 0; depth < mLastInputDepths[chan]; depth++)
            {
                unsigned int offset = (mOffsetsVector[chan] * 50) + (depth * perceptionCount) + perception;
                sprintf(text, "%f", mInputBuffer[offset]);
                layer->drawText(x, y, text);
                x += 100.0f;
            }

            y += 15.0f;
            iterName++;
            perception++;
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
    int symTable = luaL_optint(luaState, 4, -1);
    int origSymIndex = luaL_optint(luaState, 5, -1);
    int targetSymIndex = luaL_optint(luaState, 6, -1);
    addPerception(name, channel, type, symTable, origSymIndex, targetSymIndex);
    return 0;
}

