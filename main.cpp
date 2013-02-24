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
#include "savesoverviewlistmodel.h"
#include "settings.h"
#include "savesaccess.h"

#define TESTING 0

QString appVersion()
{
    return "0.3";
}

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;
    viewer.setMinimumWidth(400);
    viewer.setMinimumHeight(350);

    QStringList libPaths = QCoreApplication::libraryPaths();
    libPaths.append(QCoreApplication::applicationDirPath() + "/plugins");
    app.setLibraryPaths(libPaths);

#if TESTING
    QByteArray binary;
    long result;
    bool failed(false);
    for( long value=0; value<700000; value++ )
    {
        binary = SavesAccess::toBinary(value);
        result = SavesAccess::toLong(binary);
        if( result != value)
        {
            qDebug() << "Value" << value << "returned" << result;
            failed = true;
            break;
        }
    }
    if( failed )
    {
        qDebug() << "[FAILURE] The tests did NOT succeed!";
    }
    else
    {
        qDebug() << "[PASSED]  All tests passed.";
    }
#endif

    QCoreApplication::setApplicationName("Axe and Pick");
    QCoreApplication::setOrganizationDomain("potatominingcorp.com");
    QCoreApplication::setOrganizationName("Potato Mining Corporation");

    app.setWindowIcon(QIcon());
    viewer.setTitle(QCoreApplication::applicationName() + " v" + appVersion());

    // Register the list item types for the enumerations in QML
    qmlRegisterType<Resource>("Resource", 1,0, "Resource");

    // Load and create the settings. Give QML access.
    Settings settings;
    viewer.rootContext()->setContextProperty("settings", &settings);

    // This holds the info for all the saved games.
    SavesAccess savesAccess;
    savesAccess.setFilePath(settings.value("TimberAndStone/GameInstallationDirectory").toString());
    SavesOverviewListModel * savesOverviewModel = new SavesOverviewListModel(new SavesOverviewItem, qApp);
    savesAccess.setSavesOverviewListModel(savesOverviewModel);
    savesAccess.loadGamesList();

    // Saved game list should be completely initialized. Now we can expose
    // the data to QML.
    viewer.rootContext()->setContextProperty("savesAccess", &savesAccess);
    viewer.rootContext()->setContextProperty("savesOverviewModel", savesOverviewModel);

    // Create a model that holds our resource data, and
    // add the data to it. Then make it available to QML.
    ResourceListModel * resourceModel = new ResourceListModel(new Resource, qApp);
    savesAccess.setResourceListModel(resourceModel);
    viewer.rootContext()->setContextProperty("resourceModel", resourceModel);

    // Create the proxy model that contains the results of the filter.
    QSortFilterProxyModel * proxyResourceModel = new QSortFilterProxyModel();
    proxyResourceModel->setSourceModel(resourceModel);
    proxyResourceModel->setFilterRole(Resource::FilterStringRole);

    // This prevents unknown items from showing up in the list.
    proxyResourceModel->setFilterRegExp("^(?!unknown).*");
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
