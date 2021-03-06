/****************************************************************************
**
** Copyright (C) 2019 Open Mobile Platform LLC.
** Contact: http://jolla.com/
**
** This file is part of Qt Creator.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Digia.
**
****************************************************************************/

#pragma once

#include "sfdkglobal.h"
#include "utils.h"

#include <QList>

namespace Utils {
class Port;
class PortList;
}

namespace Sfdk {

QList<Utils::Port> toList(const Utils::PortList &portList);
Utils::PortList toPortList(const QList<Utils::Port> &portList);
Utils::PortList toPortList(const QList<quint16> &portList);

QString separator(TextStyle textStyle);

} // namespace Sfdk
