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

#include <string>
#include "misc/genericshelper.hpp"
#include "cengine/carray.h"
#include "clengine/clinstancematerial.h"

//-----------------------------------------------------------------------------

CLInstanceMaterial::CLInstanceMaterial() : CObject() {
    setSymbol("");
    setTarget("");

    m_bindVertexInputList = NULL;
}

//-----------------------------------------------------------------------------

CLInstanceMaterial::CLInstanceMaterial(const CLInstanceMaterial& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

CLInstanceMaterial::CLInstanceMaterial(const std::string& symbol, const std::string& target) : CObject() {
    setSymbol(symbol);
    setTarget(target);

    m_bindVertexInputList = NULL;
}

//-----------------------------------------------------------------------------

CLInstanceMaterial& CLInstanceMaterial::operator=(const CLInstanceMaterial& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::copyValuesFromObject(const CLInstanceMaterial& original_obj) {
    m_symbol = std::string(original_obj.symbol());
    m_target = std::string(original_obj.target());

    if ((original_obj.bindVertexInputList() &&
         original_obj.bindVertexInputList()->size() > 0)) {
        m_bindVertexInputList = (CArray<CLBindVertexInput>*)original_obj.bindVertexInputList()->retain();
    } else {
        m_bindVertexInputList = NULL;
    }
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::setBindVertexInputList(CArray<CLBindVertexInput>* _array) {
    SAFE_RELEASE(m_bindVertexInputList);
    SAFE_RETAIN(_array);
    m_bindVertexInputList = _array;
}

//-----------------------------------------------------------------------------

CArray<CLBindVertexInput>* CLInstanceMaterial::bindVertexInputList() const {
    return m_bindVertexInputList;
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::setSymbol(const std::string& symbol) {
    m_symbol = symbol;
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::setSymbolFromQString(const QString& symbol) {
    m_symbol = symbol.toStdString();
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::setTarget(const std::string& target) {
    m_target = target;
}

//-----------------------------------------------------------------------------

void CLInstanceMaterial::setTargetFromQString(const QString& target) {
    m_target = target.toStdString();
}

//-----------------------------------------------------------------------------

const std::string& CLInstanceMaterial::symbol() const {
    return m_symbol;
}

//-----------------------------------------------------------------------------

QString CLInstanceMaterial::symbolAsQString() {
    return QString::fromStdString(m_symbol);
}

//-----------------------------------------------------------------------------

const std::string& CLInstanceMaterial::target() const {
    return m_target;
}

//-----------------------------------------------------------------------------

QString CLInstanceMaterial::targetAsQString() {
    return QString::fromStdString(m_target);
}

//-----------------------------------------------------------------------------

CLInstanceMaterial::~CLInstanceMaterial(void) {
    SAFE_RELEASE(m_bindVertexInputList);
}
