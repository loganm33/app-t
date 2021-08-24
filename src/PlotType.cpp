#include "PlotType.hpp"

PlotType::PlotType()
    : label_("")
    , name_("")
    , subplots_{ }
    , position_{ }
    , size_{ }
    , legend_{ }
    , xRangeNames_{ }
    , xRangeLocs_{ }
    , yRangeData_{ }
    , yRangeDiff_{ } {
    setCheckable(true);
    setCheckState(Qt::CheckState::Unchecked);
    setEditable(false);
}

const std::string &PlotType::label() const { return label_; }

void PlotType::setLabel(const std::string &newLabel) { label_ = newLabel; }

const std::string &PlotType::name() const { return name_; }

void PlotType::setName(const std::string &newName) {
    name_ = newName;
    setText(name_.c_str());
}

const std::vector<Subplot *> &PlotType::subplots() const { return subplots_; }

void PlotType::setSubplots(const std::string &newSubplots,
                           const std::vector<Subplot *> &subplotDefs) {
    subplots_.clear();
    for (std::string subplotLabel : TextParse::strToStrVector(newSubplots)) {
        for (Subplot *subplotDef : subplotDefs) {
            if (subplotLabel == subplotDef->label()) {
                subplots_.emplace_back(subplotDef);
            }
        }
    }
}

void PlotType::setSubplots(const std::vector<Subplot *> &newSubplots) {
    subplots_ = newSubplots;
}

const std::vector<std::pair<int,int>> &PlotType::position() const {
    return position_;
}

void PlotType::setPosition(const std::string &newPosition) {
    setPosition(TextParse::strToPairVector(newPosition));
}

void PlotType::setPosition(const std::vector<std::pair<int,int>> &newPosition) {
    position_ = newPosition;
}

const std::vector<std::pair<int,int>> &PlotType::size() const { return size_; }

void PlotType::setSize(const std::string &size) {
    setSize(TextParse::strToPairVector(size));
}

void PlotType::setSize(const std::vector<std::pair<int,int>> &newSize) {
    size_ = newSize;
}

const std::vector<bool> &PlotType::legend() const { return legend_; }

void PlotType::setLegend(const std::string &legend) {
    setLegend(TextParse::strToBoolVector(legend));
}

void PlotType::setLegend(const std::vector<bool> &newLegend) {
    legend_ = newLegend;
}

const std::vector<std::string> &PlotType::xRangeNames() const {
    return xRangeNames_;
}

const std::vector<std::vector<std::pair<int,int>>> &PlotType::xRangeLocs() const {
    return xRangeLocs_;
}

void PlotType::setXRangeLocs(const std::vector<std::vector<std::pair<int,int>>> &newXRangeLocs) {
    xRangeLocs_ = newXRangeLocs;
}

const std::vector<std::pair<int,int>> &PlotType::yRangeData() const {
    return yRangeData_;
}

void PlotType::setYRangeData(const std::vector<std::pair<int,int>> &newYRangeData) {
    yRangeData_ = newYRangeData;
}

const std::vector<std::pair<int,int>> &PlotType::yRangeDiff() const {
    return yRangeDiff_;
}

void PlotType::setYRangeDiff(const std::vector<std::pair<int,int>> &newYRangeDiff) {
    yRangeDiff_ = newYRangeDiff;
}

void PlotType::setXRangeNames(const std::string &xRangeNames) {
    setXRangeNames(TextParse::strToStrVector(xRangeNames));
}

void PlotType::setXRangeNames(const std::vector<std::string> &xRangeNames) {
    xRangeNames_ = xRangeNames;
}

void PlotType::setXRangeLocs(const std::string &xRangeLocs) {
    xRangeLocs_ = { };
    size_t start;
    size_t end(0);
    std::vector<std::string> commaSepList{ };
    while ((start = xRangeLocs.find_first_not_of(',', end)) != std::string::npos) {
        end = xRangeLocs.find(',', start);
        commaSepList.emplace_back(xRangeLocs.substr(start, end - start));
    }
    std::vector<std::vector<std::string>> result{ };
    for (std::string loc : commaSepList) {
        size_t startB;
        size_t endB(0);
        std::vector<std::string> colonSepList{ };
        while ((startB = loc.find_first_not_of(':', endB)) != std::string::npos) {
            endB = loc.find(':', startB);
            colonSepList.emplace_back(loc.substr(startB, endB - startB));
        }
        result.push_back(colonSepList);
    }
    std::vector<std::vector<std::pair<int,int>>> result_pair{ };
    for (size_t xRangeIdx(0); xRangeIdx < result.size(); xRangeIdx++) {
        std::vector<std::pair<int,int>> newXSet{ };
        for (size_t xLocIdx(0); xLocIdx < (result.at(xRangeIdx).size() - 1); xLocIdx += 2) {
            int first = stoi(result.at(xRangeIdx).at(xLocIdx));
            int second = stoi(result.at(xRangeIdx).at(xLocIdx + 1));
            newXSet.emplace_back(std::make_pair(first, second));
        }
        result_pair.emplace_back(newXSet);
    }
    xRangeLocs_ = result_pair;
}

void PlotType::setYRangeData(const std::string &yRangeData) {
    setYRangeData(TextParse::strToPairVector(yRangeData));
}

void PlotType::setYRangeDiff(const std::string &yRangeDiff) {
    setYRangeDiff(TextParse::strToPairVector(yRangeDiff));
}

const int &PlotType::totalGraphs() const { return totalGraphs_; }

void PlotType::setTotalGraphs() {
    int graphCount(0);
    for (size_t sIdx(0); sIdx < subplots().size(); sIdx++) {
        graphCount += subplots().at(sIdx)->elems().size();
    }
    setTotalGraphs(graphCount);
}

void PlotType::setTotalGraphs(const int &newTotalGraphs) {
    totalGraphs_ = newTotalGraphs;
}

const int &PlotType::numSubplots() const { return numSubplots_; }

void PlotType::setNumSubplots() { setNumSubplots(subplots().size()); }

void PlotType::setNumSubplots(const int &newNumSubplots) {
    numSubplots_ = newNumSubplots;
}

const std::vector<int> PlotType::numParams() const { return numParams_; }

void PlotType::setNumParams() {
    std::vector<int> paramNums{ };
    for (size_t sIdx(0); sIdx < subplots().size(); sIdx++) {
        paramNums.emplace_back(subplots().at(sIdx)->elems().size());
    }
    setNumParams(paramNums);
}

void PlotType::setNumParams(const std::vector<int> &newNumParams) {
    numParams_ = newNumParams;
}

Subplot *PlotType::gIdx_to_subplot(const int &graphIndex) {
    int graphsPassed(graphIndex);
    for (int sIdx(0); sIdx < numSubplots(); sIdx++) {
        for (int pIdx(0); pIdx < numParams().at(sIdx); pIdx++) {
            graphsPassed--;
            if (graphsPassed < 0) { return subplots().at(sIdx); }
        }
    }
    return nullptr;
}
