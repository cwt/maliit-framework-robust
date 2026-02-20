/* * This file is part of Maliit framework *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef UT_WAYLANDINPUTMETHODVALIDATION_H
#define UT_WAYLANDINPUTMETHODVALIDATION_H

#include <QtTest/QtTest>
#include <QObject>

class Ut_WaylandInputMethodValidation : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // Test valid positions
    void testValidPositions_data();
    void testValidPositions();

    // Test cursor position exceeding text bounds
    void testCursorExceedsBounds_data();
    void testCursorExceedsBounds();

    // Test anchor position exceeding text bounds
    void testAnchorExceedsBounds_data();
    void testAnchorExceedsBounds();

    // Test both positions exceeding bounds
    void testBothExceedBounds_data();
    void testBothExceedBounds();

    // Test oversized text
    void testOversizedText_data();
    void testOversizedText();

    // Test edge cases
    void testEdgeCases_data();
    void testEdgeCases();

    // Test integer overflow scenarios (issue #131)
    void testIntegerOverflow_data();
    void testIntegerOverflow();
};

#endif
