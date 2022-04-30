#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtCore/QStandardPaths>
#include "qtquick2applicationviewer.h"
#include "filereader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QUrl fileName;
    qreal volume = 0.5;
    QStringList args = app.arguments();

    for (int i=1; i<args.count(); ++i) {
        const QString &arg = args.at(i);
        if (arg.startsWith('-')) {
            if ("-volume" == arg) {
                if (i+1 < args.count())
                    volume = 0.01 * args.at(++i).toInt();
                else
                    qWarning() << "Option \"-volume\" takes a value";
            }
            else {
                qWarning() << "Option" << arg << "ignored";
            }
        } else {
            if (fileName.isEmpty())
                fileName = QUrl(arg);
            else
                qWarning() << "Argument" << arg << "ignored";
        }
    }

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("fileName", fileName);
    viewer.rootContext()->setContextProperty("startingVolume", volume);
    viewer.rootContext()->setContextProperty("viewer", &viewer);
    viewer.setMainQmlFile(QStringLiteral("qml/ShizPlayer/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
