#ifndef PLOTTYPE_HPP
#define PLOTTYPE_HPP

#include"Subplot.hpp"

#include<string>
#include<vector>

#include<QStandardItem>

class PlotType : QStandardItem {
public:  /**< Special */
    PlotType();
    PlotType(const PlotType &other) = default;
    PlotType(PlotType &&other) noexcept = default;
    ~PlotType() = default;
public:  /**< Accessors */
    const std::string &label() const;
    void setLabel(const std::string &newLabel);
    const std::string &name() const;
    void setName(const std::string &newName);
    const std::vector<Subplot *> &subplots() const;
    void setSubplots(const std::vector<Subplot *> &newSubplots);
    void setSubplots(const std::string &newSubplots,
                     const std::vector<Subplot *> &subplotDefs);
    const std::vector<std::pair<int,int>> &position() const;
    void setPosition(const std::string &newPosition);
    void setPosition(const std::vector<std::pair<int,int>> &newPosition);
    const std::vector<std::pair<int,int>> &size() const;
    void setSize(const std::vector<std::pair<int,int>> &newSize);
    void setSize(const std::string &newSize);
    const std::vector<bool> &legend() const;
    void setLegend(const std::vector<bool> &newLegend);
    void setLegend(const std::string &newLegend);
    const std::vector<std::string> &xRangeNames() const;
    void setXRangeNames(const std::vector<std::string> &newXRangeNames);
    void setXRangeNames(const std::string &newXRangeNames);
    const std::vector<std::vector<std::pair<int,int>>> &xRangeLocs() const;
    void setXRangeLocs(const std::string &newXRangeLocs);
    void setXRangeLocs(const std::vector<std::vector<std::pair<int,int>>> &newXRangeLocs);
    const std::vector<std::pair<int,int>> &yRangeData() const;
    void setYRangeData(const std::string &newYRangeData);
    void setYRangeData(const std::vector<std::pair<int,int>> &newYRangeData);
    const std::vector<std::pair<int,int>> &yRangeDiff() const;
    void setYRangeDiff(const std::string &newYRangeDiff);
    void setYRangeDiff(const std::vector<std::pair<int,int>> &newYRangeDiff);
    const int &totalGraphs() const;
    void setTotalGraphs();
    void setTotalGraphs(const int &newTotalGraphs);
    const int &numSubplots() const;
    void setNumSubplots();
    void setNumSubplots(const int &newNumSubplots);
    const std::vector<int> numParams() const;
    void setNumParams();
    void setNumParams(const std::vector<int> &newNumParams);
    Subplot *gIdx_to_subplot(const int &gIdx);
private:  /**< Attributes */
    std::string label_;
    std::string name_;
    std::vector<Subplot *> subplots_;
    std::vector<std::pair<int,int>> position_;
    std::vector<std::pair<int,int>> size_;
    std::vector<bool> legend_;
    std::vector<std::string> xRangeNames_;
    std::vector<std::vector<std::pair<int,int>>> xRangeLocs_;
    std::vector<std::pair<int,int>> yRangeData_;
    std::vector<std::pair<int,int>> yRangeDiff_;
    int totalGraphs_;
    int numSubplots_;
    std::vector<int> numParams_;
};
#endif // PLOTTYPE_HPP
