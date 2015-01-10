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

#include "misc/color.h"
#include "clengine/clphongchild.h"

//-----------------------------------------------------------------------------

CLPhongChild::CLPhongChild(void) : CObject() {
    m_value = NULL;
}

//-----------------------------------------------------------------------------

CLPhongChild::CLPhongChild(const CLPhongChild& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

CLPhongChild& CLPhongChild::operator=(const CLPhongChild& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLPhongChild::copyValuesFromObject(const CLPhongChild& original_obj) {
    void* original_value = original_obj.getValue();
    void* copy_value = NULL;

    switch (original_obj.getValueType()) {
    case COLOR_TYPE:
        copy_value = new Color(*(Color*)original_value);
        break;
    case TEXTURE_TYPE:
        copy_value = new char[strlen((char*)original_value)];
        strcpy_s((char*)copy_value, strlen((char*)original_value)+1, (char*)original_value);
        break;
    case FLOAT_TYPE:
        copy_value = new float(*(float*)original_value);
        break;
    }
    setValue(copy_value, original_obj.getValueType());
}

//-----------------------------------------------------------------------------

void* CLPhongChild::getValue() const {
    return m_value;
}

//-----------------------------------------------------------------------------

void CLPhongChild::setValue(void* value, PhongChildType value_type) {
    m_value = value;
    setValueType(value_type);
}

//-----------------------------------------------------------------------------

CLPhongChild::PhongChildType CLPhongChild::getValueType() const {
    return m_valueType;
}

//-----------------------------------------------------------------------------

void CLPhongChild::setValueType(CLPhongChild::PhongChildType value_type) {
    m_valueType = value_type;
}

//-----------------------------------------------------------------------------

CLPhongChild::~CLPhongChild(void) {
    switch (getValueType()) {
    case COLOR_TYPE:
        SAFE_RELEASE(reinterpret_cast<Color*>(m_value));
        break;
    case TEXTURE_TYPE:
        // TODO delete m_value; crashes the engine
        break;
    case FLOAT_TYPE:
        // TODO delete m_value; crashes the engine
        break;
    }
}
