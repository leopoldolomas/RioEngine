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

#ifndef RIOENGINE_CLENGINE_CLSCENE_H_
#define RIOENGINE_CLENGINE_CLSCENE_H_

#include <map>
#include <string>
#include "typedefs.h"
#include "tinyxml/tinyxml.h"
#include "clengine/clgeometry.h"
#include "clengine/clmaterial.h"
#include "clengine/clnode.h"
#include "clengine/cleffect.h"
#include "clengine/clphong.h"
#include "clengine/clphong.h"
#include "clengine/clphongchild.h"
#include "cengine/cobject.h"
#include "cengine/carray.h"

class CLScene : public CObject {
public:
    CLScene(char* filename);

    std::vector<std::string> getNodeIdList();

    CLNode*         getNodeHavingId        (const std::string& node_id        );
    CLMaterial*     getMaterialHavingId    (const std::string& material_id    );
    CLMaterial*     getMaterialHavingName  (const std::string& material_name  );
    CLEffect*       getEffectHavingId      (const std::string& effect_id      );
    CLImageDetails* getImageDetailsHavingId(const std::string& image_id       );
    CLGeometry*     getGeometryObjHavingId (const std::string& geometry_obj_id);

    void insertImagePair   (const std::string& image_id,    CLImageDetails* image_details);
    void insertMaterialPair(const std::string& material_id, CLMaterial* material         );
    void insertEffectPair  (const std::string& effect_id,   CLEffect* effect             );
    void insertGeometryPair(const std::string& geometry_id, CLGeometry* geometry         );
    void insertNodesPair   (const std::string& node_id,     CLNode* node                 );

    bool loadImage   (TiXmlElement* image_node   );
    bool loadMaterial(TiXmlElement* material_node);
    bool loadNode    (TiXmlElement* node         );
    bool loadScene   (TiXmlElement* scene_node   );
    bool loadEffect  (TiXmlElement* effect_node  );

    const ImagesMapType*     images()       const;
    const MaterialsMapType*  materials()    const;
    const EffectsMapType*    effects()      const;
    const GeometriesMapType* geometries()   const;
    const NodesMapType*      nodes()        const;
    const CLImageDetails*    imageDetails() const;

    bool reloadMaterials();
    bool reloadImages();
    bool reloadNodes();
    bool reloadEffects();
    bool reloadGeometries();

    CLPhongChild*      getPhongChildFromNode      (TiXmlElement* node);
    CLInstanceGeometry getInstanceGeometryFromNode(TiXmlElement* instance_geometry_node);

    ~CLScene(void);

protected:
    CLScene();

private:
    bool loadGeometry(TiXmlElement* geometry_node);
    void getFloatStdVecFromSourceNode(TiXmlElement* source_node, std::vector<btVector3*>* vector_dest);
    CArray<btVector3>*    getFloatCArrayFromSourceNode(TiXmlElement*);
    CArray<TextureCoord>* getMapArrayFromSourceNode   (TiXmlElement* source_node);

    TiXmlDocument*     m_doc;
    GeometriesMapType* m_geometries;
    ImagesMapType*     m_images;
    EffectsMapType*    m_effects;
    MaterialsMapType*  m_materials;
    NodesMapType*      m_nodes;
    std::string        m_md5;
    std::vector<std::string>* m_nodesId;
};

#endif  // RIOENGINE_CLENGINE_CLSCENE_H_
