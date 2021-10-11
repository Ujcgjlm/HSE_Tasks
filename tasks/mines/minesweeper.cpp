#include "minesweeper.h"

Field::Field() {
}

Field::Field(size_t width, size_t height, size_t mines_count) {
    y_size_ = height;
    x_size_ = width;
    free_cells_count_ = height * width - mines_count;
    std::vector<bool> rnd_table(width * height);
    std::random_device rd;
    std::mt19937 g(rd());

    mines_count = std::min(mines_count, width * height);

    while (--mines_count) {
        rnd_table[mines_count] = true;
    }
    std::shuffle(rnd_table.begin(), rnd_table.end(), g);

    field_.assign(height, std::vector<CellStatus>(width));
    for (size_t i = 0; i < rnd_table.size(); ++i) {
        field_[i / width][i % width] = {rnd_table[i] ? BackStatus::Mine : BackStatus::Empty, FrontStatus::Empty};
    }
}

Field::Field(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    y_size_ = height;
    x_size_ = width;
    free_cells_count_ = height * width - cells_with_mines.size();
    field_.assign(height, std::vector<CellStatus>(width));
    for (auto& [x, y] : cells_with_mines) {
        field_[y][x] = {BackStatus::Mine, FrontStatus::Empty};
    }
    ForAllCells([&](CellStatus& cell) {
        if (cell.back != BackStatus::Mine) {
            cell = {BackStatus::Empty, FrontStatus::Empty};
        }
    });
}

Field::~Field() {
    field_.clear();
}

void Field::ForAllCells(std::function<void(CellStatus&)> func) {
    for (size_t y = 0; y < y_size_; ++y) {
        for (size_t x = 0; x < x_size_; ++x) {
            func(field_[y][x]);
        }
    }
}

void Field::SwitchFlagStatus(const Cell& cell) {
    if (field_[cell.y][cell.x].front == FrontStatus::Empty) {
        field_[cell.y][cell.x].front = FrontStatus::Flag;
    } else {
        if (field_[cell.y][cell.x].front == FrontStatus::Flag) {
            field_[cell.y][cell.x].front = FrontStatus::Empty;
        }
    }
}

bool Field::Is_victory() {
    return (free_cells_count_ == 0);
}

Field::CellStatus Field::OpenCell(const Cell& cell) {
    std::queue<Cell> all_cells;
    std::queue<Cell> current_cells;

    all_cells.push(cell);
    return field_[cell.y][cell.x];
}

std::vector<std::string> Field::RenderedField() {
    std::vector<std::string> rendered_field(y_size_);

    for (size_t y = 0; y < y_size_; ++y) {
        rendered_field[y].resize(x_size_);
        for(size_t x = 0; x < x_size_; ++x) {
            switch (field_[y][x].front) {
                case FrontStatus::Flag:
                    rendered_field[y][x] = '?';
                    break;
                case FrontStatus::Empty:
                    rendered_field[y][x] = '-';
                    break;
                case FrontStatus::Opened:
                    if (field_[y][x].back == BackStatus::Mine) {
                        rendered_field[y][x] = '*';
                    } else {
                        uint64_t mines_count = 0;
                        for (size_t i = 0; i < directions::size; ++i) {
                            if (static_cast<int64_t>(y) + directions::y_dir[i] >= 0 &&
                                static_cast<int64_t>(y) + directions::y_dir[i] < y_size_ &&
                                static_cast<int64_t>(x) + directions::x_dir[i] >= 0 &&
                                static_cast<int64_t>(x) + directions::x_dir[i] < x_size_ &&
                                field_[y + directions::y_dir[i]][x + directions::x_dir[i]].back == BackStatus::Mine) {
                                ++mines_count;
                            }
                        }
                        rendered_field[y][x] = (mines_count == 1) ? ('0' + mines_count) : '.';
                    }
                    break;
            }
        }
    }

    return rendered_field;
}

time_t Timer::Time() {
    return (finish_ == 0) ? TimeSinceStart() : finish_;
}

void Timer::Start() {
    finish_ = 0;
    start_ = (start_ == 0) ? time(nullptr) : start_;
}

void Timer::Finish() {
    finish_ = (finish_ == 0) ? TimeSinceStart() : finish_;
    start_ = 0;
}

time_t Timer::TimeSinceStart() {
    return time(nullptr) - start_;
}

void Minesweeper::LoseGame() {
    field_->ForAllCells([&](Field::CellStatus& cell) { cell.front = Field::FrontStatus::Opened; });
    game_status_ = Minesweeper::GameStatus::DEFEAT;
    timer_->Finish();
}

void Minesweeper::WinGame() {
    game_status_ = Minesweeper::GameStatus::VICTORY;
    timer_->Finish();
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    field_ = new Field(width, height, mines_count);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    field_ = new Field(width, height, cells_with_mines);
}

Minesweeper::~Minesweeper(){
    delete field_;
    delete timer_;
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    game_status_ = Minesweeper::GameStatus::NOT_STARTED;
    field_ = new Field(width, height, mines_count);
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = Minesweeper::GameStatus::NOT_STARTED;
    field_ = new Field(width, height, reinterpret_cast<const std::vector<Minesweeper::Cell>&>(cells_with_mines));
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        timer_->Start();
    }
    Field::CellStatus current_cell_status = field_->OpenCell(cell);
    if (current_cell_status.back == Field::BackStatus::Mine) {
        LoseGame();
    }
    if (field_->Is_victory()) {
        WinGame();
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        timer_->Start();
    }
    field_->SwitchFlagStatus(cell);
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

time_t Minesweeper::GetGameTime() const {
    return timer_->Time();
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    return field_->Field::RenderedField();
}
