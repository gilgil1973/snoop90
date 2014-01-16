/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <QtGui>
#include <QtXml>

#include "domitem.h"
#include "dommodel.h"

//! [0]
DomModel::DomModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
  QDomElement element = document.documentElement();
  rootItem = new DomItem(element, NULL);
}
//! [0]

//! [1]
DomModel::~DomModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}
//! [2]

//! [3]
QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // LOG_DEBUG("rolw=%d", role); // gilgil temp 2012.08.08

    if (role == Qt::BackgroundColorRole)
    {
      if (index.row() % 2 == 0)
        return qVariantFromValue(QColor(246, 246, 246));
      else
        return qVariantFromValue(QColor(255, 255, 255));
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DomItem *item = static_cast<DomItem*>(index.internalPointer());

    QDomElement element = item->element();
//! [3] //! [4]
    QStringList attributes;
    QDomNamedNodeMap attributeMap = element.attributes();

    switch (index.column()) {
        case 0:
          if (dynamic_cast<DomAttrItem*>(item) != NULL) return ((DomAttrItem*)item)->attr().name();
          else return element.nodeName();
        case 1:
          if (dynamic_cast<DomAttrItem*>(item) != NULL) return ((DomAttrItem*)item)->attr().value();
          else return QVariant();
        /*
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + "=\""
                              +attribute.nodeValue() + "\"";
            }
            return attributes.join(" ");
        */
        case 2:
            return element.nodeValue().split("\n").join(" ");
        default:
            return QVariant();
    }
}
//! [4]

//! [5]
Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;
  switch (index.column())
  {
    case 0: return Qt::ItemIsEnabled;
    case 1: return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    default: return Qt::NoItemFlags;
  }
}
//! [5]

//! [6]
QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}
//! [6]

//! [7]
QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());
//! [7]

//! [8]
    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [8]

//! [9]
QModelIndex DomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [9]

//! [10]
int DomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    if (dynamic_cast<DomAttrItem*>(parentItem) != NULL) return 0;
    return parentItem->element().attributes().count() + parentItem->element().childNodes().count();
}
//! [10]


bool DomModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole) return false;

  DomAttrItem *attrItem = static_cast<DomAttrItem*>(index.internalPointer());
  if (attrItem == NULL) return false;
  if (dynamic_cast<DomAttrItem*>(attrItem) == NULL) return false;

  switch (index.column()){
      case 0: // name
        return false;
      case 1: // value
        attrItem->attr().setValue(value.toString()); // This works - QTreeView is updated
        emit dataChanged(index, index);
        return true;
      default:
        return false;
  }
}
