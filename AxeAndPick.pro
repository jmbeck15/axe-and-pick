# Add more folders to ship with the application, here
folder_01.source = qml/AxeAndPick
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# OSX Options
Resource.files = resource_list.csv male_names.txt female_names.txt
Resource.path = Contents/MacOS
QMAKE_BUNDLE_DATA += Resource
ICON = application_icon.icns

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Required for QApplication, which requires QWidgets which is necessary
# for things like QFileDialog.
QT += widgets

# Adds the icon to the project
RC_FILE = AxeAndPick.rc

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    listmodel.cpp \
    settings.cpp \
    savesaccess.cpp \
    resourcelistmodel.cpp \
    humanlistmodel.cpp \
    savedgamelistmodel.cpp \
    violentmoblistmodel.cpp \
    neutralmoblistmodel.cpp \
    utils.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    listmodel.h \
    settings.h \
    savesaccess.h \
    resourcelistmodel.h \
    humanlistmodel.h \
    savedgamelistmodel.h \
    violentmoblistmodel.h \
    neutralmoblistmodel.h \
    utils.h

RESOURCES +=

OTHER_FILES += \
    qml/AxeAndPick/ToolbarTextInput.qml \
    resource_list.csv \
    AxeAndPick.rc \
    application_icon.ico \
    qml/AxeAndPick/SavedGameDelegate.qml \
    qml/AxeAndPick/NeutralMobDelegate.qml \
    qml/AxeAndPick/ViolentMobDelegate.qml \
    application_icon.icns \
    male_names.txt \
    female_names.txt
