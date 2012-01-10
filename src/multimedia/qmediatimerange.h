/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMEDIATIMERANGE_H
#define QMEDIATIMERANGE_H

#include <qtmultimediadefs.h>
#include "qtmedianamespace.h"
#include <QtCore/qshareddata.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Multimedia)


class QMediaTimeRangePrivate;

class Q_MULTIMEDIA_EXPORT QMediaTimeInterval
{
public:
    QMediaTimeInterval();
    QMediaTimeInterval(qint64 start, qint64 end);
    QMediaTimeInterval(const QMediaTimeInterval&);

    qint64 start() const;
    qint64 end() const;

    bool contains(qint64 time) const;

    bool isNormal() const;
    QMediaTimeInterval normalized() const;
    QMediaTimeInterval translated(qint64 offset) const;

private:
    friend class QMediaTimeRangePrivate;
    friend class QMediaTimeRange;

    qint64 s;
    qint64 e;
};

Q_MULTIMEDIA_EXPORT bool operator==(const QMediaTimeInterval&, const QMediaTimeInterval&);
Q_MULTIMEDIA_EXPORT bool operator!=(const QMediaTimeInterval&, const QMediaTimeInterval&);

class Q_MULTIMEDIA_EXPORT QMediaTimeRange
{
public:

    QMediaTimeRange();
    QMediaTimeRange(qint64 start, qint64 end);
    QMediaTimeRange(const QMediaTimeInterval&);
    QMediaTimeRange(const QMediaTimeRange &range);
    ~QMediaTimeRange();

    QMediaTimeRange &operator=(const QMediaTimeRange&);
    QMediaTimeRange &operator=(const QMediaTimeInterval&);

    qint64 earliestTime() const;
    qint64 latestTime() const;

    QList<QMediaTimeInterval> intervals() const;
    bool isEmpty() const;
    bool isContinuous() const;

    bool contains(qint64 time) const;

    void addInterval(qint64 start, qint64 end);
    void addInterval(const QMediaTimeInterval &interval);
    void addTimeRange(const QMediaTimeRange&);

    void removeInterval(qint64 start, qint64 end);
    void removeInterval(const QMediaTimeInterval &interval);
    void removeTimeRange(const QMediaTimeRange&);

    QMediaTimeRange& operator+=(const QMediaTimeRange&);
    QMediaTimeRange& operator+=(const QMediaTimeInterval&);
    QMediaTimeRange& operator-=(const QMediaTimeRange&);
    QMediaTimeRange& operator-=(const QMediaTimeInterval&);

    void clear();

private:
    QSharedDataPointer<QMediaTimeRangePrivate> d;
};

Q_MULTIMEDIA_EXPORT bool operator==(const QMediaTimeRange&, const QMediaTimeRange&);
Q_MULTIMEDIA_EXPORT bool operator!=(const QMediaTimeRange&, const QMediaTimeRange&);
Q_MULTIMEDIA_EXPORT QMediaTimeRange operator+(const QMediaTimeRange&, const QMediaTimeRange&);
Q_MULTIMEDIA_EXPORT QMediaTimeRange operator-(const QMediaTimeRange&, const QMediaTimeRange&);

#ifndef QT_NO_DEBUG_STREAM
Q_MULTIMEDIA_EXPORT QDebug operator<<(QDebug, const QMediaTimeRange &);
#endif

QT_END_NAMESPACE

QT_END_HEADER


#endif  // QMEDIATIMERANGE_H
