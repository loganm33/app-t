#include "CalPlotData.hpp"

CalPlotData::CalPlotData()
    : plotType_(nullptr)
    , data_{ } {}

CalPlotData::CalPlotData(QFile *calFile_arg, PlotType *plotType_arg)
    : CalPlotData() {
    setPlotType(plotType_arg);
    readFile(calFile_arg);
}

PlotType *CalPlotData::plotType() const { return plotType_; }

void CalPlotData::setPlotType(PlotType *newPlotType) { plotType_ = newPlotType; }

const std::vector<std::vector<QVector<QCPGraphData>>> &CalPlotData::data() const {
    return data_;
}

void CalPlotData::setData(const std::vector<std::vector<QVector<QCPGraphData>>> &newData) {
    data_ = newData;
}

void CalPlotData::readFile(const std::vector<std::vector<QVector<QCPGraphData>>> &newData) {
    data_ = newData;
}

const int &CalPlotData::numPnts() const { return numPnts_; }

void CalPlotData::setNumPnts() {
    setNumPnts(data().size());
}  // TODO Implement

void CalPlotData::setNumPnts(const int &newNumPnts) { numPnts_ = newNumPnts; }

void CalPlotData::readFile(QFile *file) {
    QVector<int> allData(readAllFileData(file));
    for (int sIdx(0); sIdx < plotType()->numSubplots(); sIdx++) {
        std::vector<QVector<QCPGraphData>> paramsData{ };
        Subplot *sPlt = plotType()->subplots().at(sIdx);
        int numWords(1);
        for (int wordNum : sPlt->words()) {
            if (wordNum > numWords) { numWords = wordNum; }
        }
        for (size_t pIdx(0);
             pIdx < plotType()->subplots().at(sIdx)->elems().size();
             pIdx++) {
            QVector<int> pts_int = getElems<int>(allData,
                                                 sPlt->elems().at(pIdx).first,
                                                 sPlt->elems().at(pIdx).second);
            if (numWords > 1) {
                pts_int = getWords<int>(pts_int, sPlt->words().at(pIdx), numWords);
            }
            QVector<QCPGraphData> pts_qcp{ };
            for (int i(0); i < pts_int.size(); i++) {
                int val_int = getBits(pts_int.at(i),
                                      sPlt->bits().at(pIdx).second,
                                      sPlt->bits().at(pIdx).first);
                if (sPlt->mods().at(pIdx) == 512) { val_int = (uint8_t)val_int; }
                double val = (double)val_int;
                if ((sPlt->multipliers().at(pIdx) > (1 + 0.01)) ||
                    (sPlt->multipliers().at(pIdx) < (1 - 0.01))) {
                    val = val * sPlt->multipliers().at(pIdx);
                }
                pts_qcp.push_back(QCPGraphData((double)i, val));
            }
            paramsData.push_back(pts_qcp);
        }
        data_.push_back(paramsData);
    }
}

QVector<int> CalPlotData::readAllFileData(QFile *calFile) const {
    QVector<int> result{ };
    if (!calFile->open(QIODevice::ReadOnly)) { return result; }
    QTextStream fStream(calFile);
    bool firstLine(true);
    while (!fStream.atEnd()) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        QString line(fStream.readLine());
        QStringList lineDataList(line.split(QRegularExpression("\\s+")));
        for (int index(0); index < lineDataList.size(); index++) {
            if (index < 2) { continue; }
            result.push_back(lineDataList.at(index).toInt());
        }
    }
    calFile->close();
    result = getElems<int>(result, 5, (result.size() - 1));
    return result;
}

int CalPlotData::getBits(const int &value,
                         const int &beginBit,
                         const int &endBit) {
    struct int15_t {
        int16_t value : 15;
        int16_t _     : 1;
    };
    struct int14_t {
        int16_t value : 14;
        int16_t _     : 2;
    };
    struct int13_t {
        int16_t value : 13;
        int16_t _     : 3;
    };
    struct int12_t {
        int16_t value : 12;
        int16_t _     : 4;
    };
    struct int11_t {
        int16_t value : 11;
        int16_t _     : 5;
    };
    struct int10_t {
        int16_t value : 10;
        int16_t _     : 6;
    };
    struct int9_t {
        int16_t value : 9;
        int16_t _     : 7;
    };
    struct int7_t {
        int8_t value : 7;
        int8_t _     : 1;
    };
    struct int6_t {
        int8_t value : 6;
        int8_t _     : 2;
    };
    struct int5_t {
        int8_t value : 5;
        int8_t _     : 3;
    };
    struct int4_t {
        int8_t value : 4;
        int8_t _     : 4;
    };
    struct int3_t {
        int8_t value : 3;
        int8_t _     : 5;
    };
    struct int2_t {
        int8_t value : 2;
        int8_t _     : 6;
    };
    struct int1_t {
        int8_t value : 1;
        int8_t _     : 7;
    };
    int numBits = endBit - beginBit + 1;
    int rawData = (((1 << numBits) - 1) & (value >> beginBit));
    switch (numBits) {
        case 16: return (int16_t)rawData;
        case 15:
            int15_t data_15b;
            data_15b.value = rawData;
            return data_15b.value;
        case 14:
            int14_t data_14b;
            data_14b.value = rawData;
            return data_14b.value;
        case 13:
            int13_t data_13b;
            data_13b.value = rawData;
            return data_13b.value;
        case 12:
            int12_t data_12b;
            data_12b.value = rawData;
            return data_12b.value;
        case 11:
            int11_t data_11b;
            data_11b.value = rawData;
            return data_11b.value;
        case 10:
            int10_t data_10b;
            data_10b.value = rawData;
            return data_10b.value;
        case 9:
            int9_t data_9b;
            data_9b.value = rawData;
            return data_9b.value;
        case 8: return (int8_t)rawData;
        case 7:
            int7_t data_7b;
            data_7b.value = rawData;
            return data_7b.value;
        case 6:
            int6_t data_6b;
            data_6b.value = rawData;
            return data_6b.value;
        case 5:
            int5_t data_5b;
            data_5b.value = rawData;
            return data_5b.value;
        case 4:
            int4_t data_4b;
            data_4b.value = rawData;
            return data_4b.value;
        case 3:
            int3_t data_3b;
            data_3b.value = rawData;
            return data_3b.value;
        case 2:
            int2_t data_2b;
            data_2b.value = rawData;
            return data_2b.value;
        case 1:
            int1_t data_1b;
            data_1b.value = rawData;
            return data_1b.value;
        default: return rawData;
    }
}
