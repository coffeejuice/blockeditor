// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "visualcardsmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    VisualCardsModel model;

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.setInitialProperties(
        {
            {"cppCardsListModel", QVariant::fromValue(&model)},
        }
    );

    engine.loadFromModule("cardmix", "Main");

    return app.exec();
}
