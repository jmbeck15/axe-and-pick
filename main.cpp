#include <QtGui/QGuiApplication>
#include <QQuickItem>
#include <QDebug>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include "qtquick2applicationviewer.h"

#include "resource.h"



void populateResourceList(ListModel * model);



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;

    // Create a model that holds our resource data, and
    // add the data to it.
    ListModel * resourceModel = new ListModel(new Resource, qApp);
    populateResourceList(resourceModel);

    // Create the proxy model that contains the results of the filter.
    QSortFilterProxyModel * proxyResourceModel = new QSortFilterProxyModel();
    proxyResourceModel->setSourceModel(resourceModel);

    // Create a regex model for filtering
    QRegExp searchRegex("*", Qt::CaseInsensitive, QRegExp::Wildcard);
    proxyResourceModel->setFilterRegExp(searchRegex);

    // Link the resource data to the GUI viewer
    viewer.rootContext()->setContextProperty("resourceModel", proxyResourceModel);
    viewer.rootContext()->setContextProperty("searchRegex", searchRegex);

    // Pull in the GUI
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));
    viewer.showExpanded();

    return app.exec();
}


void populateResourceList(ListModel * model)
{
    // Pull in the list of resource assets (name, group, etc)
    QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.txt");

    if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Opened " << assetFile.fileName();

        QTextStream assetStream(&assetFile);

        while (!assetStream.atEnd())
        {
            // Grab the next section and decide the size, etc.
            QString resourceString;
            resourceString = assetStream.readLine();

            QStringList resourceData;
            resourceData = resourceString.split(',');

            // Create a new Resource with the quantity, and use the
            // data from the ResourceAssetList to flush it out.
            model->appendRow(new Resource(resourceData[0],
                                          resourceData[1],
                                          resourceData[2],
                                          0,
                                          0));
        }
    }
    else
    {
        qDebug() << "Could not open " << assetFile.fileName();
    }

    // Pull in the file that contains the saved-game resources



}
