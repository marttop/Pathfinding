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
class Tile;
enum TileType {
    Empty,
    Selected,
    Wall,
    Target,
    Start,
    Searching
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
    Rectangle _window = { SCREEN_WIDTH - 210, 10, 200, 250 };
    bool _isDragging = false;
    Vector2 _dragOffset = { 0 };
    bool _wallCheck = true;
    bool _startCheck = false;
    bool _targetCheck = false;
    bool _isClearClicked = false;
    TileType _typeToPut = Wall;
private:
    Tile *_selectedTile = nullptr;
    Tile *_targetTile = nullptr;
    Tile *_startTile = nullptr;
    std::vector<std::vector<std::shared_ptr<Tile>>> _grid;
};

class Tile : public IEntity {
public:
    Tile(Vector2 pos, Rectangle rect, TileType type = TileType::Empty) : IEntity(pos, rect, EntityType::Tile){_type = type;}
    ~Tile() = default;

    const char* getTypeString() {
        switch (_type) {
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
    Color getColor() const {return _colors[_type];}
    void setType(const TileType &type) {_type = type;}
    TileType getType() const {return _type;}

    void update(float deltaTime, std::vector<std::shared_ptr<IEntity>>& m_entities) override;
    void draw() const override;
protected:
    TileType _type;
    Color _borderColor = BLACK;
    std::vector<Color> _colors = { DARKGRAY, YELLOW, RAYWHITE, RED, GREEN, LIGHTGRAY };
};

#endif /* !GRID_HPP_ */
