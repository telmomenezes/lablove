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

#include "UnitTest++.h"
#include "SymbolRGB.h"

#include <stdexcept>

TEST(TestCreateSymbolRGB)
{
    SymbolRGB sym(50, 100, 200);
    CHECK_EQUAL(sym.mRed, 50);
    CHECK_EQUAL(sym.mGreen, 100);
    CHECK_EQUAL(sym.mBlue, 200);
}

TEST(TestCloneSymbolRGB)
{
    SymbolRGB sym(50, 100, 200);
    SymbolRGB* sym2 = (SymbolRGB*)sym.clone();
    CHECK_EQUAL(sym2->mRed, 50);
    CHECK_EQUAL(sym2->mGreen, 100);
    CHECK_EQUAL(sym2->mBlue, 200);
    delete sym2;
}

TEST(TestSymbolRGBBind1)
{
    SymbolRGB sym1(0, 0, 0);
    SymbolRGB sym2(0, 0, 0);
    CHECK_CLOSE(sym1.bind(&sym2), 1.0f, 0.0001f);
}

TEST(TestSymbolRGBBind2)
{
    SymbolRGB sym1(0, 0, 0);
    SymbolRGB sym2(255, 255, 255);
    CHECK_CLOSE(sym1.bind(&sym2), 0.0f, 0.0001f);
}

TEST(TestSymbolRGBBind3)
{
    SymbolRGB sym1(0, 0, 0);
    SymbolRGB sym2(255, 0, 0);
    CHECK_CLOSE(sym1.bind(&sym2), 0.4227f, 0.0001f);
}

TEST(TestSymbolRGBBind4)
{
    SymbolRGB sym1(0, 0, 0);
    SymbolRGB sym2(0, 255, 0);
    CHECK_CLOSE(sym1.bind(&sym2), 0.4227f, 0.0001f);
}

TEST(TestSymbolRGBBind5)
{
    SymbolRGB sym1(0, 0, 255);
    SymbolRGB sym2(0, 0, 0);
    CHECK_CLOSE(sym1.bind(&sym2), 0.4227f, 0.0001f);
}

TEST(TestSymbolRGBBind6)
{
    SymbolRGB sym1(0, 255, 0);
    SymbolRGB sym2(255, 0, 0);
    CHECK_CLOSE(sym1.bind(&sym2), 0.1835f, 0.0001f);
}

TEST(TestSymbolRGBBind7)
{
    SymbolRGB sym1(10, 123, 54);
    SymbolRGB sym2(216, 2, 15);
    CHECK_CLOSE(sym1.bind(&sym2), 0.4519f, 0.0001f);
}

