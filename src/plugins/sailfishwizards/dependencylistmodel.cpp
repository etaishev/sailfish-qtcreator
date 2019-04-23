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

#include "dependencylistmodel.h"

namespace SailfishWizards {
namespace Internal {
/*!
 * \brief Returns string representation of dependency record structure.
 */
QString DependencyRecord::toString() const
{
    if (m_requirement == EMPTY)
        return m_name;
    QString symbol;
    switch (m_requirement) {
    case EQUAL:
        symbol = "=";
        break;
    case LESS_OR_EQUAL:
        symbol = "<=";
        break;
    case MORE_OR_EQUAL:
        symbol = ">=";
        break;
    case LESS:
        symbol = "<";
        break;
    case MORE:
        symbol = ">";
        break;
    default:
        symbol = "";
        break;
    }
    QString epochMacro = m_epoch + ":";
    QString releaseMacro = "-" + m_release;
    return m_name + " " +  symbol + " " + ((m_epoch.isEmpty()) ? "" : epochMacro)
           + m_version + ((m_release.isEmpty()) ? "" : releaseMacro);
}

/*!
 * \brief Parses and returns the dependency record object from string.
 */
DependencyRecord DependencyRecord::fromString(const QString &dependencyString)
{
    QString line = dependencyString.trimmed();
    QString name = "", versionInfo = "", version, epoch, release;
    QStringList info;
    if (line.startsWith("-"))
        line.remove(line.indexOf("-"), 1);
    info.append(line);
    QList<QString> symbols = {"<=", ">=", "=", "<", ">"};
    QString dependecySymbol = "";
    for (QString symbol : symbols) {
        if (line.contains(symbol)) {
            dependecySymbol = symbol;
            info = line.split(symbol);
            versionInfo = info.last().trimmed();
            break;
        }
    }
    name = info.first().trimmed();
    symbols.prepend("");
    if (versionInfo.contains(":")) {
        info = versionInfo.split(":");
        epoch = info.first().trimmed();
        versionInfo = info.last().trimmed();
    }
    if (versionInfo.contains("-")) {
        info = versionInfo.split("-");
        release = info.last().trimmed();
        versionInfo = info.first().trimmed();
    }
    version = versionInfo.trimmed();
    return DependencyRecord(name, Requirement(symbols.indexOf(dependecySymbol)), version, epoch,
                            release);
}

/*!
 * \brief The override comparison operator for dependency record.
 * Two records considered as equal if they have equal \c name, \c epoch, \c release, \c requirement and \c version.
 */
bool DependencyRecord::operator==(const DependencyRecord &record)
{
    return  (m_epoch == record.m_epoch
             && m_name == record.m_name
             && m_release == record.m_release
             && m_requirement == record.m_requirement
             && m_version == record.m_version);
}

/*!
 * \brief Returns the number of rows of the model.
 * \param parent The parent object index.
 */
int DependencyListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dependencyList.count();
}

/*!
 * \brief Returns the field of the components table
 * by the specified parameter.
 * \param index Item index.
 * \param role Role number.
 */
QVariant DependencyListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_dependencyList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_dependencyList.at(index.row()).toString();
    else
        return QVariant();
}

/*!
 * \brief Inserts new lines.
 * Returns \c true if successful, otherwise \c false.
 * \param position Start line number.
 * \param rows Number of lines.
 * \param parent The parent object index.
 */
bool DependencyListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_dependencyList.insert(position, DependencyRecord{});

    endInsertRows();
    return true;
}

/*!
 * \brief Deletes selected lines.
 * Returns \c true if successful, otherwise \c false.
 * \param position Start line number.
 * \param rows Number of lines.
 * \param parent The parent object instance.
 */
bool DependencyListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_dependencyList.removeAt(position);

    endRemoveRows();
    return true;
}

/*!
 * \brief Sets the whole dependency records list to the model.
 * \param dependencies List of dependency records.
 */
void DependencyListModel::setDependencyList(const QList<DependencyRecord> &dependencies)
{
    m_dependencyList = dependencies;
    emit dataChanged(createIndex(0, 0), createIndex(m_dependencyList.count() - 1, 0));
}

/*!
 * \brief Adds one dependency \a record to the model.
 * \param record Record to add.
 */
void DependencyListModel::appendRecord(const DependencyRecord &record)
{
    m_dependencyList.append(record);
    emit dataChanged(createIndex(0, 0), createIndex(m_dependencyList.count() - 1, 0));
}

/*!
 * \brief Removes dependency \a record from the model.
 * The needed record in the list defines by \c operator==().
 * \param record Record to delete.
 * \sa DependencyRecord::operator==()
 */
void DependencyListModel::removeRecord(const DependencyRecord &record)
{
    m_dependencyList.removeOne(record);
    emit dataChanged(createIndex(0, 0), createIndex(m_dependencyList.count() - 1, 0));
}

/*!
 * \brief Removes dependency record from the model at the given \a position.
 * \param position Position of the record to delete.
 */
void DependencyListModel::removeRecordAt(int position)
{
    m_dependencyList.removeAt(position);
    emit dataChanged(createIndex(0, 0), createIndex(m_dependencyList.count() - 1, 0));
}

/*!
 * \brief Removes all the records from the model.
 */
void DependencyListModel::clearRecords()
{
    m_dependencyList.clear();
    emit dataChanged(createIndex(0, 0), createIndex(m_dependencyList.count() - 1, 0));
}

/*!
 * \brief The override comparison operator for project component.
 * Two components considered as equal if they have equal \c name.
 */
bool ProjectComponent::operator==(const ProjectComponent &component)
{
    return m_name == component.m_name;
}

} // namespace Internal
} // namespace SailfishWizards