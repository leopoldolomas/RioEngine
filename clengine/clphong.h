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

#ifndef RIOENGINE_CLENGINE_CLPHONG_H_
#define RIOENGINE_CLENGINE_CLPHONG_H_

#include "cengine/cobject.h"

class CLPhongChild;

class CLPhong : public CObject {
 public:
    CLPhong();
    CLPhong::CLPhong(const CLPhong&);
    CLPhong& operator=(const CLPhong&);

    void setEmission(CLPhongChild* emission);
    CLPhongChild* getEmission() const;

    void setAmbient(CLPhongChild* ambient);
    CLPhongChild* getAmbient() const;

    void setDiffuse(CLPhongChild* diffuse);
    CLPhongChild* getDiffuse() const;

    void setSpecular(CLPhongChild* specular);
    CLPhongChild* getSpecular() const;

    void setShininess(CLPhongChild* shininess);
    CLPhongChild* getShininess() const;

    void setIndexOfRefraction(CLPhongChild* index_of_refraction);
    CLPhongChild* getIndexOfRefraction() const;

    ~CLPhong(void);

protected:
   virtual void copyValuesFromObject(const CLPhong& phong);

private:
   CLPhongChild* m_emission;
   CLPhongChild* m_ambient;
   CLPhongChild* m_diffuse;
   CLPhongChild* m_specular;
   CLPhongChild* m_shininess;
   CLPhongChild* m_indexOfRefraction;
};

#endif  // RIOENGINE_CLENGINE_CLPHONG_H_