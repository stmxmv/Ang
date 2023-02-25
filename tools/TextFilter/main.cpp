//
// Created by aojoie on 2/22/2023.
//

#include <chrono>
#include <ctime>
#include <random>
#include <fstream>
#include <string_view>
#include <unordered_set>

inline bool isASCII(char Ch) {
    return static_cast<unsigned char>(Ch) <= 127;
}

inline bool isVerticalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == '\r' || Ch == '\n');
}

inline bool isHorizontalWhitespace(char Ch) {
    return isASCII(Ch) && (Ch == ' ' || Ch == '\t' ||
                           Ch == '\f' || Ch == '\v');
}

inline bool isWhitespace(char Ch) {
    return isHorizontalWhitespace(Ch) ||
           isVerticalWhitespace(Ch);
}

class TextGenerator {

    std::mt19937_64 generator;

    static auto _get_seed() {
        std::random_device os_seed;
        auto seed = os_seed() ^ ((std::mt19937_64::result_type)
                                         std::chrono::duration_cast<std::chrono::seconds>(
                                                 std::chrono::system_clock::now().time_since_epoch())
                                                 .count() +
                                 (std::mt19937_64::result_type)
                                         std::chrono::duration_cast<std::chrono::microseconds>(
                                                 std::chrono::high_resolution_clock::now().time_since_epoch())
                                                 .count());
        return seed;
    }

    std::string getWord(std::string::size_type length, const char characterSet[], int size) {
        std::string s;

        s.reserve(length);

        while (length--) {
            auto index = std::uniform_int_distribution<>(0UL, size - 1)(generator);
            s += characterSet[index];
        }

        return s;
    }

    bool __generateTextFile(const char *path,
                          int wordCount,
                          int averageWordLength,
                          char wordSplit,
                          const char characterSet[], int setSize) {


        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        while (wordCount--) {

            int wordLength = 0;
            while (wordLength <= 1) {
                wordLength = (int) std::normal_distribution<float>((float) averageWordLength)(generator);
            }

            std::string const word = getWord(wordLength, characterSet, setSize);

            file << word;

            if (wordSplit) {
                file << wordSplit;
            }

        }

        file.close();

        return true;
    }

public:

    TextGenerator() : generator(_get_seed()) {}


    bool generateBannedTextFile(const char *path,
                                int wordCount,
                                int averageWordLength) {
        static const char chrs[] = "abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        return __generateTextFile(path, wordCount, averageWordLength, '\n', chrs, sizeof chrs - 1);
    }


    bool generateRandomTextFile(const char *path,
                                int wordCount,
                                int averageWordLength) {
        static const char chrs[] = "\r\n"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        return __generateTextFile(path, wordCount, averageWordLength, ' ', chrs, sizeof chrs - 1);
    }

};

class TextFilter {

    std::unordered_set<std::string> bannedWordSet;
    const char *curPtr;
    std::ofstream outFile;

    bool __initBannedWordSet(const char *bannedFileName) {
        std::ifstream file(bannedFileName);
        if (!file.is_open()) {
            return false;
        }

        std::string word;
        while (file >> word) {
            bannedWordSet.insert(word);
        }

        return true;
    }

    bool filter_one() {
        while (isWhitespace(*curPtr)) {
            outFile << *curPtr;
            ++curPtr;
        }

        if (!*curPtr) {
            return false;
        }

        const char *end = curPtr + 1;


        while (*end && !isWhitespace(*end)) {
            ++end;
        }

        std::string_view word = std::string_view(curPtr, end - curPtr);

        if (bannedWordSet.find(std::string(word)) != bannedWordSet.end()) {
            outFile << "**";
        } else {
            outFile << word;
        }

        curPtr = end;

        return true;
    }

public:

    bool init(const char *bannedFileName) {
        return __initBannedWordSet(bannedFileName);
    }

    bool filterFile(const char *inputFileName, const char *outputFileName) {

        std::ifstream inFile(inputFileName);
        outFile = std::ofstream(outputFileName);

        if (!inFile.is_open() || !outFile.is_open()) {
            return false;
        }

        /// load file into a single buffer
        inFile.seekg(0, std::ios::end);
        size_t size = inFile.tellg();

        std::string buffer(size, 0);
        inFile.seekg(0);
        inFile.read(buffer.data(), size);

        /// do filtering
        curPtr = buffer.c_str();

        while (filter_one()) {

        }

        curPtr = nullptr;
        outFile.close();

        return true;
    }

};


int main(int argc, const char * argv[]) {


    TextGenerator textGenerator;

    textGenerator.generateBannedTextFile("banned.txt", 100000, 5);
    textGenerator.generateRandomTextFile("rand.txt", 10000, 5);

    TextFilter textFilter;
    if (textFilter.init("banned.txt")) {

        textFilter.filterFile("rand.txt", "rand_filtered.txt");

    }

    return 0;
}