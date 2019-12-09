TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32: QMAKE_CXXFLAGS += /std:c++17
else: QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        main.cpp

HEADERS += \
    core/fixed_string.hpp \
    core/fixed_string_utils.hpp \
    core/insert_or_update.hpp \
    core/reflection.hpp
