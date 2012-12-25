#include <QtGui/QGuiApplication>
#include <QQuickItem>
#include <QDebug>
#include "qtquick2applicationviewer.h"

#include "resource.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));

    QQuickItem * root = viewer.rootObject();
    QQuickItem * square = root->findChild<QQuickItem*>("statusBarBackground");
    QQuickItem * bar = square->parentItem();
    square->setProperty("color", "red");
    bar->setHeight(80);
    viewer.showExpanded();

    ListModel * model = new ListModel(new Resource, qApp);
    model->appendRow(new Resource("Dirt", 34, 1923, model));


    return app.exec();
}
