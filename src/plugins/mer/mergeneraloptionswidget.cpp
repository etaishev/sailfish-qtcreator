/****************************************************************************
**
** Copyright (C) 2014-2016,2018-2019 Jolla Ltd.
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

#include "mergeneraloptionswidget.h"
#include "ui_mergeneraloptionswidget.h"

#include "merconstants.h"
#include "merdeployconfiguration.h"
#include "merdeploysteps.h"
#include "mersettings.h"

#include <sfdk/sdk.h>

#include <utils/utilsicons.h>

using namespace Sfdk;
using namespace Utils;

namespace Mer {
namespace Internal {

MerGeneralOptionsWidget::MerGeneralOptionsWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::MerGeneralOptionsWidget)
{
    m_ui->setupUi(this);

    m_ui->environmentFilterTextEdit->setPlainText(MerSettings::environmentFilter());
    m_ui->environmentFilterTextEdit->setEnabled(!MerSettings::isEnvironmentFilterFromEnvironment());
    m_ui->environmentFilterTextEdit->setToolTip(m_ui->environmentFilterTextEdit->toolTip()
            .arg(Constants::SAILFISH_SDK_ENVIRONMENT_FILTER));
    m_ui->environmentFilterInfoLabel->setVisible(MerSettings::isEnvironmentFilterFromEnvironment());
    m_ui->environmentFilterInfoLabel->setPixmap(Utils::Icons::WARNING.pixmap());
    m_ui->environmentFilterInfoLabel->setToolTip(
            QLatin1String("<font color=\"red\">")
            + tr("This option is currently overridden with the %1 environment variable.")
                .arg(Constants::SAILFISH_SDK_ENVIRONMENT_FILTER)
            + QLatin1String("</font>"));

    m_ui->buildHostNameLineEdit->setText(Sdk::effectiveBuildHostName());
    m_ui->buildHostNameLineEdit->setEnabled(!Sdk::customBuildHostName().isEmpty());
    m_ui->buildHostNameCustomCheckBox->setChecked(!Sdk::customBuildHostName().isEmpty());
    connect(m_ui->buildHostNameCustomCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        m_ui->buildHostNameLineEdit->setEnabled(checked);
        if (!checked)
            m_ui->buildHostNameLineEdit->setText(Sdk::defaultBuildHostName());
    });

    m_ui->rpmValidationByDefaultCheckBox->setToolTip(m_ui->rpmValidationByDefaultCheckBox->toolTip()
            .arg(MerRpmValidationStep::displayName())
            .arg(MerMb2RpmBuildConfigurationFactory::displayName()));
    m_ui->rpmValidationByDefaultCheckBox->setChecked(MerSettings::rpmValidationByDefault());

    m_ui->askBeforeStartingVmCheckBox->setChecked(MerSettings::isAskBeforeStartingVmEnabled());
    m_ui->askBeforeClosingVmCheckBox->setChecked(MerSettings::isAskBeforeClosingVmEnabled());
    m_ui->importQmakeVariablesCheckBox->setChecked(MerSettings::isImportQmakeVariablesEnabled());
    m_ui->askImportQmakeVariablesCheckBox->setEnabled(MerSettings::isImportQmakeVariablesEnabled());
    connect(m_ui->importQmakeVariablesCheckBox, &QCheckBox::toggled,
            m_ui->askImportQmakeVariablesCheckBox, &QCheckBox::setEnabled);
    m_ui->askImportQmakeVariablesCheckBox->setChecked(MerSettings::isAskImportQmakeVariablesEnabled());

    m_ui->benchLocationPathChooser->setExpectedKind(PathChooser::ExistingCommand);
    m_ui->benchLocationPathChooser->setPath(MerSettings::qmlLiveBenchLocation());
    m_ui->benchSyncWorkspaceCheckBox->setChecked(MerSettings::isSyncQmlLiveWorkspaceEnabled());

    m_ui->askImportQmakeVariablesCheckBox->setToolTip(m_ui->askImportQmakeVariablesCheckBox->toolTip().arg(Sfdk::Sdk::osVariant()));
    m_ui->askBeforeStartingVmCheckBox->setToolTip(m_ui->askBeforeStartingVmCheckBox->toolTip().arg(Sfdk::Sdk::osVariant()));
    m_ui->askBeforeClosingVmCheckBox->setToolTip(m_ui->askBeforeClosingVmCheckBox->toolTip().arg(Sfdk::Sdk::osVariant()));
    m_ui->rpmValidationInfoLabel->setText(m_ui->rpmValidationInfoLabel->text().arg(Sfdk::Sdk::osVariant()));
}

MerGeneralOptionsWidget::~MerGeneralOptionsWidget()
{
    delete m_ui;
}

void MerGeneralOptionsWidget::store()
{
    if (!MerSettings::isEnvironmentFilterFromEnvironment())
        MerSettings::setEnvironmentFilter(m_ui->environmentFilterTextEdit->toPlainText());
    Sdk::setCustomBuildHostName(m_ui->buildHostNameCustomCheckBox->isChecked()
            ? m_ui->buildHostNameLineEdit->text()
            : QString());
    MerSettings::setRpmValidationByDefault(m_ui->rpmValidationByDefaultCheckBox->isChecked());
    MerSettings::setAskBeforeStartingVmEnabled(m_ui->askBeforeStartingVmCheckBox->isChecked());
    MerSettings::setAskBeforeClosingVmEnabled(m_ui->askBeforeClosingVmCheckBox->isChecked());
    MerSettings::setQmlLiveBenchLocation(m_ui->benchLocationPathChooser->path());
    MerSettings::setSyncQmlLiveWorkspaceEnabled(m_ui->benchSyncWorkspaceCheckBox->isChecked());
    MerSettings::setImportQmakeVariablesEnabled(m_ui->importQmakeVariablesCheckBox->isChecked());
    MerSettings::setAskImportQmakeVariablesEnabled(m_ui->askImportQmakeVariablesCheckBox->isChecked());
}

QString MerGeneralOptionsWidget::searchKeywords() const
{
    QString keywords;
    const QLatin1Char sep(' ');
    QTextStream(&keywords) << sep << m_ui->environmentFilterLabel->text()
                           << sep << m_ui->rpmValidationInfoLabel->text()
                           << sep << m_ui->rpmValidationByDefaultCheckBox->text()
                           << sep << m_ui->askBeforeStartingVmCheckBox->text()
                           << sep << m_ui->qmlLiveGroupBox->title()
                           << sep << m_ui->benchLocationLabel->text()
                           << sep << m_ui->importQmakeVariablesCheckBox->text()
                           << sep << m_ui->askImportQmakeVariablesCheckBox->text()
                           ;
    keywords.remove(QLatin1Char('&'));
    return keywords;
}

} // Internal
} // Mer
