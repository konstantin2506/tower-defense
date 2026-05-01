#include "grid_service.h"

using namespace TowerDefence;

bool SingleIDGridService::add(ID id, Vector2<size_t> point)
{
    auto& elem = data_.at(point.getX(), point.getY());
    if (elem != 0) {
        return false;
    }
    elem = id;
    return true;
}

std::vector<ID> SingleIDGridService::atPoint(Vector2<size_t> point) const
{
    return std::vector(1, data_.at(point.getX(), point.getY()));
}

bool SingleIDGridService::replace(ID id, Vector2<size_t> src, Vector2<size_t> dst)
{
    if (data_.at(dst.getX(), dst.getY()) != 0) {
        return false;
    }
    data_.at(src.getX(), src.getY()) = 0;
    data_.at(dst.getX(), dst.getY()) = id;
    return true;
}



std::vector<ID> SingleIDGridService::getInRadius(Vector2<size_t> point, size_t radius) const
{

  auto res = TowerDefence::getInRadius(data_, point, radius);
  return res;
}
