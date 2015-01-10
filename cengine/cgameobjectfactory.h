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

#ifndef RIOENGINE_CENGINE_CGAMEOBJECTFACTORY_H_
#define RIOENGINE_CENGINE_CGAMEOBJECTFACTORY_H_

#include <QStringList>
#include "cengine/cobject.h"

class CGameObject;

/**
 * The user is able to select a custom type for any game object while creating
 * the game scene in the editor. So, any Type that inherits from CGameObject
 * can be attached to a CGameObject at runtime. CGameObjectFactory is used to
 * register these special types and also to instantiate game objects.
 */
class CGameObjectFactory {
public:
    /**
    * Registers the specified Type. Note: It must inherit from CGameObject!
    */
    template <class T>
    static bool addType() {
        // check whether the specified Type inherits from CGameObject
        T* newtypeobj_dummy = new T;
        CGameObject* game_obj = qobject_cast<CGameObject*>(newtypeobj_dummy);

        if(game_obj == NULL) { // then it does not inherit from CGameObject
            RE_ASSERT(false);
            return NULL;
        }

        const char* class_name = game_obj->metaObject()->className();
        QString class_name_as_qstring = QString::fromUtf8(class_name);

        if(m_types.contains(class_name_as_qstring)) {
            return false; // type already registered
        }

        // register the type in the Qt MetaType system
        qRegisterMetaType<T>(class_name);

        // add the Type (T) to the list of... (yep, you guessed right) supported types
        m_types.append(class_name_as_qstring);

        SAFE_RELEASE(newtypeobj_dummy);

        return true;
    }

    /**
     * Returns the current supported type list
     */
    static const QStringList& types();

    /**
     * Instantiates a game object with the specified type. Note: The type must
     * have been previously registered by the addType<T>() method
     */
    static CGameObject*  createInstance(const char* class_name);

    /**
     * Returns the type name associated to the specified type_id
     */
    static const QString& getTypeNameHavingId(int type_id);

    /**
     * Returns the type id associated to the class_name
     */
    static int           getObjectTypeId(const char* class_name);

private:
    static QStringList m_types;
};

#endif  // RIOENGINE_CENGINE_CGAMEOBJECTFACTORY_H_
