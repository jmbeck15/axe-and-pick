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
    QFile resourceSaveFile("C:\\Users\\jmbeck\\Desktop\\Timber and Stone pld1\\saves\\Test\\re.sav");


    // Create a model that holds our resource data, and
    // add the data to it.
    ListModel * resourceModel = new ListModel(new Resource, qApp);
    populateResourceList(resourceModel, resourceSaveFile);

    // Create the proxy model that contains the results of the filter.
    QSortFilterProxyModel * proxyResourceModel = new QSortFilterProxyModel();
    proxyResourceModel->setSourceModel(resourceModel);
    proxyResourceModel->setFilterRole(Resource::FilterStringRole);

    // Enable the case insensitivity
    proxyResourceModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // Setup sorting the resources
    //proxyResourceModel->setSortRole(Resource::TypeRole);
    //proxyResourceModel->sort(0);

    // Link the resource data to the GUI viewer
    viewer.rootContext()->setContextProperty("resourceModel", proxyResourceModel);

    Resource * testResource = new Resource("Test Resource", "mytype", "icon.jpg", 0, 0);
    viewer.rootContext()->setContextProperty("testResource", testResource);

    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));

    // Show the GUI
    viewer.showExpanded();

    return app.exec();
}


void populateResourceList(ListModel * model, QFile & resourceSaveFile)
{
    // Mask for the quantity bytes
    const unsigned char MASK = 0x3F;

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
        //for( int i=0; i<3; i++ )
        {
            // Grab the next section and decide the size, etc.
            QString resourceString;
            if (!assetStream.atEnd())
            {
                resourceString = assetStream.readLine();
            }
            else
            {
                resourceString = "unexpected data,unknown,resource-unknown.svg";
            }

            QStringList assetData;
            assetData = resourceString.split(',');

            // Temporary array to hold the bytes we read from the file.
            quint8 byteArray[4];

            char sigByte; // Most Significant Byte
            resourceSaveFile.read(&sigByte, 1);
            if( (unsigned char)sigByte >= 0xE0)
            {
                byteArray[2] = (sigByte - 0xE0);

                // Pull in the middle byte, because they must be processed together.
                char middleByte;
                resourceSaveFile.read(&middleByte, 1);
                byteArray[1] = (middleByte & MASK);
            }
            else
            {
                byteArray[2] = 0;
                byteArray[1] = sigByte - 0xC0;
            }

            char leastByte;
            resourceSaveFile.read(&leastByte, 1);
            byteArray[0] = leastByte & MASK;

            // Build the quantity out of the read bytes.
            quint32 resourceQuantity;
            resourceQuantity = byteArray[0];
            resourceQuantity |= ((byteArray[1] | (byteArray[2]<<6)) -2) << 6;

            // Create a new Resource with the quantity, and use the
            // data from the ResourceAssetList to flush it out.
            model->appendRow(new Resource(assetData[0],
                                          assetData[1],
                                          assetData[2],
                                          resourceQuantity,
                                          0));
        }
    }
    else
    {
        qDebug() << "Could not open " << assetFile.fileName();
    }

    // Pull in the file that contains the saved-game resources



}
