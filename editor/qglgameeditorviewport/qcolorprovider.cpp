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

#include "editor/QGLGameEditorViewport/qcolorprovider.h"

//-----------------------------------------------------------------------------

QColorProvider::QColorProvider() {
    reset();

    // the following color are reserved by the system, they should NOT be
    // generated by QColorProvider::nextColor()
    m_reservedColors.append(QColor().black());   // default world color
    m_reservedColors.append(QColor().blue());    // x axis manipulator
    m_reservedColors.append(QColor().red());     // y axis manipulator
    m_reservedColors.append(QColor().green());   // z axis manipulator
    m_reservedColors.append(QColor(0, 0, 254));  // x axis for Uniform Scale
    m_reservedColors.append(QColor(0, 0, 253));  // y axis for Unifom Scale
}

//-----------------------------------------------------------------------------

QColor QColorProvider::nextColor() {
    QColor new_color = createNextColor();
    while (m_reservedColors.contains(new_color)) {
        new_color = createNextColor();
    }

    return new_color;
}

//-----------------------------------------------------------------------------

QColor QColorProvider::createNextColor() {
    r++;
    if (r >= 255) {
        r = 255;  // clamp the value
        g++;
        if (g >= 255) {
            g = 255;
            b++;
            if (b >= 255) {
                b = 255;
            }
        }
    }
    return QColor(r, g, b, 255);
}

//-----------------------------------------------------------------------------

void QColorProvider::reset() {
    r = g = b = 1;
}
