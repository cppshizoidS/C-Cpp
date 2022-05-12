#ifndef LOGGER_H
#define LOGGER_H

#include <map>
#include <functional>

#include <QtGlobal>
#include <QtDebug>
#include <QDate>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

const QString DATETIME_FORMAT = "dd.MM.yyyy hh:mm:ss";
const QString LOG_FILENAME = "log.txt";
const QString LOG_STR_FORMAT = "%1 %2 %3 %4";

QString getProgramName()
{
    return QFileInfo( QCoreApplication::applicationFilePath() ).fileName();

}

void logMessagesInFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile file(LOG_FILENAME);
    auto programName = getProgramName();
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        auto datetime = QDateTime::currentDateTime().toString(DATETIME_FORMAT);
        switch (type) {
        case QtDebugMsg:
            stream << LOG_STR_FORMAT.arg(datetime, "INFO", programName, msg) << endl;
            break;
        case QtWarningMsg:
            stream << LOG_STR_FORMAT.arg(datetime, "WARNING", programName, msg) << endl;
            break;
        case QtCriticalMsg:
            stream << LOG_STR_FORMAT.arg(datetime, "CRITICAL", programName, msg) << endl;
            break;
        case QtFatalMsg:
            stream << LOG_STR_FORMAT.arg(datetime, "FATAL", programName, msg) << endl;
            QCoreApplication::exit(-1);
        }
    }

}

#endif // LOGGER_H
