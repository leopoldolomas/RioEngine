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
#include "staticfuncs.h"
#include "misc/btcustom.h"
#include "misc/genericshelper.hpp"
#include "cengine/carray.h"
#include "clengine/clpolygon.h"
#include "clengine/cltriangles.h"
#include "clengine/clmesh.h"

//-----------------------------------------------------------------------------

CLMesh::CLMesh(void) : CObject() {
    m_positions     = NULL;
    m_normals       = NULL;
    m_textureCoords = NULL;
    m_triangles     = NULL;
}

//-----------------------------------------------------------------------------

CLMesh& CLMesh::operator=(const CLMesh& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

CLMesh::CLMesh(const CLMesh& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLMesh::copyValuesFromObject(const CLMesh& original_obj) {
    m_positions     = (CArray<btVector3>*)original_obj.positions()->retain();
    m_normals       = (CArray<btVector3>*)original_obj.normals()->retain();
    m_textureCoords = (CArray<TextureCoord>*)original_obj.mapArray()->retain();
    m_triangles     = (CArray<CLTriangles>*)original_obj.triangles()->retain();
}

//-----------------------------------------------------------------------------

void CLMesh::setTriangles(CArray<CLTriangles>* triangles_array) {
    RE_ASSERT(triangles_array);
    SAFE_RELEASE(m_triangles);
    SAFE_RETAIN(triangles_array);
    m_triangles = triangles_array;
}

//-----------------------------------------------------------------------------

CArray<CLTriangles>* CLMesh::triangles() const {
    return m_triangles;
}

//-----------------------------------------------------------------------------

void CLMesh::setPositions(CArray<btVector3>* positions) {
    RE_ASSERT(positions);
    SAFE_RELEASE(m_positions);
    SAFE_RETAIN(positions);
    m_positions = positions;
}

//-----------------------------------------------------------------------------

CArray<btVector3>* CLMesh::positions() const {
    return m_positions;
}

//-----------------------------------------------------------------------------

void CLMesh::setNormals(CArray<btVector3>* normals) {
    RE_ASSERT(normals);
    SAFE_RELEASE(m_normals);
    SAFE_RETAIN(normals);
    m_normals = normals;
}

//-----------------------------------------------------------------------------

CArray<btVector3>* CLMesh::normals() const {
    return m_normals;
}

//-----------------------------------------------------------------------------

void CLMesh::setMapArray(CArray<TextureCoord>* map_array) {
    RE_ASSERT(map_array);
    SAFE_RELEASE(m_textureCoords);
    SAFE_RETAIN(map_array);
    m_textureCoords = map_array;
}

//-----------------------------------------------------------------------------

CArray<TextureCoord>* CLMesh::mapArray() const {
    return m_textureCoords;
}

//-----------------------------------------------------------------------------

CLPolygon CLMesh::getPolygonForTrianglesIndex(unsigned int index) {
    RE_ASSERT(index < m_triangles->size());

    CLTriangles triangles_obj = m_triangles->objectAtIndex(index);

    CLPolygon polygon_obj = CLPolygon();

    CArray<btVector3>* triangles_vertices = NULL;
    CArray<btVector3>* triangles_normals = NULL;
    CArray<TextureCoord>* triangles_texture_coords = NULL;

    triangles_vertices       = getLocationsArrayHavingIndicesArray<btVector3>(m_positions, triangles_obj.getVerticesIndices());
    triangles_normals        = getLocationsArrayHavingIndicesArray<btVector3>(m_normals, triangles_obj.getNormalsIndices());
    triangles_texture_coords = getLocationsArrayHavingIndicesArray<TextureCoord>(m_textureCoords, triangles_obj.getTextureCoordIndices());

    polygon_obj.setTriangleMaps(triangles_texture_coords);
    polygon_obj.setTriangleNormals(triangles_normals);
    polygon_obj.setTriangleVertices(triangles_vertices);
    polygon_obj.setMaterialId(triangles_obj.getMaterialId());

    return polygon_obj;
}

//-----------------------------------------------------------------------------

CArray<CLPolygon>* CLMesh::getPolygonsArray() {
    std::vector<CLPolygon>* polygons_array = new std::vector<CLPolygon>();

    for (unsigned int i = 0; i < m_triangles->size(); i++) {
        polygons_array->push_back(getPolygonForTrianglesIndex(i));
    }

    auto array = fixPolygonsArray(polygons_array);
    SAFE_RELEASE(polygons_array);

    return array;
}

//-----------------------------------------------------------------------------

/**
 * This method recenters object's origin, it also scales down the object (if needed)
 */
CArray<CLPolygon>* CLMesh::fixPolygonsArray( std::vector<CLPolygon>* polygons_array ) {
    QVector<float> x_values, y_values, z_values;
    for (std::vector<CLPolygon>::iterator it = polygons_array->begin();
         it != polygons_array->end();
         ++it) {
        for (uint j = 0; j < (*it).getTriangleVertices()->size(); j++) {
            btVector3 vec = (*it).getTriangleVertices()->objectAtIndex(j);
            x_values.append(vec.x());
            y_values.append(vec.y());
            z_values.append(vec.z());
        }
    }

    qSort(x_values.begin(), x_values.end(), valueLessThan);
    qSort(y_values.begin(), y_values.end(), valueLessThan);
    qSort(z_values.begin(), z_values.end(), valueLessThan);

    float x_min = x_values.first();
    float y_min = y_values.first();
    float z_min = z_values.first();

    float x_max = x_values.last();
    float y_max = y_values.last();
    float z_max = z_values.last();

    float Lx = fabs(x_max - x_min);
    float Ly = fabs(y_max - y_min);
    float Lz = fabs(z_max - z_min);

    // calculate the centroid position
    float Cx = x_min + fabs(x_max - x_min) / 2.0f;
    float Cy = y_min + fabs(y_max - y_min) / 2.0f;
    float Cz = z_min + fabs(z_max - z_min) / 2.0f;

    btVector3 centroid = btVector3(Cx, Cy, Cz);

    float max_axis_length = qMax(Lx, qMax(Ly, Lz));
    float scale_down_factor = 1.0f;
    if (max_axis_length > k_CLMesh_MaxAxisLength) {
        scale_down_factor = k_CLMesh_MaxAxisLength / max_axis_length;
    }

    std::vector<CLPolygon>* fixed_polygons = new std::vector<CLPolygon>();
    for (std::vector<CLPolygon>::iterator it = polygons_array->begin();
         it != polygons_array->end();
         ++it) {
        std::vector<btVector3> vec_array = std::vector<btVector3>();
        CLPolygon polygon = *it;
        for (uint j = 0; j < polygon.getTriangleVertices()->size(); j++) {
            btVector3 vec = polygon.getTriangleVertices()->objectAtIndex(j);
            btVector3 fixed_vec = vec - centroid;
            fixed_vec *= scale_down_factor;
            vec_array.push_back(fixed_vec);
        }
        auto triangleVerticesArray = CArray<btVector3>::fromStdVector(&vec_array);
        polygon.setTriangleVertices(triangleVerticesArray);
        fixed_polygons->push_back(polygon);
        vec_array.clear();
    }
    auto polygons_array = CArray<CLPolygon>::fromStdVector(fixed_polygons);
    fixed_polygons->clear();
    SAFE_RELEASE(fixed_polygons);

    return polygons_array;
}

//-----------------------------------------------------------------------------

CLMesh::~CLMesh(void) {
    SAFE_RELEASE(m_positions);
    SAFE_RELEASE(m_normals);
    SAFE_RELEASE(m_textureCoords);
    SAFE_RELEASE(m_triangles);
}
