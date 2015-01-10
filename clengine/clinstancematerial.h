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

#ifndef RIOENGINE_CLENGINE_CLINSTANCEMATERIAL_H_
#define RIOENGINE_CLENGINE_CLINSTANCEMATERIAL_H_

#include <string>
#include <vector>
#include "cengine/cobject.h"

template <class T> class CArray;
class CLBindVertexInput;

class CLInstanceMaterial : public CObject {
    Q_OBJECT
    Q_PROPERTY(QString Symbol READ symbolAsQString WRITE setSymbolFromQString DESIGNABLE false)
    Q_PROPERTY(QString Target READ targetAsQString WRITE setTargetFromQString DESIGNABLE false)

public:
    CLInstanceMaterial();
    CLInstanceMaterial(const CLInstanceMaterial& instance_material);
    CLInstanceMaterial(const std::string& symbolAsQString, const std::string& targetAsQString);

    CLInstanceMaterial& operator=(const CLInstanceMaterial& instance_material);

    void setBindVertexInputList(CArray<CLBindVertexInput>* _array);
    CArray<CLBindVertexInput>* bindVertexInputList() const;

    void setSymbol(const std::string& symbol);
    void setSymbolFromQString(const QString& symbol);
    void setTarget(const std::string& target);
    void setTargetFromQString(const QString& target);

    const std::string& symbol() const;
    QString            symbolAsQString();
    const std::string& target() const;
    QString            targetAsQString();

    ~CLInstanceMaterial(void);

protected:
    virtual void copyValuesFromObject(const CLInstanceMaterial& instance_material);

private:
    std::string m_symbol;
    std::string m_target;

    CArray<CLBindVertexInput>* m_bindVertexInputList;
};

#endif  // RIOENGINE_CLENGINE_CLINSTANCEMATERIAL_H_
