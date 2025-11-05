#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cardmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Windows"));  // 👇 Set Windows style globally

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    CardModel model;
    engine.setInitialProperties({
        {"cardModel", QVariant::fromValue(&model)}
    });

    engine.loadFromModule("temp_qt_quick_nested_flow", "Main");

    return app.exec();
}
