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
#include "bdebugdraw.h"

//-----------------------------------------------------------------------------

bDebugDraw::bDebugDraw(QGLShaderProgram* shader_program) {
    m_shaderProgram = shader_program;
    m_drawMode      = k_bdebugdraw_DefaultDebugMode;
}

//-----------------------------------------------------------------------------

std::map<QString, std::vector<bDebugDraw::LineInfo>>* bDebugDraw::lines() {
    return &m_lines;
}

//-----------------------------------------------------------------------------

void bDebugDraw::clearLines() {
    m_lines.clear();
}

//-----------------------------------------------------------------------------

void bDebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    bDebugDraw::LineInfo line;
    line.from   = from;
    line.to     = to;
    QString color_name = QColor(color.getX() * 255, color.getY() * 255, color.getZ() * 255).name();

    if(m_lines.count(color_name) > 0) {
        std::vector<LineInfo>* lines = &m_lines.find(color_name)->second;
        lines->push_back(line);
    } else {
        std::vector<LineInfo> lines;
        lines.push_back(line);
        m_lines.insert(std::pair<QString, std::vector<LineInfo>>(color_name, lines));
    }
}

//-----------------------------------------------------------------------------

void bDebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
                                  btScalar distance, int lifeTime, const btVector3 &color) {
    // TODO
}

//-----------------------------------------------------------------------------

void bDebugDraw::reportErrorWarning(const char *warningString) {
    LOG(warningString);
}

//-----------------------------------------------------------------------------

void bDebugDraw::draw3dText(const btVector3 &location, const char *textString) {
    // TODO
}

//-----------------------------------------------------------------------------

void bDebugDraw::setDebugMode(int debug_mode) {
    m_drawMode = debug_mode;
}

//-----------------------------------------------------------------------------

int bDebugDraw::getDebugMode() const {
    return m_drawMode;
}
