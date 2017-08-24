#include "UITransform.h"

UITransform::UITransform(const glm::vec2& screenPosition, const glm::vec2& size, float rotation) : _screenPosition(screenPosition), _size(size), _rotation(rotation)
{}

UITransform::UITransform(const UITransform& other) : _screenPosition(other._screenPosition), _size(other._size), _rotation(other._rotation)
{}
