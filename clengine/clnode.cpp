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

#include "staticfuncs.h"
#include "bullet/btcustom.h"
#include "misc/stringhelper.hpp"
#include "clengine/classetsrepository.h"
#include "clengine/clpolygon.h"
#include "clengine/clnode.h"

//-----------------------------------------------------------------------------

CLNode::CLNode(const CLNode& original_node) : CObject() {
    copyValuesFromObject(original_node);
}

//-----------------------------------------------------------------------------

CLNode::CLNode(void) : CObject() {
    m_instanceGeometries = NULL;
    m_defaultSize = btCustom::zero_vector();
    m_size = btCustom::zero_vector();
    m_transform = Transform();
    m_visible = true;
}

//-----------------------------------------------------------------------------

void CLNode::copyValuesFromObject(const CLNode& original_obj) {
    m_id = original_obj.id();
    m_transform = Transform(original_obj.constTransform());
    m_visible = true;
    m_size = original_obj.size();
    m_defaultSize = ( original_obj.defaultSize() != btCustom::zero_vector() ?
                btVector3(original_obj.defaultSize()) :
                btCustom::zero_vector());

    m_instanceGeometries = (CArray<CLInstanceGeometry>*)original_obj.getInstanceGeometries()->retain();
}

//-----------------------------------------------------------------------------

Transform& CLNode::transform() {
    return m_transform;
}

//-----------------------------------------------------------------------------

const Transform& CLNode::constTransform() const {
    return m_transform;
}

//-----------------------------------------------------------------------------

void CLNode::setTransform(const Transform& transform) {
    m_transform = transform;
}

//-----------------------------------------------------------------------------

bool CLNode::visible() const {
    return m_visible;
}

//-----------------------------------------------------------------------------

void CLNode::setVisible(bool visible) {
    m_visible = visible;
}

//-----------------------------------------------------------------------------

const std::string& CLNode::id() const {
    return m_id;
}

//-----------------------------------------------------------------------------

void CLNode::setId(const std::string &id) {
    m_id = id;
}

//-----------------------------------------------------------------------------

CLNode& CLNode::operator=(const CLNode& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLNode::copyNodeValuesFromNode(CLNode* node) {
    m_instanceGeometries = ( node->getInstanceGeometries() != NULL ?
                static_cast<CArray<CLInstanceGeometry>*>(node->getInstanceGeometries()->retain()) :
                NULL );
    m_transform = Transform(node->transform());
    m_visible = node->visible();
    m_id = node->id();
    m_size = node->size();
    m_defaultSize = ( node->defaultSize() != btCustom::zero_vector() ?
                btVector3(node->defaultSize()) :
                btCustom::zero_vector());
}

//-----------------------------------------------------------------------------

CLNode* CLNode::getCopy() {
    CLNode* copy_node = new CLNode(*this);
    return copy_node;
}

//-----------------------------------------------------------------------------

void CLNode::setInstanceGeometries(CArray<CLInstanceGeometry>* _array) {
    RE_ASSERT(_array);
    SAFE_RELEASE(m_instanceGeometries);
    SAFE_RETAIN(_array);
    m_instanceGeometries = _array;
    calcSize();
}

//-----------------------------------------------------------------------------

CArray<CLInstanceGeometry>* CLNode::getInstanceGeometries() const {
    return m_instanceGeometries;
}

//-----------------------------------------------------------------------------

QList<CLInstanceGeometry> CLNode::instanceGeometries() {
    QList<CLInstanceGeometry> vec = QList<CLInstanceGeometry>();
    for (uint i = 0; i < m_instanceGeometries->size(); i++) {
        vec.append(CLInstanceGeometry(m_instanceGeometries->objectAtIndex(i)));
    }
    return vec;
}

//-----------------------------------------------------------------------------

void CLNode::setInstanceGeometries(QList<CLInstanceGeometry> vec) {
    SAFE_RELEASE(m_instanceGeometries);
    std::vector<CLInstanceGeometry> std_vec = std::vector<CLInstanceGeometry>();
    for (int i = 0; i < vec.count(); i++) {
        std_vec.push_back(CLInstanceGeometry(vec.at(i)));
    }
    setInstanceGeometries(CArray<CLInstanceGeometry>::fromStdVector(&std_vec));
}

//-----------------------------------------------------------------------------

const btVector3& CLNode::size() const {
    return m_size;
}

//-----------------------------------------------------------------------------

const btVector3& CLNode::defaultSize() const {
    return m_defaultSize;
}

//-----------------------------------------------------------------------------

const btVector3& CLNode::calcSize() {
    if(m_defaultSize == btCustom::zero_vector()) {
        QVector<float> x_values, y_values, z_values;
        for (unsigned int i = 0; i < getInstanceGeometries()->size(); i++) {
            CLInstanceGeometry* instance_geometry = &(getInstanceGeometries()->data()[i]);
            RE_ASSERT(instance_geometry);
            std::string geometry_id = instance_geometry->getMeshURL();
            geometry_id = StringHelper::removeOccurrencesOfChar(geometry_id, '#');
            CLGeometry* geometry_object = CLASSETSREPOSITORY->getGeometryObjHavingId(geometry_id);
            if (!geometry_object) {
                return btVector3(1, 1, 1);
            }

            for (unsigned int i = 0; i < geometry_object->getPolygons()->size(); i++) {
                CArray<btVector3>* triangles_vertices = static_cast<CArray<btVector3>*>(geometry_object->getPolygons()->objectAtIndex(i).getTriangleVertices()->retain());
                for (uint j = 0; j < triangles_vertices->size(); j++) {
                    btVector3 vec = triangles_vertices->objectAtIndex(j);
                    x_values.append(vec.x());
                    y_values.append(vec.y());
                    z_values.append(vec.z());
                }
                triangles_vertices->release();
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

        m_defaultSize = btVector3(Lx, Ly, Lz);
    }
    m_size = m_defaultSize * transform().localScale();
    return m_size;
}

//-----------------------------------------------------------------------------

CLNode::~CLNode(void) {
    SAFE_RELEASE(m_instanceGeometries);
}
