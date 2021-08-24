#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include"plotwindow.hpp"
#include"PlotType.hpp"

#include<QMainWindow>
#include<QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    struct preferences {
        QDir calDataDir;
        QStringList calFiles;
        QStringList refFiles;
    };
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const preferences &prefs() const;
    void setPrefs(const preferences &newPrefs);
    Ui::MainWindow *ui() const;
    void setUi(Ui::MainWindow *newUi);
    const std::vector<PlotWindow *> &plotWindows() const;
    void setPlotWindows(const std::vector<PlotWindow *> &newPlotWindows);
    const QString &basePath() const;
    void setBasePath(const QString &newBasePath);
    QFile *calFile() const;
    void setCalFile(QFile *newCalFile);
    QFile *refFile() const;
    void setRefFile(QFile *newRefFile);
    const std::vector<Subplot *> &subplots() const;
    void setSubplots(const std::vector<Subplot *> &newSubplots);
    const std::vector<PlotType *> &plotList() const;
    void setPlotList(const std::vector<PlotType *> &newPlotList);
    void closeEvent(QCloseEvent *event);
private slots:
    void on_type_CB_currentIndexChanged();
    void on_config_CB_currentIndexChanged();
    void on_plot_BTN_clicked();
    void on_calFile_LE_editingFinished();
    void on_refFile_LE_editingFinished();
    void on_calFile_BTN_clicked();
    void on_refFile_BTN_clicked();
    void on_pushButton_clicked();
    void on_exit_BTN_clicked();
    void on_selNone_BTN_clicked();
    void on_selAll_BTN_clicked();
    void on_plots_LV_itemDoubleClicked(QListWidgetItem *item);
private:
    preferences prefs_;
    Ui::MainWindow *ui_;
    std::vector<PlotWindow *> plotWindows_;
    QString basePath_;
    QFile *calFile_;
    QFile *refFile_;
    std::vector<Subplot *> subplots_;
    std::vector<PlotType *> plotList_;
    std::vector<Subplot *> parseSubplots(QFile *decoderFile);
    std::vector<PlotType *> parseConfig(QFile *configFile);
    QStringList getConfigList(const int &typeIdx);
    void loadSettings();
    void saveSettings();
    void setCalFile();
    void setRefFile();
    void initType();
    void setType();
    void updatePlots();
    bool calFile_isValid();
    bool refFile_isValid();
    bool tableType_isValid();
    bool plotConfig_isValid();
    bool plotTypes_isValid();
    bool plotList_isValid();
};
#endif // MAINWINDOW_HPP
