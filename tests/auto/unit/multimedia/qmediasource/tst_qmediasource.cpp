/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=src/multimedia

#include <QtTest/QtTest>

#include <QtCore/qtimer.h>

#include <QtMultimedia/qmediametadata.h>
#include <qmediasource.h>
#include <qmediaservice.h>
#include <qmetadatareadercontrol.h>

#include "mockmediarecorderservice.h"
#include "mockmetadatareadercontrol.h"

class QtTestMediaObjectService : public QMediaService
{
    Q_OBJECT
public:
    QtTestMediaObjectService(QObject *parent = nullptr)
        : QMediaService(parent)
    {
    }

    QObject *requestControl(const char *iid) override
    {
        if (hasMetaData && qstrcmp(iid, QMetaDataReaderControl_iid) == 0)
            return &metaData;

        return nullptr;
    }

    void releaseControl(QObject *) override
    {
    }

    MockMetaDataReaderControl metaData;
    int metaDataRef = 0;
    bool hasMetaData = true;
};

QT_USE_NAMESPACE

class tst_QMediaSource : public QObject
{
    Q_OBJECT

private slots:
    void propertyWatch();
    void notifySignals_data();
    void notifySignals();
    void notifyInterval_data();
    void notifyInterval();

    void nullMetaDataControl();
    void isMetaDataAvailable();
    void metaDataChanged();
    void metaData_data();
    void metaData();
    void availability();

    void service();

private:
    void setupNotifyTests();
};

class QtTestMediaObject : public QMediaSource
{
    Q_OBJECT
    Q_PROPERTY(int a READ a WRITE setA NOTIFY aChanged)
    Q_PROPERTY(int b READ b WRITE setB NOTIFY bChanged)
    Q_PROPERTY(int c READ c WRITE setC NOTIFY cChanged)
    Q_PROPERTY(int d READ d WRITE setD)
public:
    QtTestMediaObject(QMediaService *service = nullptr): QMediaSource(nullptr, service) {}

    using QMediaSource::addPropertyWatch;
    using QMediaSource::removePropertyWatch;

    [[nodiscard]] int a() const { return m_a; }
    void setA(int a) { m_a = a; }

    [[nodiscard]] int b() const { return m_b; }
    void setB(int b) { m_b = b; }

    [[nodiscard]] int c() const { return m_c; }
    void setC(int c) { m_c = c; }

    [[nodiscard]] int d() const { return m_d; }
    void setD(int d) { m_d = d; }

Q_SIGNALS:
    void aChanged(int a);
    void bChanged(int b);
    void cChanged(int c);

private:
    int m_a = 0;
    int m_b = 0;
    int m_c = 0;
    int m_d = 0;
};

void tst_QMediaSource::propertyWatch()
{
    QtTestMediaObject object;
    object.setNotifyInterval(0);

    QEventLoop loop;
    connect(&object, SIGNAL(aChanged(int)), &QTestEventLoop::instance(), SLOT(exitLoop()));
    connect(&object, SIGNAL(bChanged(int)), &QTestEventLoop::instance(), SLOT(exitLoop()));
    connect(&object, SIGNAL(cChanged(int)), &QTestEventLoop::instance(), SLOT(exitLoop()));

    QSignalSpy aSpy(&object, SIGNAL(aChanged(int)));
    QSignalSpy bSpy(&object, SIGNAL(bChanged(int)));
    QSignalSpy cSpy(&object, SIGNAL(cChanged(int)));

    QTestEventLoop::instance().enterLoop(1);

    QCOMPARE(aSpy.count(), 0);
    QCOMPARE(bSpy.count(), 0);
    QCOMPARE(cSpy.count(), 0);

    int aCount = 0;
    int bCount = 0;
    int cCount = 0;

    object.addPropertyWatch("a");

    QTestEventLoop::instance().enterLoop(1);

    QVERIFY(aSpy.count() > aCount);
    QCOMPARE(bSpy.count(), 0);
    QCOMPARE(cSpy.count(), 0);
    QCOMPARE(aSpy.last().value(0).toInt(), 0);

    aCount = aSpy.count();

    object.setA(54);
    object.setB(342);
    object.setC(233);

    QTestEventLoop::instance().enterLoop(1);

    QVERIFY(aSpy.count() > aCount);
    QCOMPARE(bSpy.count(), 0);
    QCOMPARE(cSpy.count(), 0);
    QCOMPARE(aSpy.last().value(0).toInt(), 54);

    aCount = aSpy.count();

    object.addPropertyWatch("b");
    object.addPropertyWatch("d");
    object.removePropertyWatch("e");
    object.setA(43);
    object.setB(235);
    object.setC(90);

    QTestEventLoop::instance().enterLoop(1);

    QVERIFY(aSpy.count() > aCount);
    QVERIFY(bSpy.count() > bCount);
    QCOMPARE(cSpy.count(), 0);
    QCOMPARE(aSpy.last().value(0).toInt(), 43);
    QCOMPARE(bSpy.last().value(0).toInt(), 235);

    aCount = aSpy.count();
    bCount = bSpy.count();

    object.removePropertyWatch("a");
    object.addPropertyWatch("c");
    object.addPropertyWatch("e");

    QTestEventLoop::instance().enterLoop(1);

    QCOMPARE(aSpy.count(), aCount);
    QVERIFY(bSpy.count() > bCount);
    QVERIFY(cSpy.count() > cCount);
    QCOMPARE(bSpy.last().value(0).toInt(), 235);
    QCOMPARE(cSpy.last().value(0).toInt(), 90);

    bCount = bSpy.count();
    cCount = cSpy.count();

    object.setA(435);
    object.setC(9845);

    QTestEventLoop::instance().enterLoop(1);

    QCOMPARE(aSpy.count(), aCount);
    QVERIFY(bSpy.count() > bCount);
    QVERIFY(cSpy.count() > cCount);
    QCOMPARE(bSpy.last().value(0).toInt(), 235);
    QCOMPARE(cSpy.last().value(0).toInt(), 9845);

    bCount = bSpy.count();
    cCount = cSpy.count();

    object.setA(8432);
    object.setB(324);
    object.setC(443);
    object.removePropertyWatch("c");
    object.removePropertyWatch("d");

    QTestEventLoop::instance().enterLoop(1);

    QCOMPARE(aSpy.count(), aCount);
    QVERIFY(bSpy.count() > bCount);
    QCOMPARE(cSpy.count(), cCount);
    QCOMPARE(bSpy.last().value(0).toInt(), 324);
    QCOMPARE(cSpy.last().value(0).toInt(), 9845);

    bCount = bSpy.count();

    object.removePropertyWatch("b");

    QTestEventLoop::instance().enterLoop(1);

    QCOMPARE(aSpy.count(), aCount);
    QCOMPARE(bSpy.count(), bCount);
    QCOMPARE(cSpy.count(), cCount);
}

void tst_QMediaSource::setupNotifyTests()
{
    QTest::addColumn<int>("interval");
    QTest::addColumn<int>("count");

    QTest::newRow("single 750ms")
            << 750
            << 1;
    QTest::newRow("single 600ms")
            << 600
            << 1;
    QTest::newRow("x3 300ms")
            << 300
            << 3;
    QTest::newRow("x5 180ms")
            << 180
            << 5;
}

void tst_QMediaSource::notifySignals_data()
{
    setupNotifyTests();
}

void tst_QMediaSource::notifySignals()
{
    QFETCH(int, interval);
    QFETCH(int, count);

    QtTestMediaObject object;
    QSignalSpy spy(&object, SIGNAL(aChanged(int)));

    object.setNotifyInterval(interval);
    object.addPropertyWatch("a");

    QElapsedTimer timer;
    timer.start();

    QTRY_COMPARE(spy.count(), count);
}

void tst_QMediaSource::notifyInterval_data()
{
    setupNotifyTests();
}

void tst_QMediaSource::notifyInterval()
{
    QFETCH(int, interval);

    QtTestMediaObject object;
    QSignalSpy spy(&object, SIGNAL(notifyIntervalChanged(int)));

    object.setNotifyInterval(interval);
    QCOMPARE(object.notifyInterval(), interval);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().value(0).toInt(), interval);

    object.setNotifyInterval(interval);
    QCOMPARE(object.notifyInterval(), interval);
    QCOMPARE(spy.count(), 1);
}

void tst_QMediaSource::nullMetaDataControl()
{
    const QString titleKey(QLatin1String("Title"));
    const QString title(QLatin1String("Host of Seraphim"));

    QtTestMediaObjectService service;
    service.hasMetaData = false;

    QtTestMediaObject object(&service);

    QSignalSpy spy(&object, SIGNAL(metaDataChanged()));

    QCOMPARE(object.isMetaDataAvailable(), false);

    QCOMPARE(object.metaData(QMediaMetaData::Title).toString(), QString());
    QCOMPARE(object.availableMetaData(), QStringList());
    QCOMPARE(spy.count(), 0);
}

void tst_QMediaSource::isMetaDataAvailable()
{
    QtTestMediaObjectService service;
    service.metaData.setMetaDataAvailable(false);

    QtTestMediaObject object(&service);
    QCOMPARE(object.isMetaDataAvailable(), false);

    QSignalSpy spy(&object, SIGNAL(metaDataAvailableChanged(bool)));
    service.metaData.setMetaDataAvailable(true);

    QCOMPARE(object.isMetaDataAvailable(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);

    service.metaData.setMetaDataAvailable(false);

    QCOMPARE(object.isMetaDataAvailable(), false);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), false);
}

void tst_QMediaSource::metaDataChanged()
{
    QtTestMediaObjectService service;
    QtTestMediaObject object(&service);

    QSignalSpy changedSpy(&object, SIGNAL(metaDataChanged()));
    QSignalSpy changedWithValueSpy(&object, SIGNAL(metaDataChanged(QString,QVariant)));

    service.metaData.setMetaData("key", "Value");
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedWithValueSpy.count(), 1);
    QCOMPARE(changedWithValueSpy.last()[0], QVariant("key"));
    QCOMPARE(changedWithValueSpy.last()[1].value<QVariant>(), QVariant("Value"));

    service.metaData.setMetaData("key", "Value");
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedWithValueSpy.count(), 1);

    service.metaData.setMetaData("key2", "Value");
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(changedWithValueSpy.count(), 2);
    QCOMPARE(changedWithValueSpy.last()[0], QVariant("key2"));
    QCOMPARE(changedWithValueSpy.last()[1].value<QVariant>(), QVariant("Value"));
}

void tst_QMediaSource::metaData_data()
{
    QTest::addColumn<QString>("artist");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("genre");

    QTest::newRow("")
            << QString::fromLatin1("Dead Can Dance")
            << QString::fromLatin1("Host of Seraphim")
            << QString::fromLatin1("Awesome");
}

void tst_QMediaSource::metaData()
{
    QFETCH(QString, artist);
    QFETCH(QString, title);
    QFETCH(QString, genre);

    QtTestMediaObjectService service;
    service.metaData.populateMetaData();

    QtTestMediaObject object(&service);
    QVERIFY(object.availableMetaData().isEmpty());

    service.metaData.m_data.insert(QMediaMetaData::AlbumArtist, artist);
    service.metaData.m_data.insert(QMediaMetaData::Title, title);
    service.metaData.m_data.insert(QMediaMetaData::Genre, genre);

    QCOMPARE(object.metaData(QMediaMetaData::AlbumArtist).toString(), artist);
    QCOMPARE(object.metaData(QMediaMetaData::Title).toString(), title);

    QStringList metaDataKeys = object.availableMetaData();
    QCOMPARE(metaDataKeys.size(), 3);
    QVERIFY(metaDataKeys.contains(QMediaMetaData::AlbumArtist));
    QVERIFY(metaDataKeys.contains(QMediaMetaData::Title));
    QVERIFY(metaDataKeys.contains(QMediaMetaData::Genre));
}

void tst_QMediaSource::availability()
{
    {
        QtTestMediaObject nullObject(nullptr);
        QCOMPARE(nullObject.isAvailable(), false);
        QCOMPARE(nullObject.availability(), QMultimedia::ServiceMissing);
    }

    {
        QtTestMediaObjectService service;
        QtTestMediaObject object(&service);
        QCOMPARE(object.isAvailable(), true);
        QCOMPARE(object.availability(), QMultimedia::Available);
    }
}

 void tst_QMediaSource::service()
 {
     // Create the mediaobject with service.
     QtTestMediaObjectService service;
     QtTestMediaObject mediaObject1(&service);

     // Get service and Compare if it equal to the service passed as an argument in mediaObject1.
     QMediaService *service1 = mediaObject1.service();
     QVERIFY(service1 != nullptr);
     QCOMPARE(service1,&service);

     // Create the mediaobject with empty service and verify that service() returns NULL.
     QtTestMediaObject mediaObject2;
     QMediaService *service2 = mediaObject2.service();
     QVERIFY(service2 == nullptr);
}

QTEST_GUILESS_MAIN(tst_QMediaSource)
#include "tst_qmediasource.moc"