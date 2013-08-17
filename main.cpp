#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>
#include <QQuickItem>
#include <QDebug>
#include <QSettings>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include <QtWidgets/QFileDialog>
#include <QTime>
#include "qtquick2applicationviewer.h"

#include "resourcelistmodel.h"
#include "humanlistmodel.h"
#include "savedgamelistmodel.h"
#include "settings.h"
#include "savesaccess.h"

#define TESTING 0

QString appVersion()
{
    // Version History
    // ---------------------
    // 0.4 - first release, only resource editing
    // 0.5 - second release, only tweaked resource list
    // 1.0 - units: add and remove, added error display
    // 1.1 - updated for T&S 1.0.
    // 1.2 - fixed the animal life toggling on load
    // 1.3 - added coffee, gave new units coffee
    // 1.4 - updated for T&S 1.1.
    // 1.5 - fixed oddly-infrequent crash
    // 1.6 - added support for patrol points
    // 1.7 - fixed adventurer (T&S v1.2 didn't change anything)
    // 1.8 - updated for T&S 1.3 (and some other tweaks)
    // 1.9 - updated for T&S 1.4
    //
    return "1.9 (for T&S v1.4) BETA 1";
}

int main(int argc, char *argv[])
{
    // Seed the random number generator
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;
    viewer.setMinimumWidth(400);
    viewer.setMinimumHeight(350);

    QStringList libPaths = QCoreApplication::libraryPaths();
    libPaths.append(QCoreApplication::applicationDirPath() + "/plugins");
    app.setLibraryPaths(libPaths);

#if TESTING
    QByteArray binary;
    unsigned int result;
    bool failed(false);
    for( unsigned int value=0; value<65000; value++ )
    {
        binary = SavesAccess::toBinary(value);
        result = SavesAccess::toInt(binary);
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

    //
    // SAVED GAMES
    //
    // This holds the info for all the saved games.
    SavesAccess savesAccess;
    savesAccess.setFilePath(settings.value("TimberAndStone/GameInstallationDirectory").toString());
    SavedGameListModel * savedGameModel = new SavedGameListModel(new SavedGame, qApp);
    savesAccess.setSavedGameListModel(savedGameModel);
    savesAccess.loadGamesList();
    viewer.rootContext()->setContextProperty("savesAccess", &savesAccess);
    viewer.rootContext()->setContextProperty("savedGameModel", savedGameModel);

    //
    // RESOURCES
    //
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


    //
    // UNITS
    //
    // Humans
    HumanListModel * humanModel = new HumanListModel(new Human, qApp);
    savesAccess.setHumanModel(humanModel);
    viewer.rootContext()->setContextProperty("humanModel", humanModel);
    QSortFilterProxyModel * proxyHumanModel = new QSortFilterProxyModel();
    proxyHumanModel->setSourceModel(humanModel);
    proxyHumanModel->setFilterRole(Human::FilterStringRole);
    proxyHumanModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    viewer.rootContext()->setContextProperty("humanModelProxy", proxyHumanModel);
    proxyHumanModel->setFilterRegExp("^(?!unknown).*");

    // Neutral Mobs
    NeutralMobListModel * neutralMobModel = new NeutralMobListModel(new NeutralMob, qApp);
    savesAccess.setNeutralMobModel(neutralMobModel);
    viewer.rootContext()->setContextProperty("neutralMobModel", neutralMobModel);
    QSortFilterProxyModel * proxyNeutralMobModel = new QSortFilterProxyModel();
    proxyNeutralMobModel->setSourceModel(neutralMobModel);
    proxyNeutralMobModel->setFilterRole(NeutralMob::FilterStringRole);
    proxyNeutralMobModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    viewer.rootContext()->setContextProperty("neutralMobModelProxy", proxyNeutralMobModel);

    // Violent Mobs
    ViolentMobListModel * violentMobModel = new ViolentMobListModel(new ViolentMob, qApp);
    savesAccess.setViolentMobModel(violentMobModel);
    viewer.rootContext()->setContextProperty("violentMobModel", violentMobModel);
    QSortFilterProxyModel * proxyViolentMobModel = new QSortFilterProxyModel();
    proxyViolentMobModel->setSourceModel(violentMobModel);
    proxyViolentMobModel->setFilterRole(ViolentMob::FilterStringRole);
    proxyViolentMobModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    viewer.rootContext()->setContextProperty("violentMobModelProxy", proxyViolentMobModel);


    // Load the QML
    viewer.setMainQmlFile(QStringLiteral("qml/AxeAndPick/main.qml"));

    // Show the GUI
    viewer.showExpanded();

    // Start the main event loop.
    int appExecReturn = app.exec();

    // Clean up
    delete proxyResourceModel;
    delete proxyHumanModel;
    delete proxyNeutralMobModel;
    delete proxyViolentMobModel;

    return appExecReturn;
}
