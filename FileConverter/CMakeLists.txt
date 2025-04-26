cmake_minimum_required(VERSION 3.10)
project(FileConverter VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Add MinGW-specific settings
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Try to find Qt in common installation locations
list(APPEND CMAKE_PREFIX_PATH 
    "C:/Qt"
    "C:/Qt/6.8.2/mingw_64"
    "C:/Qt/6.8.2/msvc2022_64"
    "C:/Qt/6.8.2/msvc2019_64"
    "C:/Qt/6.8.2"
    "C:/Qt/Qt6.8.2"
    "C:/Qt/6.8"
    "C:/Program Files/Qt"
    "C:/Program Files/Qt/6.8.2"
    "C:/Program Files/Qt/6.8.2/mingw_64"
    "C:/Program Files/Qt/6.8.2/msvc2022_64"
    "C:/Program Files (x86)/Qt"
    "C:/Program Files (x86)/Qt/6.8.2"
)

# Try to find Qt6 first, then fall back to Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Qt6 found. Building GUI application.")
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBS Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
        message(STATUS "Qt5 found. Building GUI application.")
        set(QT_VERSION_MAJOR 5)
        set(QT_LIBS Qt5::Widgets)
    else()
        message(STATUS "Qt not found. Building command-line application only.")
        set(QT_VERSION_MAJOR 0)
    endif()
endif()

# If Qt was found, build the GUI application
if(QT_VERSION_MAJOR GREATER 0)
    # Set automoc on for Qt
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    # Include directories
    include_directories(include)
    
    # Add CLI executable
    add_executable(FileConverter 
        src/main.cpp
        src/FileConverter.cpp
    )
    
    # Add GUI executable
    add_executable(FileConverterUI WIN32
        src/qt_main.cpp
        src/FileConverter.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/MainWindow.ui
    )
    
    # Link Qt libraries to GUI executable
    target_link_libraries(FileConverterUI ${QT_LIBS})
    
    # Install targets
    install(TARGETS FileConverter FileConverterUI DESTINATION bin)
    
    # Add deployment target for Windows
    if(WIN32)
        # Find the windeployqt tool
        if(QT_VERSION_MAJOR EQUAL 6)
            find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
        else()
            find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
        endif()
        
        if(WINDEPLOYQT_EXECUTABLE)
            # Add custom command to run windeployqt after build
            add_custom_command(TARGET FileConverterUI POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE} --release --no-translations --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:FileConverterUI>
                COMMENT "Running windeployqt..."
            )
        endif()
    endif()
else()
    # Include directories
    include_directories(include)
    
    # Add CLI executable only
    add_executable(FileConverter 
        src/main.cpp
        src/FileConverter.cpp
    )
    
    # Install target
    install(TARGETS FileConverter DESTINATION bin)
endif()

# Enable testing
enable_testing()

# Add test executable - include FileConverter.cpp here too
add_executable(FileConverterTests 
    test/test_main.cpp
    src/FileConverter.cpp
)
add_test(NAME FileConverterTests COMMAND FileConverterTests)

# Add a message about dependencies
message(STATUS "Note: For image conversion, ImageMagick must be installed")
message(STATUS "Note: For video conversion, FFmpeg must be installed")