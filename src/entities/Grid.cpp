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
    //Clock updates
    static float stepClock = 0.0f;

    //Mouse update
    _mousePosition = GetMousePosition();
    bool isMouseOnUI = CheckCollisionPointRec(_mousePosition, _window);

    //Grid update
    for (const auto &lines : _grid) {
        for (const auto &tile : lines) {
            tile->update(deltaTime, m_entities);
            if (isMouseOnUI) continue;
            //Override tile update here with events if needed
            bool isCollision = CheckCollisionPointRec(_mousePosition, tile->getRect());
            if (isCollision) {
                tile->isSelected = true;
                _selectedTile = tile.get();
            }
        }
    }

    //Algorithm update
    if (_algo) {
        stepClock += deltaTime;
        if (stepClock >= 0.1f) {
            stepClock = 0.0f;
            WARNING("0.1Sec");
            _algo->makeStep();
        }
    }

    events();

    //GUI dragging update
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
    for (const auto &lines : _grid) {
        for (const auto &tile : lines) {
            tile->draw();
        }
    }
}

void Grid::drawGUI()
{
    //Draw GUI in front of the rest
    if (GuiWindowBox(_window, "Admin panel")) {
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
    GuiLabel((Rectangle){_window.x + 130, _window.y + 90, _window.width - 20, 20}, "Algorithms");

    bool wasWallChecked = _wallCheck;
    bool wasStartChecked = _startCheck;
    bool wasTargetChecked = _targetCheck;
    bool wasAStarChecked = _aStarCheck;
    bool wasDijkstraChecked = _dijkstraCheck;
    bool wasBestFitChecked = _bestFitCheck;

    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 110, 20, 20}, "Wall", &_wallCheck);
    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 135, 20, 20}, "Start", &_startCheck);
    GuiCheckBox((Rectangle){_window.x + 10, _window.y + 160, 20, 20}, "Target", &_targetCheck);
    GuiCheckBox((Rectangle){_window.x + 130, _window.y + 110, 20, 20}, "A-Star", &_aStarCheck);
    GuiCheckBox((Rectangle){_window.x + 130, _window.y + 135, 20, 20}, "Dijkstra", &_dijkstraCheck);
    GuiCheckBox((Rectangle){_window.x + 130, _window.y + 160, 20, 20}, "Best fit", &_bestFitCheck);

    if (GuiButton((Rectangle){_window.x + 10, _window.y + 185, _window.width - 20, 30}, "Clear")) _isClearClicked = true;
    if (GuiButton((Rectangle){_window.x + 10, _window.y + 220, _window.width - 20, 30}, "Start")) _isStartClicked = true;
    if (GuiButton((Rectangle){_window.x + 10, _window.y + 255, _window.width - 20, 30}, "Generate Maze")) _isGenerateClicked = true;

    if (_wallCheck != wasWallChecked) {
        if (_wallCheck) {
            _startCheck = false;
            _targetCheck = false;
            _typeToPut = TileTypeStyle::Wall;
        } else {
            _wallCheck = true;
        }
    } else if (_startCheck != wasStartChecked) {
        if (_startCheck) {
            _wallCheck = false;
            _targetCheck = false;
            _typeToPut = TileTypeStyle::Start;
        } else {
            _startCheck = true;
        }
    } else if (_targetCheck != wasTargetChecked) {
        if (_targetCheck) {
            _wallCheck = false;
            _startCheck = false;
            _typeToPut = TileTypeStyle::Target;
        } else {
            _targetCheck = true;
        }
    }

    if (_aStarCheck != wasAStarChecked) {
        if (_aStarCheck) {
            _dijkstraCheck = false;
            _bestFitCheck = false;
            _algoType = AlgoType::A_STAR;
        } else {
            _aStarCheck = true;
        }
    } else if (_dijkstraCheck != wasDijkstraChecked) {
        if (_dijkstraCheck) {
            _aStarCheck = false;
            _bestFitCheck = false;
            _algoType = AlgoType::DIJKSTRA;
        } else {
            _dijkstraCheck = true;
        }
    } else if (_bestFitCheck != wasBestFitChecked) {
        if (_bestFitCheck) {
            _aStarCheck = false;
            _dijkstraCheck = false;
            _algoType = AlgoType::BEST_FIT_SEARCH;
        } else {
            _bestFitCheck = true;
        }
    }
}



void Grid::events()
{
    //GUI events
    if (_isClearClicked) {
        INFO("Pressed - Clear");
        resetGrid();
        return;
    }
    if (_isStartClicked) {
        //Pathfinding
        INFO("Pressed - Start Solving");
        _isStartClicked = false;
        return;
    }
    if (_isGenerateClicked) {
        //Maze Genration
        INFO("Pressed - Generate Maze");
        _algo = nullptr;
        _algo = std::make_shared<GenerateDepthFirstSearch>();
        _algo->init(&_grid);
        _isGenerateClicked = false;
        return;
    }

    Rectangle titleBar = {_window.x, _window.y, _window.width, 20};
    if (CheckCollisionPointRec(_mousePosition, titleBar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _isDragging = true;
        _dragOffset = (Vector2){_mousePosition.x - _window.x, _mousePosition.y - _window.y};
        return;
    }

    if (CheckCollisionPointRec(_mousePosition, _window)) {return;};

    //Need to be the last to check
    if (!_isDragging) {
        if (IsMouseButtonDown(0)) {
            if (_selectedTile != nullptr) {
                if (_selectedTile->getTypeStyle() == TileTypeStyle::Empty) {
                    if (_typeToPut == TileTypeStyle::Start && _startTile != nullptr) return;
                    else if (_typeToPut == TileTypeStyle::Target && _targetTile != nullptr) return;
                    else if (_typeToPut == TileTypeStyle::Start && _startTile == nullptr) {
                        _startTile = _selectedTile;
                        _selectedTile->_realType = TileType::HOME;
                    }
                    else if (_typeToPut == TileTypeStyle::Target && _targetTile == nullptr) {
                        _targetTile = _selectedTile;
                        _selectedTile->_realType = TileType::GOAL;
                    }
                    _selectedTile->setTypeStyle(_typeToPut);
                    if (_typeToPut == TileTypeStyle::Wall) {
                        _selectedTile->_realType = OBSTRUCTION;
                    }
                }
            }
        }
        else if (IsMouseButtonDown(1)) {
            if (_selectedTile != nullptr) {
                if (_selectedTile->getTypeStyle() != TileTypeStyle::Empty) {
                    if (_selectedTile->getTypeStyle() == TileTypeStyle::Target) {
                        _targetTile = nullptr;
                    }
                    else if (_selectedTile->getTypeStyle() == TileTypeStyle::Start) {
                        _startTile = nullptr;
                    }
                    _selectedTile->setTypeStyle(TileTypeStyle::Empty);
                    _selectedTile->_realType = TileType::FREE;
                }
            }
        }
    }
}

void Grid::resetGrid()
{
    _isClearClicked = false;
    for (const auto &lines : _grid) {
        for (const auto &tile : lines) {
            tile->setTypeStyle(TileTypeStyle::Empty);
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
    DrawRectangleRec(_rect, _colors[_typeStyle]);
    if (isSelected) {
        DrawRectangleLines(_rect.x, _rect.y,_rect.width, _rect.height, _colors[TileTypeStyle::Selected]);
    } else {
        DrawRectangleLines(_rect.x, _rect.y,_rect.width, _rect.height, _borderColor);
    }
}
