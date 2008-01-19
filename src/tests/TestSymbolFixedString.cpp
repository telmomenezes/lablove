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
#include "SymbolFixedString.h"

#include <stdexcept>

TEST(TestCreateSymbolFixedString)
{
    SymbolFixedString sym("ABC", "AABBCC");
    CHECK(sym.getString() == "AABBCC");
    CHECK(sym.getAlphabet() == "ABC");
    CHECK_EQUAL(sym.getLength(), 6);
    CHECK_EQUAL(sym.getAlphabetLength(), 3);
}

TEST(TestCloneSymbolFixedString)
{
    SymbolFixedString sym("ABC", "AABBCC");
    SymbolFixedString* psym = (SymbolFixedString*)(sym.clone());
    CHECK(psym->getString() == "AABBCC");
    CHECK(psym->getAlphabet() == "ABC");
    CHECK_EQUAL(psym->getLength(), 6);
    CHECK_EQUAL(psym->getAlphabetLength(), 3);
    delete psym;
}

TEST(TestSymbolFixedStringBind1)
{
    SymbolFixedString sym1("ABC", "ABC");
    SymbolFixedString sym2("ABC", "ABC");
    CHECK_CLOSE(sym1.getBinding(&sym2), 0.0f, 0.0001f);
    CHECK_CLOSE(sym2.getBinding(&sym1), 0.0f, 0.0001f);
}

TEST(TestSymbolFixedStringBind2)
{
    SymbolFixedString sym1("ABC", "ABC");
    SymbolFixedString sym2("ABC", "CAB");
    CHECK_CLOSE(sym1.getBinding(&sym2), 1.0f, 0.0001f);
    CHECK_CLOSE(sym2.getBinding(&sym1), 1.0f, 0.0001f);
}

TEST(TestSymbolFixedStringBind3)
{
    SymbolFixedString sym1("ABC", "ABC");
    SymbolFixedString sym2("ABC", "CBA");
    CHECK_CLOSE(sym1.getBinding(&sym2), 2.0f / 3.0f, 0.0001f);
    CHECK_CLOSE(sym2.getBinding(&sym1), 2.0f / 3.0f, 0.0001f);
}

TEST(TestSymbolFixedStringBind4)
{
    SymbolFixedString sym1("ABCD", "ABC");
    SymbolFixedString sym2("ABCD", "ABD");
    CHECK_CLOSE(sym1.getBinding(&sym2), 1.0f / 3.0f, 0.0001f);
    CHECK_CLOSE(sym2.getBinding(&sym1), 1.0f / 3.0f, 0.0001f);
}

TEST(TestSymbolFixedStringReplace1)
{
    SymbolFixedString sym("ABCD", "ABC");
    CHECK(sym.getString() == "ABC");
    sym.replaceChar(0, 3);
    CHECK(sym.getString() == "DBC");
}

TEST(TestSymbolFixedStringReplace2)
{
    SymbolFixedString sym("ABCD", "ABC");
    CHECK(sym.getString() == "ABC");
    sym.replaceChar(0, 2);
    CHECK(sym.getString() == "CBC");
}

TEST(TestSymbolFixedStringReplace3)
{
    SymbolFixedString sym("ABCD", "ABC");
    CHECK(sym.getString() == "ABC");
    sym.replaceChar(1, 3);
    CHECK(sym.getString() == "ADC");
}

TEST(TestSymbolFixedStringReplace4)
{
    SymbolFixedString sym("ABCD", "ABC");
    CHECK(sym.getString() == "ABC");
    sym.replaceChar(2, 3);
    CHECK(sym.getString() == "ABD");
}

TEST(TestSymbolFixedStringMutate1)
{
    SymbolFixedString sym("ABCD", "ABC");
    SymbolFixedString* sym2 = (SymbolFixedString*)sym.clone();
    sym2->mutate();
    CHECK(sym.getBinding(sym2) <= 0.33334f);
    delete sym2;
}

