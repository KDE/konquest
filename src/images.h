/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_IMAGES_H
#define KONQUEST_IMAGES_H

#include <QStandardPaths>

#define IMAGE_SPLASH    QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("pics/konquest-splash.png"))
#define IMAGES_SVG      QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("pics/default_theme.svgz"))

#endif // KONQUEST_IMAGES_H
