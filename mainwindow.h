#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <myscene.h>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDateEdit>
#include <QNetworkConfigurationManager>
#include <QInputDialog>
#include <QString>
#include <QPainter>
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtAndroidExtras/QAndroidActivityResultReceiver>
#include <QFile>
/*
#define NEW_NOTE_PREFIX "##NEW-NOTE "
#define NEW_NOTE_PREFIX_LEN  11

class QNote{
public:
    QNote(const QString &title, const QString &content)
        : m_strTitle(title), m_strContent(content)
    {
    }

    QNote(const QNote &n)
        : m_strTitle(n.m_strTitle)
        , m_strContent(n.m_strContent)
    {

    }

    QNote &operator=(const QNote &r)
    {
        if(this != &r)
        {
            m_strTitle = r.m_strTitle;
            m_strContent = r.m_strContent;
        }
        return *this;
    }

    static void loadNotes(QList<QNote*> & notes);
    static void saveNotes(QList<QNote*> & notes);

    QString m_strTitle;
    QString m_strContent;
};*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void RefreshWeather();
    void WeatherDataAnalyze(QByteArray &WeaDat);
    void replyFinished(QNetworkReply *reply);
    void RefreshTime();

private slots:
    void on_light_clicked();

    void on_camera_clicked();

    void on_exit_clicked();

    void on_clock_clicked();

    void on_write_clicked();

protected:
    bool isNetWorkOnline();
    void newNote();
    //void drawBackground(QPainter &painter);
    //void paintEvent(QPaintEvent *event);


private:
    Ui::MainWindow *ui;
    myScene *sc;
    QNetworkAccessManager *manage;
    bool isNetWork;
    QPixmap *m_background;
    QString text1;
    QTimer *timer1;
    QString camera;
    QString note;
    QString alarm;


};






#endif // MAINWINDOW_H
