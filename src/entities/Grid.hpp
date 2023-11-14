/*
** EPITECH PROJECT, 2023
** raylib_base
** File description:
** Grid
*/

#ifndef GRID_HPP_
#define GRID_HPP_

#include "IEntity.hpp"
#include "IController.hpp"

enum TileTypeStyle {
    Empty,
    Selected,
    Wall,
    Target,
    Start,
    Searching
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
    NONE,
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

    unsigned int posI = 0;
    unsigned int posJ = 0;

    const char* getTypeString() {
        switch (_typeStyle) {
            case Empty: return "Empty";
            case Selected: return "Selected";
            case Wall: return "Wall";
            case Target: return "Target";
            case Start: return "Start";
            case Searching: return "Searching";
            default: return "Unknown";
        }
    }

    bool isSelected = false;
    Color getColor() const {return _colors[_typeStyle];}
    void setTypeStyle(const TileTypeStyle &typeStyle) {_typeStyle = typeStyle;}
    TileTypeStyle getTypeStyle() const {return _typeStyle;}

    void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) override;
    void draw() const override;

    TileType realType = TileType::FREE;
    bool isVisited = false;
protected:
    TileTypeStyle _typeStyle;
    Color _borderColor = BLACK;
    std::vector<Color> _colors = { DARKGRAY, YELLOW, RAYWHITE, RED, GREEN, LIGHTGRAY };
};

typedef std::vector<std::vector<std::shared_ptr<Tile>>> grid_t;

class IAlgo {
public:

    virtual void init(grid_t *grid) = 0;
    virtual void makeStep() = 0;
    virtual bool isCompleted() const {return _isCompleted;}
    virtual const char* getAlgoTypeString() {
        switch (_algoType) {
            case A_STAR: return "A*";
            case DIJKSTRA: return "Dijkstra";
            case BEST_FIT_SEARCH: return "Best Fit Search";
            case DEPTH_FIRST_SEARCH: return "Depth First Search";
            default: return "Unknown";
        }
    }
protected:
    AlgoType _algoType = AlgoType::NONE;
    grid_t *_grid = nullptr;
    bool _isCompleted = false;
private:
};


class Grid : public IEntity {
public:
    Grid(Vector2 pos, Rectangle rect);
    ~Grid();

    void events();
    void resetGrid();
    void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) override;
    void draw() const override;
    void drawGUI() override;

protected:
    Vector2 _mousePosition = GetMousePosition();
//  ---- GUI ----
    Rectangle _window = { SCREEN_WIDTH - 210, 10, 200, 300 };
    bool _isDragging = false;
    Vector2 _dragOffset = { 0 };
    bool _wallCheck = true; //Checkbox for _typeToPut
    bool _startCheck = false; //Checkbox for _typeToPut
    bool _targetCheck = false; //Checkbox for _typeToPut
    bool _aStarCheck = true; //Checkbox for _algoType
    bool _dijkstraCheck = false; //Checkbox for _algoType
    bool _bestFitCheck = false; //Checkbox for _algoType
    bool _isClearClicked = false;
    bool _isStartClicked = false;
    bool _isGenerateClicked = false;
    TileTypeStyle _typeToPut = Wall;
    AlgoType _algoType = A_STAR;
private:
    Tile *_selectedTile = nullptr;
    Tile *_targetTile = nullptr;
    Tile *_startTile = nullptr;
    grid_t _grid;
    std::shared_ptr<IAlgo> _algo = nullptr;
};

class GenerateDepthFirstSearch : public IAlgo {
    typedef std::pair<int, int> grid_pos_t;
protected:
    std::stack<std::shared_ptr<Tile>> _myStack;
    std::shared_ptr<Tile> _currentTile = nullptr;
public:
    GenerateDepthFirstSearch(){};
    ~GenerateDepthFirstSearch() = default;
    void makeStep() override;
    void init(grid_t *grid) override {
        _grid = grid;
        if (!_grid) return;

        for (const auto& line : *_grid) {
            for (const auto& tile : line) {
                if (tile) {
                    tile->setTypeStyle(TileTypeStyle::Wall);
                    tile->realType = TileType::OBSTRUCTION;
                    tile->isVisited = false;
                }
            }
        }
        _currentTile = (*_grid)[1][1];
        _algoType = AlgoType::DEPTH_FIRST_SEARCH;
    }
private:
    void convertToPathBetween(const std::shared_ptr<Tile>& current, const std::shared_ptr<Tile>& next);
    std::shared_ptr<Tile> getRandomNeighbor() const;
};

#endif /* !GRID_HPP_ */
