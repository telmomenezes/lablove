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

#include "GraphicBiomorph.h"
#include "SimulationObject.h"
#include "SymbolRGB.h"
#include "SymbolFloat.h"
#include "SymbolFloatVector.h"
#include "Simulation.h"

#include <stdexcept>

GraphicBiomorph::GraphicBiomorph(lua_State* luaState)
{
    mObject = NULL;
    mSize = 0;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mLineCount = 0;
    mX1Points = NULL;
    mY1Points = NULL;
    mX2Points = NULL;
    mY2Points = NULL;
    mCurrentLine = 0;
}

GraphicBiomorph::~GraphicBiomorph()
{
    if (mX1Points != NULL)
    {
        delete mX1Points;
        mX1Points = NULL;
    }

    if (mY1Points != NULL)
    {
        delete mY1Points;
        mY1Points = NULL;
    }

    if (mX2Points != NULL)
    {
        delete mX2Points;
        mX2Points = NULL;
    }

    if (mY2Points != NULL)
    {
        delete mY2Points;
        mY2Points = NULL;
    }

    mWindow->removeLayer(mLayer);
}

Graphic* GraphicBiomorph::clone()
{
    return new GraphicBiomorph();
}

void GraphicBiomorph::init(SimulationObject* obj, pyc::Pycasso* pycasso)
{
    mWindow = pycasso->getWindow();
    mLayer = mWindow->createLayer2D(128, 128);

    mObject = obj;

    mXIndex = obj->getNamedFloatDataIndex("x");
    if (mXIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'x' named data item");
    }
    mYIndex = obj->getNamedFloatDataIndex("y");
    if (mYIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'y' named data item");
    }
    mRotIndex = obj->getNamedFloatDataIndex("rot");
    if (mRotIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'rot' named data item");
    }

    SymbolFloat* symSize = (SymbolFloat*)obj->getSymbolByName("size");

    if (symSize == NULL)
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'size' named symbol");
    }
    else
    {
        mSize = symSize->getFloat();
    }

    SymbolRGB* color = (SymbolRGB*)(obj->getSymbolByName("color"));

    if (color != NULL)
    {
        int red = color->mRed;
        int green = color->mGreen;
        int blue = color->mBlue;

        int firstColor = 0;
        int secondColor = 0;
        int thirdColor = 0;

        int rgb[3];
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;

        if (green < rgb[firstColor])
        {
            firstColor = 1;
        }
        if (blue < rgb[firstColor])
        {
            firstColor = 2;
        }
        if (green >= rgb[secondColor])
        {
            secondColor = 1;
        }
        if (blue >= rgb[secondColor])
        {
            secondColor = 2;
        }
        if ((firstColor != 0) && (thirdColor != 0))
        {
            secondColor = 0;
        }
        if ((firstColor != 1) && (thirdColor != 1))
        {
            secondColor = 1;
        }
        if ((firstColor != 2) && (thirdColor != 2))
        {
            secondColor = 2;
        }

        rgb[thirdColor] *= 3;
        if (rgb[thirdColor] > 255)
        {
            rgb[thirdColor] = 255;
        }

        rgb[secondColor] /= 3;
        rgb[firstColor] = 0;

        mRed = rgb[0];
        mGreen = rgb[1];
        mBlue = rgb[2];
    }
    else
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'color' named symbol");
    }

    SymbolFloatVector* symFloatVector = (SymbolFloatVector*)obj->getSymbolByName("biomorph");

    if (symFloatVector == NULL)
    {
        throw std::runtime_error("Failed to initialize GraphicBiomorph: object does not define 'biomorph' named symbol");
    }
    else
    {
        for (unsigned int i = 0; i < 19; i++)
        {
            mParameters[i] = symFloatVector->getItem(i);
        }
    }

    for (unsigned int i = 0; i < 10; i++)
    {
        mParameters[i] *= 5;
    }

    mParameters[8] += 3;
    mParameters[9] += 1;
    mParameters[10] *= 19;
    mParameters[10] += 1;

    int branching = (int)mParameters[8];
    int segments = (int)mParameters[9];
    segments = 1;

    for (unsigned int i = 0; i <= branching; i++)
    {
        mLineCount += pow(2, i);
    }
    mLineCount *= segments;
    mX1Points = (float*)malloc(mLineCount * sizeof(float));
    mY1Points = (float*)malloc(mLineCount * sizeof(float));
    mX2Points = (float*)malloc(mLineCount * sizeof(float));
    mY2Points = (float*)malloc(mLineCount * sizeof(float));

    mDX[3] = mParameters[0];
    mDX[4] = mParameters[1];
    mDX[5] = mParameters[2];
    mDX[1] = -mDX[3];
    mDX[0] = -mDX[4];
    mDX[2] = 0;
    mDX[6] = 0;
    mDX[7] = -mDX[5];
    mDY[2] = mParameters[3];
    mDY[3] = mParameters[4];
    mDY[4] = mParameters[5];
    mDY[5] = mParameters[6];
    mDY[6] = mParameters[7];
    mDY[0] = mDY[4];
    mDY[1] = mDY[3];
    mDY[7] = mDY[5];

    float spacing = mParameters[10];

    mLayer->setColor(mRed, mGreen, mBlue);
    mLayer->setLineWidth(5.0f);

    for (unsigned int i = 0; i < segments; i++)
    {
        calcTree(0, -i * spacing, branching, 2);
    }

    normalizePoints();
}

void GraphicBiomorph::calcTree(float x, float y, int length, int dir)
{
    if (dir < 0)
    {
        dir += 8;
    }
    if (dir >= 8)
    {
        dir -= 8;
    }
    
    float newX = x + length * mDX[dir];
    float newY = y - length * mDY[dir];

    mX1Points[mCurrentLine] = x;
    mY1Points[mCurrentLine] = y;
    mX2Points[mCurrentLine] = newX;
    mY2Points[mCurrentLine] = newY;
    mCurrentLine++;

    if (length > 0)
    {
        calcTree(newX, newY, length - 1, dir - 1);
        calcTree(newX, newY, length - 1, dir + 1);
    }
}

void GraphicBiomorph::normalizePoints()
{
    float maxX = -9999999999.9f;
    float maxY = -9999999999.9f;
    float minX = 9999999999.9f;
    float minY = 9999999999.9f;

    for (unsigned int i = 0; i < mLineCount; i++)
    {
        if (mX1Points[i] > maxX)
        {
            maxX = mX1Points[i];
        }
        if (mX2Points[i] > maxX)
        {
            maxX = mX2Points[i];
        }
        if (mY1Points[i] > maxY)
        {
            maxY = mY1Points[i];
        }
        if (mY2Points[i] > maxY)
        {
            maxY= mY2Points[i];
        }
        if (mX1Points[i] < minX)
        {
            minX = mX1Points[i];
        }
        if (mX2Points[i] < minX)
        {
            minX = mX2Points[i];
        }
        if (mY1Points[i] < minY)
        {
            minY = mY1Points[i];
        }
        if (mY2Points[i] < minY)
        {
            minY= mY2Points[i];
        }
    }

    float spanX = maxX - minX;
    float spanY = maxY - minY;

    float side = spanX;
    if (spanY > side)
    {
        side = spanY;
    }

    spanX /= side;
    spanY /= side;

    float startX = (1.0f - spanX) * 63.5f;
    float startY = (1.0f - spanY) * 63.5f;

    for (unsigned int i = 0; i < mLineCount; i++)
    {
        mX1Points[i] -= minX;
        mY1Points[i] -= minY;
        mX2Points[i] -= minX;
        mY2Points[i] -= minY;

        mX1Points[i] /= side;
        mY1Points[i] /= side;
        mX2Points[i] /= side;
        mY2Points[i] /= side;

        mX1Points[i] *= 127;
        mY1Points[i] *= 127;
        mX2Points[i] *= 127;
        mY2Points[i] *= 127;

        mX1Points[i] += startX;
        mY1Points[i] += startY;
        mX2Points[i] += startX;
        mY2Points[i] += startY;

        mLayer->drawLine(mX1Points[i],
                            mY1Points[i],
                            mX2Points[i],
                            mY2Points[i]);

        mX1Points[i] *= mSize;
        mY1Points[i] *= mSize;
        mX2Points[i] *= mSize;
        mY2Points[i] *= mSize;
    }
}

void GraphicBiomorph::draw(pyc::Layer* layer)
{
    pyc::Layer2D* layer2D = (pyc::Layer2D*)layer;

    float rot = mObject->mFloatData[mRotIndex];
    float centerX = mObject->mFloatData[mXIndex];
    float centerY = mObject->mFloatData[mYIndex];

    float side = mSize * 3;
    float halfSide = side / 2;

    layer2D->setColor(255, 255, 255);
    layer2D->setRotation(centerX, centerY, rot - (M_PI / 2.0f));
    layer2D->drawLayer(mLayer, centerX - halfSide, centerY - halfSide, side, side);
    layer2D->clearRotation();
}

const char GraphicBiomorph::mClassName[] = "GraphicBiomorph";

Orbit<GraphicBiomorph>::MethodType GraphicBiomorph::mMethods[] = {{0,0}};

Orbit<GraphicBiomorph>::NumberGlobalType GraphicBiomorph::mNumberGlobals[] = {{0,0}};

