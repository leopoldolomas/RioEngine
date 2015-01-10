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
#include "misc/color.h"
#include "misc/transform.h"
#include "misc/stringhelper.hpp"
#include "misc/matrixstack.h"
#include "clengine/clpolygon.h"
#include "clengine/clscene.h"
#include "clengine/clrenderer.h"

//-----------------------------------------------------------------------------

bool CLRenderer::drawNode(QGLShaderProgram* program, CLScene* collada_scene,
                          CLNode* node, QColor* color) {
    if (!node->visible()) {
        return false;
    }

    for (unsigned int i = 0; i < node->getInstanceGeometries()->size(); i++) {
        CLInstanceGeometry *instance_geometry = &(node->getInstanceGeometries()->data()[i]);

        std::string geometry_id = instance_geometry->getMeshURL();
        geometry_id = StringHelper::removeOccurrencesOfChar(geometry_id, '#');
        CLGeometry* geometry_object = collada_scene->getGeometryObjHavingId(geometry_id);
        QColor renderColor;

        // let's draw each polygon
        for (unsigned int i = 0; i < geometry_object->getPolygons()->size(); i++) {
            CLPolygon* polygon_obj = &(geometry_object->getPolygons()->data()[i]);
            const GLfloat* vertices = polygon_obj->getTriangleVerticesConstData();
            const GLfloat* text_coords = polygon_obj->getTriangleMapsConstData();

            bool has_texture = false;
            GLuint *texture_id = NULL;

            if (!color) {  // if no color was specified, then try to render the texture {
                const std::string& material_id = polygon_obj->getMaterialId();
                if (strcmp(material_id.c_str(), "") != 0) {
                    CLMaterial* material = collada_scene->getMaterialHavingName(material_id);

                    if (material) {
                        // TODO get color from phong effect
                        renderColor = QColor(210, 210, 210);

                        std::string& effect_id = const_cast<std::string&>(material->instanceEffectURL());
                        effect_id = StringHelper::removeOccurrencesOfChar(effect_id, '#');
                        CLEffect* effect = collada_scene->getEffectHavingId(effect_id);

                        if (effect->hasImage()) {
                            texture_id = static_cast<GLuint*>(&collada_scene->getImageDetailsHavingId(effect->imageId())->texture);
                            has_texture = true;
                            program->setUniformValue("renderTexture", 1.0f);
                        } else {
                            program->setUniformValue("renderTexture", 0.0f);
                        }
                    } else {
                        program->setUniformValue("renderTexture", 0.0f);
                        renderColor = QColor(210, 210, 210);
                    }
                }
            } else {
                renderColor = *color;
                program->setUniformValue("renderTexture", 0.0f);
            }

            // if no material was loaded then instantiate text_coords to avoid
            // a GL error
            bool release_text_coords = false;
            if (!text_coords) {
                text_coords = new GLfloat[polygon_obj->getTriangleVertices()->size() * 2];
                release_text_coords = true;
            }

            GLfloat* color_array = new GLfloat[3];
            color_array[0] = (float)renderColor.red()    / 255.0f;
            color_array[1] = (float)renderColor.green()  / 255.0f;
            color_array[2] = (float)renderColor.blue()   / 255.0f;
            program->setAttributeValue(10, (const GLfloat*)color_array, 1, 3);
            SAFE_RELEASE(color_array);

            const float* opengl_matrix = MATRIXSTACK->getMatrix().get();
            QMatrix4x4 qmatrix_model = QMatrix4x4(opengl_matrix);

            float* raw_matrix = qmatrix_model.data();

            // scale down object's position
            raw_matrix[12] = raw_matrix[12] * k_QGLGameEditorViewport_SizeFactor;
            raw_matrix[13] = raw_matrix[13] * k_QGLGameEditorViewport_SizeFactor;
            raw_matrix[14] = raw_matrix[14] * k_QGLGameEditorViewport_SizeFactor;

            program->setUniformValue("modelMatrix", qmatrix_model);

            program->enableAttributeArray(0);
            program->enableAttributeArray(1);
            program->setAttributeArray(0, vertices, 3);
            program->setAttributeArray(1, text_coords, 2);

            if (has_texture) {
                glBindTexture(GL_TEXTURE_2D, *texture_id);
            }

            glDrawArrays(GL_TRIANGLES, 0,  // TODO use GL_LINES to draw wireframe, have in mind that special shaders should be used for wireframe mode
                         polygon_obj->getTriangleVertices()->size());

            if(release_text_coords) {
                SAFE_RELEASE_ARRAY(text_coords);
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------

bool CLRenderer::drawLine(QGLShaderProgram* program, btVector3 &from, btVector3 &to, QColor* color) {
    if (!color) {
        color = &QColor();
    }

    GLfloat color_array[3];
    color_array[0] = (float)color->red()    / 255.0f;
    color_array[1] = (float)color->green()  / 255.0f;
    color_array[2] = (float)color->blue()   / 255.0f;
    program->setAttributeValue(10, (const GLfloat*)color_array, 1, 3);

    GLfloat vertices[] = {
        (GLfloat)(from.x()), (GLfloat)(from.y()), (GLfloat)(from.z()),
        (GLfloat)(to.x()), (GLfloat)(to.y()), (GLfloat)(to.z()),
    };

    QMatrix4x4 mat;
    mat.setToIdentity();
    program->setUniformValue("modelMatrix", mat);
    program->setUniformValue("renderTexture", 0.0f);
    program->enableAttributeArray(0);
    program->setAttributeArray(0, vertices, 3);
    glDrawArrays(GL_LINES, 0, 2);

    return true;
}

//-----------------------------------------------------------------------------

bool CLRenderer::drawDebugLines(QGLShaderProgram* program, std::map<QString, std::vector<bDebugDraw::LineInfo>>* lines) {
    QMatrix4x4 mat;
    mat.setToIdentity();
    program->setUniformValue("modelMatrix", mat);
    program->setUniformValue("renderTexture", 0.0f);

    GLfloat* color_array = NULL;
    GLfloat* vertices    = NULL;

    for(std::map<QString, std::vector<bDebugDraw::LineInfo>>::const_iterator it = lines->begin();
        it != lines->end();
        ++it) {
        QColor color = QColor(it->first);
        color_array = new GLfloat[3];
        color_array[0] = (float)color.red()    / 255.0f;
        color_array[1] = (float)color.green()  / 255.0f;
        color_array[2] = (float)color.blue()   / 255.0f;
        program->setAttributeValue(10, (const GLfloat*)color_array, 1, 3);
        SAFE_RELEASE(color_array);

        for(std::vector<bDebugDraw::LineInfo>::const_iterator lineinfo_it = it->second.begin();
            lineinfo_it != it->second.end();
            ++lineinfo_it) {
            vertices = new GLfloat[6];
            const bDebugDraw::LineInfo *line = &(*lineinfo_it);
            vertices[0] = (GLfloat)(line->from.x());
            vertices[1] = (GLfloat)(line->from.y());
            vertices[2] = (GLfloat)(line->from.z());
            vertices[3] = (GLfloat)(line->to.x());
            vertices[4] = (GLfloat)(line->to.y());
            vertices[5] = (GLfloat)(line->to.z());

            program->enableAttributeArray(0);
            program->setAttributeArray(0, vertices, 3);
            glDrawArrays(GL_LINES, 0, 2);

            SAFE_RELEASE_ARRAY(vertices);
        }
    }
    return true;
}
