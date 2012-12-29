#include <QtGui/QGuiApplication>
#include <QQuickItem>
#include <QDebug>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include "qtquick2applicationviewer.h"

#include "resource.h"



void populateResourceList(ListModel * model, QFile & resourceSaveFile);



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;

    // Get the file name
    QFile resourceSaveFile("C:\\Users\\jmbeck\\Desktop\\TaS Saves\\saves - Copy\\New Settlement\\re.sav");


    // Create a model that holds our resource data, and
    // add the data to it.
    ListModel * resourceModel = new ListModel(new Resource, qApp);
    populateResourceList(resourceModel, resourceSaveFile);

    // Create the proxy model that contains the results of the filter.
    QSortFilterProxyModel * proxyResourceModel = new QSortFilterProxyModel();
    proxyResourceModel->setSourceModel(resourceModel);
    proxyResourceModel->setFilterRole(Resource::NameRole);

    // Create a regex model for filtering
    QRegExp searchRegex("Dirt", Qt::CaseInsensitive, QRegExp::Wildcard);
    proxyResourceModel->setFilterRegExp(searchRegex);

    // Link the resource data to the GUI viewer
    viewer.rootContext()->setContextProperty("resourceModel", proxyResourceModel);
    viewer.rootContext()->setContextProperty("searchRegex", searchRegex.pattern());

    // Pull in the GUI
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));
    viewer.showExpanded();

    return app.exec();
}


void populateResourceList(ListModel * model, QFile & resourceSaveFile)
{
    // Pull the save file data in.
    if (resourceSaveFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Opened Save File: " << resourceSaveFile.fileName();
    }

    // Pull in the list of resource assets (name, group, etc)
    QFile assetFile(QCoreApplication::applicationDirPath() + "/resource_list.txt");

    if (assetFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Opened Asset File: " << assetFile.fileName();

        QTextStream assetStream(&assetFile);

        while (!resourceSaveFile.atEnd())
        {
            // Grab the next section and decide the size, etc.
            QString resourceString;
            if (!assetStream.atEnd())
            {
                resourceString = assetStream.readLine();
            }
            else
            {
                resourceString = "extra data,unknown,unknown.svg";
            }

            QStringList assetData;
            assetData = resourceString.split(',');

            QByteArray resourceQuantity;
            resourceQuantity = resourceSaveFile.read(2);
//            qDebug() << "Byte: " << resourceQuantity[0]
//                     << " and "  << resourceQuantity[1];

            // Create a new Resource with the quantity, and use the
            // data from the ResourceAssetList to flush it out.
            model->appendRow(new Resource(assetData[0],
                                          assetData[1],
                                          assetData[2],
                                          resourceQuantity[1]+128,
                                          0));
        }
    }
    else
    {
        qDebug() << "Could not open " << assetFile.fileName();
    }

    // Pull in the file that contains the saved-game resources



}
