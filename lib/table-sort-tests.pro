# Reuse the production sources, Qt configuration and objects; no J engine needed.
include(lib.pro)
QT += testlib
TEMPLATE = app
TARGET = table-sort-tests
CONFIG -= dll shared app_bundle
CONFIG += console testcase
DESTDIR = ../bin/table-sort-tests
DLLDESTDIR =
SOURCES += ../tests/wd/table_sort_test.cpp
