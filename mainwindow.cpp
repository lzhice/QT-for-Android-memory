#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QTextCodec>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QInputDialog>
#include <QPalette>


using namespace QtAndroid;

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
        qDebug() << "exception occured";\
        env->ExceptionClear();\
    }

class ResultReceiver: public QAndroidActivityResultReceiver
{
public:
    ResultReceiver(QString imagePath, QLabel *view)
        : m_imagePath(imagePath), m_imageView(view)
    {

    }

    void handleActivityResult(
            int receiverRequestCode,
            int resultCode,
            const QAndroidJniObject & data)
    {
        qDebug() << "handleActivityResult, requestCode - " << receiverRequestCode
                    << " resultCode - " << resultCode
                    << " data - " << data.toString();
        //RESULT_OK == -1
        if(resultCode == -1 && receiverRequestCode == 1)
        {
            qDebug() << "captured image to - " << m_imagePath;
            qDebug() << "captured image exist - " << QFile::exists(m_imagePath);
            m_imageView->setPixmap(QPixmap(m_imagePath));
        }
    }

    QString m_imagePath;
    QLabel *m_imageView;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    text1 ="武汉";
    /*QPalette background;
    background.setBrush(QPalette::Background,QBrush(QPixmap(":/bg/images/background.png")));
    this->setPalette(background);*/
    ui->weather->setAttribute(Qt::WA_TranslucentBackground);
    ui->date->setAttribute(Qt::WA_TranslucentBackground);

    //定时器-用于显示当前时间
    QTimer *timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(RefreshTime()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(RefreshWeather()));
    timer1->start(2000);
    isNetWork = isNetWorkOnline();

    QString camera = "android.media.action.IMAGE_CAPTURE";
}

MainWindow::~MainWindow()
{
    delete m_background;
    delete ui;
}

void MainWindow::RefreshWeather()
{
    manage = new QNetworkAccessManager(this);
    QNetworkRequest network_request;
    /*设置发送数据*/
    //qDebug()<<text1;
    QString net = "http://wthrcdn.etouch.cn/weather_mini?city="+text1;
    //qDebug()<<net;
   // network_request.setUrl(QUrl("http://wthrcdn.etouch.cn/weather_mini?city="+text1));
    network_request.setUrl(QUrl(net));
    network_request.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    connect(manage,SIGNAL(finished(QNetworkReply *)),this,SLOT(replyFinished(QNetworkReply*)));
    /*发送get网络请求*/
    manage->get(network_request);

}

void MainWindow::WeatherDataAnalyze(QByteArray &WeaDat)
{
    bool ok = true;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(WeaDat,&err);

    QJsonObject root = doc.object();
    QString CityName = root.value("data").toObject().value("city").toString();
    QString AQI = "空气污染指数" + root.value("data").toObject().value("aqi").toString();//空气污染指数
    QString Temp = root.value("data").toObject().value("wendu").toString()+"℃";//当前温度
    QString Tips = root.value("data").toObject().value("ganmao").toString();//提醒

    int api = root.value("data").toObject().value("aqi").toString().toInt(&ok,10);

    QJsonValue Forecast = root.value("data").toObject().value("forecast").toArray().at(0);
    QString Weather = Forecast.toObject().value("type").toString();

    QString weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips;

    //qDebug()<<root.value("data").toObject().value("aqi").toString();
    //qDebug()<<Weather;

    if(Weather == "阴" )
    {
        weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"天好阴，别在外面晾衣服喽！";
    }
    if(Weather == "多云" || Weather == "晴" )
    {
        weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"emmmm，天气不错鸭";
    }

    if(Weather == "小雨" || Weather =="中雨" || Weather =="大雨" || Weather =="暴雨")
    {
        weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"今天要下雨啦，您带伞了没？摸摸头！";
    }
    if(Weather == "小雪" || Weather =="中雪" || Weather =="大雪" || Weather =="暴雪")
    {

        weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"要下雪啦，您快去看看吧！抱抱！";
    }
    if(api > 100)
    {
        qDebug()<<api;
        if(Weather == "小雨" || Weather =="中雨" || Weather =="大雨" || Weather =="暴雨")
        {
            weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"要下雨啦，我猜现在天雾蒙蒙滴，小心鸭";
        }
        else if(Weather == "小雪" || Weather =="中雪" || Weather =="大雪" || Weather =="暴雪")
        {
            weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"要下雪啦，您快去看看吧！注意安全鸭";
        }
        else
        {
            weatherData = CityName+'\n'+Temp+"  "+Weather+'\n'+AQI+'\n'+Tips+'\n'+"羊先生提醒您，今天空气质量不太好哦！";
        }
    }


    if(isNetWork == true)
    {
        ui->weather->setText(weatherData);
    }
    else
    {
         ui->weather->setText("小雨点儿，我猜您手机没联网");
    }


}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    WeatherDataAnalyze(response);//解析数据
    reply->deleteLater();
}

void MainWindow::on_light_clicked()
{
    //this->close();
    newNote();
}

void MainWindow::RefreshTime()
{
    QDateTime time1 = QDateTime::fromString("2017-04-22 23:34:00","yyyy-MM-dd hh:mm:ss");
    //QDateTime current_date_time

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QString current_week = current_date_time.toString("dddd");
    long long num = (time1.secsTo(current_date_time)/86400);
    QString chinese = "哎呦，我们的故事都已经开始";
    if(num % 100 == 0)
    {
        ui->date->setText(chinese.toUtf8() +QString::number(num) +"天了鸭"+'\n'+"那真是巧了！您的羊先生依然在您身边！"+'\n'+"哇！我们已经度过"+QString::number(num)+"天整了哦，嘻嘻嘻！");
    }
    else if(num % 100 == 99)
    {
        ui->date->setText(chinese.toUtf8() +QString::number(num) +"天了鸭"+'\n'+"那真是巧了！您的羊先生依然在您身边！"+'\n'+"距离我们滴"+(QString::number((num/100)+1))+"00天还有一天啦！我好高兴鸭！");
    }
    else if(num % 100 == 93)
    {
        ui->date->setText(chinese.toUtf8() +QString::number(num) +"天了鸭"+'\n'+"那真是巧了！您的羊先生依然在您身边！"+'\n'+"距离我们滴"+(QString::number((num/100)+1))+"00天还有一周啦！您期待吗？");
    }
    else
    {
        ui->date->setText(chinese.toUtf8() +QString::number(num) +"天了鸭"+'\n'+"那真是巧了！您的羊先生依然在您身边！");
    }

    //qDebug()<<"current_date_time - time1" <<time1.secsTo(current_date_time)/86400 ;
}

bool MainWindow::isNetWorkOnline()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

void MainWindow::newNote()
{
    bool ok;

    QString text = QInputDialog::getText(this, "城市", "快告诉羊先生您在的城市:", QLineEdit::Normal,
                                            "", &ok);
   if (ok && !text.isEmpty())
   {
       text1 = text;
   }
}

void MainWindow::on_camera_clicked()
{
    //timer1->stop();
    //this->close();
    QAndroidJniObject action = QAndroidJniObject::fromString("android.media.action.STILL_IMAGE_CAMERA");
    QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());
    QtAndroid::startActivity(intent, 0);
    QAndroidJniEnvironment env;
    CHECK_EXCEPTION()
}

void MainWindow::on_exit_clicked()
{
    timer1->stop();
    this->close();
}

void MainWindow::on_clock_clicked()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "VIBRATOR_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject vibrateService = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();
    jlong duration = 200;
    vibrateService.callMethod<void>("vibrate", "(J)V", duration);
    CHECK_EXCEPTION();
}

void MainWindow::on_write_clicked()
{
    /*QAndroidJniObject action = QAndroidJniObject::fromString("android.settings.SETTINGS");
        QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());
        startActivity(intent, 0);
        QAndroidJniEnvironment env;
        CHECK_EXCEPTION()*/

                QAndroidJniObject action = QAndroidJniObject::fromString("android.settings.SETTINGS");
                QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());
                QtAndroid::startActivity(intent, 0);
                QAndroidJniEnvironment env;
                CHECK_EXCEPTION()
}

/* QString packageName = "an.taobao";
 QString className = "an.taobao.MainActivity";
 QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
 QAndroidJniObject param2 = QAndroidJniObject::fromString(className);

 QAndroidJniObject intent("android/content/Intent","()V");
 intent.callObjectMethod("setClassName","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", param1.object<jstring>(), param2.object<jstring>());

 QtAndroid::startActivity(intent, 0);*/
