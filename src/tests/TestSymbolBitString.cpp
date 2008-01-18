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
#include "SymbolBitString.h"

#include <stdexcept>

TEST(TestCreateSymbolBitString1)
{
    SymbolBitString sym("1001");
    CHECK_EQUAL(sym.getBit(0), 1);
    CHECK_EQUAL(sym.getBit(1), 0);
    CHECK_EQUAL(sym.getBit(2), 0);
    CHECK_EQUAL(sym.getBit(3), 1);
    CHECK_EQUAL(sym.getLength(), 4);
}

TEST(TestCreateSymbolBitString2)
{
    SymbolBitString sym("10*01");
    CHECK_EQUAL(sym.getBit(0), 1);
    CHECK_EQUAL(sym.getBit(1), 0);
    CHECK_EQUAL(sym.getBit(2), -1);
    CHECK_EQUAL(sym.getBit(3), 0);
    CHECK_EQUAL(sym.getBit(4), 1);
    CHECK_EQUAL(sym.getLength(), 5);
}

TEST(TestCloneSymbolBitString)
{
    SymbolBitString sym("10*01");
    SymbolBitString* sym2 = (SymbolBitString*)sym.clone();
    CHECK_EQUAL(sym2->getBit(0), 1);
    CHECK_EQUAL(sym2->getBit(1), 0);
    CHECK_EQUAL(sym2->getBit(2), -1);
    CHECK_EQUAL(sym2->getBit(3), 0);
    CHECK_EQUAL(sym2->getBit(4), 1);
    CHECK_EQUAL(sym2->getLength(), 5);
    delete sym2;
}

TEST(TestSymbolBitStringDistance1)
{
    SymbolBitString sym1("0000");
    SymbolBitString sym2("0000");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance2)
{
    SymbolBitString sym1("1111");
    SymbolBitString sym2("1111");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance3)
{
    SymbolBitString sym1("0000");
    SymbolBitString sym2("1111");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 1.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance4)
{
    SymbolBitString sym1("0011");
    SymbolBitString sym2("1100");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 1.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance5)
{
    SymbolBitString sym1("0101");
    SymbolBitString sym2("1010");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 1.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance6)
{
    SymbolBitString sym1("010101");
    SymbolBitString sym2("000000");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.5f, 0.0001f);
}

TEST(TestSymbolBitStringDistance7)
{
    SymbolBitString sym1("010101");
    SymbolBitString sym2("******");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.0f, 0.0001f);
}

TEST(TestSymbolBitStringDistance8)
{
    SymbolBitString sym1("010101");
    SymbolBitString sym2("1010**");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.666666f, 0.0001f);
}

TEST(TestSymbolBitStringDistance9)
{
    SymbolBitString sym1("0");
    SymbolBitString sym2("01");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.5, 0.0001f);
}

TEST(TestSymbolBitStringDistance10)
{
    SymbolBitString sym1("11");
    SymbolBitString sym2("1");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.5, 0.0001f);
}

TEST(TestSymbolBitStringDistance11)
{
    SymbolBitString sym1("*");
    SymbolBitString sym2("**");
    float distance = sym1.getDistance(&sym2);
    CHECK_CLOSE(distance, 0.5, 0.0001f);
}

