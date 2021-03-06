/****************************************************************************
**
** Copyright (C) 2020 Open Mobile Platform LLC.
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

#include "cmakecommand.h"

#include <mer/merconstants.h>
#include <sfdk/sfdkconstants.h>
#include <utils/qtcassert.h>
#include <utils/qtcprocess.h>

#include <QDir>

using namespace Utils;

CMakeCommand::CMakeCommand()
{

}

QString CMakeCommand::name() const
{
    return QLatin1String("cmake");
}

int CMakeCommand::execute()
{
    if (arguments().contains(QLatin1String("-E"))
            && arguments().contains(QLatin1String("capabilities"))) {
        fprintf(stdout, "%s", capabilities().data());
        return 0;
    } else if (arguments().contains(QLatin1String("--version"))) {
        m_cacheFile = QLatin1String(Sfdk::Constants::CMAKE_VERSION_CACHE);
    } else if (arguments().contains(QLatin1String("--help"))) {
        QTC_ASSERT(false, return 1);
    }

    if (!m_cacheFile.isEmpty()) {
        m_cacheFile.prepend(sdkToolsPath() + QDir::separator());

        if (QFile::exists(m_cacheFile)) {
            QFile cacheFile(m_cacheFile);
            if (!cacheFile.open(QIODevice::ReadOnly)) {
                fprintf(stderr, "%s",qPrintable(QString::fromLatin1("Cannot read cached file \"%1\"").arg(m_cacheFile)));
                fflush(stderr);
                return 1;
            }
            fprintf(stdout, "%s", cacheFile.readAll().constData());
            fflush(stdout);
            return 0;
        }

        return 1;
    }

    // FIXME If merssh is passed "'Foo Bar'", Command::arguments() returns ["'Foo", "Bar'"].
    const bool abortOnMeta = false;
    QtcProcess::SplitError splitError;
    const QStringList splitArguments = QtcProcess::splitArgs(arguments().join(QLatin1Char(' ')),
            OsTypeLinux, abortOnMeta, &splitError);
    QTC_ASSERT(splitError == QtcProcess::SplitOk, return 1);

    QStringList filteredArguments;
    for (const QString &argument : splitArguments) {
        // See MerSdkManager::ensureCmakeToolIsSet()
        if (argument.startsWith(QLatin1String("-DCMAKE_CXX_COMPILER:STRING="))
                || argument.startsWith(QLatin1String("-DCMAKE_C_COMPILER:STRING="))
                || argument.startsWith(QLatin1String("-DCMAKE_PREFIX_PATH:STRING="))
                || argument.startsWith(QLatin1String("-DQT_QMAKE_EXECUTABLE:STRING="))) {
            continue;
        }

        filteredArguments.append(argument);
    }

    QString command = QLatin1String("mb2") +
                      QLatin1String(" -t ") +
                      targetName() +
                      QLatin1Char(' ') + QtcProcess::joinArgs(filteredArguments, OsTypeLinux) + QLatin1Char(' ');

    return executeRemoteCommand(command);
}

bool CMakeCommand::isValid() const
{
    return Command::isValid() && !targetName().isEmpty() && !sdkToolsPath().isEmpty();
}

QByteArray CMakeCommand::capabilities()
{
    return R"({
        "generators": [
            {
                "extraGenerators": [
                    "CodeBlocks"
                ],
                "name": "Unix Makefiles",
                "platformSupport": false,
                "toolsetSupport": false
            }
        ],
        "serverMode": false
    })";
}
