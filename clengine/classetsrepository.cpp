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

#include "misc/directoryhelper.hpp"
#include "clengine/classetsrepository.h"

//-----------------------------------------------------------------------------

CLAssetsRepository::CLAssetsRepository() : CLScene() {
    std::string resources_dir = DirectoryHelper::getProjectDirectory();
    resources_dir.append("assets\\");

    m_resourcesDir = resources_dir;
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::loadResources() {
    CLASSETSREPOSITORY;  // instantiates the shared instance if needed

    std::vector<std::string> collada_files = DirectoryHelper::getFileList(m_resourcesDir, "dae");

    for (std::vector<std::string>::iterator it = collada_files.begin();
         it != collada_files.end();
         ++it) {
        std::string file = *it;
        std::string file_path = m_resourcesDir;
        file_path.append(file);

        CLScene* scene = new CLScene((char*)file_path.c_str());
        CLASSETSREPOSITORY->addScene(scene);
        SAFE_RELEASE(scene);
    }
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addScene(CLScene* scene) {
    addImagesFromScene(scene);
    addMaterialsFromScene(scene);
    addEffectsFromScene(scene);
    addGeometriesFromScene(scene);
    addNodesFromScene(scene);
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addImagesFromScene(CLScene* scene) {
    auto it = scene->images()->begin();

    for (; it != scene->images()->end(); it++) {
        std::string image_id = it->first;
        if (getImageDetailsHavingId(image_id)) {
            // TODO rename the image if needed
        }

        CLImageDetails* copy_obj = new CLImageDetails(*it->second);
        insertImagePair(image_id, copy_obj);
    }
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addMaterialsFromScene(CLScene* scene) {
    auto it = scene->materials()->begin();

    for (; it != scene->materials()->end(); it++) {
        std::string material_id = it->first;
        if (getMaterialHavingId(material_id)) {
            // TODO rename the material if needed
        }

        CLMaterial* copy_obj = new CLMaterial(*it->second);
        insertMaterialPair(material_id, copy_obj);
    }
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addEffectsFromScene(CLScene* scene) {
    auto it = scene->effects()->begin();

    for (; it != scene->effects()->end(); it++) {
        std::string effect_id = it->first;
        if (getEffectHavingId(effect_id)) {
            // TODO rename the effect if needed
        }

        CLEffect* copy_obj = new CLEffect(*it->second);
        insertEffectPair(effect_id, copy_obj);
    }
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addGeometriesFromScene(CLScene* scene) {
    auto it = scene->geometries()->begin();

    for (; it != scene->geometries()->end(); it++) {
        std::string geometry_id = it->first;
        if (getGeometryObjHavingId(geometry_id)) {
            // TODO rename the geometry if needed
        }

        CLGeometry* original_obj = static_cast<CLGeometry*>(it->second);
        CLGeometry* copy_obj = new CLGeometry(*original_obj);
        insertGeometryPair(geometry_id, copy_obj);
    }
}

//-----------------------------------------------------------------------------

void CLAssetsRepository::addNodesFromScene(CLScene* scene) {
    auto it = scene->nodes()->begin();

    for (; it != scene->nodes()->end(); it++) {
        std::string node_id = it->first;
        if (getNodeHavingId(node_id)) {
            // TODO rename the node if needed
        }

        CLNode* copy_obj = new CLNode(*it->second);
        insertNodesPair(node_id, copy_obj);
    }
}
