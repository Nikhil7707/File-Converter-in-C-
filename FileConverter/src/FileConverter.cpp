#include "FileConverter.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace converter {

// Specific converter implementations
class TxtToCsvConverter : public FormatConverter {
public:
    bool convert(const std::string& inputFile, const std::string& outputFile) override {
        std::ifstream input(inputFile);
        std::ofstream output(outputFile);
        
        if (!input || !output) {
            std::cerr << "Error opening files!" << std::endl;
            return false;
        }
        
        std::string line;
        while (std::getline(input, line)) {
            // Simple conversion: replace spaces with commas
            std::replace(line.begin(), line.end(), ' ', ',');
            output << line << std::endl;
        }
        
        return true;
    }
};

class CsvToTxtConverter : public FormatConverter {
public:
    bool convert(const std::string& inputFile, const std::string& outputFile) override {
        std::ifstream input(inputFile);
        std::ofstream output(outputFile);
        
        if (!input || !output) {
            std::cerr << "Error opening files!" << std::endl;
            return false;
        }
        
        std::string line;
        while (std::getline(input, line)) {
            // Simple conversion: replace commas with spaces
            std::replace(line.begin(), line.end(), ',', ' ');
            output << line << std::endl;
        }
        
        return true;
    }
};

// FileConverter implementation
FileConverter::FileConverter() {
    initConverters();
}

FileConverter::~FileConverter() = default;

void FileConverter::initConverters() {
    // Add converters for different format pairs
    converters_[{FileFormat::TXT, FileFormat::CSV}] = std::make_unique<TxtToCsvConverter>();
    converters_[{FileFormat::CSV, FileFormat::TXT}] = std::make_unique<CsvToTxtConverter>();
    
    // Add more converters as needed
}

bool FileConverter::convert(const std::string& inputFile, const std::string& outputFile) {
    FileFormat inputFormat = detectFormat(inputFile);
    FileFormat outputFormat = detectFormat(outputFile);
    
    if (inputFormat == FileFormat::UNKNOWN || outputFormat == FileFormat::UNKNOWN) {
        std::cerr << "Unknown file format!" << std::endl;
        return false;
    }
    
    // Text format conversions
    if ((inputFormat == FileFormat::TXT || inputFormat == FileFormat::CSV || 
         inputFormat == FileFormat::JSON || inputFormat == FileFormat::XML ||
         inputFormat == FileFormat::HTML || inputFormat == FileFormat::MD) &&
        (outputFormat == FileFormat::TXT || outputFormat == FileFormat::CSV || 
         outputFormat == FileFormat::JSON || outputFormat == FileFormat::XML ||
         outputFormat == FileFormat::HTML || outputFormat == FileFormat::MD)) {
        
        // JSON to TXT conversion
        if (inputFormat == FileFormat::JSON && outputFormat == FileFormat::TXT) {
            return convertJsonToTxt(inputFile, outputFile);
        }
        
        // Use Pandoc for text format conversions
        QProcess process;
        QStringList args;
        
        // Check if Pandoc is available
        process.start("pandoc", QStringList() << "--version");
        process.waitForFinished();
        if (process.exitCode() != 0) {
            std::cerr << "Pandoc is not installed!" << std::endl;
            return false;
        }
        
        // Determine input format for Pandoc
        QString inFormat;
        switch (inputFormat) {
            case FileFormat::TXT: inFormat = "plain"; break;
            case FileFormat::CSV: inFormat = "csv"; break;
            case FileFormat::JSON: inFormat = "json"; break;
            case FileFormat::XML: inFormat = "xml"; break;
            case FileFormat::HTML: inFormat = "html"; break;
            case FileFormat::MD: inFormat = "markdown"; break;
            default: inFormat = "plain";
        }
        
        // Determine output format for Pandoc
        QString outFormat;
        switch (outputFormat) {
            case FileFormat::TXT: outFormat = "plain"; break;
            case FileFormat::CSV: outFormat = "csv"; break;
            case FileFormat::JSON: outFormat = "json"; break;
            case FileFormat::XML: outFormat = "xml"; break;
            case FileFormat::HTML: outFormat = "html"; break;
            case FileFormat::MD: outFormat = "markdown"; break;
            default: outFormat = "plain";
        }
        
        args << QString::fromStdString(inputFile)
             << "-f" << inFormat
             << "-t" << outFormat
             << "-o" << QString::fromStdString(outputFile);
        
        process.start("pandoc", args);
        process.waitForFinished();
        
        return (process.exitCode() == 0);
    }
    
    // Image format conversions
    else if ((inputFormat == FileFormat::JPG || inputFormat == FileFormat::PNG || 
              inputFormat == FileFormat::GIF || inputFormat == FileFormat::BMP ||
              inputFormat == FileFormat::TIFF || inputFormat == FileFormat::WEBP ||
              inputFormat == FileFormat::SVG || inputFormat == FileFormat::ICO) &&
             (outputFormat == FileFormat::JPG || outputFormat == FileFormat::PNG || 
              outputFormat == FileFormat::GIF || outputFormat == FileFormat::BMP ||
              outputFormat == FileFormat::TIFF || outputFormat == FileFormat::WEBP ||
              outputFormat == FileFormat::SVG || outputFormat == FileFormat::ICO)) {
        
        // Use ImageMagick for image conversions
        QProcess process;
        
        // Check if ImageMagick is available
        process.start("magick", QStringList() << "-version");
        process.waitForFinished();
        if (process.exitCode() != 0) {
            std::cerr << "ImageMagick is not installed!" << std::endl;
            return false;
        }
        
        QStringList args;
        args << "convert"
             << QString::fromStdString(inputFile)
             << QString::fromStdString(outputFile);
        
        process.start("magick", args);
        process.waitForFinished();
        
        return (process.exitCode() == 0);
    }
    
    // Audio format conversions
    else if ((inputFormat == FileFormat::MP3 || inputFormat == FileFormat::WAV || 
              inputFormat == FileFormat::AAC || inputFormat == FileFormat::FLAC ||
              inputFormat == FileFormat::OGG || inputFormat == FileFormat::WMA) &&
             (outputFormat == FileFormat::MP3 || outputFormat == FileFormat::WAV || 
              outputFormat == FileFormat::AAC || outputFormat == FileFormat::FLAC ||
              outputFormat == FileFormat::OGG || outputFormat == FileFormat::WMA)) {
        
        // Use FFmpeg for audio conversions
        QProcess process;
        
        // Check if FFmpeg is available
        process.start("ffmpeg", QStringList() << "-version");
        process.waitForFinished();
        if (process.exitCode() != 0) {
            std::cerr << "FFmpeg is not installed!" << std::endl;
            return false;
        }
        
        QStringList args;
        args << "-i" << QString::fromStdString(inputFile)
             << "-y" // Overwrite output file if it exists
             << QString::fromStdString(outputFile);
        
        process.start("ffmpeg", args);
        process.waitForFinished();
        
        return (process.exitCode() == 0);
    }
    
    // Video format conversions
    else if ((inputFormat == FileFormat::MP4 || inputFormat == FileFormat::AVI || 
              inputFormat == FileFormat::MOV || inputFormat == FileFormat::MKV ||
              inputFormat == FileFormat::WMV || inputFormat == FileFormat::FLV ||
              inputFormat == FileFormat::WEBM || inputFormat == FileFormat::M4V) &&
             (outputFormat == FileFormat::MP4 || outputFormat == FileFormat::AVI || 
              outputFormat == FileFormat::MOV || outputFormat == FileFormat::MKV ||
              outputFormat == FileFormat::WMV || outputFormat == FileFormat::FLV ||
              outputFormat == FileFormat::WEBM || outputFormat == FileFormat::M4V)) {
        
        // Use FFmpeg for video conversions
        QProcess process;
        
        // Check if FFmpeg is available
        process.start("ffmpeg", QStringList() << "-version");
        process.waitForFinished();
        if (process.exitCode() != 0) {
            std::cerr << "FFmpeg is not installed!" << std::endl;
            return false;
        }
        
        QStringList args;
        args << "-i" << QString::fromStdString(inputFile)
             << "-y" // Overwrite output file if it exists
             << QString::fromStdString(outputFile);
        
        process.start("ffmpeg", args);
        process.waitForFinished();
        
        return (process.exitCode() == 0);
    }
    
    // Document format conversions
    else if ((inputFormat == FileFormat::PDF || inputFormat == FileFormat::DOCX ||
              inputFormat == FileFormat::ODT || inputFormat == FileFormat::RTF) &&
             (outputFormat == FileFormat::PDF || outputFormat == FileFormat::DOCX ||
              outputFormat == FileFormat::ODT || outputFormat == FileFormat::RTF)) {
        
        // Use LibreOffice for document conversions
        QProcess process;
        QString libreOfficePath = "soffice"; // You have LibreOffice in your PATH
        
        // Get file paths
        QFileInfo inputFileInfo(QString::fromStdString(inputFile));
        QFileInfo outputFileInfo(QString::fromStdString(outputFile));
        QString outputDir = outputFileInfo.absolutePath();
        
        // Debug output
        std::cout << "Converting document: " << inputFile << " to " << outputFile << std::endl;
        std::cout << "Input format: " << static_cast<int>(inputFormat) << ", Output format: " << static_cast<int>(outputFormat) << std::endl;
        
        QStringList args;
        
        // Special handling for PDF to DOCX conversion
        if (inputFormat == FileFormat::PDF && outputFormat == FileFormat::DOCX) {
            std::cout << "Using special PDF to DOCX conversion..." << std::endl;
            
            args << "--headless" 
                 << "--infilter=writer_pdf_import" 
                 << "--convert-to" << "docx" 
                 << "--outdir" << outputDir
                 << QString::fromStdString(inputFile);
        } else {
            // Standard conversion for other document formats
            args << "--headless"
                 << "--convert-to" << QString::fromStdString(getExtension(outputFormat).substr(1))
                 << "--outdir" << outputDir
                 << QString::fromStdString(inputFile);
        }
        
        // Start the process with the found LibreOffice path
        std::cout << "Running LibreOffice with command: " << libreOfficePath.toStdString() << " " 
                  << args.join(" ").toStdString() << std::endl;
        
        process.start(libreOfficePath, args);
        process.waitForFinished(-1); // Wait indefinitely
        
        // Debug output
        std::cout << "LibreOffice exit code: " << process.exitCode() << std::endl;
        std::cout << "LibreOffice stdout: " << process.readAllStandardOutput().toStdString() << std::endl;
        std::cout << "LibreOffice stderr: " << process.readAllStandardError().toStdString() << std::endl;
        
        // LibreOffice creates the output file with the same name as input but different extension
        QString expectedOutput = QDir(outputDir).filePath(
            inputFileInfo.completeBaseName() + QString::fromStdString(getExtension(outputFormat)));
        
        std::cout << "Looking for output file: " << expectedOutput.toStdString() << std::endl;
        
        // Check if the file exists
        if (QFile::exists(expectedOutput)) {
            // If the expected output file exists and is different from the requested output path
            if (expectedOutput != QString::fromStdString(outputFile)) {
                // Remove existing output file if it exists
                if (QFile::exists(QString::fromStdString(outputFile))) {
                    QFile::remove(QString::fromStdString(outputFile));
                }
                
                // Rename the file
                bool renameSuccess = QFile::rename(expectedOutput, QString::fromStdString(outputFile));
                std::cout << "File rename result: " << (renameSuccess ? "success" : "failed") << std::endl;
                return renameSuccess;
            }
            return true;
        }
        
        // Try alternative approach for PDF to DOCX if the first method failed
        if (inputFormat == FileFormat::PDF && outputFormat == FileFormat::DOCX) {
            std::cout << "First method failed, trying alternative approach..." << std::endl;
            
            // Clear previous arguments
            args.clear();
            args << "--headless"
                 << "--convert-to" << "docx:writer_pdf_import" 
                 << "--outdir" << outputDir
                 << QString::fromStdString(inputFile);
                 
            process.start(libreOfficePath, args);
            process.waitForFinished(-1);
            
            std::cout << "Alternative method exit code: " << process.exitCode() << std::endl;
            std::cout << "Alternative method stdout: " << process.readAllStandardOutput().toStdString() << std::endl;
            std::cout << "Alternative method stderr: " << process.readAllStandardError().toStdString() << std::endl;
            
            // Check again for the output file
            if (QFile::exists(expectedOutput)) {
                if (expectedOutput != QString::fromStdString(outputFile)) {
                    if (QFile::exists(QString::fromStdString(outputFile))) {
                        QFile::remove(QString::fromStdString(outputFile));
                    }
                    bool renameSuccess = QFile::rename(expectedOutput, QString::fromStdString(outputFile));
                    std::cout << "File rename result (alternative method): " << (renameSuccess ? "success" : "failed") << std::endl;
                    return renameSuccess;
                }
                return true;
            }
        }
        
        std::cerr << "Output file not found after conversion!" << std::endl;
        return false;
    }
    
    // Use registered converters for any other conversions
    auto it = converters_.find({inputFormat, outputFormat});
    if (it != converters_.end()) {
        return it->second->convert(inputFile, outputFile);
    }
    
    std::cerr << "Conversion not supported!" << std::endl;
    return false;
}

std::vector<FileFormat> FileConverter::getSupportedFormats() const {
    std::vector<FileFormat> formats;
    formats.push_back(FileFormat::TXT);
    formats.push_back(FileFormat::CSV);
    formats.push_back(FileFormat::JSON);
    formats.push_back(FileFormat::XML);
    return formats;
}

FileFormat FileConverter::detectFormat(const std::string& filename) {
    // Extract extension manually instead of using filesystem
    std::string extension;
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos) {
        extension = filename.substr(pos);
    }
    
    // Convert to lowercase
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    // Text formats
    if (extension == ".txt") return FileFormat::TXT;
    if (extension == ".csv") return FileFormat::CSV;
    if (extension == ".json") return FileFormat::JSON;
    if (extension == ".xml") return FileFormat::XML;
    if (extension == ".html") return FileFormat::HTML;
    if (extension == ".md") return FileFormat::MD;
    
    // Image formats
    if (extension == ".jpg" || extension == ".jpeg") return FileFormat::JPG;
    if (extension == ".png") return FileFormat::PNG;
    if (extension == ".gif") return FileFormat::GIF;
    if (extension == ".bmp") return FileFormat::BMP;
    if (extension == ".tiff" || extension == ".tif") return FileFormat::TIFF;
    if (extension == ".webp") return FileFormat::WEBP;
    if (extension == ".svg") return FileFormat::SVG;
    if (extension == ".ico") return FileFormat::ICO;
    
    // Video formats
    if (extension == ".mp4") return FileFormat::MP4;
    if (extension == ".avi") return FileFormat::AVI;
    if (extension == ".mov") return FileFormat::MOV;
    if (extension == ".mkv") return FileFormat::MKV;
    if (extension == ".wmv") return FileFormat::WMV;
    if (extension == ".flv") return FileFormat::FLV;
    if (extension == ".webm") return FileFormat::WEBM;
    if (extension == ".m4v") return FileFormat::M4V;
    
    // Audio formats
    if (extension == ".mp3") return FileFormat::MP3;
    if (extension == ".wav") return FileFormat::WAV;
    if (extension == ".aac") return FileFormat::AAC;
    if (extension == ".flac") return FileFormat::FLAC;
    if (extension == ".ogg") return FileFormat::OGG;
    if (extension == ".wma") return FileFormat::WMA;
    
    // Document formats
    if (extension == ".pdf") return FileFormat::PDF;
    if (extension == ".docx") return FileFormat::DOCX;
    if (extension == ".odt") return FileFormat::ODT;
    if (extension == ".rtf") return FileFormat::RTF;
    
    return FileFormat::UNKNOWN;
}

std::string FileConverter::getExtension(FileFormat format) {
    switch (format) {
        // Text formats
        case FileFormat::TXT: return ".txt";
        case FileFormat::CSV: return ".csv";
        case FileFormat::JSON: return ".json";
        case FileFormat::XML: return ".xml";
        case FileFormat::HTML: return ".html";
        case FileFormat::MD: return ".md";
        
        // Image formats
        case FileFormat::JPG: return ".jpg";
        case FileFormat::PNG: return ".png";
        case FileFormat::GIF: return ".gif";
        case FileFormat::BMP: return ".bmp";
        case FileFormat::TIFF: return ".tiff";
        case FileFormat::WEBP: return ".webp";
        case FileFormat::SVG: return ".svg";
        case FileFormat::ICO: return ".ico";
        
        // Video formats
        case FileFormat::MP4: return ".mp4";
        case FileFormat::AVI: return ".avi";
        case FileFormat::MOV: return ".mov";
        case FileFormat::MKV: return ".mkv";
        case FileFormat::WMV: return ".wmv";
        case FileFormat::FLV: return ".flv";
        case FileFormat::WEBM: return ".webm";
        case FileFormat::M4V: return ".m4v";
        
        // Audio formats
        case FileFormat::MP3: return ".mp3";
        case FileFormat::WAV: return ".wav";
        case FileFormat::AAC: return ".aac";
        case FileFormat::FLAC: return ".flac";
        case FileFormat::OGG: return ".ogg";
        case FileFormat::WMA: return ".wma";
        
        // Document formats
        case FileFormat::PDF: return ".pdf";
        case FileFormat::DOCX: return ".docx";
        case FileFormat::ODT: return ".odt";
        case FileFormat::RTF: return ".rtf";
        
        default: return "";
    }
}

bool FileConverter::convertJsonToTxt(const std::string& inputPath, const std::string& outputPath) {
    try {
        // Use QProcess to call the appropriate tool for conversion
        QProcess process;
        
        // Check if Pandoc is available
        process.start("pandoc", QStringList() << "--version");
        process.waitForFinished();
        bool pandocAvailable = (process.exitCode() == 0);
        
        if (pandocAvailable) {
            // Use Pandoc for conversion
            QStringList args;
            args << QString::fromStdString(inputPath) 
                 << "-o" << QString::fromStdString(outputPath)
                 << "--from=json" << "--to=plain";
            
            process.start("pandoc", args);
            process.waitForFinished();
            
            return (process.exitCode() == 0);
        } else {
            // Fallback to manual conversion using Qt's JSON parser
            QFile inputFile(QString::fromStdString(inputPath));
            if (!inputFile.open(QIODevice::ReadOnly)) {
                return false;
            }
            
            QByteArray jsonData = inputFile.readAll();
            inputFile.close();
            
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (jsonDoc.isNull()) {
                return false;
            }
            
            QFile outputFile(QString::fromStdString(outputPath));
            if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return false;
            }
            
            QTextStream out(&outputFile);
            
            // Convert JSON to plain text
            if (jsonDoc.isObject()) {
                convertJsonObjectToText(jsonDoc.object(), out, 0);
            } else if (jsonDoc.isArray()) {
                convertJsonArrayToText(jsonDoc.array(), out, 0);
            }
            
            outputFile.close();
            return true;
        }
    } catch (const std::exception& e) {
        // Log the error
        qDebug() << "Error converting JSON to TXT: " << e.what();
        return false;
    }
}

void FileConverter::convertJsonObjectToText(const QJsonObject& obj, QTextStream& out, int indent) {
    QString indentStr(indent, ' ');
    
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        out << indentStr << it.key() << ": ";
        
        if (it.value().isObject()) {
            out << "\n";
            convertJsonObjectToText(it.value().toObject(), out, indent + 2);
        } else if (it.value().isArray()) {
            out << "\n";
            convertJsonArrayToText(it.value().toArray(), out, indent + 2);
        } else {
            out << valueToString(it.value()) << "\n";
        }
    }
}

void FileConverter::convertJsonArrayToText(const QJsonArray& arr, QTextStream& out, int indent) {
    QString indentStr(indent, ' ');
    
    for (int i = 0; i < arr.size(); ++i) {
        out << indentStr << "- ";
        
        if (arr[i].isObject()) {
            out << "\n";
            convertJsonObjectToText(arr[i].toObject(), out, indent + 2);
        } else if (arr[i].isArray()) {
            out << "\n";
            convertJsonArrayToText(arr[i].toArray(), out, indent + 2);
        } else {
            out << valueToString(arr[i]) << "\n";
        }
    }
}

QString FileConverter::valueToString(const QJsonValue& value) {
    if (value.isString()) {
        return value.toString();
    } else if (value.isBool()) {
        return value.toBool() ? "true" : "false";
    } else if (value.isDouble()) {
        return QString::number(value.toDouble());
    } else if (value.isNull()) {
        return "null";
    }
    return "";
}

} // namespace converter