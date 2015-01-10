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

#ifndef RIOENGINE_CLENGINE_CLINSTANCEGEOMETRY_H_
#define RIOENGINE_CLENGINE_CLINSTANCEGEOMETRY_H_

#include <vector>
#include <string>
#include "cengine/cobject.h"
#include "clengine/clinstancematerial.h"

class CLInstanceGeometry : public CObject {
    Q_OBJECT
    Q_PROPERTY(QString MeshURL
               READ meshURL
               WRITE setMeshURL
               DESIGNABLE false)

    Q_PROPERTY(QList<CLInstanceMaterial> InstanceMaterials
               READ instanceMaterials
               WRITE setInstanceMaterials
               DESIGNABLE false)
public:
    CLInstanceGeometry(void);
    CLInstanceGeometry(const CLInstanceGeometry& original_obj);
    CLInstanceGeometry& operator=(const CLInstanceGeometry& original_obj);

    void setInstanceMaterials(CArray<CLInstanceMaterial>* instance_materials);
    CArray<CLInstanceMaterial>* getInstanceMaterials() const;

    void setMeshURL(const std::string& url);
    const std::string& getMeshURL() const;

    QString meshURL();
    void setMeshURL(const QString& url);

    QList<CLInstanceMaterial> instanceMaterials();
    void setInstanceMaterials(QList<CLInstanceMaterial> instance_materials);

    ~CLInstanceGeometry(void);

protected:
    virtual void copyValuesFromObject(const CLInstanceGeometry& original_obj);

private:
    std::string m_meshUrl;
    CArray<CLInstanceMaterial>* m_instanceMaterials;
};

#endif  // RIOENGINE_CLENGINE_CLINSTANCEGEOMETRY_H_
