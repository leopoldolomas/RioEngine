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

#include "qproperty.h"

//-----------------------------------------------------------------------------

QProperty::QProperty() : CObject() {
}

//-----------------------------------------------------------------------------

QProperty::QProperty(QString name, QVariant value) : CObject() {
    setObjectName(name);
    setValue(value);
}

//-----------------------------------------------------------------------------

QProperty::QProperty(const QProperty& original_obj) {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

const QProperty& QProperty::operator=(const QProperty& original_obj) {
    if(this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void QProperty::copyValuesFromObject(const QProperty& original_obj) {
    setObjectName(original_obj.objectName());
    setValue(original_obj.value());
}

//-----------------------------------------------------------------------------

const QVariant QProperty::value() const {
    return m_value;
}

//-----------------------------------------------------------------------------

void QProperty::setValue(const QVariant& value) {
    m_value = value;
}
