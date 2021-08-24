#ifndef CALPLOTDATA_HPP
#define CALPLOTDATA_HPP

#include<qcustomplot.h>
#include"PlotType.hpp"

class CalPlotData {
public:  /**< Static */
    static CalPlotData *delta(CalPlotData *calA, CalPlotData *calB) {
        CalPlotData *result = new CalPlotData;
        std::vector<std::vector<QVector<QCPGraphData>>> sData{ };
        for (size_t sIdx(0); sIdx < calA->data().size(); sIdx++) {
            std::vector<QVector<QCPGraphData>> pData{ };
            for (size_t pIdx(0); pIdx < calA->data().at(sIdx).size(); pIdx++) {
                QVector<QCPGraphData> gData{ };
                for (int gIdx(0); gIdx < calA->data().at(sIdx).at(pIdx).size(); gIdx++) {
                    gData.push_back({ });
                    gData[gIdx].key = gIdx;
                    gData[gIdx].value = calA->data().at(sIdx).at(pIdx).at(gIdx).value -
                                        calB->data().at(sIdx).at(pIdx).at(gIdx).value;
                }
                pData.push_back(gData);
            }
            sData.push_back(pData);
        }
        result->readFile(sData);
        return result;
    }
    template<typename Numeric>
    static QVector<Numeric> getElems(const QVector<Numeric> &allData,
                                     const int &firstElem,
                                     const int &lastElem) {
        QVector<Numeric> result(allData);
        result.erase(result.begin() + lastElem + 1, result.end());
        result.erase(result.begin(), result.begin() + firstElem);
        return result;
    }
    template<typename Numeric>
    static QVector<Numeric> getWords(const QVector<Numeric> &allData,
                                     const int &wordNum,
                                     const int &totalWords) {
        QVector<Numeric> result{ };
        int curWord(1);
        for (Numeric elem : allData) {
            if (curWord == wordNum) { result.push_back(elem); }
            if (curWord == totalWords) { curWord = 1; }
            else { curWord++; }
        }
        return result;
    }
    static QVector<QCPGraphData> setElemsToZero(const QVector<QCPGraphData> &dataIn,
                                                const int &startIdx,
                                                const int &endIdx) {
        QVector<QCPGraphData> result(dataIn);
        for (int elemIdx(startIdx); elemIdx <= endIdx; elemIdx++) {
            result[elemIdx].value = 0;
        }
        return result;
    }
public:  /**< Special */
    CalPlotData();
    CalPlotData(QFile *calFile, PlotType *plotTypeIn);
    CalPlotData(const CalPlotData &other) = default;
    CalPlotData(CalPlotData &&other) noexcept = default;
    ~CalPlotData() = default;
public:  /**< Accessors */
    PlotType *plotType() const;
    void setPlotType(PlotType *newPlotType);
    const std::vector<std::vector<QVector<QCPGraphData>>> &data() const;
    const int &numPnts() const;
    void setNumPnts();
    void setNumPnts(const int &newNumPnts);
    void setData(const std::vector<std::vector<QVector<QCPGraphData>>> &newData);
    void readFile(const std::vector<std::vector<QVector<QCPGraphData>>> &newData);
    void readFile(QFile *calFile);
private:  /**< Attributes */
    PlotType *plotType_;
    std::vector<std::vector<QVector<QCPGraphData>>> data_;
    int numPnts_;
    QVector<int> readAllFileData(QFile *calFile) const;
    int getBits(const int &value, const int &beginBit, const int &endBit);
};
#endif // CALPLOTDATA_HPP
