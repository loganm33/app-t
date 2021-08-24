#ifndef SUBPLOT_HPP
#define SUBPLOT_HPP

#include"TextParse.hpp"

#include<string>
#include<vector>

class Subplot {
public:
    Subplot();
    Subplot(const Subplot &other) = default;
    Subplot(Subplot &&other) noexcept = default;
    ~Subplot() = default;
public:
    const std::string &label() const;
    void setLabel(const std::string &newLabel);
    const std::string &title() const;
    void setTitle(const std::string &newTitle);
    const std::vector<std::string> &names() const;
    void setNames(const std::string &newNames);
    const std::vector<std::pair<int, int>> &elems() const;
    void setElems(const std::string &newElems);
    const std::vector<int> &words() const;
    void setWords(const std::string &newWords);
    const std::vector<std::pair<int, int>> &bits() const;
    void setBits(const std::string &newBits);
    const std::vector<int> &mods() const;
    void setMods(const std::string &newMods);
    const std::vector<float> &multipliers() const;
    void setMultipliers(const std::string &newMultipliers);
private:
    std::string label_;
    std::string title_;
    std::vector<std::string> names_;
    std::vector<std::pair<int, int>> elems_;
    std::vector<int> words_;
    std::vector<std::pair<int, int>> bits_;
    std::vector<int> mods_;
    std::vector<float> multipliers_;
    static const uint32_t CHAR_IDX_OF_SUBPLOT_ID;
};
#endif // SUBPLOT_H
