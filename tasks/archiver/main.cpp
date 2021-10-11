#include <iostream>
#include <bitset>
#include <vector>
#include <Haffman/Haffman.h>
#include <io.h>
#include <fstream>
#include <windows.h>
#include <chrono>

const std::string HELP_MESSAGE =
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿    ANIME ARCHIVER    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⢛⣽⡟⠁⠄⠄⣀⡠⠔⠂⠈⠄⠄⠄⡀⠠⡀⠄⠄⠄⠄⠄⠄⠉⠉⠛⠿⢶⣦⣤⣄⣘⢦    ⠄⣾⣿⡇⢸⣿⣿⣿⠄⠈⣿⣿⣿⣿⠈⣿⡇⢹⣿⣿⣿⡇⡇⢸⣿⣿⡇⣿⣿⣿    ⣿⣿⣿⣿⠿⢋⣩⣤⣴⣶⣶⣦⣙⣉⣉⣉⣉⣙⡛⢋⣥⣶⣶⣶⣶⣶⣬⡙⢿⣿\n"
        "⣾⠏⢀⣠⡶⠛⠁⠄⠄⠄⣰⠄⠄⣼⣷⡀⢻⣷⣤⡀⠄⢀⡀⠄⠄⠄⠄⠄⠈⠙⠻⣿⣿    ⢠⣿⣿⡇⢸⣿⣿⣿⡇⠄⢹⣿⣿⣿⡀⣿⣧⢸⣿⣿⣿⠁⡇⢸⣿⣿⠁⣿⣿⣿    ⣿⣿⠟⣡⣶⣿⢟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠙\n"
        "⣯⣴⠟⠋⠄⠄⠄⢠⡎⢰⣿⡇⢰⣿⣿⣿⣜⣿⣿⣿⣄⠘⣿⣄⠄⠄⠄⠄⠄⠄⠄⠄⠙    ⢸⣿⣿⡇⠸⣿⣿⣿⣿⡄⠈⢿⣿⣿⡇⢸⣿⡀⣿⣿⡿⠸⡇⣸⣿⣿⠄⣿⣿⣿    ⣿⢋⣼⣿⠟⣱⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⢿⣿⣿⣿⣿⣧\n"
        "⠟⠁⠄⠄⠄⠄⢠⣿⣇⣿⠿⢻⣸⣿⣿⣟⢉⡛⠻⠿⣿⣷⣿⣿⡆⣦⠄⠄⠄⠄⠄⠄⠄    ⢸⣿⡿⠷⠄⠿⠿⠿⠟⠓⠰⠘⠿⣿⣿⡈⣿⡇⢹⡟⠰⠦⠁⠈⠉⠋⠄⠻⢿⣿    ⠃⣾⣯⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⡈⢿⣿⣿⣿⣿\n"
        "⠄⠄⠄⠄⣠⡇⣾⣿⣭⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠄⠄⠄⠄⠄⠄    ⢨⡑⠶⡏⠛⠐⠋⠓⠲⠶⣭⣤⣴⣦⣭⣥⣮⣾⣬⣴⡮⠝⠒⠂⠂⠘⠉⠿⠖⣬    ⢰⣶⣼⣿⣷⣿⣽⠿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡌⣿⣷⡀⠛⢿⣿⣿\n"
        "⠄⠄⠄⢀⣿⣷⠿⠿⠿⠿⢟⢻⣿⣿⣿⣿⣿⣿⣥⢒⣉⠭⠤⢉⡉⠛⠿⡆⠄⠄⠄⠄⠄    ⠈⠉⠄⡀⠄⣀⣀⣀⣀⠈⢛⣿⣿⣿⣿⣿⣿⣿⣿⣟⠁⣀⣤⣤⣠⡀⠄⡀⠈⠁    ⢃⣺⣿⣿⣿⢿⠏⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡾⣿⣿⣿⣷⢹⣿⣷⣄⠄⠈⠉\n"
        "⠄⠄⠄⠘⠉⣀⣴⠖⠉⣡⠒⣿⣿⣿⣿⣿⣿⣿⣷⣿⠁⠄⠄⠆⠹⣦⠄⡄⠄⠄⠄⠄⠄    ⠄⠠⣾⡀⣾⣿⣧⣼⣿⡿⢠⣿⣿⣿⣿⣿⣿⣿⣿⣧⣼⣿⣧⣼⣿⣿⢀⣿⡇⠄    ⡼⣻⣿⣷⣿⠏⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣞⣿⣿⣿⠸⣿⣿⣿⣿⣶⣤\n"
        "⠄⠄⠄⢰⡀⣿⡇⠄⠄⢀⠄⢸⣿⣿⣿⣿⣿⣿⣿⣿⣠⡄⠠⠄⢀⣏⣼⠁⠄⠄⠄⠄⠄    ⡀⠄⠻⣷⡘⢿⣿⣿⡿⢣⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣜⢿⣿⣿⡿⢃⣾⠟⢁⠈    ⣇⣿⡿⣿⠏⣸⣎⣻⣟⣿⣿⣿⢿⣿⣿⣿⣿⠟⣩⣼⢆⠻⣿⡆⣿⣿⣿⣿⣿⣿\n"
        "⠄⠄⠄⠈⣿⣾⣿⡴⠶⣁⡤⢸⣿⣿⣿⣿⣿⣿⣋⣉⣦⣝⣚⣃⣾⣿⠏⠄⠄⠄⠄⠄⠄    ⢃⢻⣶⣬⣿⣶⣬⣥⣶⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣷⣶⣶⣾⣿⣷⣾⣾⢣    ⢸⣿⡿⠋⠈⠉⠄⠉⠻⣽⣿⣿⣯⢿⣿⣿⡻⠋⠉⠄⠈⠑⠊⠃⣿⣿⣿⣿⣿⣿\n"
        "⠄⠄⠄⠄⠘⣿⣿⣿⣶⣾⣿⣿⣿⣏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣠⡎⠄⠄⠄⠄⠄    ⡄⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠘    ⣿⣿⠄⠄⣰⠱⠿⠄⠄⢨⣿⣿⣿⣿⣿⣿⡆⢶⠷⠄⠄⢄⠄⠄⣿⣿⣿⣿⣿⣿\n"
        "⠄⠄⠄⠄⢰⣜⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⠁⠄⠄⠄⠄⠄    ⣿⡐⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢠⢃    ⣿⣿⠘⣤⣿⡀⣤⣤⣤⢸⣿⣿⣿⣿⣿⣿⡇⢠⣤⣤⡄⣸⣀⡆⣿⣿⣿⣿⣿⣿\n"
        "⠄⠄⠄⠄⠄⢿⣿⣿⣿⣿⣿⣿⡛⠛⠉⠉⠉⠉⠉⣽⣿⣿⣿⣿⣿⣿⡇⠄⠄⠄⠄⠄⠄    ⣿⣷⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋⢀⠆⣼    ⣿⣿⡀⣿⣿⣷⣌⣉⣡⣾⣿⣿⣿⣿⣿⣿⣿⣌⣛⣋⣴⣿⣿⢣⣿⣿⣿⣿⡟⣿\n"
        "⠄⠄⠄⠄⠄⠈⠻⣿⣿⣿⣿⣿⣧⠄⠄⠄⠄⠄⣴⣿⣿⣿⣿⣿⡿⠋⠄⠄⠄⠄⠄⠄⠄    ⣿⣿⣷⡀⠄⠈⠛⢿⣿⣿⣿⣿⣷⣶⣶⣶⣶⣶⣿⣿⣿⣿⣿⠿⠋⠠⠂⢀⣾⣿    ⢹⣿⢸⣿⣻⣶⣿⢿⣿⣿⣿⢿⣿⣿⣻⣿⣿⣿⡿⣿⣭⡿⠻⢸⣿⣿⣿⣿⡇⢹\n"
        "⠄⠄⠄⠄⠄⠄⠄⠄⠉⠛⠿⣿⣿⣿⣶⣶⣶⣿⣿⣿⣿⠟⠋⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄    ⣿⣿⣿⣧⠄⠄⢵⢠⣈⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢋⡁⢰⠏⠄⠄⣼⣿⣿    ⠈⣿⡆⠻⣿⣏⣿⣿⣿⣿⣿⡜⣭⣍⢻⣿⣿⣿⣿⣿⣛⣿⠃⣿⣿⣿⣿⡿⠄⣼\n"
        "⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠈⠙⠛⠿⠿⠟⠋⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄    ⢻⣿⣿⣿⡄⢢⠨⠄⣯⠄⠄⣌⣉⠛⠻⠟⠛⢋⣉⣤⠄⢸⡇⣨⣤⠄⢸⣿⣿⣿    ⣦⠘⣿⣄⠊⠛⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣼⣿⣿⣿⡿⠁⠄⠟\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿    ANIME ARCHIVER    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "Made by: Ujcgjlm\n"
        "For encode:\n"
        "Add flag -c file_out file_in1 (file_in2 ...)\n"
        "For decode:\n"
        "Add flag -d file_in";

struct InvalidArguments : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "archive -h for help";
    }
};

size_t FileSize(const std::string& fileName) {
    std::ifstream f(fileName);
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    return size;
}

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> args(argv, argv + argc);
    size_t before_compress = 0;
    size_t after_compress = 0;

    if (argc > 2) {
        if (argc >= 3 && args[1] == "-d") {
            if (std::ifstream(args[2])){
                std::ifstream input(args[2], std::ios::binary);
                Haffman::Decode(input);
            } else {
                std::cout << "No such file exists.";
                return 0;
            }
        } else {
            if (argc > 3 && args[1] == "-c") {
                std::vector<std::istream *> streams;
                for (size_t i = 3; i < argc; ++i) {
                    if (std::ifstream(args[i])){
                        streams.emplace_back(new std::ifstream(args[i], std::ios::binary));
                        before_compress += FileSize(args[i]);
                    } else {
                        std::cout << "No such file exists: " << args[i];
                        return 0;
                    }
                }
                std::ofstream output(args[2], std::ios::binary);
                Haffman::Encode(output, streams, std::vector(args.begin() + 3, args.end()));
                after_compress = FileSize(args[2]);
                std::cout << "Archive file size: " << after_compress * sizeof(char) << "\n";
                std::cout << "Percentage of archive file to original: " << static_cast<double>(after_compress) * 100. / static_cast<double>(before_compress)  << "\n";
            } else {
                throw InvalidArguments();
            }
        }
    } else {
        if (argc == 2 && args[1] == "-h") {
            std::cout << HELP_MESSAGE;
        } else {
            throw InvalidArguments();
        }
    }

    std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>> (finish -  start);
    std::cout << "Spent time: " << time_span.count() << " s" << "\n";

    return 0;
}
