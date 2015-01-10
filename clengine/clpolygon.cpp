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

#include "constants.h"
#include "cengine/carray.h"
#include "clengine/clpolygon.h"

//-----------------------------------------------------------------------------

CLPolygon::CLPolygon(void) : CObject() {
    m_triangleVertices             = NULL;
    m_triangleNormals              = NULL;
    m_textCoords                   = NULL;
    m_triangleVerticesConstData    = NULL;
    m_triangleMapsConstData        = NULL;
}

//-----------------------------------------------------------------------------

CLPolygon::CLPolygon(const CLPolygon& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLPolygon::copyValuesFromObject(const CLPolygon& original_obj) {
    if (original_obj.getTriangleMaps()) {
        m_textCoords = static_cast<CArray<TextureCoord>*>(original_obj.getTriangleMaps()->retain());
    } else {
        m_textCoords = NULL;
    }
    
    m_triangleNormals              = static_cast<CArray<btVector3>*>(original_obj.getTriangleNormals()->retain());
    m_triangleVertices             = static_cast<CArray<btVector3>*>(original_obj.getTriangleVertices()->retain());
    m_triangleVerticesConstData    = NULL;
    m_triangleMapsConstData        = NULL;
    m_materialId                    = original_obj.getMaterialId();
}

//-----------------------------------------------------------------------------

CLPolygon& CLPolygon::operator=(const CLPolygon& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLPolygon::setMaterialId(const std::string& material_id) {
    m_materialId = material_id;
}

//-----------------------------------------------------------------------------

const std::string& CLPolygon::getMaterialId() const {
    return m_materialId;
}

//-----------------------------------------------------------------------------

void CLPolygon::setTriangleVertices(CArray<btVector3>* triangles_vertices) {
    RE_ASSERT(triangles_vertices);
    SAFE_RELEASE(m_triangleVertices);
    SAFE_RETAIN(triangles_vertices);
    m_triangleVertices = triangles_vertices;
}

//-----------------------------------------------------------------------------

void CLPolygon::setTriangleNormals(CArray<btVector3>* triangles_normals) {
    RE_ASSERT(triangles_normals);
    SAFE_RELEASE(m_triangleNormals);
    SAFE_RETAIN(triangles_normals);
    m_triangleNormals = triangles_normals;
}

//-----------------------------------------------------------------------------

void CLPolygon::setTriangleMaps(CArray<TextureCoord>* triangles_texture_coords) {
    // RE_ASSERT(triangles_texture_coords); m_texCoords can be NULL
    SAFE_RELEASE(m_textCoords);
    SAFE_RETAIN(triangles_texture_coords);
    m_textCoords = triangles_texture_coords;
}

//-----------------------------------------------------------------------------

CArray<btVector3>* CLPolygon::getTriangleVertices() const {
    return m_triangleVertices;
}

//-----------------------------------------------------------------------------

float* CLPolygon::getTriangleVerticesConstData() {
    if(m_triangleVerticesConstData) {
        return m_triangleVerticesConstData;
    }

    int count = m_triangleVertices->size() * 3;
    unsigned int index = 0;
    m_triangleVerticesConstData = new float[count];

    for (unsigned int i = 0; i < m_triangleVertices->size(); i++) {
        btVector3* vec = &m_triangleVertices->objectAtIndex(i);
        (*vec) *= k_QGLGameEditorViewport_SizeFactor;
        m_triangleVerticesConstData[index++] = vec->x();
        m_triangleVerticesConstData[index++] = vec->y();
        m_triangleVerticesConstData[index++] = vec->z();
    }

    return m_triangleVerticesConstData;
}

//-----------------------------------------------------------------------------

CArray<btVector3>* CLPolygon::getTriangleNormals() const {
    return m_triangleNormals;
}

//-----------------------------------------------------------------------------

CArray<TextureCoord>* CLPolygon::getTriangleMaps() const {
    return m_textCoords;
}

//-----------------------------------------------------------------------------

float* CLPolygon::getTriangleMapsConstData() {
    if (!m_textCoords) {
        return NULL;
    }

    if(m_triangleMapsConstData) {
        return m_triangleMapsConstData;
    }

    int count = m_textCoords->size() * 2;
    unsigned int index = 0;
    m_triangleMapsConstData = new float[count];

    for (unsigned int i = 0; i < m_textCoords->size(); i++) {
        m_triangleMapsConstData[index++] = m_textCoords->objectAtIndex(i).s;
        m_triangleMapsConstData[index++] = 1 - m_textCoords->objectAtIndex(i).t;
    }

    return m_triangleMapsConstData;
}

//-----------------------------------------------------------------------------

CLPolygon::~CLPolygon(void) {
    SAFE_RELEASE(m_textCoords);
    SAFE_RELEASE(m_triangleNormals);
    SAFE_RELEASE(m_triangleVertices);
    SAFE_RELEASE_ARRAY(m_triangleVerticesConstData);
    SAFE_RELEASE_ARRAY(m_triangleMapsConstData);
}
