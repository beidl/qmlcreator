#ifndef EXTERNALPROJECTPICKER_H
#define EXTERNALPROJECTPICKER_H

#include <QObject>
#include <QUrl>

class ExternalProjectPicker : public QObject
{
    Q_OBJECT
public:
    explicit ExternalProjectPicker(QObject *parent = nullptr);
    ~ExternalProjectPicker();

public slots:
    void startImport();
    QString openFileOrPath(const QByteArray &encodedData);
    void closeFile(QUrl url);
    QString getDirNameForBookmark(const QByteArray& encodedData);

signals:
    void documentSelected(QByteArray document);
};

#endif // EXTERNALPROJECTPICKER_H
