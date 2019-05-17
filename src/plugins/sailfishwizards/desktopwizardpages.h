/****************************************************************************
**
** Copyright (C) 2019 Open Mobile Platform LLC
** Contact: https://community.omprussia.ru/open-source
**
** This file is part of Qt Creator.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included
** in the packaging of this file. Please review the following information
** to ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: https://www.gnu.org/licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once

#include <QObject>
#include <QWizard>
#include <coreplugin/basefilewizard.h>
#include <QFileDialog>
#include <QDialog>
#include <QtWidgets>
#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QMap>
#include <QTableView>
#include <QModelIndex>

#include "ui_desktopfilesettingpage.h"
#include "ui_desktopiconpage.h"
#include "ui_desktopselectpage.h"


namespace SailfishWizards {
namespace Internal {

/*!
  * \brief Allows to select the name and location of
  * the .desktop file.
  * \sa QWizardPage
  */
class DesktopLocationPage : public QWizardPage
{
    Q_OBJECT
public:
    DesktopLocationPage(const QString &defaultPath);
    static bool validateInput(bool condition, QWidget *widget, QLabel *errorLabel,
                              const QString &errorMessage);
    bool isComplete() const override;
    QString getDefaultPath() const;
    int getIconsIndex() const;
    void findIconsInsertingLine();

private:
    Ui::DesktopSelectPage m_pageUi;
    QString m_defaultPath;
    int m_iconsIndex;

signals:
    void directoryChosen(const QString &);

private slots:
    void chooseFileDirectory();
};

/*!
  * \brief Allows to select application icons.
  * \sa QWizardPage
  */
class DesktopIconPage : public QWizardPage
{
    Q_OBJECT
public:
    DesktopIconPage();
    bool validateInput(bool condition, QWidget *widget, QLabel *errorLabel,
                       const QString &errorMessage) const;
    Ui::DesktopIconPage Ui();
    bool isComplete() const override;

private:
    Ui::DesktopIconPage m_pageUi;
    QPushButton *m_initiator;
    void cleanupPage() override;

private slots:
    void chooseFileDirectory();
    void updateInputFields();
    void updateIconPreview(QString iconPath);
};

/*!
  * \brief Class for application's name translations.
  */
class TranslationData
{
public:
    TranslationData();
    TranslationData(const QString &language, const QString &translation);

    void setTranslation (const QString &translation);
    void setLanguage (const QString &language);
    QString getTranslation() const;
    QString getLanguage() const;
    QStringList getLanguages() const;
    QStringList getLanguageCodes() const;
    QMap<QString, QString> getLanguageCodeMap() const;

private:
    QString m_language;
    QString m_translation;
    QMap<QString, QString> m_languageCodeMap;
    void fillLanguageCodeMap();
};

/*!
  * \brief Class for the list of application's name translations.
  * \sa QAbstractTableModel
*/
class TranslationTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TranslationTableModel(QObject *parent = nullptr);
    enum translationRoles {
        TranslationRole = Qt::DisplayRole,
        LanguageRole = Qt::DisplayRole + 1,
    };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setLanguageList(const QStringList &languages);

    QList<TranslationData> getTranslationData() const;
    void clearTranslations();
    enum Column {
        LANGUAGE = 0,
        TRANSLATION
    };
    typedef QHash< Column, QVariant > TranslationLocation;
    void setTranslations(const QList<TranslationLocation> &translations);
    QList<TranslationLocation> getTranslations() const;

private:
    QList<TranslationLocation> m_translations;

};

/*!
  * \brief Allows to fill the fields of the .desktop file.
  * \sa QWizardPage
*/
class DesktopSettingPage : public QWizardPage
{
    Q_OBJECT
public:
    DesktopSettingPage(const QString &defaultPath, TranslationTableModel *langTableModel,
                       QWidget *parent = nullptr);
    ~DesktopSettingPage();

    Ui::DesktopFileSettingPage Ui();
    TranslationTableModel *getLangTableModel() const;
    bool isComplete() const override;
    void cleanupPage() override;

private:
    TranslationTableModel *m_langTableModel;
    Ui::DesktopFileSettingPage m_settingUi;

signals:
    void translationsCleaned();
};

} // namespace Internal
} // namespace SailfishWizards
