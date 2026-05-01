#include "pathfinding.h"

#include <queue>

#include "castle/i_castle_service.h"
#include <ranges>

using namespace TowerDefence;


PathfindingService::PathfindingService(const IConfigReader &config, IWorldMap& worldMap, Vector2<size_t> castlePosition) : worldMap_(worldMap), castlePosition_(castlePosition)
{
    static const std::unordered_map<std::string, MoveType> moveTypeMap = {
        {"landed", MoveType::LANDED},
        {"air", MoveType::AIR},
    };

    auto types = config.getItems("move_types");
    for (const auto& [typeName, cfg] : types) {
        addMoveType(moveTypeMap.at(typeName), *cfg);
    }
}

Vector2<int> PathfindingService::getPath(MoveType type, Vector2<size_t> point) const
{
    return pathMaps_.at(type).at(point.getX(), point.getY());
}

void PathfindingService::update()
{
    if (worldMap_.isUpdated()) {
        for (auto& [type, map] : pathMaps_) {
            map = calculatePathMap(type);
        }
        worldMap_.updateHandled();
    }
}

void PathfindingService::addMoveType(MoveType type, const IConfigReader& config)
{

    std::unordered_map<TileType, size_t> weights;
    for (const auto& [tileType, weight] : config.getSize_ts("weights")) {
        weights.emplace(tileTypeMap.at(tileType), weight);
    }
    weightsMap_.emplace(type, std::unordered_map(std::move(weights)));

    const auto size = worldMap_.size();
    pathMaps_.emplace(type, Matrix<Vector2<int>>(size.getX(), size.getY()));
}

Matrix<Vector2<int>> PathfindingService::calculatePathMap(MoveType type) const
{
    constexpr size_t ORTHOGONAL_COST = 10;
    constexpr size_t DIAGONAL_COST = 14;

    const auto size = worldMap_.size();
    const auto rows = size.getX();
    const auto cols = size.getY();
    constexpr size_t INF = std::numeric_limits<size_t>::max();

    auto& weights = weightsMap_.at(type);

    Matrix<size_t> dist(rows, cols, INF);
    Matrix<Vector2<int>> arrows(rows, cols, {0, 0});

    using Node = std::tuple<size_t, size_t, size_t>;
    std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;

    dist[castlePosition_.getX()][castlePosition_.getY()] = 0;
    pq.emplace(0, castlePosition_.getX(), castlePosition_.getY());

    static const std::vector<Vector2<int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    while (!pq.empty()) {
        auto [d, x, y] = pq.top();
        pq.pop();
        if (d > dist[x][y]) continue;

        for (auto dir : directions) {
            int nextX_int = static_cast<int>(x) + dir.getX();
            int nextY_int = static_cast<int>(y) + dir.getY();

            if (nextX_int < 0 || nextX_int >= static_cast<int>(rows) ||
                nextY_int < 0 || nextY_int >= static_cast<int>(cols)) { continue; }

            const auto nextX = static_cast<size_t>(nextX_int);
            const auto nextY = static_cast<size_t>(nextY_int);
            size_t w = weights.at(worldMap_.getTile(nextX, nextY));
            if (w == INF) continue;

            size_t cost = (abs(dir.getX()) + abs(dir.getY()) == 2) ? DIAGONAL_COST : ORTHOGONAL_COST;
            size_t nextD = d + cost + w;

            if (nextD < dist[nextX][nextY]) {
                dist[nextX][nextY] = nextD;
                arrows[nextX][nextY] = {-dir.getX(), -dir.getY()};
                pq.emplace(nextD, nextX, nextY);
            }
        }
    }
    /*
    int x = 0;
    std::cout << std::endl;
    for (auto& vec: arrows) {
        std::cout << vec.toString() <<" ";
        x = (x + 1) % cols;
        if (x == 0) std::cout << std::endl;
    }
*/
    return arrows;
}
