#pragma once

#include <vector>
#include <deque>
#include <fstream>

class BitHandler {
public:

    BitHandler(std::istream *fin, std::ostream *fout);

    explicit BitHandler(std::istream *fin);

    explicit BitHandler(std::ostream *fout);

    explicit BitHandler();

    void SetIStream(std::istream *fin);

    void SetOStream(std::ostream *fout);

    std::vector<bool> Read(size_t count_of_bits);

    size_t ReadInteger(size_t count_of_bits);

    void Write(std::vector<bool>);

    bool FinishWrite();

    static std::vector<bool> FormIntegerToNineBinary(size_t number);

private:
    std::deque<bool> read_buffer_;
    std::deque<bool> write_buffer_;

    std::istream *fin_ = nullptr;
    std::ostream *fout_ = nullptr;

    bool PushBuffer_();

    bool GetBuffer_(size_t count_of_bytes);
};
