#include <bitset>
#include <algorithm>
#include "BitHandler.h"
#include "BitHandlerExceptions.h"

BitHandler::BitHandler(std::istream *fin, std::ostream *fout) : fin_(fin), fout_(fout) {}

BitHandler::BitHandler(std::istream *fin) : fin_(fin) {}

BitHandler::BitHandler(std::ostream *fout) : fout_(fout) {}

BitHandler::BitHandler() = default;

void BitHandler::Write(std::vector<bool> input) {
    write_buffer_.insert(write_buffer_.end(), input.begin(), input.end());
    PushBuffer_();
}

std::vector<bool> BitHandler::Read(size_t count_of_bits) {
    if (read_buffer_.size() < count_of_bits) {
        if (!GetBuffer_((count_of_bits - read_buffer_.size() + 7) / 8)) {
            throw CantReadFromFile();
        }
    }
    std::vector<bool> out = std::vector(read_buffer_.begin(), read_buffer_.begin() + static_cast<int>(count_of_bits));
    read_buffer_.erase(read_buffer_.begin(), read_buffer_.begin() + static_cast<int>(count_of_bits));
    return out;
}

size_t BitHandler::ReadInteger(size_t count_of_bits) {
    std::vector<bool> bits = Read(count_of_bits);
    std::bitset<64> set = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        set[i] = bits[bits.size() - 1 - i];
    }
    return set.to_ullong();
}

bool BitHandler::PushBuffer_() {
    while (write_buffer_.size() >= 8) {
        std::bitset<8> cur_byte;
        for (size_t i = 0; i < 8; ++i) {
            cur_byte[i] = write_buffer_.front();
            write_buffer_.pop_front();
        }
        unsigned char byte = static_cast<char>(cur_byte.to_ulong());
        fout_->put(byte);
    }
    return true;
}

bool BitHandler::GetBuffer_(size_t count_of_bytes) {
    for (size_t j = 0; j < count_of_bytes; ++j) {
        char byte = 0;
        fin_->read(&byte, 1);
        if (fin_->gcount() != 1) {
            return false;
        }
        std::bitset<8> cur_byte = byte;
        for (size_t i = 0; i < 8; ++i) {
            read_buffer_.push_back(cur_byte[i]);
        }
    }
    return true;
}

bool BitHandler::FinishWrite() {
    while (write_buffer_.size() % 8 != 0) {
        write_buffer_.push_back(false);
    }
    return PushBuffer_();
}

void BitHandler::SetIStream(std::istream *fin) {
    fin_ = fin;
}

void BitHandler::SetOStream(std::ostream *fout) {
    fout_ = fout;
}

std::vector<bool> BitHandler::FormIntegerToNineBinary(size_t number) {
    std::bitset<9> bitSet = number;
    std::vector<bool> bitVector;
    for (size_t i = 0; i < 9; ++i) {
        bitVector.emplace_back(bitSet[8 - i]);
    }
    return bitVector;
}
