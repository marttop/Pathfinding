#include "Grid.hpp"


Grid::Grid(Vector2 pos, Rectangle rect) : IEntity(pos, rect, EntityType::Grid)
{
    unsigned int NUM_TILES_X = SCREEN_WIDTH / (10 * TILE_SCALE);
    unsigned int NUM_TILES_Y = SCREEN_HEIGHT / (10 * TILE_SCALE);
    // Calculate tile size
    float tileWidth = static_cast<float>(SCREEN_WIDTH) / NUM_TILES_X;
    float tileHeight = static_cast<float>(SCREEN_HEIGHT) / NUM_TILES_Y;

    _grid.resize(NUM_TILES_Y);
    for (unsigned int i = 0; i < NUM_TILES_Y; i++) {
        _grid[i].resize(NUM_TILES_X);
        for (unsigned int j = 0; j < NUM_TILES_X; j++) {
            Vector2 tilePos = Vector2{static_cast<float>(j) * tileWidth, static_cast<float>(i) * tileHeight};
            _grid[i][j] = std::make_shared<Tile>(tilePos, Rectangle{tilePos.x, tilePos.y, tileWidth, tileHeight});
        }
    }
}

Grid::~Grid()
{
}

// -----------GRID-----------
void Grid::update(float deltaTime, std::vector<std::shared_ptr<IEntity>> &m_entities)
{
    _mousePosition = GetMousePosition();
    for (auto lines : _grid) {
        for (auto tile : lines) {
            tile->update(deltaTime, m_entities);

            //Override tile update here with events if needed
            bool isCollision = CheckCollisionPointRec(_mousePosition, tile->getRect());
            if (isCollision) {
                tile->isSelected = true;
                _selectedTile = tile.get();
            }
        }
    }

    events();

    if (_isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        _window.x = _mousePosition.x - _dragOffset.x;
        _window.y = _mousePosition.y - _dragOffset.y;
    }

    if (_isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        _isDragging = false;
    }
    _selectedTile = nullptr;
}

void Grid::draw() const
{
    for (auto lines : _grid) {
        for (auto tile : lines) {
            tile->draw();
        }
    }
}

void Grid::drawGUI()
{
    //Draw GUI in front of the rest
    if (GuiWindowBox(_window, "Information")) {
        // Close button pressed
    }
    int i = (int)_mousePosition.y / (10 * TILE_SCALE);
    int j = (int)_mousePosition.x / (10 * TILE_SCALE);
    GuiLabel((Rectangle){_window.x + 10, _window.y + 30, _window.width - 20, 20}, TextFormat("Mouse Position : %d, %d", (int)_mousePosition.x, (int)_mousePosition.y));
    GuiLabel((Rectangle){_window.x + 10, _window.y + 50, _window.width - 20, 20}, TextFormat("Current index  : %d, %d", i, j));
    if (i >= 0 && i < _grid.size() && j >= 0 && j < _grid[i].size()) {
        GuiLabel((Rectangle){_window.x + 10, _window.y + 70, _window.width - 20, 20}, TextFormat("Tile type : %s", _grid[i][j] ? _grid[i][j]->getTypeString() : "NaN"));
    }
    GuiLabel((Rectangle){_window.x + 10, _window.y + 90, _window.width - 20, 20}, "Type to put");

    bool wasWallChecked = _wallCheck;
    bool wasStartChecked = _startCheck;
    bool wasTargetChecked = _targetCheck;

    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 110, 20, 20}, "Wall", &_wallCheck);
    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 130, 20, 20}, "Start", &_startCheck);
    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 150, 20, 20}, "Target", &_targetCheck);

    if (GuiButton((Rectangle){_window.x + 10, _window.y + 180, _window.width - 20, 30}, "Clear")) _isClearClicked = true;

    if (_wallCheck != wasWallChecked) {
        if (_wallCheck) {
            _startCheck = false;
            _targetCheck = false;
            _typeToPut = TileType::Wall;
        } else {
            _wallCheck = true;
        }
    } else if (_startCheck != wasStartChecked) {
        if (_startCheck) {
            _wallCheck = false;
            _targetCheck = false;
            _typeToPut = TileType::Start;
        } else {
            _startCheck = true;
        }
    } else if (_targetCheck != wasTargetChecked) {
        if (_targetCheck) {
            _wallCheck = false;
            _startCheck = false;
            _typeToPut = TileType::Target;
        } else {
            _targetCheck = true;
        }
    }
}



void Grid::events()
{
    //GUI events
    if (_isClearClicked) {
        resetGrid();
        return;
    }

    Rectangle titleBar = {_window.x, _window.y, _window.width, 20};
    if (CheckCollisionPointRec(_mousePosition, titleBar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _isDragging = true;
        _dragOffset = (Vector2){_mousePosition.x - _window.x, _mousePosition.y - _window.y};
        return;
    }

    if (CheckCollisionPointRec(_mousePosition, _window)) {return;};

    if (!_isDragging) {
        if (IsMouseButtonDown(0)) {
            if (_selectedTile != nullptr) {
                if (_selectedTile->getType() == TileType::Empty) {
                    if (_typeToPut == TileType::Start && _startTile != nullptr) return;
                    else if (_typeToPut == TileType::Start && _startTile == nullptr) _startTile = _selectedTile;
                    if (_typeToPut == TileType::Target && _targetTile != nullptr) return;
                    else if (_typeToPut == TileType::Target && _targetTile == nullptr) _targetTile = _selectedTile;
                    _selectedTile->setType(_typeToPut);
                }
            }
        }
        else if (IsMouseButtonDown(1)) {
            if (_selectedTile != nullptr) {
                if (_selectedTile->getType() != TileType::Empty) {
                    if (_selectedTile->getType() == TileType::Target) {
                        _targetTile = nullptr;
                    }
                    else if (_selectedTile->getType() == TileType::Start) {
                        _startTile = nullptr;
                    }
                    _selectedTile->setType(TileType::Empty);
                }
            }
        }
    }
}

void Grid::resetGrid()
{
    _isClearClicked = false;
    for (auto lines : _grid) {
        for (auto tile : lines) {
            tile->setType(TileType::Empty);
            _selectedTile = nullptr;
            _startTile = nullptr;
            _targetTile = nullptr;
        }
    }
}

// -----------TILE-----------
void Tile::update(float deltaTime, std::vector<std::shared_ptr<IEntity>> &m_entities)
{
    isSelected = false;
}

void Tile::draw() const
{
    DrawRectangleRec(_rect, _colors[_type]);
    if (isSelected) {
        DrawRectangleLines(_rect.x, _rect.y,_rect.width, _rect.height, _colors[TileType::Selected]);
    } else {
        DrawRectangleLines(_rect.x, _rect.y,_rect.width, _rect.height, _borderColor);
    }
}
