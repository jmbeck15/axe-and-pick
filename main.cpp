#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>
#include <QQuickItem>
#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include <QtWidgets/QFileDialog>
#include "qtquick2applicationviewer.h"

#include "resourcelistmodel.h"
#include "savedgamelistmodel.h"
#include "settings.h"
#include "savesaccess.h"


void populateResourceList(ResourceListModel * model, QFile & resourceSaveFile);


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;
    viewer.setMinimumWidth(400);
    viewer.setMinimumHeight(350);

    QCoreApplication::setApplicationName("Axe and Pick");
    QCoreApplication::setOrganizationDomain("potatominingcorp.com");
    QCoreApplication::setOrganizationName("Potato Mining Corporation");

    // Register the list item types for the enumerations in QML
    qmlRegisterType<Resource>("Resource", 1,0, "Resource");

    // Load and create the settings. Give QML access.
    Settings settings;
    viewer.rootContext()->setContextProperty("settings", &settings);

    // This holds the info for all the saved games.
    SavesAccess savesAccess;
    savesAccess.setFilePath(settings.value("TimberAndStone/GameInstallationDirectory").toString());
    SavedGameListModel * savedGameModel = new SavedGameListModel(new SavedGameItem, qApp);
    savesAccess.setSavedGameListModel(savedGameModel);
    savesAccess.loadGamesList();

    // Saved game list should be completely initialized. Now we can expose
    // the data to QML.
    viewer.rootContext()->setContextProperty("savesAccess", &savesAccess);
    viewer.rootContext()->setContextProperty("savedGameModel", savedGameModel);

    // DEBUG: Just to load the resources. Will be removed shortly.
    QFile resourceSaveFile("C:\\Users\\jmbeck\\Desktop\\TaS Saves\\saves\\New Settlement\\re.sav");


    // Create a model that holds our resource data, and
    // add the data to it. Then make it available to QML.
    ResourceListModel * resourceModel = new ResourceListModel(new Resource, qApp);
    populateResourceList(resourceModel, resourceSaveFile);
    viewer.rootContext()->setContextProperty("resourceModel", resourceModel);


    // Create the proxy model that contains the results of the filter.
    QSortFilterProxyModel * proxyResourceModel = new QSortFilterProxyModel();
    proxyResourceModel->setSourceModel(resourceModel);
    proxyResourceModel->setFilterRole(Resource::FilterStringRole);
    viewer.rootContext()->setContextProperty("resourceModelProxy", proxyResourceModel);

    // Enable the case insensitivity
    proxyResourceModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // Setup sorting the resources
    //proxyResourceModel->setSortRole(Resource::TypeRole);
    //proxyResourceModel->sort(0);

    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));

    // Show the GUI
    viewer.showExpanded();

    return app.exec();
}


void populateResourceList(ResourceListModel * model, QFile & resourceSaveFile)
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
        long byteNumber(0);

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
            byteNumber++;
            if( (unsigned char)sigByte >= 0xE0)
            {
                byteArray[2] = (sigByte - 0xE0);

                // Pull in the middle byte, because they must be processed together.
                char middleByte;
                resourceSaveFile.read(&middleByte, 1);
                byteNumber++;
                byteArray[1] = (middleByte & MASK);
            }
            else
            {
                byteArray[2] = 0;
                byteArray[1] = sigByte - 0xC0;
            }

            char leastByte;
            resourceSaveFile.read(&leastByte, 1);
            byteNumber++;
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
                                          byteNumber));
        }
    }
    else
    {
        qDebug() << "Could not open " << assetFile.fileName();
    }

    // Pull in the file that contains the saved-game resources
}
