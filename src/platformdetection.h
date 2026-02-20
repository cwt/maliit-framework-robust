/* * This file is part of Maliit framework *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef PLATFORMDETECTION_H
#define PLATFORMDETECTION_H

#include <QString>

/*! \brief Detect the available display server platform
 * \return The platform name to use ("wayland", "xcb", or empty if unknown)
 * 
 * This function checks if Wayland is actually available on the system.
 * If Wayland is requested but not available, it falls back to X11 (xcb).
 * 
 * The detection logic:
 * 1. Check QT_QPA_PLATFORM environment variable (user override)
 * 2. If set to "wayland", verify WAYLAND_DISPLAY is available
 * 3. If Wayland is not available, fall back to "xcb"
 * 4. If not set, auto-detect based on environment
 */
QString detectAvailablePlatform();

#endif // PLATFORMDETECTION_H
