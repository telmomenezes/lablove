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
#include "SymbolFloat.h"

#include <stdexcept>

TEST(TestCreateSymbolFloat)
{
    SymbolFloat sym(1.0f);
    CHECK_CLOSE(sym.getFloat(), 1.0f, 0.001f);
    CHECK_CLOSE(sym.getMin(), 0.0f, 0.001f);
    CHECK_CLOSE(sym.getMax(), 1.0f, 0.001f);
}

TEST(TestCreateSymbolFloat2)
{
    SymbolFloat sym(-1.0f, -3.0f, 6.0f);
    CHECK_CLOSE(sym.getFloat(), -1.0f, 0.001f);
    CHECK_CLOSE(sym.getMin(), -3.0f, 0.001f);
    CHECK_CLOSE(sym.getMax(), 6.0f, 0.001f);
}

TEST(TestCloneSymbolFloat)
{
    SymbolFloat sym(-1.0f, -3.0f, 6.0f);
    CHECK_CLOSE(sym.getFloat(), -1.0f, 0.001f);
    CHECK_CLOSE(sym.getMin(), -3.0f, 0.001f);
    CHECK_CLOSE(sym.getMax(), 6.0f, 0.001f);
    SymbolFloat* psym = (SymbolFloat*)(sym.clone());
    CHECK_CLOSE(psym->getFloat(), -1.0f, 0.001f);
    CHECK_CLOSE(psym->getMin(), -3.0f, 0.001f);
    CHECK_CLOSE(psym->getMax(), 6.0f, 0.001f);
    delete psym;
}

TEST(TestBindSymbolFloat1)
{
    SymbolFloat sym1(0.0f);
    SymbolFloat sym2(1.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 1.0f, 0.001f);
}

TEST(TestBindSymbolFloat2)
{
    SymbolFloat sym1(1.0f);
    SymbolFloat sym2(1.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 0.0f, 0.001f);
}

TEST(TestBindSymbolFloat3)
{
    SymbolFloat sym1(0.5f);
    SymbolFloat sym2(1.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 0.5f, 0.001f);
}

TEST(TestBindSymbolFloat4)
{
    SymbolFloat sym1(10.0f, 0.0f, 20.0f);
    SymbolFloat sym2(20.0f, 0.0f, 20.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 0.5f, 0.001f);
}

TEST(TestBindSymbolFloat5)
{
    SymbolFloat sym1(10.0f, -20.0f, 20.0f);
    SymbolFloat sym2(20.0f, -20.0f, 20.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 0.25f, 0.001f);
}

TEST(TestBindSymbolFloat6)
{
    SymbolFloat sym1(-20.0f, -20.0f, 20.0f);
    SymbolFloat sym2(20.0f, -20.0f, 20.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 1.0f, 0.001f);
}

TEST(TestBindSymbolFloat7)
{
    SymbolFloat sym1(-20.0f, -20.0f, 20.0f);
    SymbolFloat sym2(40.0f, -20.0f, 20.0f);
    float bind = sym1.proximity(&sym2);
    CHECK_CLOSE(bind, 1.0f, 0.001f);
}

TEST(TestMutateSymbolFloat1)
{
    SymbolFloat sym(0.5f);
    sym.mutate();
    CHECK(sym.getFloat() >= 0.0f);
    CHECK(sym.getFloat() <= 1.0f);
}

TEST(TestMutateSymbolFloat2)
{
    SymbolFloat sym(0.0f);
    sym.mutate();
    CHECK(sym.getFloat() >= 0.0f);
    CHECK(sym.getFloat() <= 1.0f);
}

TEST(TestMutateSymbolFloat3)
{
    SymbolFloat sym(1.0f);
    sym.mutate();
    CHECK(sym.getFloat() >= 0.0f);
    CHECK(sym.getFloat() <= 1.0f);
}

TEST(TestMutateSymbolFloat4)
{
    SymbolFloat sym(0.0f, -0.5f, 0.5f);
    sym.mutate();
    CHECK(sym.getFloat() >= -0.5f);
    CHECK(sym.getFloat() <= 0.5f);
}

TEST(TestMutateSymbolFloat5)
{
    SymbolFloat sym(-0.5f, -0.5f, 0.5f);
    sym.mutate();
    CHECK(sym.getFloat() >= -0.5f);
    CHECK(sym.getFloat() <= 0.5f);
}

TEST(TestMutateSymbolFloat6)
{
    SymbolFloat sym(0.5f, -0.5f, 0.5f);
    sym.mutate();
    CHECK(sym.getFloat() >= -0.5f);
    CHECK(sym.getFloat() <= 0.5f);
}

