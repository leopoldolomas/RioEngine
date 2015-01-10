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

#ifndef RIOENGINE_EDITOR_SERIALIZATION_CUSTOMSERIALIZATION_HPP_
#define RIOENGINE_EDITOR_SERIALIZATION_CUSTOMSERIALIZATION_HPP_

#include <QMetaType>
#include "cengine/cgameobject.h"
#include "clengine/clinstancegeometry.h"

//-----------------------------------------------------------------------------

template<class T>
void serialize_qobject(QDataStream &out, const void* obj) {
    out << *(reinterpret_cast<T*>((void*)obj));
}

//-----------------------------------------------------------------------------

template<class T>
void deserialize_qobject(QDataStream &in, void* obj) {
    in >> *(reinterpret_cast<T*>(obj));
}

//------------------------------------------------------------------- btVector3

void vec3_save_operator(QDataStream &out, const void* obj) {
    btVector3 vec = *(reinterpret_cast<btVector3*>((void*)obj));
    out << vec.x()
        << vec.y()
        << vec.z();
}

//-----------------------------------------------------------------------------

void vec3_load_operator(QDataStream &in, void* dest) {
    float x, y, z;
    in >> x >> y >> z;
    btVector3* dest_vec = static_cast<btVector3*>(dest);
    *dest_vec = btVector3(x, y, z);
}

//---------------------------------------------------------------- btQuaternion

void quat_save_operator(QDataStream &out, const void* obj) {
    btQuaternion vec = *(reinterpret_cast<btQuaternion*>((void*)obj));
    out << vec.x()
        << vec.y()
        << vec.z()
        << vec.w();
}

//-----------------------------------------------------------------------------

void quat_load_operator(QDataStream &in, void* dest) {
    float x, y, z, w;
    in >> x >> y >> z >> w;
    btQuaternion* dest_vec = static_cast<btQuaternion*>(dest);
    *dest_vec = btQuaternion(x, y, z, w);
}

// ------------------------------------------------------ QtCustomSerialization

class QCustomSerialization {
 public:
    static void registerCustomStreamOperators() {
        int id = QMetaType::type("btQuaternion");
        QMetaType::registerStreamOperators(id,
                                           quat_save_operator,
                                           quat_load_operator);

        id = QMetaType::type("btVector3");
        QMetaType::registerStreamOperators(id,
                                           vec3_save_operator,
                                           vec3_load_operator);

        id = QMetaType::type("QList<CGameObject>");
        QMetaType::registerStreamOperators(id,
                                           serialize_qobject<QList<CGameObject>>,
                                           deserialize_qobject<QList<CGameObject>>);

        id = QMetaType::type("QList<CLInstanceGeometry>");
        QMetaType::registerStreamOperators(id,
                                           serialize_qobject<QList<CLInstanceGeometry>>,
                                           deserialize_qobject<QList<CLInstanceGeometry>>);

        id = QMetaType::type("QList<CLInstanceMaterial>");
        QMetaType::registerStreamOperators(id,
                                           serialize_qobject<QList<CLInstanceMaterial>>,
                                           deserialize_qobject<QList<CLInstanceMaterial>>);

        id = QMetaType::type("QList<QProperty>");
        QMetaType::registerStreamOperators(id,
                                           serialize_qobject<QList<QProperty>>,
                                           deserialize_qobject<QList<QProperty>>);
    }

private:
   QCustomSerialization() {}
};

#endif // RIOENGINE_EDITOR_SERIALIZATION_CUSTOMSERIALIZATION_HPP_
