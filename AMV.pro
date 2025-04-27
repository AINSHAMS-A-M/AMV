TEMPLATE = app
TARGET = vote

QT       += core gui widgets
CONFIG   += c++23

# Warn on all
QMAKE_CXXFLAGS += -Wall -Wextra

# Include paths
INCLUDEPATH += src \
               src/db \
               src/services \
               src/ui

# Source files
SOURCES += main.cpp \
           $$files(src/db/*.cpp) \
           $$files(src/services/*.cpp) \
           $$files(src/ui/*.cpp)

# Header files
HEADERS += $$files(src/db/*.hpp) \
           $$files(src/services/*.hpp) \
           $$files(src/ui/*.hpp) \
           $$files(src/ui/*.h) \
           $$files(src/*.hpp)

# UI forms
FORMS += $$files(src/ui/*.ui)

# Deployment (optional)
unix:!android {
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}
