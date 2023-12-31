/*
** raylib_base
** File description:
** DepthFirstSearch
*/

#include "Grid.hpp"

void DepthFirstSearch::makeStep()
{
    if (!_currentTile) {
        ERROR("DepthFirstSearch::makeStep() : no _currentTile set");
        return;
    }
    std::shared_ptr<Tile> nextNeighbor = getRandomNeighbor();
    if (nextNeighbor) {
        nextNeighbor->setTypeStyle(TileTypeStyle::Searching);
        nextNeighbor->realType = TileType::FREE;
        convertToPathBetween(_currentTile, nextNeighbor);
        _myStack.push(_currentTile);
        _currentTile = nextNeighbor;
        _currentTile->isVisited = true;
        _totalOperations++;
    } else {
        if (!_myStack.empty()) {
            _currentTile->setTypeStyle(TileTypeStyle::Empty);
            _currentTile = _myStack.top();
            _currentTile->setTypeStyle(TileTypeStyle::Empty);
            if (_currentTile->inBtwWall) {
                _currentTile->inBtwWall->setTypeStyle(TileTypeStyle::Empty);
                _currentTile->inBtwWall = nullptr;
            }
            _myStack.pop();
            _totalOperations++;
        } else {
            _currentTile = nullptr;
            _isCompleted = true;
            return;
        }
    }
}

void DepthFirstSearch::convertToPathBetween(const std::shared_ptr<Tile>& current, const std::shared_ptr<Tile>& next)
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
        _currentTile->inBtwWall = wallTile;
        wallTile->setTypeStyle(TileTypeStyle::Searching);
        wallTile->realType = TileType::FREE;
    }
}

std::shared_ptr<Tile> &DepthFirstSearch::getRandomNeighbor()
{
    std::vector<grid_pos_t> unvisitedNeighbors;
    static std::shared_ptr<Tile> nullTile = nullptr;

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
        _totalOperations++;
        return (*_grid)[selectedPos.first][selectedPos.second];
    }

    return nullTile;
}
