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

#include <QtOpenGL>
#include "macro.h"
#include "constants.h"
#include "transform_common.h"
#include "tinyxml/tinyxmlhelper.hpp"
#include "misc/color.h"
#include "misc/transform.h"
#include "misc/md5.h"
#include "misc/stringhelper.hpp"
#include "misc/genericshelper.hpp"
#include "misc/directoryhelper.hpp"
#include "clengine/clmesh.h"
#include "clengine/clinstancematerial.h"
#include "clengine/clbindvertexinput.h"
#include "clengine/clinstancegeometry.h"
#include "clengine/cltriangles.h"
#include "clengine/clscene.h"

//-----------------------------------------------------------------------------

#define ADDMD5PREFIX(x) StringHelper::addPrefix(m_md5, x)

//-----------------------------------------------------------------------------

CLScene::CLScene() : CObject() {
    m_geometries  = new GeometriesMapType();
    m_images      = new ImagesMapType();
    m_materials   = new MaterialsMapType();
    m_effects     = new EffectsMapType();
    m_nodes       = new NodesMapType();
    m_nodesId     = new std::vector<std::string>();
}

//-----------------------------------------------------------------------------

CLScene::CLScene(char* filename) : CObject() {
    m_geometries    = NULL;
    m_images        = NULL;
    m_materials     = NULL;
    m_effects       = NULL;
    m_nodes         = NULL;
    m_nodesId       = NULL;

    m_doc = new TiXmlDocument(filename);
    RE_ASSERT(m_doc->LoadFile());
    RE_ASSERT(m_doc->FirstChildElement("COLLADA"));

    std::string relative_path = StringHelper::removeOccurrencesOfString(
                std::string((const char*)filename),
                DirectoryHelper::getProjectPath());
    m_md5 = md5(relative_path);
    unsigned int size = m_md5.size();
    unsigned int index = size - k_CLScene_MD5MaxCharactersCount;
    m_md5 = m_md5.substr(index, k_CLScene_MD5MaxCharactersCount).append("_");

    reloadImages();
    reloadNodes();
    reloadMaterials();
    reloadEffects();
    reloadGeometries();

    m_doc->Clear();
    SAFE_RELEASE(m_doc);
}

//-----------------------------------------------------------------------------

std::vector<std::string> CLScene::getNodeIdList() {
    std::vector<std::string> nodes_list;

    auto it = m_nodes->begin();

    for (; it != m_nodes->end(); it++) {
        nodes_list.push_back(it->first);
    }

    return nodes_list;
}

//-----------------------------------------------------------------------------

CLNode* CLScene::getNodeHavingId(const std::string& node_id) {
    return (m_nodes->count(node_id) == 0 ? NULL : m_nodes->at(node_id));
}

//-----------------------------------------------------------------------------

CLMaterial* CLScene::getMaterialHavingId(const std::string& material_id) {
    return (m_materials->count(material_id) == 0 ? NULL : m_materials->at(material_id));
}

//-----------------------------------------------------------------------------

CLEffect* CLScene::getEffectHavingId(const std::string& effect_id) {
    return (m_effects->count(effect_id) == 0 ? NULL : m_effects->at(effect_id));
}

//-----------------------------------------------------------------------------

CLImageDetails* CLScene::getImageDetailsHavingId(const std::string& image_id) {
    return (m_images->count(image_id) == 0 ? NULL : m_images->at(image_id));
}

//-----------------------------------------------------------------------------

CLGeometry* CLScene::getGeometryObjHavingId(const std::string& geometry_obj_id) {
    return (m_geometries->count(geometry_obj_id) == 0 ? NULL : m_geometries->at(geometry_obj_id));
}

//-----------------------------------------------------------------------------

void CLScene::insertImagePair(const std::string& image_id, CLImageDetails* image_details) {
    m_images->insert(ImagePairType(image_id, image_details));
}

//-----------------------------------------------------------------------------

void CLScene::insertMaterialPair(const std::string& material_id, CLMaterial* material) {
    m_materials->insert(MaterialPairType(material_id, material));
}

//-----------------------------------------------------------------------------

void CLScene::insertEffectPair(const std::string& effect_id, CLEffect* effect) {
    m_effects->insert(EffectPairType(effect_id, effect));
}

//-----------------------------------------------------------------------------

void CLScene::insertGeometryPair(const std::string& geometry_id, CLGeometry* geometry) {
    m_geometries->insert(GeometryPairType(geometry_id, geometry));
}

//-----------------------------------------------------------------------------

void CLScene::insertNodesPair(const std::string& node_id, CLNode* node) {
    m_nodes->insert(NodePairType(node_id, node));
}

//-----------------------------------------------------------------------------

bool CLScene::loadImage(TiXmlElement* image_node) {
    std::string image_id = image_node->Attribute("id");

    LOG("Loading image with id: %s", image_id.c_str());

    std::string image_name = image_node->Attribute("name");
    std::string image_filename = image_node->FirstChildElement("init_from")->GetText();

    CLImageDetails *image_detail = new CLImageDetails;
    image_detail->name = ADDMD5PREFIX(image_name);
    image_detail->filename = image_filename;
    image_detail->texture = 0;

    insertImagePair(ADDMD5PREFIX(image_id), image_detail);

    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::loadMaterial(TiXmlElement* material_node) {
    std::string material_id = material_node->Attribute("id");
    LOG("Loading material with id: %s", material_id.c_str());

    std::string material_name = "";

    if (material_node->Attribute("id")) {
        material_id = material_node->Attribute("id");
    }

    if (material_node->Attribute("name")) {
        material_name = material_node->Attribute("name");
    }

    std::string instance_effect_url = material_node->FirstChildElement("instance_effect")->Attribute("url");
    CLMaterial* material = new CLMaterial(ADDMD5PREFIX(material_id), ADDMD5PREFIX(material_name), ADDMD5PREFIX(instance_effect_url));
    insertMaterialPair(ADDMD5PREFIX(material_id), material);

    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::loadNode(TiXmlElement* node) {
    std::string node_id = node->Attribute("id");

    LOG("Loading node: %s", node_id.c_str());

    CLNode* cl_node = new CLNode();
    TiXmlElement *instance_geometry_node = node->FirstChildElement("instance_geometry");
    std::vector<CLInstanceGeometry>* instance_geometries_vector = new std::vector<CLInstanceGeometry>();

    CLInstanceGeometry instance_geometry;
    while (instance_geometry_node) {
        instance_geometry = getInstanceGeometryFromNode(instance_geometry_node);
        instance_geometries_vector->push_back(instance_geometry);
        instance_geometry_node = instance_geometry_node->NextSiblingElement("instance_geometry");
    }
    cl_node->setInstanceGeometries(CArray<CLInstanceGeometry>::fromStdVector(instance_geometries_vector));
    SAFE_RELEASE(instance_geometries_vector);

    TiXmlElement* scale_node = node->FirstChildElement("scale");
    std::string scale = (scale_node ? scale_node->GetText() : "1 1 1");

    std::vector<float>* scale_array = (std::vector<float>*)TinyXMLHelper::getVectorFromString(scale.c_str(), TinyXMLHelper::Float);
    cl_node->transform().setLocalScale(btVector3(scale_array->at(0), scale_array->at(1), scale_array->at(2)));
    scale_array->clear();
    SAFE_RELEASE(scale_array);

    TiXmlElement* location_node = node->FirstChildElement("translate");
    std::string location = (location_node ? location_node->GetText() : "0 0 0");
    std::vector<float>* location_array = (std::vector<float>*)TinyXMLHelper::getVectorFromString(location.c_str(), TinyXMLHelper::Float);
    cl_node->transform().setLocalPosition(btVector3(location_array->at(0), location_array->at(1), location_array->at(2)));
    SAFE_RELEASE(location_array);

    cl_node->transform().setLocalRotation(btQuaternion(0, 0, 0));
    cl_node->setId(node_id);
    insertNodesPair(node_id, cl_node);

    if (!m_nodesId) {
        m_nodesId = new std::vector<std::string>();
    }

    m_nodesId->push_back(node_id);

    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::loadScene(TiXmlElement* scene_node) {
    TiXmlElement *current_node = scene_node->FirstChildElement("node");

    while (current_node) {
        if (current_node->FirstChildElement("instance_geometry") != NULL) {
            loadNode(current_node);
        }
        current_node = current_node->NextSiblingElement("node");
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::loadEffect(TiXmlElement* effect_node) {
    TiXmlElement* pc_node = effect_node->FirstChildElement("profile_COMMON");
    CLEffect* effect = new CLEffect();
    CLPhong* phong = NULL;

    if (pc_node) {
        TiXmlElement* tc_node = pc_node->FirstChildElement("technique");
        const char* sid = tc_node->Attribute("sid");
        if (strcmp(sid, "common") == 0) {
            phong = new CLPhong();
            TiXmlElement* phong_node = tc_node->FirstChildElement("phong");

            if (!phong_node) {
                phong_node = tc_node->FirstChildElement("blinn");
            }

            TiXmlElement* emission_node = phong_node->FirstChildElement("emission");
            CLPhongChild* emission_child = getPhongChildFromNode(emission_node);
            phong->setEmission(emission_child);

            TiXmlElement* ambient_node = phong_node->FirstChildElement("ambient");
            CLPhongChild* ambient_child = getPhongChildFromNode(ambient_node);
            phong->setAmbient(ambient_child);

            TiXmlElement* diffuse_node = phong_node->FirstChildElement("diffuse");
            CLPhongChild* diffuse_child = getPhongChildFromNode(diffuse_node);
            phong->setDiffuse(diffuse_child);

            TiXmlElement* specular_node = phong_node->FirstChildElement("specular");
            CLPhongChild* specular_child = getPhongChildFromNode(specular_node);
            phong->setSpecular(specular_child);

            TiXmlElement* shininess_node = phong_node->FirstChildElement("shininess");
            CLPhongChild* shininess_child = getPhongChildFromNode(shininess_node);
            phong->setShininess(shininess_child);

            TiXmlElement* index_refraction_node = phong_node->FirstChildElement("index_of_refraction");
            CLPhongChild* index_refraction_child = getPhongChildFromNode(index_refraction_node);
            phong->setIndexOfRefraction(index_refraction_child);

            if (phong->getDiffuse()->getValueType() == CLPhongChild::TEXTURE_TYPE) {
                const char* sampler_str = (const char*)phong->getDiffuse()->getValue();

                TiXmlElement* newparam_node = pc_node->FirstChildElement("newparam");
                while (newparam_node) {
                    std::string alternative = sampler_str;
                    alternative = alternative.append("-sampler");
                    if (strcmp(newparam_node->Attribute("sid"), sampler_str) == 0 ||
                            strcmp(newparam_node->Attribute("sid"), alternative.c_str()) == 0) {
                        const char* source_str = newparam_node->FirstChildElement("sampler2D")->FirstChildElement("source")->GetText();

                        TiXmlElement* inner_newparam_node = pc_node->FirstChildElement("newparam");
                        while (inner_newparam_node) {
                            if (strcmp(inner_newparam_node->Attribute("sid"), source_str) == 0) {
                                std::string image_id = ADDMD5PREFIX(
                                            inner_newparam_node->FirstChildElement("surface")->FirstChildElement("init_from")->GetText());
                                effect->setImageId(image_id);
                                break;
                            }
                            inner_newparam_node = inner_newparam_node->NextSiblingElement("newparam");
                        }
                    }

                    if (effect->hasImage()) {
                        break;
                    }
                    newparam_node = newparam_node->NextSiblingElement("newparam");
                }
            }
        }
        effect->setPhong(phong);
    }
    insertEffectPair(ADDMD5PREFIX(effect_node->Attribute("id")), effect);

    return true;
}

//-----------------------------------------------------------------------------

const NodesMapType* CLScene::nodes() const {
    return m_nodes;
}

//-----------------------------------------------------------------------------

const ImagesMapType* CLScene::images() const {
    return m_images;
}

//-----------------------------------------------------------------------------

const MaterialsMapType* CLScene::materials() const {
    return m_materials;
}

//-----------------------------------------------------------------------------

const EffectsMapType* CLScene::effects() const {
    return m_effects;
}

//-----------------------------------------------------------------------------

const GeometriesMapType* CLScene::geometries() const {
    return m_geometries;
}

//-----------------------------------------------------------------------------

const CLImageDetails* CLScene::imageDetails() const {
    return NULL;
}

//-----------------------------------------------------------------------------

bool CLScene::reloadMaterials() {
    if (m_materials) {
        GenericsHelper::deleteMap(m_materials);  // unload current materials
    }

    m_materials = new MaterialsMapType();

    TiXmlNode *root = m_doc->FirstChildElement("COLLADA");
    TiXmlNode *materials_node = root->FirstChild("library_materials");

    if (!materials_node) {
        return false;
    }

    TiXmlElement *current_material = materials_node->FirstChildElement("material");

    while (current_material) {
        loadMaterial(current_material);
        current_material = current_material->NextSiblingElement();
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::reloadImages() {
    if (m_images) {
        GenericsHelper::deleteMap(m_images); // unload current images
    }

    m_images = new ImagesMapType();

    TiXmlNode *root = m_doc->FirstChildElement("COLLADA");
    TiXmlNode *images_node = root->FirstChild("library_images");

    if (!images_node) {
        return false;
    }

    TiXmlElement *current_image = images_node->FirstChildElement("image");

    while (current_image) {
        loadImage(current_image);
        current_image = current_image->NextSiblingElement();
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::reloadNodes() {
    if (m_nodes) {
        GenericsHelper::deleteMap(m_nodes);  // unload the current nodes
    }

    m_nodes = new NodesMapType();

    TiXmlNode *root = m_doc->FirstChildElement("COLLADA");
    TiXmlNode *materials_node = root->FirstChild("library_visual_scenes");
    TiXmlElement *current_node = materials_node->FirstChildElement("visual_scene");

    while (current_node) {
        loadScene(current_node);
        current_node = current_node->NextSiblingElement();
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::reloadEffects() {
    if (m_effects) {
        GenericsHelper::deleteMap(m_effects);  // unload current effects
    }

    m_effects = new EffectsMapType();

    TiXmlNode *root = m_doc->FirstChildElement("COLLADA");
    TiXmlNode *effects_node = root->FirstChild("library_effects");

    if (!effects_node) {
        return false;
    }

    TiXmlElement *effect_node = effects_node->FirstChildElement("effect");

    while (effect_node) {
        loadEffect(effect_node);
        effect_node = effect_node->NextSiblingElement("effect");
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLScene::reloadGeometries() {
    if (m_geometries) {
        GenericsHelper::deleteMap(m_geometries);  // unload current geometries
    }

    m_geometries = new GeometriesMapType();

    TiXmlNode *root = m_doc->FirstChildElement("COLLADA");
    TiXmlNode *geometry_node = root->FirstChild("library_geometries");
    TiXmlElement *current_geometry = geometry_node->FirstChildElement("geometry");

    while (current_geometry) {
        loadGeometry(current_geometry);
        current_geometry = current_geometry->NextSiblingElement();
    }
    return true;
}

//-----------------------------------------------------------------------------

CLMaterial* CLScene::getMaterialHavingName(const std::string& material_name) {
    MaterialsMapType::const_iterator it = m_materials->begin();

    for (; it != m_materials->end(); it++) {
        CLMaterial* material_obj = it->second;
        if (material_obj->name() == material_name ||
                material_obj->id() == material_name) {
            return material_obj;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------

bool CLScene::loadGeometry(TiXmlElement* geometry_node) {
    const char *geometry_id        = geometry_node->Attribute("id");

    CLGeometry* geometry_object    = new CLGeometry();
    TiXmlElement *mesh_node        = geometry_node->FirstChildElement("mesh");
    TiXmlElement *source_node      = mesh_node->FirstChildElement("source");
    std::string str_positions      = geometry_id;
    std::string str_normals        = geometry_id;
    std::string str_map_array      = geometry_id;
    std::string str_map_array_alt1 = geometry_id;

    str_positions       += "-positions";
    str_normals         += "-normals";
    str_map_array       += "-map-0";  // blender looks for "-x" suffix...
    str_map_array_alt1    += "-map";  //  mesh lab doesn't

    const char* obj_id = NULL;

    while (source_node) {
        obj_id = source_node->Attribute("id");

        if (obj_id) {
            if (strcmp(obj_id, str_positions.c_str()) == 0) {
                geometry_object->getMesh()->setPositions(getFloatCArrayFromSourceNode(source_node));
            }
            else if (strcmp(obj_id, str_normals.c_str()) == 0) {
                geometry_object->getMesh()->setNormals(getFloatCArrayFromSourceNode(source_node));
            }
            else if (strcmp(obj_id, str_map_array.c_str()) == 0 ||
                     strcmp(obj_id, str_map_array_alt1.c_str()) == 0) {
                geometry_object->getMesh()->setMapArray(getMapArrayFromSourceNode(source_node));
            }
        }
        source_node = source_node->NextSiblingElement();
    }
    SAFE_RELEASE(obj_id);

    TiXmlElement *triangles_node = mesh_node->FirstChildElement("triangles");
    std::vector<CLTriangles>* triangles_vector = new std::vector<CLTriangles>();
    std::vector<int> vertices_indices;
    std::vector<int> normals_indices;
    std::vector<int> textcoords_indices;

    while (triangles_node) {
        vertices_indices   = std::vector<int>();
        normals_indices    = std::vector<int>();
        textcoords_indices = std::vector<int>();

        int input_nodes_count = 0, vertices_offset = 0, normals_offset = 0, texture_coords_offset = 0;

        TiXmlElement* input_node = triangles_node->FirstChildElement("input");
        while (input_node) {
            if (strcmp(input_node->Attribute("semantic"), "VERTEX")          == 0) {
                vertices_offset = atoi(input_node->Attribute("offset"));
            } else if (strcmp(input_node->Attribute("semantic"), "NORMAL")   == 0) {
                normals_offset = atoi(input_node->Attribute("offset"));
            } else if (strcmp(input_node->Attribute("semantic"), "TEXCOORD") == 0) {
                texture_coords_offset = atoi(input_node->Attribute("offset"));
            }

            input_nodes_count++;
            input_node = input_node->NextSiblingElement("input");
        }

        CLTriangles triangles_obj = CLTriangles();

        const char* p_value_as_str = triangles_node->FirstChildElement("p")->GetText();
        std::vector<int>* indices = static_cast<std::vector<int>*>(TinyXMLHelper::getVectorFromString(p_value_as_str, TinyXMLHelper::Integer));

        for (unsigned int i = vertices_offset; i < indices->size(); i += input_nodes_count) {
            vertices_indices.push_back(indices->at(i));
        }

        for (unsigned int i = normals_offset; i < indices->size(); i += input_nodes_count) {
            normals_indices.push_back(indices->at(i));
        }

        for (unsigned int i = texture_coords_offset; i < indices->size(); i += input_nodes_count) {
            textcoords_indices.push_back(indices->at(i));
        }

        const char* material_id_cstr = triangles_node->Attribute("material");
        std::string material_id = (material_id_cstr ? material_id_cstr : "");
        triangles_obj.setMaterialId(ADDMD5PREFIX(material_id));
        triangles_obj.setVerticesIndices(CArray<int>::fromStdVector(&vertices_indices));
        triangles_obj.setNormalsIndices(CArray<int>::fromStdVector(&normals_indices));
        triangles_obj.setTextureCoordsIndices(CArray<int>::fromStdVector(&textcoords_indices));

        indices->clear();
        SAFE_RELEASE(indices);

        triangles_vector->push_back(triangles_obj);
        triangles_node = triangles_node->NextSiblingElement();
    }

    geometry_object->getMesh()->setTriangles(CArray<CLTriangles>::fromStdVector(triangles_vector));
    triangles_vector->clear();
    SAFE_RELEASE(triangles_vector);

    geometry_object->setPolygons(geometry_object->getMesh()->getPolygonsArray());

    int len = strlen(geometry_id) + 1;
    char *tmp = new char[len];
    strncpy_s(tmp, len, geometry_id, len);
    insertGeometryPair(ADDMD5PREFIX(tmp), geometry_object);
    SAFE_RELEASE_ARRAY(tmp);

    return true;
}

//-----------------------------------------------------------------------------

void CLScene::getFloatStdVecFromSourceNode(TiXmlElement* source_node, std::vector<btVector3*>* vector_dest) {
    const char* float_array_as_string = source_node->FirstChildElement("float_array")->GetText();

    std::vector<GLfloat>* position_array = new std::vector<GLfloat>();

    char *tmp = new char[strlen(float_array_as_string)];
    strcpy_s(tmp, strlen(float_array_as_string)+1, float_array_as_string);

    char* pch;
    char* context = NULL;

    pch = strtok_s (tmp," ", &context);

    while (pch != NULL) {
        position_array->push_back((float)atof(pch));

        if (position_array->size() == 3) {
            btVector3* new_point = new btVector3(position_array->at(0), position_array->at(1), position_array->at(2));
            vector_dest->push_back(new_point);

            SAFE_RELEASE(position_array);
            position_array = new std::vector<GLfloat>();
        }
        pch = strtok_s (NULL, " ", &context);
    }
    SAFE_RELEASE(position_array);
    SAFE_RELEASE(pch);
}

//-----------------------------------------------------------------------------

CArray<btVector3>* CLScene::getFloatCArrayFromSourceNode(TiXmlElement* source_node) {
    const char* float_array_as_string = source_node->FirstChildElement("float_array")->GetText();

    std::vector<GLfloat> position_array;

    int len = strlen(float_array_as_string) + 1;
    char *tmp = new char[len];
    strncpy_s(tmp, len, float_array_as_string, len);

    char* pch;
    char* context = NULL;

    pch = strtok_s (tmp," ", &context);

    btAlignedObjectArray<btVector3>* vector_dest = new btAlignedObjectArray<btVector3>();

    while (pch != NULL) {
        position_array.push_back((float)atof(pch));

        if (position_array.size() == 3) {
            btVector3 new_point = btVector3(position_array.at(0), position_array.at(1), position_array.at(2));
            vector_dest->push_back(new_point);
            position_array = std::vector<GLfloat>();
        }
        pch = strtok_s (NULL, " ", &context);
    }
    SAFE_RELEASE(pch);
    SAFE_RELEASE_ARRAY(tmp);

    CArray<btVector3>* array = CArray<btVector3>::fromBtAlignedObjectArray(vector_dest);
    vector_dest->clear();
    SAFE_RELEASE(vector_dest);

    return array;
}

//-----------------------------------------------------------------------------

CArray<TextureCoord>* CLScene::getMapArrayFromSourceNode(TiXmlElement* source_node) {
    const char* float_array_as_string = source_node->FirstChildElement("float_array")->GetText();
    std::vector<GLfloat> position_array;

    int len = strlen(float_array_as_string) + 1;
    char* tmp = new char[len];
    strncpy_s(tmp, len, float_array_as_string, len);

    char* pch;
    char* context = NULL;

    pch = strtok_s (tmp," ", &context);

    std::vector<TextureCoord>* vector_dest = new std::vector<TextureCoord>();

    while (pch != NULL) {
        position_array.push_back((float)atof(pch));

        if (position_array.size() == 2) {
            TextureCoord new_texcoord = TextureCoord();
            new_texcoord.s = position_array.at(0);
            new_texcoord.t = 1.0f - position_array.at(1);
            vector_dest->push_back(new_texcoord);

            position_array = std::vector<GLfloat>();
        }
        pch = strtok_s (NULL, " ", &context);
    }
    SAFE_RELEASE_ARRAY(tmp);
    SAFE_RELEASE(pch);

    CArray<TextureCoord>* array = CArray<TextureCoord>::fromStdVector(vector_dest);
    SAFE_RELEASE(vector_dest);
    return array;
}

//-----------------------------------------------------------------------------

CLInstanceGeometry CLScene::getInstanceGeometryFromNode(TiXmlElement* instance_geometry_node) {
    CLInstanceGeometry instance_geometry = CLInstanceGeometry();
    instance_geometry.setMeshURL(ADDMD5PREFIX(instance_geometry_node->Attribute("url")));
    TiXmlElement *bind_material_node = instance_geometry_node->FirstChildElement("bind_material");
    std::vector<CLInstanceMaterial> instance_materials_vec = std::vector<CLInstanceMaterial>();

    if (bind_material_node) {
        TiXmlElement *material_node = bind_material_node->FirstChildElement("technique_common")->FirstChildElement("instance_material");

        std::vector<CLBindVertexInput>* bindvertexinput_array = NULL;

        while (material_node) {
            std::string symbol = ADDMD5PREFIX(material_node->Attribute("symbol"));
            std::string target = ADDMD5PREFIX(material_node->Attribute("target"));

            CLInstanceMaterial material_instance = CLInstanceMaterial(symbol, target);

            TiXmlElement *bind_vertex_node = material_node->FirstChildElement("bind_vertex_input");

            bindvertexinput_array = new std::vector<CLBindVertexInput>();

            while (bind_vertex_node) {
                std::string semantic = bind_vertex_node->Attribute("semantic");
                std::string input_semantic = bind_vertex_node->Attribute("input_semantic");
                std::string input_set_as_string = "0";

                if (bind_vertex_node->Attribute("input_set")) {
                    input_set_as_string = bind_vertex_node->Attribute("input_set");
                }

                int input_set = atoi(input_set_as_string.c_str());

                CLBindVertexInput bind_vertex = CLBindVertexInput(semantic, input_semantic, input_set);
                bindvertexinput_array->push_back(bind_vertex);
                bind_vertex_node = bind_vertex_node->NextSiblingElement("bind_vertex_input");
            }

            if (bindvertexinput_array->size() > 0) {
                material_instance.setBindVertexInputList(CArray<CLBindVertexInput>::fromStdVector(bindvertexinput_array));
            } else {
                material_instance.setBindVertexInputList(NULL);
            }
            
            SAFE_RELEASE(bindvertexinput_array);
            instance_materials_vec.push_back(material_instance);
            material_node = material_node->NextSiblingElement("instance_material");
        }
    }
    instance_geometry.setInstanceMaterials(CArray<CLInstanceMaterial>::fromStdVector(&instance_materials_vec));
    instance_materials_vec.clear();

    return instance_geometry;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLScene::getPhongChildFromNode(TiXmlElement* node) {
    CLPhongChild* child = new CLPhongChild();
    TiXmlElement* color_node   = node->FirstChildElement("color");
    TiXmlElement* texture_node = node->FirstChildElement("texture");
    TiXmlElement* float_node   = node->FirstChildElement("float");

    if (color_node) {
        std::vector<float>* colorArray = (std::vector<float>*)TinyXMLHelper::getVectorFromString(color_node->GetText(), TinyXMLHelper::Float);
        Color* color = new Color(colorArray->at(0), colorArray->at(1), colorArray->at(2));
        SAFE_RELEASE(colorArray);

        child->setValue(color, CLPhongChild::COLOR_TYPE);
    } else if (texture_node) {
        child->setValue((void*)texture_node->Attribute("texture"), CLPhongChild::TEXTURE_TYPE);
    } else if (float_node) {
        float val = (float)atof(float_node->GetText());
        child->setValue(&val, CLPhongChild::FLOAT_TYPE);
    }

    child->autorelease();
    return child;
}

//-----------------------------------------------------------------------------

CLScene::~CLScene(void) {
    GenericsHelper::deleteMap(m_images);
    GenericsHelper::deleteMap(m_materials);
    GenericsHelper::deleteMap(m_effects);
    GenericsHelper::deleteMap(m_geometries);
    GenericsHelper::deleteMap(m_nodes);
    SAFE_RELEASE(m_nodesId);
}
