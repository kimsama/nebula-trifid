//------------------------------------------------------------------------------
//  physicsbody.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/character.h"

namespace Physics
{
#if (__USE_BULLET__)
	__ImplementClass(Physics::Character, 'PHCH', Bullet::BulletCharacter);
#elif (__USE_HAVOK__)	
	__ImplementClass(Physics::Character, 'PHCH', Havok::HavokCharacterRigidBody);
#else
#error "Physics::Character not implemented"
#endif
}