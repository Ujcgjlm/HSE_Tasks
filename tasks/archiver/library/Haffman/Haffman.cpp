#include <iostream>
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <bitset>
#include "Haffman.h"
#include "../BitHandler/BitHandler.h"
#include "../Treap/Treap.h"
#include "../Bor/Bor.h"
#include "HaffmanExceptions.h"

bool ReadChar(std::istream *in, unsigned char &symbol) {
    symbol = in->get();
    return in->gcount() == 1;
}

class Comp {
public:
    bool operator()(Bor *a, Bor *b) const {
        return a->GetFrequency() < b->GetFrequency();
    }
};

void IncreaseBinary(std::vector<bool> &binary) {
    for (auto it = binary.rbegin(); it < binary.rend(); ++it) {
        if (!*it) {
            *it = true;
            break;
        } else {
            *it = false;
        }
    }
}

void Haffman::Decode(std::istream &in) {
    BitHandler bit_in(&in);
    size_t file_condition = ONE_MORE_FILE;
    while (file_condition == ONE_MORE_FILE) {
        size_t alpha_size = bit_in.ReadInteger(9);
        std::vector<size_t> symbols(alpha_size);
        for (auto &symbol: symbols) {
            symbol = bit_in.ReadInteger(9);
        }

        size_t pos = 0;
        std::vector<bool> current_path;
        Bor *bor = new Bor();
        while (pos < symbols.size()) {
            current_path.push_back(false);
            size_t count_of_cur_path_size = bit_in.ReadInteger(9);
            while (count_of_cur_path_size > 0) {
                bor->AddPath(symbols[pos], current_path);
                IncreaseBinary(current_path);
                --count_of_cur_path_size;
                ++pos;
            }
        }

        std::string file_name;
        size_t file_name_symbol = 0;
        while (file_name_symbol != FILENAME_END) {
            Bor::Node *pointer = bor->GetRoot();
            while (!pointer->is_terminal) {
                pointer = bor->Move(pointer, bit_in.Read(1).front());
                if (pointer == nullptr) {
                    throw UnknownSymbol();
                }
            }
            file_name_symbol = pointer->symbol;
            if (file_name_symbol != FILENAME_END) {
                if (file_name_symbol >= 256) {
                    throw UnknownSymbol();
                }
                file_name += static_cast<char>(file_name_symbol);
            }
        }

        std::ofstream out(file_name, std::ios::binary);

        size_t file_symbol = 0;
        while (file_symbol != ARCHIVE_END && file_symbol != ONE_MORE_FILE) {
            Bor::Node *pointer = bor->GetRoot();
            while (!pointer->is_terminal) {
                pointer = bor->Move(pointer, bit_in.Read(1).front());
                if (pointer == nullptr) {
                    throw UnknownSymbol();
                }
            }
            file_symbol = pointer->symbol;
            if (file_symbol != ARCHIVE_END && file_symbol != ONE_MORE_FILE) {
                if (file_symbol >= 256) {
                    throw UnknownSymbol();
                }
                out.put(static_cast<char>(file_symbol));
            }
        }
        file_condition = file_symbol;
        delete bor;
    }
}

void
Haffman::Encode(std::ostream &out, const std::vector<std::istream *> &ins, const std::vector<std::string> &names = {}) {
    BitHandler handler_out(&out);
    for (size_t idx = 0; idx < ins.size(); ++idx) {
        std::istream *in = ins[idx];
        std::string file_name = names[idx];
        std::vector<size_t> symbols_count(COUNT_OF_DIFFERENT_SYMBOLS, 0);

        for (unsigned char symbol: file_name) {
            ++symbols_count[symbol];
        }

        unsigned char symbol_from_file = 0;
        while (ReadChar(in, symbol_from_file)) {
            ++symbols_count[symbol_from_file];
        }

        if (idx + 1 < ins.size()) {
            symbols_count[FILENAME_END] = symbols_count[ONE_MORE_FILE] = 1;
        } else {
            symbols_count[FILENAME_END] = symbols_count[ARCHIVE_END] = 1;
        }

        Treap<Bor *, Comp> priority_queue;
        for (size_t cur_symbol = 0; cur_symbol < COUNT_OF_DIFFERENT_SYMBOLS; ++cur_symbol) {
            if (symbols_count[cur_symbol] != 0) {
                priority_queue.Insert(new Bor(cur_symbol, symbols_count[cur_symbol]));
            }
        }

        while (priority_queue.Size() > 1) {
            priority_queue.Insert(new Bor(priority_queue.CutTop(), priority_queue.CutTop()));
        }

        std::vector<Bor::SymbolPath> symbol_paths = priority_queue.CutTop()->GetPathToEachTerminalVertex();
        std::sort(symbol_paths.begin(), symbol_paths.end(), [](const Bor::SymbolPath &a, const Bor::SymbolPath &b) {
            return std::tuple(a.pathSize, a.symbol) < std::tuple(b.pathSize, b.symbol);
        });

        std::vector<bool> previous;
        std::unordered_map<size_t, std::vector<bool>> dict;
        for (const auto&[symbol, path_size]: symbol_paths) {
            IncreaseBinary(previous);
            while (path_size != previous.size()) {
                previous.emplace_back(false);
            }
            dict[symbol] = previous;
        }

        handler_out.Write(BitHandler::FormIntegerToNineBinary(dict.size()));

        for (const auto&[symbol, _]: symbol_paths) {
            handler_out.Write(BitHandler::FormIntegerToNineBinary(symbol));
        }

        for (size_t current_path_size = 1, sizes_number = 0, idx_paths = 0;
             idx_paths <= symbol_paths.size(); ++idx_paths) {
            if (idx_paths == symbol_paths.size()) {
                handler_out.Write(BitHandler::FormIntegerToNineBinary(sizes_number));
            } else {
                if (symbol_paths[idx_paths].pathSize != current_path_size) {
                    while (symbol_paths[idx_paths].pathSize != current_path_size) {
                        handler_out.Write(BitHandler::FormIntegerToNineBinary(sizes_number));
                        sizes_number = 0;
                        ++current_path_size;
                    }
                }
                ++sizes_number;
            }
        }

        for (const auto &symbol: file_name) {
            handler_out.Write(dict[symbol]);
        }
        handler_out.Write(dict[FILENAME_END]);

        in->clear();
        in->seekg(0);

        while (ReadChar(in, symbol_from_file)) {
            handler_out.Write(dict[symbol_from_file]);
        }

        if (idx + 1 < ins.size()) {
            handler_out.Write(dict[ONE_MORE_FILE]);
        } else {
            handler_out.Write(dict[ARCHIVE_END]);
        }
    }
    handler_out.FinishWrite();
}

