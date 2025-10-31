#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMetaType>
#include "cardsmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<FieldListModel*>("FieldListModel*");
    qRegisterMetaType<ChunkListModel*>("ChunkListModel*");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    CardsModel model; // Created and owned in C++
    engine.setInitialProperties({
        {"cardsModel", QVariant::fromValue(&model)}
    });
    engine.loadFromModule("test_nested_lists_2025_10_31", "Main");

    return app.exec();
}
