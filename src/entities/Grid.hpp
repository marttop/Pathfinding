/*
** raylib_base
** File description:
** Grid
*/

#ifndef GRID_HPP_
#define GRID_HPP_

#include "IEntity.hpp"
#include "IController.hpp"

enum AStarSet {
    NONE,
    OPEN,
    CLOSED
};

enum TileTypeStyle {
    Empty,
    Selected,
    Wall,
    Target,
    Start,
    Searching,
    Searched,
    Path,
};

enum TileType {
    FREE,
    PATH,
    OBSTRUCTION,
    SEARCH,
    HOME,
    GOAL
};

enum AlgoType {
    UNKNOWN,
    A_STAR,
    DIJKSTRA,
    BEST_FIT_SEARCH,
    DEPTH_FIRST_SEARCH
};


class Tile : public IEntity {
public:
    Tile(Vector2 pos, Rectangle rect, TileTypeStyle typeStyle = TileTypeStyle::Empty) : IEntity(pos, rect, EntityType::Tile) {
        _typeStyle = typeStyle;
    }
    ~Tile() = default;

    int posI = 0;
    int posJ = 0;

    inline const char* getTypeString() {
        switch (_typeStyle) {
            case Empty: return "Empty";
            case Selected: return "Selected";
            case Wall: return "Wall";
            case Target: return "Target";
            case Start: return "Start";
            case Searching: return "Searching";
            case Searched: return "Searched";
            default: return "Unknown";
        }
    }

    bool isSelected = false;
    inline Color getColor() const {return _colors[_typeStyle];}
    inline void setTypeStyle(const TileTypeStyle &typeStyle) {_typeStyle = typeStyle;}
    inline TileTypeStyle getTypeStyle() const {return _typeStyle;}

    void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) override;
    void draw() const override;

    int f();
    inline void init () {
        isVisited = false;
        inBtwWall = nullptr;
        parent = nullptr;
        setType = AStarSet::NONE;
        gCost = 0;
        hCost = 0;
        if (getTypeStyle() == TileTypeStyle::Searching ||
            getTypeStyle() == TileTypeStyle::Searched ||
            getTypeStyle() == TileTypeStyle::Path) {
            setTypeStyle(TileTypeStyle::Empty);
        }
    }


    //Used only by Algos
    TileType realType = TileType::FREE;

    //Used only by DepthFirstSearch
    bool isVisited = false;
    std::shared_ptr<Tile> inBtwWall = nullptr;

    //AStar only
    AStarSet setType = AStarSet::NONE;
    std::shared_ptr<Tile> parent = nullptr;
    int gCost = 0;
    int hCost = 0;
protected:
    TileTypeStyle _typeStyle;
    Color _borderColor = BLACK;
    std::vector<Color> _colors = { LIGHTGRAY, YELLOW, DARKGRAY, MAGENTA, BLUE, GREEN, RED, PINK };
};

typedef std::vector<std::vector<std::shared_ptr<Tile>>> grid_t;
typedef std::pair<int, int> grid_pos_t;

class IAlgo {
public:

    virtual void init(grid_t *grid) = 0;
    virtual void makeStep() = 0;
    virtual bool isCompleted() const {return _isCompleted;}
    virtual unsigned int getTotalOperations() const {return _totalOperations;}
    inline virtual const char* getAlgoTypeString() {
        switch (_algoType) {
            case A_STAR: return "A*";
            case DIJKSTRA: return "Dijkstra";
            case BEST_FIT_SEARCH: return "Best Fit Search";
            case DEPTH_FIRST_SEARCH: return "Depth First Search";
            default: return "Unknown";
        }
    }
    inline static const char* getAlgoTypeString(AlgoType type) {
        switch (type) {
            case A_STAR: return "A*";
            case DIJKSTRA: return "Dijkstra";
            case BEST_FIT_SEARCH: return "Best Fit Search";
            case DEPTH_FIRST_SEARCH: return "Depth First Search";
            default: return "Unknown";
        }
    }
protected:
    AlgoType _algoType = AlgoType::UNKNOWN;
    grid_t *_grid = nullptr;
    bool _isCompleted = false;
    unsigned int _totalOperations = 0;
private:
};


class Grid : public IEntity {
public:
    Grid(Vector2 pos, Rectangle rect);
    ~Grid();

    void events();
    void resetGrid(bool isWall);
    void cleanGrid();
    void initAlgo(AlgoType type);
    void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) override;
    void draw() const override;
    void drawGUI() override;

protected:
    Vector2 _mousePosition = GetMousePosition();
//  ---- GUI ----
    Rectangle _window = { SCREEN_WIDTH - 210, 10, 200, 430 };
    bool _isDragging = false;
    Vector2 _dragOffset = { 0 };
    bool _wallCheck = true; //Checkbox for _typeToPut
    bool _startCheck = false; //Checkbox for _typeToPut
    bool _targetCheck = false; //Checkbox for _typeToPut
    bool _aStarCheck = true; //Checkbox for _algoType
    bool _dijkstraCheck = false; //Checkbox for _algoType
    bool _bestFitCheck = false; //Checkbox for _algoType

    //Buttons
    bool _isClearClicked = false;
    bool _isCleanClicked = false;
    bool _isStartClicked = false;
    bool _isStopClicked = false;
    bool _isGenerateClicked = false;
    TileTypeStyle _typeToPut = Wall;
    AlgoType _algoType = A_STAR;
private:
    unsigned int _steps = 0;
    unsigned int _totalOperations = 0;
    float _algoTime = 0;
    Tile *_selectedTile = nullptr;
    Tile *_targetTile = nullptr;
    Tile *_startTile = nullptr;
    grid_t _grid;
    std::shared_ptr<IAlgo> _algo = nullptr;
};

class DepthFirstSearch : public IAlgo {
protected:
    std::stack<std::shared_ptr<Tile>> _myStack;
    std::shared_ptr<Tile> _currentTile = nullptr;
public:
    DepthFirstSearch(){};
    ~DepthFirstSearch() = default;
    void makeStep() override;
    inline void init(grid_t *grid) override {
        _grid = grid;
        if (!_grid) return;

        for (const auto& line : *_grid) {
            for (const auto& tile : line) {
                if (tile) {
                    tile->setTypeStyle(TileTypeStyle::Wall);
                    tile->realType = TileType::OBSTRUCTION;
                    tile->init();
                }
            }
        }
        _currentTile = (*_grid)[1][1];
        _algoType = AlgoType::DEPTH_FIRST_SEARCH;
    }
private:
    void convertToPathBetween(const std::shared_ptr<Tile>& current, const std::shared_ptr<Tile>& next);
    std::shared_ptr<Tile> &getRandomNeighbor();
};


struct CompareNode {
    bool operator()(const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b) const {
        return a->f() > b->f();
    }
};

class AStar : public IAlgo {
protected:
    std::shared_ptr<Tile> _startTile = nullptr;
    std::shared_ptr<Tile> _endTile = nullptr;
    std::unordered_set<std::shared_ptr<Tile>> _closed;
    std::priority_queue<std::shared_ptr<Tile>, std::vector<std::shared_ptr<Tile>>, CompareNode> _open;
public:
    AStar(){};
    ~AStar() = default;
    void makeStep() override;
    inline void init(grid_t *grid) override {
        _algoType = AlgoType::A_STAR;
        _grid = grid;
        if (!_grid) return;

        for (const auto& line : *_grid) {
            for (const auto& tile : line) {
                if (tile->realType == TileType::GOAL) _endTile = tile;
                else if (tile->realType == TileType::HOME) _startTile = tile;
            }
        }
        if (!_endTile || !_startTile) {
            throw std::runtime_error(std::string(getAlgoTypeString()) + std::string(" [No start or destination]"));
        }
        for (const auto& line : *_grid) {
            for (const auto& tile : line) {
                tile->init();
                int distance = AStar::manhattan(tile, _endTile);
                tile->hCost = distance;
                tile->gCost = -1;
            }
        }
        _startTile->gCost = 0;
        _open.push(_startTile);
    }

    inline const int manhattan(const std::shared_ptr<Tile> &a, const std::shared_ptr<Tile> &b) {
        // Manhattan distance
        int distance = abs(a->posI - b->posI) + abs(a->posJ - b->posJ);
        return distance * 10;
    }
    void reconstructPath();
private:
    std::vector<std::shared_ptr<Tile>> getNeighbors(const std::shared_ptr<Tile>& tile);
    bool _isPathReconstructing = false;
};

#endif /* !GRID_HPP_ */
