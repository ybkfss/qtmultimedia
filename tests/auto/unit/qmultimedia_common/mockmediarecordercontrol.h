/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MOCKRECORDERCONTROL_H
#define MOCKRECORDERCONTROL_H

#include <QUrl>

#include "qmediarecordercontrol.h"

class MockMediaRecorderControl : public QMediaRecorderControl
{
    Q_OBJECT

public:
    MockMediaRecorderControl(QObject *parent = 0):
        QMediaRecorderControl(parent),
        m_state(QMediaRecorder::StoppedState),
        m_status(QMediaRecorder::LoadedStatus),
        m_position(0),
        m_muted(false),
        m_settingAppliedCount(0)
    {
    }

    QUrl outputLocation() const
    {
        return m_sink;
    }

    bool setOutputLocation(const QUrl &sink)
    {
        m_sink = sink;
        return true;
    }

    QMediaRecorder::State state() const
    {
        return m_state;
    }

    QMediaRecorder::Status status() const
    {
        return m_status;
    }

    qint64 duration() const
    {
        return m_position;
    }

    bool isMuted() const
    {
        return m_muted;
    }

    void applySettings()
    {
        m_settingAppliedCount++;
    }

    using QMediaRecorderControl::error;

public slots:
    void record()
    {
        m_state = QMediaRecorder::RecordingState;
        m_status = QMediaRecorder::RecordingStatus;
        m_position=1;
        emit stateChanged(m_state);
        emit statusChanged(m_status);
        emit durationChanged(m_position);

        QUrl actualLocation = m_sink.isEmpty() ? QUrl::fromLocalFile("default_name.mp4") : m_sink;
        emit actualLocationChanged(actualLocation);
    }

    void pause()
    {
        m_state = QMediaRecorder::PausedState;
        m_status = QMediaRecorder::PausedStatus;
        emit stateChanged(m_state);
        emit statusChanged(m_status);
    }

    void stop()
    {
        m_position=0;
        m_state = QMediaRecorder::StoppedState;
        m_status = QMediaRecorder::LoadedStatus;
        emit stateChanged(m_state);
        emit statusChanged(m_status);
    }

    void setState(QMediaRecorder::State state)
    {
        switch (state) {
        case QMediaRecorder::StoppedState:
            stop();
            break;
        case QMediaRecorder::PausedState:
            pause();
            break;
        case QMediaRecorder::RecordingState:
            record();
            break;
        }
    }


    void setMuted(bool muted)
    {
        if (m_muted != muted)
            emit mutedChanged(m_muted = muted);
    }

public:
    QUrl       m_sink;
    QMediaRecorder::State m_state;
    QMediaRecorder::Status m_status;
    qint64     m_position;
    bool m_muted;
    int m_settingAppliedCount;
};

#endif // MOCKRECORDERCONTROL_H
