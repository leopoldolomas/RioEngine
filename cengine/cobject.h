//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem?, you face it like a man."
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

#ifndef RIOENGINE_CENGINE_COBJECT_H_
#define RIOENGINE_CENGINE_COBJECT_H_

#include "macro.h"

#define SAFE_RETAIN(x)          CObject::safeRetain(x)
#define SAFE_RELEASE(x)         CObject::safeRelease(x)
#define SAFE_RELEASE_ARRAY(x)   CObject::safeReleaseArray(x)

/**
 * CObject is the base class for every custom type in RioEngine
 *
 * You should never call 'delete' on any CObject because another object
 * might be referencing it. By convention you should always release a CObject
 * by calling its release() method or using the 'SAFE_RELEASE(x)' macro.
 */
class CObject : public QObject {
 public:
    /**
     * Default constructor. Don't forget to call it when subclassing!.
     */
    CObject();

    /**
     * Copy constructor. Don't forget to call it when subclassing!.
     */
    explicit CObject(const CObject& object);

    CObject& operator=(const CObject& object);

    // ---------- Reference counting methods

    /**
     * Increments the object's reference count by 1
     */
    CObject* retain();

    /**
     * Decrements the object's reference count by 1
     */
    unsigned int release() const;

    /**
     * By calling this method the object will be added to the current CAutoreleasePool
     * and will be released in a later time.
     */
    CObject* autorelease();

    /**
     * Returns the number that indicates how many times the object has been referenced
     * by other objects.
     */
    unsigned int retainCount() const;

    /**
     * Returns true only if this object inherits from CObject.
     */
    bool isValidCObject() const;

    /**
     * Destructor declared as virtual in order to force it to be called when
     * deallocating any subclass of CObject.
     */
    virtual ~CObject();

    /**
     * Retains the pointer's object only if it inherits from CObject.
     */
    static void safeRetain(void* pointer) {
        if (!pointer) {
            return;
        }

        CObject* ref_as_cobject = reinterpret_cast<CObject*>(pointer);

        if (ref_as_cobject->isValidCObject()) {
            ref_as_cobject->retain();
        }
    }

    /**
     * Releases the pointer's object only if it inherits from CObject. Otherwise
     * the pointer gets deallocated
     */
    template<class T>
    static void safeRelease(T* pointer)
    {
        if (pointer) {
            CObject* ref_as_cobject = reinterpret_cast<CObject*>(pointer);
            if (ref_as_cobject->isValidCObject()) {
                ref_as_cobject->release();
            }
            else {
                delete pointer;
            }
            pointer = NULL;
        }
    }

    template<class T>
    static void safeReleaseArray(T* array)
    {
        if (array) {
            delete[] array;
            array = NULL;
        }
    }

    static void safeRelease(const char* pointer) {
        safeReleaseArray(pointer);
    }

 protected:
    /**
     * Self-explanatory. Don't forget to override it! (if needed)
     */
    virtual void copyValuesFromObject(const CObject& object);

 private:
    /**
     * Controls the lifetime of the object. When the CObject is initialized its
     * __refCount value is set to 1 (see retain() and release() methods). When
     * the value reaches zero the object gets deallocated.
     */
    mutable unsigned int __refCount;

    /**
     * Points to "this" only if it inherits from CObject
     */
    void* __selfPointer;
};
Q_DECLARE_METATYPE(CObject)
QDataStream &operator<<(QDataStream &ds, const CObject &obj);
QDataStream &operator>>(QDataStream &ds, CObject &obj);

#endif   // RIOENGINE_CENGINE_COBJECT_H_
