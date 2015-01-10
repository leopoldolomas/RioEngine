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

#include <QMetaProperty>
#include <QDataStream>
#include "cengine/cautoreleasepool.h"
#include "cengine/cobject.h"

//-----------------------------------------------------------------------------

CObject::CObject() : QObject() {
    __refCount = 1;
    __selfPointer = this;
}

//-----------------------------------------------------------------------------

CObject::CObject(const CObject&) {
    __refCount = 1;
    __selfPointer = this;
}

//-----------------------------------------------------------------------------

CObject& CObject::operator=(const CObject&) {
    __refCount = 1;
    __selfPointer = this;

    return *this;
}

//-----------------------------------------------------------------------------

void CObject::copyValuesFromObject(const CObject&) {
    RE_ASSERT(false); //  copyValuesFromObject() must be overridden
}

//-----------------------------------------------------------------------------

CObject* CObject::retain() {
    __refCount++;

    return this;
}

//-----------------------------------------------------------------------------

unsigned int CObject::release() const {
    RE_ASSERT(__refCount != UINT_MAX);  // object already deallocated?
    unsigned int retain_count = --__refCount;

    if (retain_count == 0) {
        __refCount = UINT_MAX; // flag it as deallocated
        delete this;
    }
    return retain_count;
}

//-----------------------------------------------------------------------------

CObject* CObject::autorelease() {
    CAUTORELEASEPOOL->addObject(this);
    return this;
}

//-----------------------------------------------------------------------------

unsigned int CObject::retainCount() const {
    return __refCount;
}

//-----------------------------------------------------------------------------

bool CObject::isValidCObject() const {
    return ( __selfPointer == this );
}

//-----------------------------------------------------------------------------

QDataStream &operator<<(QDataStream &ds, const CObject &obj) {
    for (int i=0; i<obj.metaObject()->propertyCount(); ++i) {
        if (obj.metaObject()->property(i).isStored(&obj)) {
            const char* prop_name = obj.metaObject()->property(i).name();
            LOG("(%s) - Serializing property: %s", obj.metaObject()->className(), prop_name);
            ds << obj.metaObject()->property(i).read(&obj);
        }
    }
    return ds;
}

//-----------------------------------------------------------------------------

QDataStream &operator>>(QDataStream &ds, CObject &obj) {
    QVariant var;
    for (int i=0; i<obj.metaObject()->propertyCount(); ++i) {
        if (obj.metaObject()->property(i).isStored(&obj)) {
            const char* prop_name = obj.metaObject()->property(i).name();
            LOG("(%s) - Deserializing property: %s", obj.metaObject()->className(), prop_name);
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}

//-----------------------------------------------------------------------------

CObject::~CObject() {
}
