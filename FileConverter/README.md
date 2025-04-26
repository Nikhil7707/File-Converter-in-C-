# FileConverter

A versatile file conversion utility written in C++ that allows you to convert between various file formats.

## Features

- Convert between text formats (TXT, CSV)
- Convert between image formats (JPG, PNG, GIF, BMP) using ImageMagick
- Convert between video formats (MP4, AVI, MOV, MKV) using FFmpeg
- Simple command-line interface
- Extensible architecture for adding new converters

## Prerequisites

- C++17 compatible compiler (GCC, MinGW, etc.)
- CMake 3.10 or higher
- For image conversion: ImageMagick
- For video conversion: FFmpeg

## Building the Project

1. Clone the repository
2. Create a build directory:
```
mkdir build
cd build
```
3. Configure with CMake:
```
cmake -G "MinGW Makefiles" ..
```
4. Build the project:
```
mingw32-make
```

## Usage

### Command Line Interface

```
FileConverter input_file output_file
```

Examples:
- Convert TXT to CSV: `FileConverter input.txt output.csv`
- Convert CSV to TXT: `FileConverter input.csv output.txt`
- Convert JPG to PNG: `FileConverter input.jpg output.png`
- Convert MP4 to AVI: `FileConverter input.mp4 output.avi`

### Supported Formats

- Text: TXT, CSV
- Image: JPG, PNG, GIF, BMP
- Video: MP4, AVI, MOV, MKV

## Testing

Run the tests with:
```
FileConverterTests
```

## Project Structure

- `include/` - Header files
- `src/` - Source files
- `test/` - Test files
- `build/` - Build output (generated)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- ImageMagick for image conversion
- FFmpeg for video conversion
