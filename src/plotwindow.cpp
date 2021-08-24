#include "plotwindow.hpp"
#include "./ui_plotwindow.h"

const std::vector<QPen> PlotWindow::PENS = {QPen(QColor(0x0000FF), 1.5),
                                            QPen(QColor(0x008000), 1.5),
                                            QPen(QColor(0xFF0000), 1.5),
                                            QPen(QColor(0x808000), 1.5),
                                            QPen(QColor(0x8000FF), 1.5),
                                            QPen(QColor(0xFF8000), 1.5),
                                            QPen(QColor(0x00FF00), 1.5),
                                            QPen(QColor(0xFF00FF), 1.5),
                                            QPen(QColor(0x800000), 1.5),
                                            QPen(QColor(0x0080FF), 1.5),
                                            QPen(QColor(0x808080), 1.5),
                                            QPen(QColor(0x8080FF), 1.5),
                                            QPen(QColor(0x408080), 1.5),};

PlotWindow::PlotWindow()
    : QMainWindow(nullptr)
    , ui_(new Ui::PlotWindow)
    , plotType_(nullptr)
    , cal_(nullptr)
    , ref_(nullptr)
    , delta_(nullptr) {}

PlotWindow::PlotWindow(QWidget *parent,
                       QFile *calFileIn,
                       QFile *refFileIn,
                       PlotType *plotTypeIn)
    : QMainWindow(parent)
    , ui_(new Ui::PlotWindow)
    , plotType_(plotTypeIn)
    , cal_(nullptr)
    , ref_(nullptr)
    , delta_(nullptr) {
    ui_->setupUi(this);
    connectSignals();
    cal_ = new CalPlotData(calFileIn, plotType_);
    ref_ = new CalPlotData(refFileIn, plotType_);
    delta_ = CalPlotData::delta(cal_, ref_);
    setModels();
    QString calFileNameOnly(QFileInfo(calFileIn->fileName()).fileName());
    ui_->calFileName_label->setText(calFileNameOnly);
    QString refFileNameOnly(QFileInfo(refFileIn->fileName()).fileName());
    ui_->refFileName_label->setText(refFileNameOnly);
    clearPlot();
    setSubplots();
    setTitles();
    setLegend();
    updatePlot();
}

PlotWindow::~PlotWindow() { delete ui_; }

Ui::PlotWindow *PlotWindow::ui() const { return ui_; }

void PlotWindow::setUi(Ui::PlotWindow *newUi) { ui_ = newUi; }

const PlotType *PlotWindow::plotType() const { return plotType_; }

void PlotWindow::setPlotType(PlotType *newPlotType) {
    plotType_ = newPlotType;
}

CalPlotData *PlotWindow::cal() const { return cal_; }

void PlotWindow::setCal(CalPlotData *newCal) { cal_ = newCal; }

CalPlotData *PlotWindow::ref() const { return ref_; }

void PlotWindow::setRef(CalPlotData *newRef) { ref_ = newRef; }

CalPlotData *PlotWindow::delta() const { return delta_; }

void PlotWindow::setDelta(CalPlotData *newDelta) { delta_ = newDelta; }

void PlotWindow::contextMenuRequest(QPoint pos) {
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QCPAxisRect *arSelected = nullptr;
    for (QCPAxisRect *ar : ui_->qPlot->axisRects()) {
        if (ar->rect().contains(pos)) { arSelected = ar; }
        else { continue; }
    }
    // TODO Finish implementation
    if (arSelected != nullptr) {
        menu->addAction("Save graph", this, SLOT(saveGraph()));
    }
    menu->popup(ui_->qPlot->mapToGlobal(pos));
}

void PlotWindow::saveGraph() { return; }

void PlotWindow::graphDoubleClicked(QCPAbstractPlottable *plottable, int dataIdx) {
    return;
}

void PlotWindow::on_plotType_cBox_currentIndexChanged(const int &index) {
    if ((index > 2) || (index < 0)) { return; }
    else { updatePlot(); }
}

void PlotWindow::on_xRange_cBox_currentIndexChanged(const int &index) {
    if (((size_t)index >= plotType()->xRangeLocs().size()) || (index < 0)) {
        return;
    } else { updatePlot(); }
}

void PlotWindow::on_yZoom_cBox_currentIndexChanged(const int &index) {
    // TODO Make max index relative
    if ((index < 0) || (index > 8)) { return; }
    else { updatePlot(); }
}

void PlotWindow::on_yOffset_lineEdit_textEdited(const QString &arg1) {
    if (arg1 == "") { return; }
    else { updatePlot(); }
}

void PlotWindow::connectSignals() {
    ui_->qPlot->setInteractions(QCP::iSelectPlottables | QCP::iSelectAxes);
    ui_->qPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui_->qPlot, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequest(QPoint)));
    connect(ui_->qPlot, SIGNAL(plottableDoubleClick(QCPAbstractPlottable *, int, QMouseEvent *)),
            this, SLOT(graphDoubleClicked(QCPAbstractPlottable *, int)));
}

void PlotWindow::setModels() {
    QStringList xNames{ };
    for (const std::string &xName : plotType()->xRangeNames()) {
        xNames.push_back(xName.c_str());
    }
    ui_->xRange_cBox->blockSignals(true);
    ui_->xRange_cBox->setModel(new QStringListModel(xNames));
    ui_->xRange_cBox->setCurrentIndex(0);
    ui_->xRange_cBox->blockSignals(false);
    std::vector<double> yZooms{0.1, 0.2, 0.25, 0.5, 1.0, 2.0, 4.0, 5.0, 10.0};
    QStringList yZooms_str{ };
    for (double yZoom : yZooms) {
        QString cStyleResult = QString("%1").arg(yZoom, 5);
        yZooms_str.push_back(cStyleResult);
    }
    ui_->yZoom_cBox->blockSignals(true);
    ui_->yZoom_cBox->setModel(new QStringListModel(yZooms_str));
    ui_->yZoom_cBox->setEditable(false);
    ui_->yZoom_cBox->setCurrentIndex(4);
    ui_->yZoom_cBox->blockSignals(false);
    ui_->yOffset_lineEdit->blockSignals(true);
    ui_->yOffset_lineEdit->setText("0.0");
    ui_->yOffset_lineEdit->blockSignals(false);
    setWindowTitle(plotType()->name().c_str());
}

void PlotWindow::clearPlot() {
    ui_->qPlot->plotLayout()->clear();
    ui_->qPlotLegend->plotLayout()->clear();
}

void PlotWindow::setSubplots() {
    size_t numSubplots = plotType()->subplots().size();
    std::vector<QCPAxisRect *> axRects = getAxRects(numSubplots);
    for (size_t sIdx(0); sIdx < axRects.size(); sIdx++) {
        for (size_t pIdx(0);
             pIdx < plotType()->subplots().at(sIdx)->elems().size();
             pIdx++) {
            QCPGraph *newGraph = ui_->qPlot->addGraph(axRects.at(sIdx)->axis(QCPAxis::atBottom),
                                                     axRects.at(sIdx)->axis(QCPAxis::atLeft));
            newGraph->setName(plotType()->subplots().at(sIdx)->names().at(pIdx).c_str());
            newGraph->setLineStyle(QCPGraph::lsLine);
            newGraph->setPen(PENS.at(pIdx));
        }
        for (QCPAxis *axis : axRects.at(sIdx)->axes()) {
            axis->setLayer("axes");
            axis->grid()->setLayer("grid");
        }
    }
    switch (numSubplots) {
        case 1:
            ui_->qPlot->plotLayout()->addElement(0, 0, axRects.at(0));
            break;
        case 2:
            ui_->qPlot->plotLayout()->addElement(0, 0, axRects.at(0));
            ui_->qPlot->plotLayout()->addElement(1, 0, axRects.at(1));
            break;
        case 3:
            ui_->qPlot->plotLayout()->addElement(0, 0, axRects.at(0));
            ui_->qPlot->plotLayout()->addElement(0, 1, axRects.at(1));
            ui_->qPlot->plotLayout()->addElement(1, 0, axRects.at(2));
            break;
        case 4:
            ui_->qPlot->plotLayout()->addElement(0, 0, axRects.at(0));
            ui_->qPlot->plotLayout()->addElement(0, 1, axRects.at(1));
            ui_->qPlot->plotLayout()->addElement(1, 0, axRects.at(2));
            ui_->qPlot->plotLayout()->addElement(1, 1, axRects.at(3));
            break;
        default: return;
    }
}

void PlotWindow::updatePlot() {
    setGraphData();
    setAxes();
    ui_->qPlot->replot();
}

void PlotWindow::setTitles() {
    std::vector<QCPTextElement *> subplotTitles{ };
    for (size_t idx(0); idx < plotType()->subplots().size(); idx++) {
        QCPTextElement *title = new QCPTextElement(ui_->qPlot);
        title->setText(plotType()->subplots().at(idx)->title().c_str());
        title->setFont(QFont("sans", 10, QFont::Bold));
        subplotTitles.push_back(title);
    }
    switch (subplotTitles.size()) {
        case 1:
            ui_->qPlot->plotLayout()->insertRow(0);
            ui_->qPlot->plotLayout()->addElement(0, 0, subplotTitles.at(0));
            break;
        case 2:
            ui_->qPlot->plotLayout()->insertRow(0);
            ui_->qPlot->plotLayout()->addElement(0, 0, subplotTitles.at(0));
            ui_->qPlot->plotLayout()->insertRow(2);
            ui_->qPlot->plotLayout()->addElement(2, 0, subplotTitles.at(1));
            break;
        case 3:
            ui_->qPlot->plotLayout()->insertRow(0);
            ui_->qPlot->plotLayout()->addElement(0, 0, subplotTitles.at(0));
            ui_->qPlot->plotLayout()->addElement(0, 1, subplotTitles.at(1));
            ui_->qPlot->plotLayout()->insertRow(2);
            ui_->qPlot->plotLayout()->addElement(2, 0, subplotTitles.at(2));
            break;
        case 4:
            ui_->qPlot->plotLayout()->insertRow(0);
            ui_->qPlot->plotLayout()->addElement(0, 0, subplotTitles.at(0));
            ui_->qPlot->plotLayout()->addElement(0, 1, subplotTitles.at(1));
            ui_->qPlot->plotLayout()->insertRow(2);
            ui_->qPlot->plotLayout()->addElement(2, 0, subplotTitles.at(2));
            ui_->qPlot->plotLayout()->addElement(2, 1, subplotTitles.at(3));
            break;
        default:
            break;
    }
}

void PlotWindow::setLegend() {
    int numLegendItems(0);
    for (size_t sIdx(0);
         sIdx < plotType()->subplots().size();
         sIdx++) {
        if (plotType()->legend().at(sIdx)) {
            QCPLegend *legend = new QCPLegend;
            ui_->qPlotLegend->plotLayout()->addElement(legend);
            legend->setLayer("legend");
            for (size_t pIdx(0);
                 pIdx < plotType()->subplots().at(sIdx)->elems().size();
                 pIdx++) {
                numLegendItems++;
                legend->addItem(new QCPPlottableLegendItem(legend,
                                                           ui_->qPlot->graph(s_pIdx_to_gIdx(sIdx,
                                                                                           pIdx))));
            }
        }
    }
    if (numLegendItems) {
        ui_->qPlotLegend->setVisible(true);
        ui_->qPlotLegend->setBackground(Qt::GlobalColor::transparent);
        ui_->qPlotLegend->setAttribute(Qt::WA_OpaquePaintEvent, false);
        ui_->qPlotLegend->plotLayout()->elementAt(0)->
                setMaximumSize(50, (numLegendItems * 20));
        ui_->qPlotLegend->setMaximumWidth(100);
    } else { ui_->qPlotLegend->setVisible(false); }
}

void PlotWindow::setGraphData() {
    int plotTypeIdx = ui_->plotType_cBox->currentIndex();
    for (int gIdx(0); gIdx < ui_->qPlot->graphCount(); gIdx++) {
        int sIdx = gIdx_to_sIdx(gIdx);
        int pIdx = gIdx_to_pIdx(gIdx);
        switch (plotTypeIdx) {
        case 0:
            ui_->qPlot->graph(gIdx)->data()->set(delta_->data().at(sIdx).at(pIdx));
            break;
        case 1:
            ui_->qPlot->graph(gIdx)->data()->set(cal_->data().at(sIdx).at(pIdx));
            break;
        case 2:
            ui_->qPlot->graph(gIdx)->data()->set(ref_->data().at(sIdx).at(pIdx));
            break;
        default: return;
        }
    }
}

void PlotWindow::setAxes() {
    for (int gIdx(0); gIdx < ui_->qPlot->graphCount(); gIdx++) {
        int sIdx = gIdx_to_sIdx(gIdx);
        int xRangeIdx = ui_->xRange_cBox->currentIndex();
        double xLower = plotType()->xRangeLocs().at(xRangeIdx).at(0).first;
        double xUpper = plotType()->xRangeLocs().at(xRangeIdx).at(0).second;
        ui_->qPlot->graph(gIdx)->keyAxis()->setRange(xLower, xUpper);
        double yZoom = ui_->yZoom_cBox->currentText().toDouble();
        double yOffset = ui_->yOffset_lineEdit->text().toDouble();
        if (ui_->plotType_cBox->currentIndex() == 0) {
            double yLower = ((plotType()->yRangeDiff().at(sIdx).first / yZoom)
                            + yOffset);
            double yUpper = ((plotType()->yRangeDiff().at(sIdx).second / yZoom)
                            + yOffset);
            ui_->qPlot->graph(gIdx)->valueAxis()->setRange(yLower, yUpper);
        } else {
            double yLower = ((plotType()->yRangeData().at(sIdx).first / yZoom)
                            + yOffset);
            double yUpper = ((plotType()->yRangeData().at(sIdx).second / yZoom)
                            + yOffset);
            ui_->qPlot->graph(gIdx)->valueAxis()->setRange(yLower, yUpper);
        }
    }
}

int PlotWindow::gIdx_to_sIdx(const int &gIdx) {
    int gTotal(0);
    for (size_t sIdx(0); sIdx < plotType()->subplots().size(); sIdx++) {
        gTotal += plotType()->subplots().at(sIdx)->elems().size();
        if (gTotal > gIdx) { return sIdx; }
        else { continue; }
    }
    return -1;
}

int PlotWindow::gIdx_to_pIdx(const int &gIdx) {
    int gTotal(0);
    for (size_t sIdx(0); sIdx < plotType()->subplots().size(); sIdx++) {
        for (size_t pIdx(0);
             pIdx < plotType()->subplots().at(sIdx)->elems().size();
             pIdx++) {
            if (++gTotal > gIdx) { return pIdx; }
            else { continue; }
        }
    }
    return -1;
}

int PlotWindow::s_pIdx_to_gIdx(const int &sIdxArg, const int &pIdxArg) {
    int gCount(0);
    for (size_t sIdx(0); sIdx < plotType()->subplots().size(); sIdx++) {
        for (size_t pIdx(0);
             pIdx < plotType()->subplots().at(sIdx)->elems().size();
             pIdx++) {
            if (((int)sIdx == sIdxArg) && ((int)pIdx == pIdxArg)) { return gCount; }
            gCount++;
        }
    }
    return -1;
}

std::vector<QCPAxisRect *> PlotWindow::getAxRects(const int &numSubplots) {
    std::vector<QCPAxisRect *> result{ };
    for (int count(0); count < numSubplots; count++) {
        QCPAxisRect *newAxisRect = new QCPAxisRect(ui_->qPlot, false);
        newAxisRect->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
        newAxisRect->axis(QCPAxis::atLeft)->ticker()->setTickCount(4);
        newAxisRect->axis(QCPAxis::atBottom)->ticker()->setTickCount(4);
        newAxisRect->axis(QCPAxis::atLeft)->grid()->setVisible(true);
        newAxisRect->axis(QCPAxis::atBottom)->grid()->setVisible(true);
        result.push_back(newAxisRect);
    }
    return result;
}
