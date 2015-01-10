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

#ifndef RIOENGINE_CENGINE_CARRAY_H_
#define RIOENGINE_CENGINE_CARRAY_H_

#include <vector>
#include "bullet/btBulletDynamicsCommon.h"
#include "cengine/cobject.h"

/**
 * Very simple Array Wrapper class that holds an array of the specified type
 * along with its size. It is immutable once initalized.
 * PS: Very shallow compared to std::vector, but it was very fun to work with it!
 */
template <class T>
class CArray : public CObject {
public:
    /**
     * @brief CArray
     * @param _array The raw array
     * @param size The size of the array
     */
    CArray(T* _array, unsigned int size) {
        RE_ASSERT(!IS_POINTER(T));
        RE_ASSERT(_array);

        m_array = _array;
        m_size  = size;
    }

    //-----------------------------------------------------------------------------

    /**
     * @brief CArray
     * @param _array The original CArray object
     */
    CArray(const CArray& _array) {
        RE_ASSERT(_array.data());
        RE_ASSERT(_array.size() > 0); // TODO is this really necessary?. It should be valid  to create a copy of an empty array.

        m_size = _array.size();
        m_array = new T[m_size];

        T* data = (T*)_array.data();

        for (unsigned int i = 0; i < m_size; i++) {
            m_array[i] = data[i];
        }
    }

    //-----------------------------------------------------------------------------

    /**
     * Returns the object at the specified index
     */
    T objectAtIndex(unsigned int index) const {
        RE_ASSERT(m_array);
        RE_ASSERT(index < m_size);

        return m_array[index];
    }

    //-----------------------------------------------------------------------------

    /**
     * Returns the raw array
     */
    T* data() const {
        return m_array;
    }

    //-----------------------------------------------------------------------------

    /**
     * Returns the size of the array
     */
    unsigned int size() const {
        return m_size;
    }

    //-----------------------------------------------------------------------------

    /**
     * Creates a std::vector object from a CArray object
     */
    std::vector<T>* toStdVector() const {
        RE_ASSERT(m_array);

        std::vector<T>* _vector = new std::vector<T>();
        for (unsigned int i = 0; i < m_size; i++) {
            _vector->push_back(m_array[i]);
        }

        return _vector;
    }

    //-----------------------------------------------------------------------------

    /**
     * Creates a CArray object from a std::vector object
     */
    static CArray* fromStdVector(std::vector<T>* vec) {
        RE_ASSERT(vec);

        unsigned int vector_size = vec->size();
        T* data = new T[vector_size];

        for (unsigned int i = 0; i < vector_size; i++) {
            data[i] = vec->at(i);
        }

        CArray<T>* array_dest = new CArray<T>(data, vector_size);
        array_dest->autorelease();

        return array_dest;
    }

    //-----------------------------------------------------------------------------

    /**
     * Creates a CArray object from a btAlignedObjectArray object
     */
    static CArray* fromBtAlignedObjectArray(btAlignedObjectArray<T>* vec) {
        RE_ASSERT(vec);

        unsigned int vector_size = vec->size();
        T* data = new T[vector_size];

        for (unsigned int i = 0; i < vector_size; i++) {
            data[i] = vec->at(i);
        }

        CArray<T>* array_dest = new CArray<T>(data, vector_size);
        array_dest->autorelease();

        return array_dest;
    }

    //-----------------------------------------------------------------------------

    ~CArray() {
        SAFE_RELEASE_ARRAY(m_array);
    }

protected:
    unsigned int    m_size;
    T*              m_array;
};

#endif  // RIOENGINE_CENGINE_CARRAY_H_
