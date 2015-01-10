//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem?, you face it like a man."
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

#include "cengine/cgameobjectfactory.h"
#include "editor/qpropertyeditor/cobjecttypesenumproperty.h"

//-----------------------------------------------------------------------------

CObjectTypesEnumProperty::CObjectTypesEnumProperty(const QString& name,
                                                   QObject* propertyObject,
                                                   QObject* parent)
    : EnumProperty(name, propertyObject, parent) {
    m_enum.clear();
}

//-----------------------------------------------------------------------------

void CObjectTypesEnumProperty::setEnumValues(QStringList values) {
    m_enum = values;
}

//-----------------------------------------------------------------------------

QVariant CObjectTypesEnumProperty::value(int role /* = Qt::UserRole */) const {
    if(role == Qt::DisplayRole){
        if (m_propertyObject){
            // resolve the value to the corresponding enum key
            int index = m_propertyObject->property(qPrintable(objectName())).toInt();
            const QStringList& types = CGameObjectFactory::types();
            if(index < 0 || (uint)index >= types.size())
            {
                return QVariant();
            }
            return types.at(index);
        } else{
            return QVariant();
        }
    } else {
        return Property::value(role);
    }
}

//-----------------------------------------------------------------------------

bool CObjectTypesEnumProperty::setEditorData(QWidget *editor, const QVariant& data)
{
    QComboBox* combo = 0;
    if(combo = qobject_cast<QComboBox*>(editor)) {
        int value = data.toInt();
        combo->setCurrentIndex(value);
    } else {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

void CObjectTypesEnumProperty::valueChanged(const QString item) {
    int index = CGameObjectFactory::getObjectTypeId(qPrintable(item));
    setValue( index < 0 ? QVariant() : QVariant(index) );
}
