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

#include "misc/genericshelper.hpp"
#include "cengine/carray.h"
#include "clengine/clinstancegeometry.h"

//-----------------------------------------------------------------------------

CLInstanceGeometry::CLInstanceGeometry(void) : CObject() {
    m_meshUrl = "";
    m_instanceMaterials = NULL;
}

//-----------------------------------------------------------------------------

CLInstanceGeometry::CLInstanceGeometry(const CLInstanceGeometry& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLInstanceGeometry::copyValuesFromObject(const CLInstanceGeometry& original_obj) {
    m_meshUrl = std::string(original_obj.getMeshURL());
    m_instanceMaterials = (CArray<CLInstanceMaterial>*)original_obj.getInstanceMaterials()->retain();
}

//-----------------------------------------------------------------------------

CLInstanceGeometry& CLInstanceGeometry::operator=(const CLInstanceGeometry& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLInstanceGeometry::setInstanceMaterials(CArray<CLInstanceMaterial>* instance_materials) {
    RE_ASSERT(instance_materials);
    SAFE_RELEASE(m_instanceMaterials);
    SAFE_RETAIN(instance_materials);
    m_instanceMaterials = instance_materials;
}

//-----------------------------------------------------------------------------

CArray<CLInstanceMaterial>* CLInstanceGeometry::getInstanceMaterials() const {
    return m_instanceMaterials;
}

//-----------------------------------------------------------------------------

void CLInstanceGeometry::setMeshURL(const std::string& url) {
    m_meshUrl = url;
}

//-----------------------------------------------------------------------------

const std::string& CLInstanceGeometry::getMeshURL() const {
    return m_meshUrl;
}

//-----------------------------------------------------------------------------

QString CLInstanceGeometry::meshURL() {
    return QString::fromStdString(m_meshUrl);
}

//-----------------------------------------------------------------------------

void CLInstanceGeometry::setMeshURL(const QString& url) {
    m_meshUrl = url.toStdString();
}

//-----------------------------------------------------------------------------

QList<CLInstanceMaterial> CLInstanceGeometry::instanceMaterials() {
    QList<CLInstanceMaterial> vec = QList<CLInstanceMaterial>();
    for (uint i = 0; i < m_instanceMaterials->size(); i++) {
        vec.append(CLInstanceMaterial(m_instanceMaterials->objectAtIndex(i)));
    }
    return vec;
}

//-----------------------------------------------------------------------------

void CLInstanceGeometry::setInstanceMaterials(QList<CLInstanceMaterial> instance_materials) {
    SAFE_RELEASE(m_instanceMaterials);
    std::vector<CLInstanceMaterial> std_vec = std::vector<CLInstanceMaterial>();
    for (QList<CLInstanceMaterial>::iterator it = instance_materials.begin();
         it != instance_materials.end();
         ++it) {
        std_vec.push_back(*it);
    }
    setInstanceMaterials(CArray<CLInstanceMaterial>::fromStdVector(&std_vec));
}

//-----------------------------------------------------------------------------

CLInstanceGeometry::~CLInstanceGeometry(void) {
    SAFE_RELEASE(m_instanceMaterials);
}
