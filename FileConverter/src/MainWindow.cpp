#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileConverter(),
    isDarkMode(false)
{
    ui->setupUi(this);
    
    // Set window properties for a more modern look
    setWindowTitle("Modern File Converter");
    
    // Manually add all supported formats to the input combo box
    // Text formats
    ui->inputFormatCombo->addItem(".txt");
    ui->inputFormatCombo->addItem(".csv");
    ui->inputFormatCombo->addItem(".json");
    ui->inputFormatCombo->addItem(".xml");
    ui->inputFormatCombo->addItem(".html");
    ui->inputFormatCombo->addItem(".md");
    
    // Image formats
    ui->inputFormatCombo->addItem(".jpg");
    ui->inputFormatCombo->addItem(".png");
    ui->inputFormatCombo->addItem(".gif");
    ui->inputFormatCombo->addItem(".bmp");
    ui->inputFormatCombo->addItem(".tiff");
    ui->inputFormatCombo->addItem(".webp");
    ui->inputFormatCombo->addItem(".svg");
    ui->inputFormatCombo->addItem(".ico");
    
    // Video formats
    ui->inputFormatCombo->addItem(".mp4");
    ui->inputFormatCombo->addItem(".avi");
    ui->inputFormatCombo->addItem(".mov");
    ui->inputFormatCombo->addItem(".mkv");
    ui->inputFormatCombo->addItem(".wmv");
    ui->inputFormatCombo->addItem(".flv");
    ui->inputFormatCombo->addItem(".webm");
    ui->inputFormatCombo->addItem(".m4v");
    
    // Audio formats
    ui->inputFormatCombo->addItem(".mp3");
    ui->inputFormatCombo->addItem(".wav");
    ui->inputFormatCombo->addItem(".aac");
    ui->inputFormatCombo->addItem(".flac");
    ui->inputFormatCombo->addItem(".ogg");
    ui->inputFormatCombo->addItem(".wma");
    
    // Document formats
    ui->inputFormatCombo->addItem(".pdf");
    ui->inputFormatCombo->addItem(".docx");
    ui->inputFormatCombo->addItem(".odt");
    ui->inputFormatCombo->addItem(".rtf");
    
    // Connect signals
    connect(ui->inputFormatCombo, &QComboBox::currentTextChanged, this, &MainWindow::updateOutputFormats);
    connect(ui->inputFileEdit, &QLineEdit::textChanged, this, &MainWindow::updateOutputFileName);
    
    // Initial update
    updateOutputFormats();
    
    // Check dependencies
    // checkDependencies();
    
    // Apply modern style (default to light mode)
    setLightMode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseInputButton_clicked()
{
    QString selectedFormat = ui->inputFormatCombo->currentText();
    QString filter = getFileFilter(selectedFormat);
    
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), filter);
        
    if (!fileName.isEmpty()) {
        ui->inputFileEdit->setText(fileName);
        updateOutputFileName();
    }
}

void MainWindow::on_browseOutputFolderButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
        tr("Select Output Folder"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        
    if (!folderPath.isEmpty()) {
        ui->outputFolderEdit->setText(folderPath);
        updateOutputFileName();
    }
}

void MainWindow::on_convertButton_clicked()
{
    QString inputFile = ui->inputFileEdit->text();
    QString outputFile = getOutputFilePath();
    
    if (inputFile.isEmpty() || outputFile.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select input file and output folder."));
        return;
    }
    
    ui->convertButton->setEnabled(false);
    ui->statusBar->showMessage("Converting...");
    
    bool success = fileConverter.convert(inputFile.toStdString(), outputFile.toStdString());
    
    ui->convertButton->setEnabled(true);
    
    if (success) {
        ui->statusBar->showMessage("Conversion completed successfully", 5000);
        QMessageBox::information(this, tr("Success"), tr("File conversion completed successfully."));
    } else {
        ui->statusBar->showMessage("Conversion failed", 5000);
        QMessageBox::critical(this, tr("Error"), tr("File conversion failed."));
    }
}

void MainWindow::updateOutputFormats()
{
    // Get the selected input format
    QString inputExtension = ui->inputFormatCombo->currentText();
    
    // Clear output format combo box
    ui->outputFormatCombo->clear();
    
    // Text formats
    if (inputExtension == ".txt" || inputExtension == ".csv" || 
        inputExtension == ".json" || inputExtension == ".xml" || 
        inputExtension == ".html" || inputExtension == ".md") {
        
        if (inputExtension != ".txt") ui->outputFormatCombo->addItem(".txt");
        if (inputExtension != ".csv") ui->outputFormatCombo->addItem(".csv");
        if (inputExtension != ".json") ui->outputFormatCombo->addItem(".json");
        if (inputExtension != ".xml") ui->outputFormatCombo->addItem(".xml");
        if (inputExtension != ".html") ui->outputFormatCombo->addItem(".html");
        if (inputExtension != ".md") ui->outputFormatCombo->addItem(".md");
    }
    
    // Image formats
    else if (inputExtension == ".jpg" || inputExtension == ".png" || 
             inputExtension == ".gif" || inputExtension == ".bmp" ||
             inputExtension == ".tiff" || inputExtension == ".webp" ||
             inputExtension == ".svg" || inputExtension == ".ico") {
        
        if (inputExtension != ".jpg") ui->outputFormatCombo->addItem(".jpg");
        if (inputExtension != ".png") ui->outputFormatCombo->addItem(".png");
        if (inputExtension != ".gif") ui->outputFormatCombo->addItem(".gif");
        if (inputExtension != ".bmp") ui->outputFormatCombo->addItem(".bmp");
        if (inputExtension != ".tiff") ui->outputFormatCombo->addItem(".tiff");
        if (inputExtension != ".webp") ui->outputFormatCombo->addItem(".webp");
        if (inputExtension != ".svg") ui->outputFormatCombo->addItem(".svg");
        if (inputExtension != ".ico") ui->outputFormatCombo->addItem(".ico");
    }
    
    // Video formats
    else if (inputExtension == ".mp4" || inputExtension == ".avi" || 
             inputExtension == ".mov" || inputExtension == ".mkv" ||
             inputExtension == ".wmv" || inputExtension == ".flv" ||
             inputExtension == ".webm" || inputExtension == ".m4v") {
        
        if (inputExtension != ".mp4") ui->outputFormatCombo->addItem(".mp4");
        if (inputExtension != ".avi") ui->outputFormatCombo->addItem(".avi");
        if (inputExtension != ".mov") ui->outputFormatCombo->addItem(".mov");
        if (inputExtension != ".mkv") ui->outputFormatCombo->addItem(".mkv");
        if (inputExtension != ".wmv") ui->outputFormatCombo->addItem(".wmv");
        if (inputExtension != ".flv") ui->outputFormatCombo->addItem(".flv");
        if (inputExtension != ".webm") ui->outputFormatCombo->addItem(".webm");
        if (inputExtension != ".m4v") ui->outputFormatCombo->addItem(".m4v");
    }
    
    // Audio formats
    else if (inputExtension == ".mp3" || inputExtension == ".wav" || 
             inputExtension == ".aac" || inputExtension == ".flac" ||
             inputExtension == ".ogg" || inputExtension == ".wma") {
        
        if (inputExtension != ".mp3") ui->outputFormatCombo->addItem(".mp3");
        if (inputExtension != ".wav") ui->outputFormatCombo->addItem(".wav");
        if (inputExtension != ".aac") ui->outputFormatCombo->addItem(".aac");
        if (inputExtension != ".flac") ui->outputFormatCombo->addItem(".flac");
        if (inputExtension != ".ogg") ui->outputFormatCombo->addItem(".ogg");
        if (inputExtension != ".wma") ui->outputFormatCombo->addItem(".wma");
    }
    
    // Document formats
    else if (inputExtension == ".pdf" || inputExtension == ".docx" || 
             inputExtension == ".odt" || inputExtension == ".rtf") {
        
        if (inputExtension != ".pdf") ui->outputFormatCombo->addItem(".pdf");
        if (inputExtension != ".docx") ui->outputFormatCombo->addItem(".docx");
        if (inputExtension != ".odt") ui->outputFormatCombo->addItem(".odt");
        if (inputExtension != ".rtf") ui->outputFormatCombo->addItem(".rtf");
    }
    
    // Update output filename after changing format
    updateOutputFileName();
}

void MainWindow::updateOutputFileName()
{
    QString inputFile = ui->inputFileEdit->text();
    if (!inputFile.isEmpty()) {
        QFileInfo fileInfo(inputFile);
        QString outputFormat = ui->outputFormatCombo->currentText();
        
        // Only set the default name if the output filename edit is empty
        if (ui->outputFileNameEdit->text().isEmpty()) {
            QString outputFileName = fileInfo.completeBaseName() + outputFormat;
            ui->outputFileNameEdit->setText(outputFileName);
        } else {
            // Make sure the extension matches the selected format
            QString currentName = ui->outputFileNameEdit->text();
            QFileInfo outFileInfo(currentName);
            if (outFileInfo.suffix() != outputFormat.mid(1)) {
                // Replace extension but keep the base name
                QString newName = outFileInfo.completeBaseName() + outputFormat;
                ui->outputFileNameEdit->setText(newName);
            }
        }
    } else {
        ui->outputFileNameEdit->clear();
    }
}

QString MainWindow::getOutputFilePath()
{
    QString outputFolder = ui->outputFolderEdit->text();
    QString outputFileName = ui->outputFileNameEdit->text();
    
    if (outputFolder.isEmpty() || outputFileName.isEmpty()) {
        return "";
    }
    
    return QDir(outputFolder).filePath(outputFileName);
}

QString MainWindow::getFileFilter(const QString& extension)
{
    QString formatName = extension.mid(1).toUpper(); // Remove the dot and convert to uppercase
    return QString("%1 Files (*%2);;All Files (*)").arg(formatName, extension);
}

bool MainWindow::isConversionSupported(converter::FileFormat input, converter::FileFormat output)
{
    // Text formats
    if ((input == converter::FileFormat::TXT || input == converter::FileFormat::CSV || 
         input == converter::FileFormat::JSON || input == converter::FileFormat::XML) &&
        (output == converter::FileFormat::TXT || output == converter::FileFormat::CSV || 
         output == converter::FileFormat::JSON || output == converter::FileFormat::XML)) {
        return true;
    }
    
    // For other formats, we'll need to check if the required dependencies are installed
    // and handle the conversion through external tools
    
    // For now, let's assume all other conversions are handled by external tools
    // if the dependencies are installed
    return checkDependencies();
}

bool MainWindow::checkDependencies()
{
    bool imageMagickInstalled = false;
    bool ffmpegInstalled = false;
    bool pandocInstalled = false;
    bool libreOfficeInstalled = false;
    
    // Check for ImageMagick
    QProcess process;
    process.start("magick", QStringList() << "-version");
    process.waitForFinished();
    imageMagickInstalled = (process.exitCode() == 0);
    
    // Check for FFmpeg
    process.start("ffmpeg", QStringList() << "-version");
    process.waitForFinished();
    ffmpegInstalled = (process.exitCode() == 0);
    
    // Check for Pandoc
    process.start("pandoc", QStringList() << "--version");
    process.waitForFinished();
    pandocInstalled = (process.exitCode() == 0);
    
    // Check for LibreOffice
    process.start("soffice", QStringList() << "--version");
    process.waitForFinished();
    libreOfficeInstalled = (process.exitCode() == 0);
    
    // Update warning label
    QStringList missingDeps;
    if (!imageMagickInstalled) missingDeps << "ImageMagick";
    if (!ffmpegInstalled) missingDeps << "FFmpeg";
    if (!pandocInstalled) missingDeps << "Pandoc";
    if (!libreOfficeInstalled) missingDeps << "LibreOffice";
    
    QString warningText;
    if (!missingDeps.isEmpty()) {
        warningText = "Warning: Missing dependencies: " + missingDeps.join(", ") + ". Some conversions unavailable.";
        ui->dependencyWarningLabel->setStyleSheet("color: #FF6D6D;");
    } else {
        // warningText = "All dependencies installed. Full conversion capabilities available.";
        ui->dependencyWarningLabel->setStyleSheet("color: green;");
    }
    
    ui->dependencyWarningLabel->setText(warningText);
    
    return imageMagickInstalled && ffmpegInstalled && pandocInstalled && libreOfficeInstalled;
}

void MainWindow::on_actionInstallDependencies_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Install Dependencies");
    msgBox.setText("Required Dependencies:");
    msgBox.setInformativeText(
        "1. ImageMagick - For image conversion\n"
        "   Download from: https://imagemagick.org/script/download.php\n\n"
        "2. FFmpeg - For audio/video conversion\n"
        "   Download from: https://ffmpeg.org/download.html\n\n"
        "3. Pandoc - For document and text format conversion\n"
        "   Download from: https://pandoc.org/installing.html\n\n"
        "4. LibreOffice - For office document conversion\n"
        "   Download from: https://www.libreoffice.org/download/\n\n"
        "After installation, make sure to restart the application."
    );
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionDarkMode_triggered()
{
    setDarkMode();
}

void MainWindow::on_actionLightMode_triggered()
{
    setLightMode();
}

void MainWindow::setDarkMode()
{
    isDarkMode = true;
    
    // Dark mode color scheme
    QString style = 
        "QMainWindow, QDialog {"
        "   background-color: #2D2D30;"
        "   color: #FFFFFF;"
        "}"
        "QWidget {"
        "   background-color: #2D2D30;"
        "   color: #FFFFFF;"
        "}"
        "QPushButton {"
        "   background-color: #0078D7;"
        "   color: white;"
        "   border: none;"
        "   padding: 5px 15px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1C97EA;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #00559B;"
        "}"
        "QLineEdit, QComboBox {"
        "   background-color: #3E3E42;"
        "   color: #FFFFFF;"
        "   border: 1px solid #555555;"
        "   border-radius: 4px;"
        "   padding: 2px 4px;"
        "}"
        "QGroupBox {"
        "   border: 1px solid #555555;"
        "   border-radius: 4px;"
        "   margin-top: 8px;"
        "   font-weight: bold;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 3px 0 3px;"
        "}"
        "QProgressBar {"
        "   border: 1px solid #555555;"
        "   border-radius: 4px;"
        "   text-align: center;"
        "   background-color: #3E3E42;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #0078D7;"
        "   width: 10px;"
        "   margin: 0.5px;"
        "}"
        "QMenuBar {"
        "   background-color: #2D2D30;"
        "   color: #FFFFFF;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #3E3E42;"
        "}"
        "QMenu {"
        "   background-color: #2D2D30;"
        "   color: #FFFFFF;"
        "   border: 1px solid #555555;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #3E3E42;"
        "}";
    
    qApp->setStyleSheet(style);
    
    // Update UI elements that need special handling
    ui->dependencyWarningLabel->setStyleSheet("color: #FF6D6D;");
}

void MainWindow::setLightMode()
{
    isDarkMode = false;
    
    // Light mode color scheme
    QString style = 
        "QMainWindow, QDialog {"
        "   background-color: #F0F0F0;"
        "   color: #000000;"
        "}"
        "QWidget {"
        "   background-color: #F0F0F0;"
        "   color: #000000;"
        "}"
        "QPushButton {"
        "   background-color: #0078D7;"
        "   color: white;"
        "   border: none;"
        "   padding: 5px 15px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1C97EA;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #00559B;"
        "}"
        "QLineEdit, QComboBox {"
        "   background-color: #FFFFFF;"
        "   color: #000000;"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 4px;"
        "   padding: 2px 4px;"
        "}"
        "QGroupBox {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 4px;"
        "   margin-top: 8px;"
        "   font-weight: bold;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 3px 0 3px;"
        "}"
        "QProgressBar {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 4px;"
        "   text-align: center;"
        "   background-color: #FFFFFF;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #0078D7;"
        "   width: 10px;"
        "   margin: 0.5px;"
        "}"
        "QMenuBar {"
        "   background-color: #F0F0F0;"
        "   color: #000000;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #E0E0E0;"
        "}"
        "QMenu {"
        "   background-color: #F0F0F0;"
        "   color: #000000;"
        "   border: 1px solid #CCCCCC;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #E0E0E0;"
        "}";
    
    qApp->setStyleSheet(style);
    
    // Update UI elements that need special handling
    ui->dependencyWarningLabel->setStyleSheet("color: #D32F2F;");
}