#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqml.h>

#include "BlockModel.h"   // your custom model

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Create the model instance
    BlockModel model;

    // Expose it to QML under the name "blockModel"
    engine.rootContext()->setContextProperty("blockModel", &model);

    qmlRegisterUncreatableType<BlockModel>("blockeditor", 1, 0, "BlockModel", "Enums only");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("blockeditor", "Main");

    return app.exec();
}
