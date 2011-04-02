/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <QTest>
#include <QSignalSpy>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include "testpagerecognizer.h"
#include "gui/page.h"
#include "gui/language_i18n.h"

#define private public
#include "gui/pagerecognizer.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

TestPageRecognizer::TestPageRecognizer(QObject *parent) :
    QObject(parent)
{
}

void TestPageRecognizer::testConstruct() {
    PageRecognizer r;
}

void TestPageRecognizer::testRecognize() {
    PageRecognizer r;
    QSignalSpy failed(&r, SIGNAL(failed(QString)));
    QSignalSpy opened(&r, SIGNAL(opened()));
    QSignalSpy formatted(&r, SIGNAL(formatted()));
    QSignalSpy recognized(&r, SIGNAL(recognized()));

    // NULL page
    r.setPage(NULL);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);

    failed.clear();

    // non-exist page
    Page * none = new Page("none");
    r.setPage(none);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);
    QVERIFY(none->hasFlag(Page::RECOGNITION_FAILED));

    failed.clear();

    // invalid page image format
    Page * invalid = new Page(CF_IMAGE_DIR "/invalid.png");
    r.setPage(invalid);
    r.recognize();
    QCOMPARE(failed.count(), 1);
    QCOMPARE(opened.count(), 0);
    QCOMPARE(formatted.count(), 0);
    QCOMPARE(recognized.count(), 0);
    QVERIFY(invalid->hasFlag(Page::RECOGNITION_FAILED));

    failed.clear();
    return;

    // valid english
    Page * eng = new Page(CF_IMAGE_DIR "/english.png");
    r.setPage(eng);
    r.recognize();
    QCOMPARE(eng->ocrText().trimmed(), QString("ENGLISH"));
    QCOMPARE(failed.count(), 0);
    QCOMPARE(opened.count(), 1);
    QCOMPARE(formatted.count(), 1);
    QCOMPARE(recognized.count(), 1);
    QVERIFY(!eng->hasFlag(Page::RECOGNITION_FAILED));

    return;

    // valid russian
    Page * rus = new Page(CF_IMAGE_DIR "/russian.png");
    r.setPage(rus);
    r.recognize();
    QCOMPARE(rus->ocrText().trimmed(), QString("PYCCK VI Vl"));
    QVERIFY(rus->isRecognized());
    QVERIFY(!rus->hasFlag(Page::RECOGNITION_FAILED));

    r.setLanguage(::LANGUAGE_RUSSIAN);
    r.recognize();
    QCOMPARE(rus->ocrText().trimmed(), QString::fromUtf8("РУССКИЙ"));

    delete none;
    delete eng;
    delete rus;
}

void TestPageRecognizer::testLoadImage() {
    PageRecognizer r;
    Page * eng = new Page(CF_IMAGE_DIR "/english.png");
    r.setPage(eng);

    QImage img = r.loadImage();
    QCOMPARE(img.size(), eng->imageSize());

    eng->setPageArea(QRect(1, 2, 30, 40));
    QCOMPARE(r.loadImage().size(), QSize(30, 40));

    eng->rotate(90);
    QCOMPARE(r.loadImage().size(), QSize(40, 30));

    eng->scale(12);
    QCOMPARE(r.loadImage().size(), QSize(40, 30));

    delete eng;
}

void TestPageRecognizer::testAbort() {
    PageRecognizer r;
    QSignalSpy aborted(&r, SIGNAL(aborted()));

    Page p(CF_IMAGE_DIR "/english.png");
    r.setPage(&p);
    QTimer::singleShot(0, &r, SLOT(abort()));
    QCoreApplication::processEvents();

    QCOMPARE(aborted.count(), 1);
    QVERIFY(!p.hasFlag(Page::RECOGNIZED));

    // abort only once
    bool ok = r.recognize();
    QVERIFY(!ok);

    // try again
    ok = r.recognize();
    QVERIFY(ok);
    QVERIFY(p.hasFlag(Page::RECOGNIZED));

    // again block
    QTimer::singleShot(10, &r, SLOT(abort()));
    r.setStageSleep(PageRecognizer::OPEN, 100);
    ok = r.recognize();
    QCOMPARE(aborted.count(), 2);
    QVERIFY(!ok);
}

void TestPageRecognizer::testPercents() {
    PageRecognizer r;
    QSignalSpy percents(&r, SIGNAL(percentsDone(int)));
    Page eng(CF_IMAGE_DIR "/english.png");
    r.setPage(&eng);
    r.recognize();

    QCOMPARE(percents.count(), 7);
    QCOMPARE(percents.at(0).at(0).toInt(), 10);
    QCOMPARE(percents.at(1).at(0).toInt(), 20);
    QCOMPARE(percents.at(2).at(0).toInt(), 30);
    QCOMPARE(percents.at(3).at(0).toInt(), 80);
    QCOMPARE(percents.at(4).at(0).toInt(), 90);
    QCOMPARE(percents.at(5).at(0).toInt(), 95);
    QCOMPARE(percents.at(6).at(0).toInt(), 100);
}

void TestPageRecognizer::testSlotConnections() {
    PageRecognizer r;
    Page eng(CF_IMAGE_DIR "/english.png");
    r.setPage(&eng);
    connect(this, SIGNAL(test()), &r, SLOT(recognize()));
    QSignalSpy done(&r, SIGNAL(done()));
    emit test();
    QCOMPARE(done.count(), 1);

    disconnect(this, SIGNAL(test()), &r, SLOT(recognize()));
    connect(this, SIGNAL(test()), &r, SLOT(abort()));
    QSignalSpy aborted(&r, SIGNAL(aborted()));
    emit test();
    QCOMPARE(done.count(), 1);
    QCOMPARE(aborted.count(), 1);
}

QTEST_MAIN(TestPageRecognizer);