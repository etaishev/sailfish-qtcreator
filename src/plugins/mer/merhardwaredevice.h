/****************************************************************************
**
** Copyright (C) 2012-2015,2019 Jolla Ltd.
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

#ifndef MERHARDWAREDEVICE_H
#define MERHARDWAREDEVICE_H

#include "merdevice.h"

namespace Mer {
namespace Internal {

class MerHardwareDevice : public MerDevice
{
    Q_DECLARE_TR_FUNCTIONS(Mer::Internal::MerDevice)
public:
    typedef QSharedPointer<MerHardwareDevice> Ptr;
    typedef QSharedPointer<const MerHardwareDevice> ConstPtr;

    static Ptr create() { return Ptr(new MerHardwareDevice); }

    ProjectExplorer::IDevice::Ptr clone() const override;

    ProjectExplorer::IDeviceWidget* createWidget() override;

protected:
    MerHardwareDevice();
};

}
}

#endif // MERHARDWAREDEVICE_H
