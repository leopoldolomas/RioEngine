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

#ifndef RIOENGINE_CLENGINE_CLMESH_H_
#define RIOENGINE_CLENGINE_CLMESH_H_

#include <vector>
#include "cengine/cobject.h"

template <class T> class CArray;
class   CLTriangles;
class   CLPolygon;
struct  TextureCoord;

/**
 * CLMesh objects contain raw data (vertices, normals and textures),
 * CLTriangle objects are used to create CLPolygon objects, which are used by
 * CLRenderer to draw a textured polygon. CLPolygons objects are contained
 * in CLGeometry objects.
 */
class CLMesh : public CObject {
public:
    CLMesh(void);
    CLMesh(const CLMesh& mesh);
    CLMesh& operator=(const CLMesh& mesh);

    void setPositions(CArray<btVector3>*    positions);
    void setNormals  (CArray<btVector3>*    normals);
    void setMapArray (CArray<TextureCoord>* map_array);
    void setTriangles(CArray<CLTriangles>*  triangles_array);

    CArray<btVector3>*    normals()   const;
    CArray<btVector3>*    positions() const;
    CArray<TextureCoord>* mapArray()  const;
    CArray<CLTriangles>*  triangles() const;

    /**
     * Creates the CLPolygon object for the specified CLTriangle array index
     */
    CLPolygon getPolygonForTrianglesIndex(unsigned int index);

    /**
     * Returns a CLPolygon object for each CLTriangle object
     */
    CArray<CLPolygon>* getPolygonsArray();

    CArray<CLPolygon>* fixPolygonsArray(std::vector<CLPolygon>* polygons_array);

    template<class T>
    CArray<T>* getLocationsArrayHavingIndicesArray(CArray<T>* locations_array,
                                                   CArray<int>* indices_array) {
        if (!locations_array) {
            return NULL;
        }

        RE_ASSERT(indices_array);

        btAlignedObjectArray<T> vec =  btAlignedObjectArray<T>();

        for (unsigned int i = 0; i < indices_array->size(); i++) {
            T original_obj = locations_array->objectAtIndex(indices_array->objectAtIndex(i));
            vec.push_back(T(original_obj));
        }
        CArray<T>* array = CArray<T>::fromBtAlignedObjectArray(&vec);
        vec.clear();
        return array;
    }

    ~CLMesh(void);

protected:
    virtual void copyValuesFromObject(const CLMesh& mesh);

private:
    CArray<btVector3>*    m_positions;
    CArray<btVector3>*    m_normals;
    CArray<TextureCoord>* m_textureCoords;
    CArray<CLTriangles>*  m_triangles;
};

#endif  // RIOENGINE_CLENGINE_CLMESH_H_
