#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "../include/FileConverter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseInputButton_clicked();
    void on_browseOutputFolderButton_clicked(); // Changed from browseOutputButton
    void on_convertButton_clicked();
    void updateOutputFormats();
    void updateOutputFileName(); // New function to update output filename
    void on_actionInstallDependencies_triggered();
    void on_actionDarkMode_triggered();
    void on_actionLightMode_triggered();

private:
    Ui::MainWindow *ui;
    converter::FileConverter fileConverter;
    QString getFileFilter(const QString& extension);
    bool isConversionSupported(converter::FileFormat input, converter::FileFormat output);
    QString getOutputFilePath(); // Helper to construct the output file path
    bool checkDependencies();
    void applyStyleSheet(const QString& stylePath);
    void setDarkMode();
    void setLightMode();
    bool isDarkMode;
};

#endif // MAINWINDOW_H