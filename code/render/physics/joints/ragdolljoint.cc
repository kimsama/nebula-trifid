//------------------------------------------------------------------------------
//  ragdolljoint.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file, LTU Skelleftea
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/joints/ragdolljoint.h"

namespace Physics
{
#if (__USE_BULLET__)
	__ImplementClass(Physics::RagdollJoint, 'RGJT', Physics::BaseRagdollJoint);
#elif(__USE_PHYSX__)
	__ImplementClass(Physics::RagdollJoint, 'RGJT', Physics::BaseRagdollJoint);
#elif(__USE_HAVOK__)
	__ImplementClass(Physics::RagdollJoint, 'RGJT', Havok::HavokRagdollJoint);
#else
#error "Physics::RagdollJoint not implemented"
#endif
}

