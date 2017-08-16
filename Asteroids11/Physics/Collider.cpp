#include "Collider.h"

Collider::Collider(Actor* owner, bool isTrigger) : _physxShape(nullptr), _owner(owner), _bIsTrigger(isTrigger)
{

}

Collider::~Collider()
{

}
