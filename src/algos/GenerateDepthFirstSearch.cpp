/*
** raylib_base
** File description:
** GenerateDepthFirstSearch
*/

#include "Grid.hpp"

void GenerateDepthFirstSearch::makeStep()
{
    if (!_currentTile) {
        ERROR("GenerateDepthFirstSearch::makeStep() : no _currentTile set");
        return;
    }
    std::shared_ptr<Tile> nextNeighbor = getRandomNeighbor();
    if (nextNeighbor) {
        nextNeighbor->setTypeStyle(TileTypeStyle::Empty);
        nextNeighbor->realType = TileType::FREE;
        convertToPathBetween(_currentTile, nextNeighbor);
        _myStack.push(_currentTile);
        _currentTile = nextNeighbor;
        _currentTile->isVisited = true;
    } else {
        if (!_myStack.empty()) {
            _currentTile = _myStack.top();
            _myStack.pop();
        } else {
            _currentTile = nullptr;
            _isCompleted = true;
            return;
        }
    }
}

void GenerateDepthFirstSearch::convertToPathBetween(const std::shared_ptr<Tile>& current, const std::shared_ptr<Tile>& next)
{
    std::shared_ptr<Tile> wallTile;

    if (next->posJ > current->posJ) {
        // Wall is to the right of current
        wallTile = (*_grid)[current->posI][current->posJ + 1];
    } else if (next->posJ < current->posJ) {
        // Wall is to the left of current
        wallTile = (*_grid)[current->posI][current->posJ - 1];
    } else if (next->posI > current->posI) {
        // Wall is below current
        wallTile = (*_grid)[current->posI + 1][current->posJ];
    } else if (next->posI < current->posI) {
        // Wall is above current
        wallTile = (*_grid)[current->posI - 1][current->posJ];
    }

    if (wallTile) {
        wallTile->setTypeStyle(TileTypeStyle::Empty);
        wallTile->realType = TileType::FREE;
    }
}

std::shared_ptr<Tile> GenerateDepthFirstSearch::getRandomNeighbor() const
{
    std::vector<grid_pos_t> unvisitedNeighbors;

    std::vector<grid_pos_t> possibleNeighbors = {
        {_currentTile->posI - 2, _currentTile->posJ}, // Up
        {_currentTile->posI + 2, _currentTile->posJ}, // Down
        {_currentTile->posI, _currentTile->posJ - 2}, // Left
        {_currentTile->posI, _currentTile->posJ + 2}  // Right
    };

    for (const auto& pos : possibleNeighbors) {
        if (pos.first >= 1 && pos.first < _grid->size() - 1 &&
            pos.second >= 1 && pos.second < (*_grid)[0].size() - 1 &&
            !(*_grid)[pos.first][pos.second]->isVisited) {
            unvisitedNeighbors.push_back(pos);
        }
    }

    if (!unvisitedNeighbors.empty()) {
        int randomIndex = GetRandomValue(0, static_cast<int>(unvisitedNeighbors.size()) - 1);
        grid_pos_t selectedPos = unvisitedNeighbors[randomIndex];
        return (*_grid)[selectedPos.first][selectedPos.second];
    }

    return nullptr;
}
