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

#include "cengine/cgameobjectfactory.h"
#include "shootingrange/cplayer.h"
#include "shootingrange/cenemylauncher.h"
#include "shootingrange/cenemytarget.h"
#include "shootingrange/cbullet.h"
#include "editor/qpropertyeditor/customtypes.h"
#include "editor/qpropertyeditor/vec3fproperty.h"
#include "editor/qpropertyeditor/property.h"

namespace CustomTypes
{
	void registerTypes()
	{
		static bool registered = false;
		if (!registered)
		{
            qRegisterMetaType<btQuaternion>("btQuaternion");
            qRegisterMetaType<btVector3>("btVector3");
            qRegisterMetaType<CObject>("CObject");           
            qRegisterMetaType<QList<CGameObject>>("QList<CGameObject>");
            qRegisterMetaType<QList<CLInstanceGeometry>>("QList<CLInstanceGeometry>");
            qRegisterMetaType<QList<CLInstanceMaterial>>("QList<CLInstanceMaterial>");
            qRegisterMetaType<QList<QProperty>>("QList<QProperty>");

            // -------- The following types must be available from CGameObjectFactory
            CGameObjectFactory::addType<CGameObject>();
            CGameObjectFactory::addType<CAliveEntity>();
            CGameObjectFactory::addType<CEnemy>();
            CGameObjectFactory::addType<CPlayer>();
            CGameObjectFactory::addType<CEnemyLauncher>();
            CGameObjectFactory::addType<CEnemyTarget>();
            CGameObjectFactory::addType<CBullet>();
            // -----------------------------------------------------------------------

			registered = true;
		}
	}

	Property* createCustomProperty(const QString& name, QObject* propertyObject, Property* parent)
	{
		int userType = propertyObject->property(qPrintable(name)).userType();
        if (userType == QMetaType::type("btVector3")) {
			return new Vec3fProperty(name, propertyObject, parent);
        }
        else {
			return new Property(name, propertyObject, parent);
        }
	}
}