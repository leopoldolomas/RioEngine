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
#include "clengine/clmesh.h"
#include "clengine/clgeometry.h"

//-----------------------------------------------------------------------------

CLGeometry::CLGeometry() : CObject() {
    m_mesh = new CLMesh();
    m_polygons = NULL;
}

//-----------------------------------------------------------------------------

CLGeometry::CLGeometry(const CLGeometry& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLGeometry::copyValuesFromObject(const CLGeometry& original_obj) {
    m_mesh = (CLMesh*)original_obj.getMesh()->retain();
    m_polygons = NULL;

    CArray<CLPolygon>* polygons_array = original_obj.getPolygons();
    setPolygons(polygons_array);
}

//-----------------------------------------------------------------------------

CLGeometry& CLGeometry::operator=(const CLGeometry& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLGeometry::setPolygons(CArray<CLPolygon>* polygons_array) {
    RE_ASSERT(polygons_array);
    SAFE_RELEASE(m_polygons);
    SAFE_RETAIN(polygons_array);
    m_polygons = polygons_array;
}

//-----------------------------------------------------------------------------

CArray<CLPolygon>* CLGeometry::getPolygons() const {
    return m_polygons;
}

//-----------------------------------------------------------------------------

CLMesh* CLGeometry::getMesh() const {
    return m_mesh;
}

//-----------------------------------------------------------------------------

CLGeometry::~CLGeometry() {
    SAFE_RELEASE(m_polygons);
    SAFE_RELEASE(m_mesh);
}
