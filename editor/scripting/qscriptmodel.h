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

#ifndef RIOENGINE_EDITOR_SCRIPTING_QSCRIPTMODEL_H_
#define RIOENGINE_EDITOR_SCRIPTING_QSCRIPTMODEL_H_

#include <QAbstractListModel>

class QScriptModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Column { ColName, ColType };

    QScriptModel(QObject *parent = 0);

    QList<QString>* children() const;

    int rowCount   (const QModelIndex &parent = QModelIndex())    const;
    int columnCount(const QModelIndex &parent = QModelIndex())    const;
    QVariant data  (const QModelIndex &index, int role)           const;
    Qt::ItemFlags flags(const QModelIndex &index)                 const;
    QVariant headerData(int section, Qt::Orientation o, int role) const;

    void addChild(QString child);
    void removeChild(QString child_name);
    void clear();

    ~QScriptModel();

protected:
    QList<QString>* m_children;
};

#endif // RIOENGINE_EDITOR_SCRIPTING_QSCRIPTMODEL_H_
