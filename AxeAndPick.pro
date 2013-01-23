# Add more folders to ship with the application, here
folder_01.source = qml/AxeAndPick
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Required for QApplication, which requires QWidgets which is necessary
# for things like QFileDialog.
QT += widgets

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    listmodel.cpp \
    settings.cpp \
    savesaccess.cpp \
    resourcelistmodel.cpp \
    savesoverviewlistmodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    listmodel.h \
    settings.h \
    savesaccess.h \
    resourcelistmodel.h \
    savesoverviewlistmodel.h

RESOURCES +=

OTHER_FILES += \
    resource_list.txt \
    qml/AxeAndPick/ToolbarTextInput.qml \
    qml/AxeAndPick/SavesOverviewDelegate.qml
