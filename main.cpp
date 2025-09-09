#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "OperationModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    OperationModel model;

    // Your 10 records, indices 0..9
    const QStringList labels = {
        "Upset",
        "Upset 3 Bites",
        "Upset Small",
        "Upset Chamfering",
        "Draw",
        "Draw Change Direction",
        "Cut Hot",
        "Cut Sawing",
        "Pierce",         // added to reach 10
        "Swage"           // added to reach 10
    };

    QVector<operationLib> ops;
    ops.reserve(labels.size());
    for (int i = 0; i < labels.size(); ++i)
        ops.push_back({ i, labels.at(i) });
    model.setOperations(std::move(ops));

    QQmlApplicationEngine engine;

    // Expose model as 'lib' to QML
    engine.rootContext()->setContextProperty("lib", &model);
    engine.loadFromModule("Layouts", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    // Set QML property 'libRef' on the root object
    QObject *root = engine.rootObjects().first();
    root->setProperty("libRef", QVariant::fromValue(&model));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Layouts", "Main");

    return app.exec();
}

