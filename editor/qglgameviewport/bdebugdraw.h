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

#ifndef RIOENGINE_EDITOR_QGLGAMEVIEWPORT_BDEBUGDRAW_H
#define RIOENGINE_EDITOR_QGLGAMEVIEWPORT_BDEBUGDRAW_H

#include <QGLShaderProgram>
#include "bullet/btBulletDynamicsCommon.h"

class bDebugDraw : public btIDebugDraw {
public:
    struct LineInfo {
        btVector3 from, to;
    };

    bDebugDraw(QGLShaderProgram* shader_program = NULL);
    std::map<QString, std::vector<LineInfo>>* lines();

    void clearLines();

    virtual void    drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
    virtual void    drawContactPoint(const btVector3 &PointOnB,
                                     const btVector3 &normalOnB,
                                     btScalar distance,
                                     int lifeTime,
                                     const btVector3 &color);
    virtual void    reportErrorWarning(const char *warningString);
    virtual void    draw3dText(const btVector3 &location, const char *textString);
    virtual void    setDebugMode(int debugMode);
    virtual int     getDebugMode() const;

private:
    int m_drawMode;

    QGLShaderProgram* m_shaderProgram;
    std::map<QString, std::vector<LineInfo>> m_lines;
};

#endif //  RIOENGINE_EDITOR_QGLGAMEVIEWPORT_BDEBUGDRAW_H
