#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

// Add Qt includes
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTextStream>

namespace converter {

enum class FileFormat {
    UNKNOWN,
    TXT,
    CSV,
    JSON,
    XML,
    HTML,
    MD,
    // Image formats
    JPG,
    PNG,
    GIF,
    BMP,
    TIFF,
    WEBP,
    SVG,
    ICO,
    // Video formats
    MP4,
    AVI,
    MOV,
    MKV,
    WMV,
    FLV,
    WEBM,
    M4V,
    // Audio formats
    MP3,
    WAV,
    AAC,
    FLAC,
    OGG,
    WMA,
    // Document formats
    PDF,
    DOCX,
    ODT,
    RTF
};

// Abstract base class for format converters
class FormatConverter {
public:
    virtual ~FormatConverter() = default;
    virtual bool convert(const std::string& inputFile, const std::string& outputFile) = 0;
};

class FileConverter {
public:
    FileConverter();
    ~FileConverter();
    
    // Main conversion method
    bool convert(const std::string& inputFile, const std::string& outputFile);
    
    // Format detection and utilities
    static FileFormat detectFormat(const std::string& filePath);
    static std::string getExtension(FileFormat format);
    std::vector<FileFormat> getSupportedFormats() const;
    
private:
    void initConverters();
    
    // JSON to TXT conversion methods
    bool convertJsonToTxt(const std::string& inputPath, const std::string& outputPath);
    void convertJsonObjectToText(const QJsonObject& obj, QTextStream& out, int indent);
    void convertJsonArrayToText(const QJsonArray& arr, QTextStream& out, int indent);
    QString valueToString(const QJsonValue& value);
    
    // Map to store converters for different format pairs
    std::map<std::pair<FileFormat, FileFormat>, std::unique_ptr<FormatConverter>> converters_;
};

} // namespace converter