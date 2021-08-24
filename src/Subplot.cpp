#include "Subplot.hpp"

const uint32_t Subplot::CHAR_IDX_OF_SUBPLOT_ID = 8;

Subplot::Subplot()
    : label_("")
    , title_("")
    , names_{ }
    , elems_{ }
    , words_{ }
    , bits_{ }
    , mods_{ }
    , multipliers_{ } {}

const std::string &Subplot::label() const { return label_; }

void Subplot::setLabel(const std::string &newLabel) {
    std::string subplotLabel = TextParse::reduce(newLabel);
    label_ = subplotLabel.substr(CHAR_IDX_OF_SUBPLOT_ID);
}

const std::string &Subplot::title() const { return title_; }

void Subplot::setTitle(const std::string &newTitle) { title_ = newTitle; }

const std::vector<std::string> &Subplot::names() const { return names_; }

void Subplot::setNames(const std::string &newNames) {
    names_ = TextParse::strToStrVector(newNames);
}

const std::vector<std::pair<int,int>> &Subplot::elems() const { return elems_; }

void Subplot::setElems(const std::string &newElems) {
    std::vector<std::pair<int,int>> newElems_pairVec{ };
    newElems_pairVec = TextParse::strToPairVector(newElems);
    size_t idx(0);
    while (idx < newElems_pairVec.size()) {
        if (elems().at(idx).first > elems().at(idx).second) { return; }
        else if ((elems().at(idx).first < 0) ||
                 (elems().at(idx).second < 0)) { return; }
        idx++;
    }
}

const std::vector<int> &Subplot::words() const { return words_; }

void Subplot::setWords(const std::string &newWords) {
    words_ = TextParse::strToIntVector(newWords);
}

const std::vector<std::pair<int,int>> &Subplot::bits() const { return bits_; }

void Subplot::setBits(const std::string &newBits) {
    bits_ = TextParse::strToPairVector(newBits);
}

const std::vector<int> &Subplot::mods() const { return mods_; }

void Subplot::setMods(const std::string &newMods) {
    mods_ = TextParse::strToIntVector(newMods);
}

const std::vector<float> &Subplot::multipliers() const { return multipliers_; }

void Subplot::setMultipliers(const std::string &newMultipliers) {
    multipliers_ = TextParse::strToFloatVector(newMultipliers);
}
