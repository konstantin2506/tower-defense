#include "grid_service.h"

#include <ranges>
#include <algorithm>
#include <mutex>

using namespace TowerDefence;

bool VecIDGridService::add(ID id, Vector2<size_t> point)
{
    data_.at(point.getX(), point.getY()).push_back(id);
    return true;
}



std::vector<ID> VecIDGridService::atPoint(Vector2<size_t> point) const
{
    return data_.at(point.getX(), point.getY());
}

bool VecIDGridService::replace(ID id, Vector2<size_t> src, Vector2<size_t> dst)
{
#ifdef PARALLEL
    if (src.getX() == dst.getX() && src.getY() == dst.getY()) {
        return true;
    }

    auto& mutex1 = cellMutexes_.at(src.getX(), src.getY());
    auto& mutex2 = cellMutexes_.at(dst.getX(), dst.getY());

    std::lock(mutex1, mutex2);

    std::lock_guard lock1(mutex1, std::adopt_lock);
    std::lock_guard lock2(mutex2, std::adopt_lock);
#endif

    auto it = std::ranges::find(data_.at(src.getX(), src.getY()), id);
    if (it == data_.at(src.getX(), src.getY()).end())
        throw std::invalid_argument("VecIDGridService::replace(): incorrect id");

    data_.at(src.getX(), src.getY()).erase(it);
    data_.at(dst.getX(), dst.getY()).push_back(id);
    return true;
}


std::vector<ID> VecIDGridService::getInRadius(Vector2<size_t> point, size_t radius) const
{
    auto vectors = TowerDefence::getInRadius(data_, point, radius);
    auto joinedVectors = vectors
                         | std::views::join
                         | std::views::filter([](ID id){return id != 0;});
    std::vector result(joinedVectors.begin(), joinedVectors.end());

    return result;
}



