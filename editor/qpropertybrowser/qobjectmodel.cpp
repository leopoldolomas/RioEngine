//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem, you face it like a man."
// 
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
// software, either in source code form or as a compiled binary, for any purpose,
// commercial or non-commercial, and by any means.
// 
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be
// an overt act of relinquishment in perpetuity of all present and future
// rights to this software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//--------------------------------------------------------------- @License ends

#include "qobjectmodel.h"

//-----------------------------------------------------------------------------

QObjectModel::QObjectModel(QObject *parent) : QAbstractListModel(parent) {
    m_children = new QList<QObject*>();
}

//-----------------------------------------------------------------------------

QList<QObject*>* QObjectModel::children() const {
    return m_children;
}

//-----------------------------------------------------------------------------

int QObjectModel::rowCount(const QModelIndex &) const {
    return m_children->count();
}

//-----------------------------------------------------------------------------

int QObjectModel::columnCount(const QModelIndex &) const {
    return 3;
}

//-----------------------------------------------------------------------------

QVariant QObjectModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_children->count()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return m_children->at(index.row())->objectName();
    }
    else {
        return QVariant();
    }
}

//-----------------------------------------------------------------------------

QVariant QObjectModel::headerData(int section, Qt::Orientation o, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (o == Qt::Horizontal) {
        return QString("Column %1").arg(section);
    } else {
        return QString("Row %1").arg(section);
    }
}

//-----------------------------------------------------------------------------

Qt::ItemFlags QObjectModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

//-----------------------------------------------------------------------------

void QObjectModel::addChild(QObject* child) {
    m_children->append(child);
    emit dataChanged(QModelIndex(), QModelIndex());
}

//-----------------------------------------------------------------------------

void QObjectModel::removeChild(QString child_name) {
    for (int i = 0; i < m_children->count(); i++) {
        if (m_children->at(i)->objectName() == child_name) {
            m_children->removeAt(i);
            emit dataChanged(QModelIndex(), QModelIndex());
            return;
        }
    }
}

//-----------------------------------------------------------------------------

void QObjectModel::clear() {
    m_children->clear();
}

//-----------------------------------------------------------------------------

QObjectModel::~QObjectModel() {
    m_children->clear();
    delete m_children;
}
