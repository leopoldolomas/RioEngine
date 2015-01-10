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

#include "clengine/clphongchild.h"
#include "clengine/clphong.h"

//-----------------------------------------------------------------------------

CLPhong::CLPhong(const CLPhong& original_obj) : CObject() {
   copyValuesFromObject(original_obj);
}

//-----------------------------------------------------------------------------

CLPhong::CLPhong() : CObject() {
    m_ambient   = NULL;
    m_diffuse   = NULL;
    m_emission  = NULL;
    m_specular  = NULL;
    m_shininess = NULL;
    m_indexOfRefraction = NULL;
}

//-----------------------------------------------------------------------------

CLPhong& CLPhong::operator=(const CLPhong& original_obj) {
    if (this == &original_obj) {
        return *this;
    }

    copyValuesFromObject(original_obj);

    return *this;
}

//-----------------------------------------------------------------------------

void CLPhong::copyValuesFromObject(const CLPhong& original_obj) {
    m_ambient   = (CLPhongChild*)original_obj.getAmbient()->retain();
    m_diffuse   = (CLPhongChild*)original_obj.getDiffuse()->retain();
    m_emission  = (CLPhongChild*)original_obj.getEmission()->retain();
    m_specular  = (CLPhongChild*)original_obj.getSpecular()->retain();
    m_shininess = (CLPhongChild*)original_obj.getShininess()->retain();
    m_indexOfRefraction = (CLPhongChild*)original_obj.getIndexOfRefraction()->retain();
}

//-----------------------------------------------------------------------------

void CLPhong::setEmission(CLPhongChild* emission) {
    RE_ASSERT(emission);
    SAFE_RELEASE(m_emission);
    SAFE_RETAIN(emission);
    m_emission = emission;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getEmission() const {
    return m_emission;
}

//-----------------------------------------------------------------------------

void CLPhong::setAmbient(CLPhongChild* ambient) {
    RE_ASSERT(ambient);
    SAFE_RELEASE(m_ambient);
    SAFE_RETAIN(ambient);
    m_ambient = ambient;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getAmbient() const {
    return m_ambient;
}

//-----------------------------------------------------------------------------

void CLPhong::setDiffuse(CLPhongChild* diffuse) {
    RE_ASSERT(diffuse);
    SAFE_RELEASE(m_diffuse);
    SAFE_RETAIN(diffuse);
    m_diffuse = diffuse;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getDiffuse() const {
    return m_diffuse;
}

//-----------------------------------------------------------------------------

void CLPhong::setSpecular(CLPhongChild* specular) {
    RE_ASSERT(specular);
    SAFE_RELEASE(m_specular);
    SAFE_RETAIN(specular);
    m_specular = specular;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getSpecular() const {
    return m_specular;
}

//-----------------------------------------------------------------------------

void CLPhong::setShininess(CLPhongChild* shininess) {
    RE_ASSERT(shininess);
    SAFE_RELEASE(m_shininess);
    SAFE_RETAIN(shininess);
    m_shininess = shininess;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getShininess() const {
    return m_shininess;
}

//-----------------------------------------------------------------------------

void CLPhong::setIndexOfRefraction(CLPhongChild* index_of_refraction) {
    RE_ASSERT(index_of_refraction);
    SAFE_RELEASE(m_indexOfRefraction);
    SAFE_RETAIN(index_of_refraction);
    m_indexOfRefraction = index_of_refraction;
}

//-----------------------------------------------------------------------------

CLPhongChild* CLPhong::getIndexOfRefraction() const {
    return m_indexOfRefraction;
}

//-----------------------------------------------------------------------------

CLPhong::~CLPhong(void) {
    SAFE_RELEASE(m_emission);
    SAFE_RELEASE(m_ambient);
    SAFE_RELEASE(m_diffuse);
    SAFE_RELEASE(m_specular);
    SAFE_RELEASE(m_shininess);
    SAFE_RELEASE(m_indexOfRefraction);
}
