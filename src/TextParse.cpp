#include"TextParse.hpp"

std::string TextParse::trim(const std::string &str,
                            const std::string &whitespace) {
    std::string result("");
    const uint32_t str_begin = str.find_first_not_of(whitespace);
    if (str_begin == std::string::npos) { return ""; }
    const uint32_t str_end = str.find_last_not_of(whitespace);
    const uint32_t str_range = (str_end - str_begin) + 1;
    result = str.substr(str_begin, str_range);
    return result;
}

std::string TextParse::reduce(const std::string &str,
                              const std::string &fill,
                              const std::string &whitespace) {
    std::string result("");
    result = trim(str, whitespace);
    uint32_t begin_space = result.find_first_of(whitespace);
    while (begin_space != std::string::npos) {
        const int end_space = result.find_first_not_of(whitespace, begin_space);
        const int range = end_space - begin_space;
        result.replace(begin_space, range, fill);
        const int new_start = begin_space + fill.length();
        begin_space = result.find_first_of(whitespace, new_start);
    }
    return result;
}

std::string TextParse::removeComments(const std::string &str,
                                      const char &delimiter) {
    if (str.find(delimiter) != std::string::npos) {
        return str.substr(0, str.find(delimiter));
    } else { return str; }
}

QString TextParse::removeFileExtension(const QString &fileName) {
    return fileName.split(".", QString::SkipEmptyParts).at(0);
}

QStringList TextParse::removeFileExtensions(const QStringList &fileNames) {
    QStringList result{ };
    for (QString file : fileNames) {
        result.push_back(removeFileExtension(file));
    }
    return result;
}

std::pair<int, int> TextParse::strToRangePair(const std::string &str,
                                              const char &delimiter) {
    std::pair<int,int> result{ };
    int colonIndex = str.find(delimiter);
    result.first = stoi(str.substr(0, colonIndex));
    result.second = stoi(str.substr(colonIndex + 1));
    return result;
}

std::vector<std::string> TextParse::strToStrVector(const std::string &str,
                                                   const char &delimiter) {
    std::vector<std::string> result{ };
    std::stringstream sStream(reduce(str));
    while (sStream.good()) {
        std::string subStr("");
        std::getline(sStream, subStr, delimiter);
        result.emplace_back(reduce(subStr));
    }
    return result;
}

std::vector<float> TextParse::strToFloatVector(const std::string &str,
                                               const char &delimiter) {
    std::vector<float> result{ };
    std::stringstream sStream(reduce(str));
    while (sStream.good()) {
        std::string subStr("");
        std::getline(sStream, subStr, delimiter);
        result.emplace_back(stof(reduce(subStr)));
    }
    return result;
}

std::vector<bool> TextParse::strToBoolVector(const std::string &str,
                                             const char &delimiter) {
    std::vector<bool> result{ };
    std::stringstream sStream(reduce(str));
    while (sStream.good()) {
        std::string subStr("");
        std::getline(sStream, subStr, delimiter);
        if (reduce(subStr) == "0") { result.emplace_back(false); }
        else { result.emplace_back(true); }
    }
    return result;
}

std::vector<int> TextParse::strToIntVector(const std::string &str,
                                           const char &delimiter) {
    std::vector<int> result{ };
    std::stringstream sStream(reduce(str));
    while (sStream.good()) {
        std::string subStr("");
        std::getline(sStream, subStr, delimiter);
        result.emplace_back(stoi(reduce(subStr)));
    }
    return result;
}

std::vector<std::pair<int, int>> TextParse::strToPairVector(const std::string &str,
                                                            const char &delimiter) {
    std::vector<std::pair<int, int>> result{ };
    std::stringstream sStream(reduce(str));
    while (sStream.good()) {
        std::string subStr("");
        std::getline(sStream, subStr, delimiter);
        subStr = reduce(subStr);
        result.emplace_back(strToRangePair(subStr));
    }
    return result;
}
