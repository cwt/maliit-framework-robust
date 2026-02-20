/* * This file is part of Maliit framework *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef WAYLANDINPUTMETHODVALIDATION_P_H
#define WAYLANDINPUTMETHODVALIDATION_P_H

#include <QtCore>

Q_DECLARE_LOGGING_CATEGORY(lcWaylandConnection)

// Validation result for surrounding text positions
struct SurroundingTextValidation {
    bool valid;
    uint32_t cursor;
    uint32_t anchor;
};

/*! \brief Validate and sanitize cursor and anchor positions for surrounding text
 * \param utf8_text The UTF-8 encoded text to validate against
 * \param cursor The cursor position from Wayland protocol
 * \param anchor The anchor position from Wayland protocol
 * \return Validation result with sanitized positions
 * 
 * This function prevents crashes from invalid Wayland protocol data by:
 * - Clamping cursor/anchor positions that exceed text bounds
 * - Rejecting unreasonably large text (>1MB)
 * 
 * \internal
 */
inline SurroundingTextValidation validateSurroundingTextPositions(
    const QByteArray &utf8_text, uint32_t cursor, uint32_t anchor)
{
    SurroundingTextValidation result;
    result.valid = true;
    result.cursor = cursor;
    result.anchor = anchor;

    // Validate cursor and anchor positions are within the UTF-8 text bounds
    // This prevents crashes from invalid Wayland protocol data
    if (cursor > static_cast<uint32_t>(utf8_text.size())) {
        qWarning(lcWaylandConnection)
            << "Invalid cursor position" << cursor
            << "exceeds text size" << utf8_text.size()
            << ", clamping to text size";
        result.cursor = utf8_text.size();
    }

    if (anchor > static_cast<uint32_t>(utf8_text.size())) {
        qWarning(lcWaylandConnection)
            << "Invalid anchor position" << anchor
            << "exceeds text size" << utf8_text.size()
            << ", clamping to text size";
        result.anchor = utf8_text.size();
    }

    // Additional sanity check: reject unreasonably large values
    // Normal surrounding text should be < 100KB; 1MB is a safe upper bound
    constexpr uint32_t MAX_SURROUNDING_TEXT_BYTES = 1024 * 1024;
    if (utf8_text.size() > static_cast<int>(MAX_SURROUNDING_TEXT_BYTES)) {
        qWarning(lcWaylandConnection)
            << "Surrounding text too large:" << utf8_text.size()
            << "bytes, ignoring event";
        result.valid = false;
    }

    return result;
}

#endif // WAYLANDINPUTMETHODVALIDATION_P_H
