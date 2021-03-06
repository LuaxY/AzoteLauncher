#ifndef UPDATER_H
#define UPDATER_H

#include <QThread>
#include <QElapsedTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QSettings>
#include <QMutex>
#include <QWaitCondition>

#include "../http/http.h"
#include "../logger/logger.h"
#include "../utils/system.h"

enum SelfUpdateStatus
{
    OK_TO_CONTINUE,
    UPDATE_FAILED,
    UPDATE_IN_PROGRESS
};

class Updater : public QThread
{
    Q_OBJECT

private:
    Logger* log;
    volatile bool continueUpgrading;
    QElapsedTimer downloadTime;
    QVector<QString> updatedFiles;
    int currentClientVersion;
    QString updateFileName;
    QSettings* settings;
    OperatingSystem os;
    bool bPause;
    QMutex sync;
    QWaitCondition pauseCond;
    QString serverName;

    int filesCount;
    int progressStep;
    int updateCounter;
    int fileCounter;

    void run();
    void processUpdate(Http* http);
    void getCurrentVersionFromConfig();
    QJsonObject getInfoFile(Http* http);
    QJsonObject getUpdateFile(Http* http, QString url);
    bool updateGameFiles(Http* http, QString url, QJsonArray files, QString pathPrefix, QString urlPrefix);
    bool checkIfFileRequireUpdate(QString path, QString md5);
    bool updateGameFile(Http* http, QString url, QString name, QString urlName);

public:
    explicit Updater(QString _serverName, QThread* parent = 0);
    ~Updater();
    void stopProcess();
    void resume();
    void pause();
    bool isNeedUpdate();
    int getCurrentVersion();

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void updateProgressBarTotal(const int value);
    void updateProgressPercent(const QString value);
    void updateDownloadSpeed(const QString speed);
    void updateStatus(const QString status);
    void updateFinished();
};

#endif // UPDATER_H
