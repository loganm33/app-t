#ifndef TEXTPARSE_HPP
#define TEXTPARSE_HPP

#include<sstream>
#include<string>

#include<QTextStream>
#include<QString>
#include<QList>

namespace TextParse {
    std::string trim(const std::string &str,
                     const std::string &whitespace = " \t");
    std::string reduce(const std::string &str,
                       const std::string &fill = " ",
                       const std::string &whitespace = " \t");
    std::string removeComments(const std::string &str,
                               const char &delimiter = '#');
    QString removeFileExtension(const QString &fileName);
    QStringList removeFileExtensions(const QStringList &fileNames);
    std::pair<int, int> strToRangePair(const std::string &str,
                                       const char &delimiter = ':');
    std::vector<std::string> strToStrVector(const std::string &str,
                                            const char &delimiter = ',');
    std::vector<float> strToFloatVector(const std::string &str,
                                        const char &delimiter = ',');
    std::vector<bool> strToBoolVector(const std::string &str,
                                      const char &delimiter = ',');
    std::vector<int> strToIntVector(const std::string &str,
                                    const char &delimiter = ',');
    std::vector<std::pair<int, int>> strToPairVector(const std::string &str,
                                                     const char &delimiter = ',');

};
#endif // TEXTPARSE_HPP
