/*
** raylib_base
** File description:
** AStar
*/
#include "Grid.hpp"

void AStar::reconstructPath()
{
    auto current = _endTile;
    if (_endTile && _endTile != _startTile) {
        _endTile->setTypeStyle(TileTypeStyle::Path);
        _endTile = _endTile->parent;
    } else {
        _isPathReconstructing = false;
        _isCompleted = true;
    }
}


void AStar::makeStep() {
    if (_isPathReconstructing) {
        reconstructPath();
        return;
    }

    if (_open.empty()) {
        // No path found
        _isCompleted = true;
        INFO("{0} -> No path found", getAlgoTypeString());
        return;
    }

    _totalOperations++;

    auto current = _open.top();
    _open.pop();

    // If the current node is already in the closed set, ignore it and return.
    // This prevents processing an outdated instance.
    if (current->setType == AStarSet::CLOSED) {
        return;
    }

    if (current == _endTile) {
        // Path found
        for (const auto& line : *_grid) {
            for (const auto& tile : line) {
                if (tile->getTypeStyle() == TileTypeStyle::Searching ||
                    tile->getTypeStyle() == TileTypeStyle::Searched ||
                    tile->getTypeStyle() == TileTypeStyle::Path) {
                    tile->setTypeStyle(TileTypeStyle::Empty);
                }
            }
        }
        _isPathReconstructing = true;
        if (_endTile->parent) _endTile = _endTile->parent;
        return;
    }

    current->setType = AStarSet::CLOSED;
    _closed.insert(current);
    if (current != _endTile && current != _startTile) {
        current->setTypeStyle(TileTypeStyle::Searched);
    }
    _totalOperations++;

    for (auto& neighbor : getNeighbors(current)) {
        _totalOperations++;

        if (neighbor->setType == AStarSet::CLOSED || neighbor->realType == TileType::OBSTRUCTION) {
            continue;
        }

        int tentative_gCost = current->gCost + manhattan(current, neighbor);

        if (neighbor->gCost == -1 || tentative_gCost < neighbor->gCost) {
            neighbor->parent = current;
            neighbor->gCost = tentative_gCost;

            if (neighbor->setType != AStarSet::OPEN) {
                neighbor->setType = AStarSet::OPEN;
                if (neighbor != _endTile && neighbor != _startTile) {
                    neighbor->setTypeStyle(TileTypeStyle::Searching);
                }
                _open.push(neighbor);
            }
        }
    }
}



std::vector<std::shared_ptr<Tile>> AStar::getNeighbors(const std::shared_ptr<Tile>& tile)
{
    std::vector<std::shared_ptr<Tile>> neighbors;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    for (const auto& dir : directions) {
        int newI = tile->posI + dir.first;
        int newJ = tile->posJ + dir.second;

        if (newI >= 0 && newI < _grid->size() && newJ >= 0 && newJ < (*_grid)[0].size()) {
            auto neighbor = (*_grid)[newI][newJ];
            if (neighbor->realType != TileType::OBSTRUCTION) {
                neighbors.push_back(neighbor);
            }
        }
    }

    return neighbors;
}

