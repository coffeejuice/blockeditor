#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "blockslistmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    BlocksListModel blocksListModel;

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.setInitialProperties(
        {
            {"blocksListModel", QVariant::fromValue(&blocksListModel)},
        }
    );

    engine.loadFromModule("blockeditor", "Main");

    return app.exec();
}
