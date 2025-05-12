TEMPLATE = app
TARGET = "AMV Vote"

QT       += widgets
CONFIG   += c++23

# Include paths
INCLUDEPATH += src \
               src/db \
               src/services \
               src/ui

# Source files
SOURCES += main.cpp \
           $$files(src/db/*.cpp) \
           $$files(src/services/*.cpp) \
           $$files(src/ui/*.cpp) \

# Header files
HEADERS += $$files(src/db/*.hpp) \
           $$files(src/services/*.hpp) \
           $$files(src/ui/*.hpp) \
           $$files(src/ui/*.h) \
           $$files(src/*.hpp) \

# Deployment (if linux)
unix:!android {
    target.path = /opt/$${TARGET}/bin
    INSTALLS += target
}

RESOURCES += \
    imgs/resources.qrc

RC_FILE = app_icon.rc
