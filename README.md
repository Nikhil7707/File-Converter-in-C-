# 🔄 File Converter

![File Converter Banner]

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Supported Formats](#supported-formats)
- [Technologies Used](#technologies-used)
- [Architecture](#architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [Development](#development)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [Acknowledgments](#acknowledgments)

## 📝 Overview

The File Converter is a comprehensive, cross-platform application designed to convert files between various formats. It addresses the common need for format conversion in today's digital environment where users frequently encounter files in multiple formats that need to be transformed for compatibility with different applications and devices.

## ✨ Features

- **🔄 Versatile Format Support**: Convert between a wide range of file types
- **👥 Dual Interface**: Both GUI and CLI options for different user preferences
- **🌐 Cross-Platform**: Works on Windows, macOS, and Linux
- **🔌 External Tool Integration**: Leverages powerful tools for specialized conversions
- **🎨 Theming Support**: Light and dark mode options for user comfort
- **📊 Progress Tracking**: Real-time conversion progress display
- **🛠️ Dependency Management**: Built-in tools to help install required dependencies
- **🔍 Format Detection**: Automatic detection of input file formats

## 📁 Supported Formats

### Text Formats
- TXT, CSV, JSON, XML, HTML, MD

### Image Formats
- JPG, PNG, GIF, BMP, TIFF, WEBP, SVG, ICO

### Video Formats
- MP4, AVI, MOV, MKV, WMV, FLV, WEBM, M4V

### Audio Formats
- MP3, WAV, AAC, FLAC, OGG, WMA

### Document Formats
- PDF, DOCX, ODT, RTF

## 💻 Technologies Used

### Core Technologies
- **C++**: Primary programming language for performance and efficiency
- **Qt Framework**: For cross-platform GUI development and core functionality
  - Qt Widgets for interface components
  - Qt Core for file handling and data structures
  - Qt GUI for graphical elements

### Build System
- **CMake**: Cross-platform build system for generating build files

### External Dependencies
- **Pandoc**: For document and text format conversions
- **ImageMagick**: For image processing and conversion
- **FFmpeg**: For audio and video format conversions
- **LibreOffice**: For office document conversions

### Development Tools
- **Qt Creator**: IDE for development
- **Git**: Version control system
- **GitHub Actions**: CI/CD for automated testing and building

## 🏗️ Architecture

The File Converter is built with a modular architecture centered around the following components:

- **FileFormat Enum**: Defines all supported file formats
- **FormatConverter Base Class**: Abstract interface for all format converters
- **FileConverter Core**: Manages the conversion process and format detection
- **GUI Module**: Handles user interaction through a graphical interface
- **CLI Module**: Provides command-line functionality for advanced users
- **External Tool Integration**: Manages interaction with third-party conversion tools

## 🔧 Installation

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- Qt 5.12 or higher
- CMake 3.10 or higher
- External dependencies based on conversion needs:
  - Pandoc for document conversions
  - ImageMagick for image conversions
  - FFmpeg for audio/video conversions
  - LibreOffice for office document conversions

### Steps

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/FileConverter.git
   cd FileConverter
