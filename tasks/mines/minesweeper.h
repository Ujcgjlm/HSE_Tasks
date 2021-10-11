#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include <ctime>

class Field {
public:

    Field();
    Field(size_t width, size_t height, size_t mines_count);
    Field(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);
    ~Field();

    enum class BackStatus { Empty, Mine };
    enum class FrontStatus { Empty, Flag, Opened };
    struct CellStatus {
        BackStatus back;
        FrontStatus front;
        bool is_mine_near = false;
    };

    void ForAllCells(std::function<void(CellStatus&)> func);
    void SwitchFlagStatus(const Cell& cell);
    CellStatus OpenCell(const Cell& cell);

    std::vector<std::string> RenderedField();

    bool Is_victory();
private:
    size_t y_size_ = 0;
    size_t x_size_ = 0;
    size_t free_cells_count_ = 0;
    std::vector<std::vector<CellStatus>> field_;
};

class Timer {
public:
    time_t Time();
    void Start();
    void Finish();

private:
    time_t start_ = 0;
    time_t finish_ = 0;

    time_t TimeSinceStart();
};

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    ~Minesweeper();

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    void LoseGame();
    void WinGame();

    Minesweeper::GameStatus game_status_ = Minesweeper::GameStatus::NOT_STARTED;
    Field* field_;
    Timer* timer_;
};