/* * This file is part of Maliit framework *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#include "ut_waylandinputmethodvalidation.h"
#include "waylandinputmethodvalidation_p.h"

#include <QDebug>

namespace {
    const uint32_t MAX_SURROUNDING_TEXT_BYTES = 1024 * 1024;
}

void Ut_WaylandInputMethodValidation::initTestCase()
{
}

void Ut_WaylandInputMethodValidation::cleanupTestCase()
{
}

void Ut_WaylandInputMethodValidation::init()
{
}

void Ut_WaylandInputMethodValidation::cleanup()
{
}

//
// Test valid positions
//

void Ut_WaylandInputMethodValidation::testValidPositions_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");

    QTest::newRow("empty text, zero positions") 
        << QString() << static_cast<uint32_t>(0) << static_cast<uint32_t>(0);
    QTest::newRow("empty text, positions at end") 
        << QString() << static_cast<uint32_t>(0) << static_cast<uint32_t>(0);
    QTest::newRow("simple text, cursor at start") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(0);
    QTest::newRow("simple text, cursor at end") 
        << QString("hello") << static_cast<uint32_t>(5) << static_cast<uint32_t>(5);
    QTest::newRow("simple text, cursor in middle") 
        << QString("hello") << static_cast<uint32_t>(2) << static_cast<uint32_t>(2);
    QTest::newRow("simple text, selection") 
        << QString("hello") << static_cast<uint32_t>(1) << static_cast<uint32_t>(4);
    QTest::newRow("unicode text") 
        << QString("你好世界") << static_cast<uint32_t>(6) << static_cast<uint32_t>(6);
}

void Ut_WaylandInputMethodValidation::testValidPositions()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    QVERIFY(result.valid);
    QCOMPARE(result.cursor, cursor);
    QCOMPARE(result.anchor, anchor);
}

//
// Test cursor position exceeding text bounds
//

void Ut_WaylandInputMethodValidation::testCursorExceedsBounds_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");
    QTest::addColumn<uint32_t>("expectedCursor");

    QTest::newRow("cursor slightly exceeds") 
        << QString("hello") << static_cast<uint32_t>(10) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(5);
    QTest::newRow("cursor greatly exceeds") 
        << QString("hello") << static_cast<uint32_t>(1000) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(5);
    QTest::newRow("cursor exceeds by 1") 
        << QString("hello") << static_cast<uint32_t>(6) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(5);
}

void Ut_WaylandInputMethodValidation::testCursorExceedsBounds()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);
    QFETCH(uint32_t, expectedCursor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    QVERIFY(result.valid);
    QCOMPARE(result.cursor, expectedCursor);
    QCOMPARE(result.anchor, anchor);
}

//
// Test anchor position exceeding text bounds
//

void Ut_WaylandInputMethodValidation::testAnchorExceedsBounds_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");
    QTest::addColumn<uint32_t>("expectedAnchor");

    QTest::newRow("anchor slightly exceeds") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(10)
        << static_cast<uint32_t>(5);
    QTest::newRow("anchor greatly exceeds") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(1000)
        << static_cast<uint32_t>(5);
    QTest::newRow("anchor exceeds by 1") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(6)
        << static_cast<uint32_t>(5);
}

void Ut_WaylandInputMethodValidation::testAnchorExceedsBounds()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);
    QFETCH(uint32_t, expectedAnchor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    QVERIFY(result.valid);
    QCOMPARE(result.cursor, cursor);
    QCOMPARE(result.anchor, expectedAnchor);
}

//
// Test both positions exceeding bounds
//

void Ut_WaylandInputMethodValidation::testBothExceedBounds_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");
    QTest::addColumn<uint32_t>("expectedCursor");
    QTest::addColumn<uint32_t>("expectedAnchor");

    QTest::newRow("both slightly exceed") 
        << QString("hello") << static_cast<uint32_t>(10) << static_cast<uint32_t>(20)
        << static_cast<uint32_t>(5) << static_cast<uint32_t>(5);
    QTest::newRow("both greatly exceed") 
        << QString("hello") << static_cast<uint32_t>(1000) << static_cast<uint32_t>(2000)
        << static_cast<uint32_t>(5) << static_cast<uint32_t>(5);
}

void Ut_WaylandInputMethodValidation::testBothExceedBounds()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);
    QFETCH(uint32_t, expectedCursor);
    QFETCH(uint32_t, expectedAnchor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    QVERIFY(result.valid);
    QCOMPARE(result.cursor, expectedCursor);
    QCOMPARE(result.anchor, expectedAnchor);
}

//
// Test oversized text
//

void Ut_WaylandInputMethodValidation::testOversizedText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");

    // Create text larger than 1MB
    QString largeText(MAX_SURROUNDING_TEXT_BYTES + 1, 'a');
    QTest::newRow("text exceeds 1MB") 
        << largeText << static_cast<uint32_t>(100) << static_cast<uint32_t>(100);

    // Text exactly at limit should be valid
    QString exactText(MAX_SURROUNDING_TEXT_BYTES, 'a');
    QTest::newRow("text at exactly 1MB") 
        << exactText << static_cast<uint32_t>(100) << static_cast<uint32_t>(100);
}

void Ut_WaylandInputMethodValidation::testOversizedText()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    if (utf8_text.size() > static_cast<int>(MAX_SURROUNDING_TEXT_BYTES)) {
        QVERIFY(!result.valid);
    } else {
        QVERIFY(result.valid);
    }
}

//
// Test edge cases
//

void Ut_WaylandInputMethodValidation::testEdgeCases_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");
    QTest::addColumn<uint32_t>("expectedCursor");
    QTest::addColumn<uint32_t>("expectedAnchor");

    // Empty text with zero positions
    QTest::newRow("empty text, zero positions") 
        << QString("") << static_cast<uint32_t>(0) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(0) << static_cast<uint32_t>(0);

    // Empty text with non-zero positions (should clamp to 0)
    QTest::newRow("empty text, non-zero positions") 
        << QString("") << static_cast<uint32_t>(100) << static_cast<uint32_t>(200)
        << static_cast<uint32_t>(0) << static_cast<uint32_t>(0);

    // Single character text
    QTest::newRow("single char, valid positions") 
        << QString("a") << static_cast<uint32_t>(1) << static_cast<uint32_t>(1)
        << static_cast<uint32_t>(1) << static_cast<uint32_t>(1);

    QTest::newRow("single char, exceeds") 
        << QString("a") << static_cast<uint32_t>(2) << static_cast<uint32_t>(2)
        << static_cast<uint32_t>(1) << static_cast<uint32_t>(1);

    // Position exactly at boundary (should be valid)
    QTest::newRow("position at exact boundary") 
        << QString("hello") << static_cast<uint32_t>(5) << static_cast<uint32_t>(5)
        << static_cast<uint32_t>(5) << static_cast<uint32_t>(5);
}

void Ut_WaylandInputMethodValidation::testEdgeCases()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);
    QFETCH(uint32_t, expectedCursor);
    QFETCH(uint32_t, expectedAnchor);

    const QByteArray utf8_text = text.toUtf8();
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    QVERIFY(result.valid);
    QCOMPARE(result.cursor, expectedCursor);
    QCOMPARE(result.anchor, expectedAnchor);
}

//
// Test integer overflow scenarios (issue #131)
//

void Ut_WaylandInputMethodValidation::testIntegerOverflow_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<uint32_t>("cursor");
    QTest::addColumn<uint32_t>("anchor");
    QTest::addColumn<uint32_t>("expectedCursor");
    QTest::addColumn<uint32_t>("expectedAnchor");

    // Issue #131: Large unsigned values that would overflow when cast to signed int
    // Value 2154852192 from issue #131
    QTest::newRow("issue 131 anchor value") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(2154852192u)
        << static_cast<uint32_t>(0) << static_cast<uint32_t>(5);

    // Maximum uint32_t value
    QTest::newRow("max uint32 cursor") 
        << QString("hello") << static_cast<uint32_t>(4294967295u) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(5) << static_cast<uint32_t>(0);

    QTest::newRow("max uint32 anchor") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(4294967295u)
        << static_cast<uint32_t>(0) << static_cast<uint32_t>(5);

    // Values that would become negative when cast to int32
    QTest::newRow("value > INT32_MAX cursor") 
        << QString("hello") << static_cast<uint32_t>(3000000000u) << static_cast<uint32_t>(0)
        << static_cast<uint32_t>(5) << static_cast<uint32_t>(0);

    QTest::newRow("value > INT32_MAX anchor") 
        << QString("hello") << static_cast<uint32_t>(0) << static_cast<uint32_t>(3000000000u)
        << static_cast<uint32_t>(0) << static_cast<uint32_t>(5);
}

void Ut_WaylandInputMethodValidation::testIntegerOverflow()
{
    QFETCH(QString, text);
    QFETCH(uint32_t, cursor);
    QFETCH(uint32_t, anchor);
    QFETCH(uint32_t, expectedCursor);
    QFETCH(uint32_t, expectedAnchor);

    const QByteArray utf8_text = text.toUtf8();
    // This should NOT crash even with very large uint32 values
    const SurroundingTextValidation result = validateSurroundingTextPositions(utf8_text, cursor, anchor);

    // The function should handle these gracefully by clamping
    QVERIFY(result.valid);
    QCOMPARE(result.cursor, expectedCursor);
    QCOMPARE(result.anchor, expectedAnchor);
}

QTEST_MAIN(Ut_WaylandInputMethodValidation)
