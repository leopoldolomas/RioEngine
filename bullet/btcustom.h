#ifndef RIOENGINE_BULLET_BTCUSTOM_H_
#define RIOENGINE_BULLET_BTCUSTOM_H_

#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/matrix/matrices.h"

namespace btCustom {

btVector3 getAxisX(btMatrix3x3 &m);
btVector3 getAxisY(btMatrix3x3 &m);
btVector3 getAxisZ(btMatrix3x3 &m);

btVector3 getAxisX(const btQuaternion &q);
btVector3 getAxisY(const btQuaternion &q);
btVector3 getAxisZ(const btQuaternion &q);

btVector3 zero_vector();
btVector3 right();
btVector3 right(const btQuaternion& q);
btVector3 up();
btVector3 up(const btQuaternion& q);
btVector3 forward();
btVector3 forward(const btQuaternion& q);

Matrix4 btQuaternionToMatrix4(const btQuaternion& q);

void print(const btVector3& vec);
void print(const btVector3* vec);
void print(const btMatrix3x3& matrix);
void print(const btQuaternion& quat);

}

#endif // RIOENGINE_BULLET_BTCUSTOM_H_
