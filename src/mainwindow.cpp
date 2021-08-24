#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , prefs_(preferences())
    , ui_(new Ui::MainWindow)
    , plotWindows_{ }
    , basePath_("")
    , calFile_(new QFile)
    , refFile_(new QFile)
    , subplots_{ }
    , plotList_{ } {
    ui_->setupUi(this);
    loadSettings();
    setWindowTitle("RDT 2.1.0");
    setBasePath(QDir::currentPath());
    initType();
}

MainWindow::~MainWindow() { delete ui_; }

const MainWindow::preferences &MainWindow::prefs() const { return prefs_; }

void MainWindow::setPrefs(const preferences &newPrefs) { prefs_ = newPrefs; }

Ui::MainWindow *MainWindow::ui() const { return ui_; }

void MainWindow::setUi(Ui::MainWindow *newUi) { ui_ = newUi; }

const std::vector<PlotWindow *> &MainWindow::plotWindows() const {
    return plotWindows_;
}

void MainWindow::setPlotWindows(const std::vector<PlotWindow *> &newPlotWindows) {
    plotWindows_ = newPlotWindows;
}

const QString &MainWindow::basePath() const { return basePath_; }

void MainWindow::setBasePath(const QString &newBasePath) {
    basePath_ = newBasePath;
}

QFile *MainWindow::calFile() const { return calFile_; }

void MainWindow::setCalFile(QFile *newCalFile) { calFile_ = newCalFile; }

QFile *MainWindow::refFile() const { return refFile_; }

void MainWindow::setRefFile(QFile *newRefFile) { refFile_ = newRefFile; }

const std::vector<Subplot *> &MainWindow::subplots() const {
    return subplots_;
}

void MainWindow::setSubplots(const std::vector<Subplot *> &newSubplots) {
    subplots_ = newSubplots;
}

const std::vector<PlotType *> &MainWindow::plotList() const {
    return plotList_;
}

void MainWindow::setPlotList(const std::vector<PlotType *> &newPlotList) {
    plotList_ = newPlotList;
}


void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}

void MainWindow::loadSettings() {
    QSettings settings;
    prefs_.calDataDir = QDir(settings.value("calFileDir").toString());
    prefs_.calFiles = settings.value("calFiles").value<QStringList>();
    prefs_.refFiles = settings.value("refFiles").value<QStringList>();
}

void MainWindow::saveSettings() {
    QSettings settings;
    settings.setValue("calFileDir", prefs_.calDataDir.absolutePath());
    settings.setValue("calFiles", QVariant::fromValue(prefs_.calFiles));
    settings.setValue("refFiles", QVariant::fromValue(prefs_.refFiles));
}

void MainWindow::on_calFile_BTN_clicked() {
    QString base(QDir::currentPath());
    if (!prefs_.calDataDir.absolutePath().isEmpty()) {
        base = prefs_.calDataDir.absolutePath();
    }
    QDir calDataFile = QFileDialog::getOpenFileName(this,
                                                    "Select Calibration File",
                                                    base);
    ui_->calFile_LE->setText(calDataFile.absolutePath());
    setCalFile();
}

void MainWindow::on_refFile_BTN_clicked() {
    QString base(QDir::currentPath());
    if (!prefs_.calDataDir.absolutePath().isEmpty()) {
        base = prefs_.calDataDir.absolutePath();
    }
    QDir refDataFile = QFileDialog::getOpenFileName(this,
                                                    "Select Reference File",
                                                    base);
    ui_->refFile_LE->setText(refDataFile.absolutePath());
    setRefFile();
}

void MainWindow::on_calFile_LE_editingFinished() { setCalFile(); }

void MainWindow::on_selAll_BTN_clicked() {
    for (int index(0); index < ui_->plots_LV->count(); index++) {
        ui_->plots_LV->item(index)->setCheckState(Qt::CheckState::Checked);
    }
}

void MainWindow::on_selNone_BTN_clicked() {
    for (int index(0); index < ui_->plots_LV->count(); index++) {
        ui_->plots_LV->item(index)->setCheckState(Qt::CheckState::Unchecked);
    }
}

void MainWindow::on_refFile_LE_editingFinished() {
    setRefFile();
}

void MainWindow::on_type_CB_currentIndexChanged() {
    if (!tableType_isValid()) { return; }
    setType();
}

void MainWindow::on_config_CB_currentIndexChanged() {
    if (!plotConfig_isValid()) { return; }
    updatePlots();
}

void MainWindow::on_pushButton_clicked() {
    for (PlotWindow *plotWin : plotWindows()) {
        plotWin->close();
        delete plotWin;
    }
    plotWindows_.clear();
}

void MainWindow::on_exit_BTN_clicked() {
    close();
}

void MainWindow::on_plot_BTN_clicked() {
    if (!calFile_isValid() ||
        !refFile_isValid() ||
        !plotList_isValid()) { return; }
    for (int index(0); index < ui_->plots_LV->count(); index++) {
        if (ui()->plots_LV->item(index)->checkState() == Qt::CheckState::Checked) {
            PlotWindow *newPlotWindow = new PlotWindow(this, calFile(),
                                                       refFile(), plotList().at(index));
            plotWindows_.emplace_back(newPlotWindow);
            newPlotWindow->show();
        }
    }
}

void MainWindow::setCalFile() {
    if (ui_->calFile_LE->text() == "") { return; }
    else if (!QFile(ui_->calFile_LE->text()).exists()) { return; }
    else {
        calFile_ = new QFile(ui_->calFile_LE->text());
        prefs_.calDataDir = calFile_->fileName();
    }
}

void MainWindow::setRefFile() {
    if (ui_->refFile_LE->text() == "") { return; }
    else if (!QFile(ui_->refFile_LE->text()).exists()) { return; }
    else {
        refFile_ = new QFile(ui_->refFile_LE->text());
        prefs_.calDataDir = calFile_->fileName();
    }
}

void MainWindow::initType() {
    ui_->type_CB->blockSignals(true);
    ui_->type_CB->addItem("Dynamic");
    ui_->type_CB->addItem("Factory");
    ui_->type_CB->setCurrentIndex(0);
    ui_->type_CB->blockSignals(false);
    setType();
}

void MainWindow::setType() {
    if (!tableType_isValid()) { return; }
    ui_->config_CB->blockSignals(true);
    ui_->config_CB->clear();
    ui_->config_CB->addItems(getConfigList(ui_->type_CB->currentIndex()));
    ui_->config_CB->setCurrentIndex(0);
    ui_->config_CB->blockSignals(false);
    updatePlots();
}

void MainWindow::updatePlots() {
    ui_->plots_LV->blockSignals(true);
    ui_->plots_LV->clear();
    switch (ui_->type_CB->currentIndex()) {
        case 0: {
            plotList_ = parseConfig(new QFile(basePath() +
                                             "/config/DF Dynamic Calibration/Plots/" +
                                             ui_->config_CB->currentText()));
            break;
        }
        case 1: {
            plotList_ = parseConfig(new QFile(basePath() +
                                             "/config/DF Factory Calibration/Plots/" +
                                             ui_->config_CB->currentText()));
            break;
        }
        default: {
            plotList_ = { };
            break;
        }
    }
    for (PlotType *plot : plotList_) {
        ui_->plots_LV->addItem(plot->name().c_str());
    }
    for (int index(0); index < ui_->plots_LV->count(); index++) {
        ui_->plots_LV->item(index)->setCheckState(Qt::CheckState::Unchecked);
    }
    ui_->plots_LV->blockSignals(false);
}

QStringList MainWindow::getConfigList(const int &typeIdx) {
    switch (typeIdx) {
        case 0: {
            subplots_ = parseSubplots(new QFile(basePath_ +
                                                "/config/DF Dynamic Calibration/decoder.ini"));
            return QDir(basePath_ +
                        "/config/DF Dynamic Calibration/Plots").entryList({QDir::Filter::Files,
                                                                           QDir::Filter::Readable,
                                                                           QDir::Filter::NoDotAndDotDot});
        }
        case 1: {
            subplots_ = parseSubplots(new QFile(basePath_ +
                                                "/config/DF Factory Calibration/decoder.ini"));
            return QDir(basePath_ +
                        "/config/DF Factory Calibration/Plots").entryList({QDir::Filter::Files,
                                                                           QDir::Filter::Readable,
                                                                           QDir::Filter::NoDotAndDotDot});
        }
        default: {
            subplots_.clear();
            return { };
        }
    }
}

std::vector<Subplot *> MainWindow::parseSubplots(QFile *decoderFile) {
    std::vector<Subplot *> result{ };
    if (decoderFile->open(QIODevice::ReadOnly)) {
        std::string categoryLabel("");
        QTextStream in(decoderFile);
        while (!in.atEnd()) {
            QString qsLine = in.readLine();
            std::string line = qsLine.toStdString();
            line = TextParse::reduce(line);
            line = TextParse::removeComments(line);
            if (line.find('[') != std::string::npos) {
                int nameStartIdx = line.find('[') + 1;
                int nameEndIdx = line.find(']') - 1;
                int nameLength = nameEndIdx - nameStartIdx + 1;
                categoryLabel = line.substr(nameStartIdx, nameLength);
                if (categoryLabel != "Main") {
                    Subplot *newSubplot = new Subplot();
                    newSubplot->setLabel(categoryLabel);
                    result.push_back(newSubplot);
                }
            } else if (line.find_first_not_of(' ') != std::string::npos) {
                int assignIdx = line.find_first_of('=');
                std::string key = TextParse::reduce(line.substr(0, assignIdx));
                std::string value = TextParse::reduce(line.substr(assignIdx + 1,
                                                      line.size() - (assignIdx- 1)));
                if (categoryLabel == "Main") { continue; }
                else {
                    if (key == "Title") {
                        result.back()->setTitle(value);
                    } else if (key == "Parm_names") {
                        result.back()->setNames(value);
                    } else if (key == "Parm_elems") {
                        result.back()->setElems(value);
                    } else if (key == "Parm_words") {
                        result.back()->setWords(value);
                    } else if (key == "Parm_bits") {
                        result.back()->setBits(value);
                    } else if (key == "Parm_mod") {
                        result.back()->setMods(value);
                    } else if (key == "Parm_multiplier") {
                        result.back()->setMultipliers(value);
                    }
                }
            }
        }
    }
    decoderFile->close();
    return result;
}

std::vector<PlotType *> MainWindow::parseConfig(QFile *configFile) {
    if (!configFile->open(QIODevice::ReadOnly)) { return { }; }
    std::string label("");
    QTextStream in(configFile);
    std::vector < PlotType * > result{ };
    while (!in.atEnd()) {
        std::string line_str = in.readLine().toStdString();
        line_str = TextParse::removeComments(TextParse::reduce(line_str));
        if (line_str.find('[') != std::string::npos) {
            label = line_str.substr((line_str.find('[') + 1),
                                    (line_str.find(']')
                                    - (line_str.find('[') + 1)));

            if (label != "Main") {
                PlotType *plot = new PlotType();
                plot->setLabel(label);
                result.emplace_back(plot);
            }
        } else if (line_str.find_first_not_of(' ') != std::string::npos) {
            int opIndex = line_str.find_first_of('=');
            std::string key = TextParse::reduce(line_str.substr(0, opIndex));
            std::string value = line_str.substr((opIndex + 1),
                                                (line_str.size() -
                                                 (opIndex - 1)));
            if (label == "Main") { continue; }
            else {
                if (key == "Plot_name") {
                    result.back()->setName(value);
                } else if (key == "SubPlots") {
                    result.back()->setSubplots(value, subplots_);
                } else if (key == "Plot_position") {
                    result.back()->setPosition(value);
                } else if (key == "Plot_size") {
                    result.back()->setSize(value);
                } else if (key == "Plot_legend") {
                    result.back()->setLegend(value);
                } else if (key == "Xrange_names") {
                    result.back()->setXRangeNames(value);
                } else if (key == "Xrange_locs") {
                    result.back()->setXRangeLocs(value);
                } else if (key == "Yrange_data") {
                    result.back()->setYRangeData(value);
                } else if (key == "Yrange_diff") {
                    result.back()->setYRangeDiff(value);
                }
            }
        }
    }
    configFile->close();
    return result;
}

void MainWindow::on_plots_LV_itemDoubleClicked(QListWidgetItem *item) {
    if (item->checkState() == Qt::CheckState::Checked) {
        item->setCheckState(Qt::CheckState::Unchecked);
    } else { item->setCheckState(Qt::CheckState::Checked); }
}

bool MainWindow::calFile_isValid() {
    if (ui_->calFile_LE->text().isEmpty()) { return false; }
    else if (!QFile(ui_->calFile_LE->text()).exists()) { return false; }
    else if (!QFile(ui_->calFile_LE->text()).isReadable()) { return false; }
    else { return true; }
}

bool MainWindow::refFile_isValid() {
    if (ui_->refFile_LE->text().isEmpty()) { return false; }
    else if (!QFile(ui_->refFile_LE->text()).exists()) { return false; }
    else if (!QFile(ui_->refFile_LE->text()).isReadable()) { return false; }
    else { return true; } }

bool MainWindow::tableType_isValid() {
    if (ui_->type_CB->currentIndex() > 1) { return false; }
    else if (ui_->type_CB->currentIndex() < 0) { return false; }
    else { return true; }
}

bool MainWindow::plotConfig_isValid() {
    if (ui_->config_CB->findText(ui_->config_CB->currentText()) < 0) { return false; }
    else { return true; }
}

bool MainWindow::plotTypes_isValid() {
    int plotsSelCount(0);
    for (int idx(0); idx < ui_->plots_LV->count(); idx++) {
        if (ui_->plots_LV->item(idx)->checkState() == Qt::CheckState::Checked) {
            plotsSelCount++;
        }
    }
    if (plotsSelCount < 1) { return false; }
    else { return true; }
}

bool MainWindow::plotList_isValid() {
    if (plotList().empty()) { return false; }
    else { return true; }
}
