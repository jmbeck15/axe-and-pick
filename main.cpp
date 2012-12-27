#include <QtGui/QGuiApplication>
#include <QQuickItem>
#include <QDebug>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"

#include "resource.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;

    // Create a model that holds our resource data.
    ListModel * model = new ListModel(new Resource, qApp);
    model->appendRow(new Resource("Dirt", "Basic", "dirt.svg", 34, 1923, model));
    model->appendRow(new Resource("Stone", "Basic", "stone.svg", 5, 256, model));
    model->appendRow(new Resource("Wood Logs", "Basic", "wood.svg", 9, 453, model));

    // Link the resource data to the GUI viewer
    viewer.rootContext()->setContextProperty("resourceModel", model);

    // Pull in the GUI
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
