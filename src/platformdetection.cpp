/* * This file is part of Maliit framework *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#include "platformdetection.h"

#include <QFile>
#include <QProcessEnvironment>
#include <QDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcPlatformDetection, "maliit.platform.detection")

namespace {

bool isWaylandDisplayAvailable()
{
    // Check if WAYLAND_DISPLAY is set and the socket exists
    const QString waylandDisplay = qgetenv("WAYLAND_DISPLAY");
    if (waylandDisplay.isEmpty()) {
        qCDebug(lcPlatformDetection) << "WAYLAND_DISPLAY not set";
        return false;
    }

    // Wayland displays are typically in /run/user/<uid>/ or /tmp/
    // Common locations:
    // - /run/user/<uid>/wayland-0
    // - /run/user/<uid>/wayland-<display>
    // - XDG_RUNTIME_DIR/wayland-<display>
    const QString xdgRuntimeDir = qgetenv("XDG_RUNTIME_DIR");
    if (!xdgRuntimeDir.isEmpty()) {
        const QString socketPath = xdgRuntimeDir + QLatin1Char('/') + waylandDisplay;
        if (QFile::exists(socketPath)) {
            qCDebug(lcPlatformDetection) << "Found Wayland socket at:" << socketPath;
            return true;
        }
        qCDebug(lcPlatformDetection) << "Wayland socket not found at:" << socketPath;
    }

    // Fallback: check /tmp for wayland socket (less common but possible)
    const QString tmpSocketPath = QLatin1String("/tmp/") + waylandDisplay;
    if (QFile::exists(tmpSocketPath)) {
        qCDebug(lcPlatformDetection) << "Found Wayland socket at:" << tmpSocketPath;
        return true;
    }

    // If we can't find the socket but WAYLAND_DISPLAY is set,
    // we assume Wayland might still be available (socket could be in
    // a non-standard location or the display might be remote)
    qCWarning(lcPlatformDetection) << "WAYLAND_DISPLAY is set but socket not found. Wayland may not be available.";
    return false;
}

bool isX11DisplayAvailable()
{
    // Check if DISPLAY is set for X11
    const QString display = qgetenv("DISPLAY");
    if (display.isEmpty()) {
        qCDebug(lcPlatformDetection) << "DISPLAY not set";
        return false;
    }

    qCDebug(lcPlatformDetection) << "X11 DISPLAY:" << display;
    return true;
}

} // anonymous namespace

QString detectAvailablePlatform()
{
    // 1. Check for explicit user override via QT_QPA_PLATFORM
    const QString qtQpaPlatform = qgetenv("QT_QPA_PLATFORM");
    if (!qtQpaPlatform.isEmpty()) {
        qCDebug(lcPlatformDetection) << "QT_QPA_PLATFORM is set to:" << qtQpaPlatform;

        // If user explicitly requested Wayland, verify it's available
        if (qtQpaPlatform.startsWith("wayland")) {
            if (isWaylandDisplayAvailable()) {
                return "wayland";
            }
            qCWarning(lcPlatformDetection) << "Wayland requested but not available. Falling back to X11.";
            // Fall through to try X11
        }

        // If user explicitly requested xcb or other platform, respect it
        if (qtQpaPlatform == "xcb" || qtQpaPlatform == "minimal" ||
            qtQpaPlatform == "offscreen" || qtQpaPlatform == "linuxfb") {
            return qtQpaPlatform;
        }

        // For other platforms, return as-is
        return qtQpaPlatform;
    }

    // 2. Auto-detect: Prefer Wayland if available
    if (isWaylandDisplayAvailable()) {
        qCInfo(lcPlatformDetection) << "Auto-detected Wayland session";
        return "wayland";
    }

    // 3. Fall back to X11
    if (isX11DisplayAvailable()) {
        qCInfo(lcPlatformDetection) << "Auto-detected X11 session";
        return "xcb";
    }

    // 4. No display server detected
    qCWarning(lcPlatformDetection) << "No display server detected. Qt will use default platform.";
    return QString();
}
