#ifndef LAYOUT_DOMINO_FILEWRITER_H
#define LAYOUT_DOMINO_FILEWRITER_H

#include <iostream>
#include <fstream>

class FileWriter {
private:
    std::ofstream file;

    template <typename... Args>
    void write(std::ostream& stream, Args&&... args) {
        (stream << ... << args) << '\n';
    }

public:
    explicit FileWriter(const std::string& filename) {
        try {
            file.open(filename, std::fstream::out);
            if (!file.is_open()) {
                std::cerr <<"open() failed for file " << filename << std::endl;
                return;
            }
        }
        catch (std::exception& exception) {
            std::cerr << "open() failed for file " << filename << exception.what() << std::endl;
        }
    }

    ~FileWriter() {
        file.close();
    }

    template<typename... Args>
    void write_file(const Args&... args) {
        write(file, args...);
    }

};


#endif //LAYOUT_DOMINO_FILEWRITER_H
