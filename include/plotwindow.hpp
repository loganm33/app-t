#ifndef PLOTWINDOW_HPP
#define PLOTWINDOW_HPP

#include"CalPlotData.hpp"
#include"qcustomplot.h"
#include"PlotType.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PlotWindow; }
QT_END_NAMESPACE

class PlotWindow : public QMainWindow {
    Q_OBJECT
public:
    static const std::vector<QPen> PENS;
    PlotWindow();
    explicit PlotWindow(QWidget *parent,
                        QFile *calFileIn,
                        QFile *refFileIn,
                        PlotType *plotTypeIn);
    ~PlotWindow();
    Ui::PlotWindow *ui() const;
    void setUi(Ui::PlotWindow *newUi);
    const PlotType *plotType() const;
    void setPlotType(PlotType *newPlotType);
    CalPlotData *cal() const;
    void setCal(CalPlotData *newCal);
    CalPlotData *ref() const;
    void setRef(CalPlotData *newRef);
    CalPlotData *delta() const;
    void setDelta(CalPlotData *newDelta);
private Q_SLOTS:
    void contextMenuRequest(QPoint pos);
    void saveGraph();
    void graphDoubleClicked(QCPAbstractPlottable *plottable, int dataIdx);
    void on_plotType_cBox_currentIndexChanged(const int &index);
    void on_xRange_cBox_currentIndexChanged(const int &index);
    void on_yZoom_cBox_currentIndexChanged(const int &index);
    void on_yOffset_lineEdit_textEdited(const QString &arg1);
private:
    Ui::PlotWindow *ui_;
    PlotType *plotType_;
    CalPlotData *cal_;
    CalPlotData *ref_;
    CalPlotData *delta_;
    void connectSignals();
    void setModels();
    void clearPlot();
    void setSubplots();
    void updatePlot();
    void setTitles();
    void setLegend();
    void setGraphData();
    void setAxes();
    int gIdx_to_sIdx(const int &gIdx);
    int gIdx_to_pIdx(const int &gIdx);
    int s_pIdx_to_gIdx(const int &sIdx, const int &pIdx);
    std::vector<QCPAxisRect *> getAxRects(const int &count);
};
#endif // PLOTWINDOW_HPP
