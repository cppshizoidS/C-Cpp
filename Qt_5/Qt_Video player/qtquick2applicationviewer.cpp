#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QtWidgets/QFileDialog>
#include <QtCore/QStandardPaths>
#include <QtCore/QDebug>

class QtQuick2ApplicationViewerPrivate
{
    QString mainQmlFile;
    friend class QtQuick2ApplicationViewer;
    static QString adjustPath(const QString &path);
};

QString QtQuick2ApplicationViewerPrivate::adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif !defined(Q_OS_ANDROID)
    const QString pathInInstallDir =
            QString::fromLatin1("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
#endif
    return path;
}

QtQuick2ApplicationViewer::QtQuick2ApplicationViewer(QWindow *parent)
    : QQuickView(parent)
    , d(new QtQuick2ApplicationViewerPrivate())
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setFlags( flags() |
              Qt::WindowTitleHint |
              Qt::WindowSystemMenuHint |
              Qt::WindowMinimizeButtonHint |
              Qt::WindowMaximizeButtonHint |
              Qt::WindowCloseButtonHint |
              Qt::WindowFullscreenButtonHint);

#ifdef Q_OS_ANDROID
    engine()->setBaseUrl(QUrl::fromLocalFile("/"));
#endif
}

QtQuick2ApplicationViewer::~QtQuick2ApplicationViewer()
{
    delete d;
}

void QtQuick2ApplicationViewer::setMainQmlFile(const QString &file)
{
    d->mainQmlFile = QtQuick2ApplicationViewerPrivate::adjustPath(file);
    setSource(QUrl::fromLocalFile(d->mainQmlFile));
}

void QtQuick2ApplicationViewer::addImportPath(const QString &path)
{
    engine()->addImportPath(QtQuick2ApplicationViewerPrivate::adjustPath(path));
}

void QtQuick2ApplicationViewer::showExpanded()
{
#if defined(Q_WS_SIMULATOR)
    showFullScreen();
#else
    show();
#endif
}

void QtQuick2ApplicationViewer::suggestResize(const QSize &size)
{
    if (size.isValid()) {
        if (windowState() == Qt::WindowFullScreen)
            toggleFullscreen();
        resize(size);
    }
}

void QtQuick2ApplicationViewer::toggleFullscreen()
{
    if (windowState() == Qt::WindowFullScreen) {
        showNormal();
    } else {
        showFullScreen();
    }
}

QString QtQuick2ApplicationViewer::openFileDialog()
{
    if (windowState() == Qt::WindowFullScreen)
        toggleFullscreen();

    return QFileDialog::getOpenFileName(0, "Video File", QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).first(), "");
}
