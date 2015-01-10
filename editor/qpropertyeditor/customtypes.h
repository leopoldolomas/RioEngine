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

#ifndef RIOENGINE_EDITOR_QPROPERTYEDITOR_CUSTOMTYPES_H_
#define RIOENGINE_EDITOR_QPROPERTYEDITOR_CUSTOMTYPES_H_

#include <QVariant>
#include "bullet/btBulletCollisionCommon.h"
#include "cengine/cgameobject.h"
#include "clengine/clinstancegeometry.h"
#include "clengine/clinstancematerial.h"
#include "editor/qproperty/qproperty.h"

class Property;
class QObject;

Q_DECLARE_METATYPE(btQuaternion)
Q_DECLARE_METATYPE(btVector3)
Q_DECLARE_METATYPE(QList<QProperty>)
Q_DECLARE_METATYPE(QList<CGameObject>)
Q_DECLARE_METATYPE(QList<CLInstanceGeometry>)
Q_DECLARE_METATYPE(QList<CLInstanceMaterial>)

namespace CustomTypes
{
void registerTypes();
Property* createCustomProperty(const QString& name, QObject* propertyObject, Property* parent);
}

#endif // RIOENGINE_EDITOR_QPROPERTYEDITOR_CUSTOMTYPES_H_
