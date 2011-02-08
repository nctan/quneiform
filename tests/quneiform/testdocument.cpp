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
#include "testdocument.h"
#include "gui/document.h"
#include "gui/page.h"

TestDocument::TestDocument(QObject *parent) :
    QObject(parent)
{
}

void TestDocument::testConstruct() {
    Document doc;
    QCOMPARE(doc.fileName(), QString("untitled.qpf"));
    QCOMPARE(doc.language(), -1);
    QVERIFY(!doc.isChanged());
    QVERIFY(doc.isNew());
}

void TestDocument::testAppend() {
    Document doc;
    QSignalSpy changed(&doc, SIGNAL(changed()));

    doc.append(new Page(""));

    QCOMPARE(changed.count(), 1);
}

QTEST_MAIN(TestDocument)
