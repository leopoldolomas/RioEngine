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

#include <QFile>
#include <QDataStream>
#include "buildconfig.h"
#include "editor/serialization/qserializer.h"

//-----------------------------------------------------------------------------

QSerializer::QSerializer() {
}

//-----------------------------------------------------------------------------

void QSerializer::serialize(CObject* obj, QString path) {
#ifdef RE_DEBUG
    LOG("------------");
    LOG("Serializing object of type: %s with name: %s into path: %s",
        obj->metaObject()->className(),
        qPrintable(obj->objectName()),
        qPrintable(path));
#endif
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *obj;
    file.close();
#ifdef RE_DEBUG
    LOG("------------");
#endif
}

//-----------------------------------------------------------------------------

void QSerializer::deserialize(CObject* obj, QString path) {
#ifdef RE_DEBUG
    LOG("------------");
    LOG("Deserializing object of type: %s from path: %s",
        obj->metaObject()->className(), path.toStdString().c_str());
#endif
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> *obj;
    file.close();
#ifdef RE_DEBUG
    LOG("------------");
#endif
}
