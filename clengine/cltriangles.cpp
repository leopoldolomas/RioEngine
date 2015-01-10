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

#include "cengine/carray.h"
#include "clengine/cltriangles.h"

//-----------------------------------------------------------------------------

CLTriangles::CLTriangles(void) : CObject() {
    m_normalsIndices        = NULL;
    m_verticesIndices       = NULL;
    m_textureCoordIndices   = NULL;
}

//-----------------------------------------------------------------------------

CLTriangles::CLTriangles(const CLTriangles& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLTriangles::copyValuesFromObject(const CLTriangles& original_obj) {
    m_normalsIndices      = (CArray<int>*)original_obj.getNormalsIndices()->retain();
    m_textureCoordIndices = (CArray<int>*)original_obj.getTextureCoordIndices()->retain();
    m_verticesIndices     = (CArray<int>*)original_obj.getVerticesIndices()->retain();

    setMaterialId(original_obj.getMaterialId());
}

//-----------------------------------------------------------------------------

CLTriangles& CLTriangles::operator=(const CLTriangles& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLTriangles::setMaterialId(const std::string& material_id) {
    m_materialId = material_id;
}

//-----------------------------------------------------------------------------

const std::string& CLTriangles::getMaterialId() const {
    return m_materialId;
}

//-----------------------------------------------------------------------------

CArray<int>* CLTriangles::getVerticesIndices() const {
    return m_verticesIndices;
}

//-----------------------------------------------------------------------------

CArray<int>* CLTriangles::getNormalsIndices() const {
    return m_normalsIndices;
}

//-----------------------------------------------------------------------------

CArray<int>* CLTriangles::getTextureCoordIndices() const {
    return m_textureCoordIndices;
}

//-----------------------------------------------------------------------------

void CLTriangles::setVerticesIndices(CArray<int>* indices) {
    RE_ASSERT(indices);
    SAFE_RELEASE(m_verticesIndices);
    SAFE_RETAIN(indices);
    m_verticesIndices = indices;
}

//-----------------------------------------------------------------------------

void CLTriangles::setNormalsIndices(CArray<int>* indices) {
    RE_ASSERT(indices);
    SAFE_RELEASE(m_normalsIndices);
    SAFE_RETAIN(indices);
    m_normalsIndices = indices;
}

//-----------------------------------------------------------------------------

void CLTriangles::setTextureCoordsIndices(CArray<int>* indices) {
    RE_ASSERT(indices);
    SAFE_RELEASE(m_textureCoordIndices);
    SAFE_RETAIN(indices);
    m_textureCoordIndices = indices;
}

//-----------------------------------------------------------------------------

CLTriangles::~CLTriangles(void) {
    SAFE_RELEASE(m_normalsIndices);
    SAFE_RELEASE(m_verticesIndices);
    SAFE_RELEASE(m_textureCoordIndices);
}
