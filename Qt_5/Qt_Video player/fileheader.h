#include <QtCore/QObject>

class FileReader : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QString readFile(const QString &fileName);
};

