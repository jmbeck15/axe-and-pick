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
    model->appendRow(new Resource("Dirt", 34, 1923, model));
    model->appendRow(new Resource("Stone", 5, 256, model));
    model->appendRow(new Resource("Wood Logs", 9, 453, model));

    // Link the resource data to the GUI viewer
    viewer.rootContext()->setContextProperty("resourceModel", model);

    // Pull in the GUI
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));
    viewer.showExpanded();

    // DEBUG: Mess with junk and stuff.
//    QQuickItem * root = viewer.rootObject();
//    QQuickItem * square = root->findChild<QQuickItem*>("statusBarBackground");
//    QQuickItem * bar = square->parentItem();
//    square->setProperty("color", "red");
//    bar->setHeight(80);


    //Resource test("banana",3,4);
    //QHash<int, QByteArray> mynames = model->find("Dirt")->roleNames();
    //qDebug() << "Model: " << mynames;


    return app.exec();
}
