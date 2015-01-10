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

#include "clengine/clphong.h"
#include "clengine/cleffect.h"

//-----------------------------------------------------------------------------

CLEffect::CLEffect(void) : CObject() {
    setImageId("");
    m_phong = NULL;
}

//-----------------------------------------------------------------------------

CLEffect::CLEffect(const CLEffect& original_obj) : CObject() {
    copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

void CLEffect::copyValuesFromObject(const CLEffect& original_obj) {
    setImageId(original_obj.imageId());
    m_phong = (CLPhong*)original_obj.phong()->retain();
}

//-----------------------------------------------------------------------------

CLEffect& CLEffect::operator=(const CLEffect& original_obj) {
    if (this == &original_obj)
        return *this;

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLEffect::setImageId(const std::string& image_id) {
    m_imageId = image_id;
}

//-----------------------------------------------------------------------------

const std::string& CLEffect::imageId() const {
    return m_imageId;
}

//-----------------------------------------------------------------------------

void CLEffect::setPhong(CLPhong* phong) {
    RE_ASSERT(phong);
    SAFE_RELEASE(m_phong);
    m_phong = phong;
}

//-----------------------------------------------------------------------------

CLPhong* CLEffect::phong() const {
    return m_phong;
}

//-----------------------------------------------------------------------------

bool CLEffect::hasImage() {
    return imageId() != "";
}

//-----------------------------------------------------------------------------

CLEffect::~CLEffect(void) {
    SAFE_RELEASE(m_phong);
}
