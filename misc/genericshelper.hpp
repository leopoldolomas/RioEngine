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

#ifndef RIOENGINE_MISC_GENERICSHELPER_H_
#define RIOENGINE_MISC_GENERICSHELPER_H_

//-----------------------------------------------------------------------------

#include "typedefs.h"
#include "cengine/cobject.h"

//-----------------------------------------------------------------------------

class GenericsHelper {
public:

    /**
     * Creates a deep copy of the given vector.
     * It assumes that the objects inside the original Vector are not static.
     */
    template<class T>
    static std::vector<T*>* getCopyFromVector(std::vector<T*>& original_vec) {
        std::vector<T*>* copy_vec = new std::vector<T*>();

        for (std::vector<T*>::iterator it = original_vec.begin();
             it != original_vec.end();
             ++it) {
            T* copy_obj = new T(*(*it));
            copy_vec->push_back(copy_obj);
        }
        return copy_vec;
    }

    //-----------------------------------------------------------------------------

    /**
     * Deletes every object inside the specified vector and the vector itself.
     * It assumes that the objects inside the original Vector are not static.
     */
    template <class T>
    static void deleteVector(std::vector<T*>* vec) {
        if (!vec) return;

        for (std::vector<T*>::iterator it = vec->begin();
             it != vec->end();
             ++it) {
            SAFE_RELEASE(*it);
        }

        SAFE_RELEASE(vec);
        vec = NULL;
    }

    //-----------------------------------------------------------------------------

    template <class T>
    static int getIndexOfObjectInVector(std::vector<T*>* vec, T* obj_to_find) {
        if (!vec || !obj_to_find) {
            return -1;
        }

        for (std::vector<T*>::iterator it = vec->begin();
             it != vec->end();
             ++it) {
            if (*it == obj_to_find) {
                return std::distance(vec->begin(), it);
            }
        }
        return -1;
    }

    //-----------------------------------------------------------------------------

    template <class T>
    static bool vectorContainsObject(std::vector<T>* vec, T object_to_find) {
        return getIndexOfObjectInVector(vec, object_to_find) > -1;
    }

    //-----------------------------------------------------------------------------

    /**
      * Removes an specific object from the specified vector
      * It assumes that the objects inside the original vector are not static.
      */
    template <class T>
    static bool deleteObjectFromVector(std::vector<T*>* vec, void* object_to_delete) {
        int index = -1;

        for (std::vector<T*>::iterator it = vec->begin();
             it != vec->end();
             ++it) {
            if (*it == object_to_delete) {
                index = std::distance(vec->begin(), it);
                break;
            }
        }

        if (index > -1) {
            void* ref = vec->at(index);
            vec->erase(vec->begin() + index);
            SAFE_RELEASE(ref);
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------------

    /**
      * Deletes every object inside the specified Map object.
      */
    template <class TKey, class TVal>
    static void deleteMap(std::map<TKey, TVal*>* map) {
        if (!map) {
            return;
        }

        std::map<TKey, TVal*>::iterator it = map->begin();

        for (; it != map->end(); it++) {
            SAFE_RELEASE(it->second);
        }

        SAFE_RELEASE(map);
    }

    //-----------------------------------------------------------------------------

    template <class T>
    static bool replaceObjectInVector(std::vector<T*>* vec, T* object_to_replace, T* replacement) {
        int index = getIndexOfObjectInVector(vec, object_to_replace);

        if (index < 0) {
            return false;
        }

        SAFE_RELEASE(object_to_replace);
        SAFE_RETAIN(replacement);
        vec->at(index) = replacement;

        return true;
    }
};

#endif // RIOENGINE_MISC_GENERICSHELPER_H_
