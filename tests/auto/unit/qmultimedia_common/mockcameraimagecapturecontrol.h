/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MOCKCAMERACAPTURECONTROL_H
#define MOCKCAMERACAPTURECONTROL_H

#include <QDateTime>
#include <QTimer>

#include "qcameraimagecapturecontrol.h"
#include "qcameracontrol.h"
#include "mockcameracontrol.h"

class MockCaptureControl : public QCameraImageCaptureControl
{
    Q_OBJECT
public:
    MockCaptureControl(MockCameraControl *cameraControl, QObject *parent = 0)
        : QCameraImageCaptureControl(parent), m_cameraControl(cameraControl), m_captureRequest(0), m_ready(true), m_captureCanceled(false)
    {
    }

    ~MockCaptureControl()
    {
    }

    QCameraImageCapture::DriveMode driveMode() const { return QCameraImageCapture::SingleImageCapture; }
    void setDriveMode(QCameraImageCapture::DriveMode) {}

    bool isReadyForCapture() const { return m_ready && m_cameraControl->state() == QCamera::ActiveState; }

    int capture(const QString &fileName)
    {
        if (isReadyForCapture()) {
            m_fileName = fileName;
            m_captureRequest++;
            emit readyForCaptureChanged(m_ready = false);
            QTimer::singleShot(5, this, SLOT(captured()));
            return m_captureRequest;
        } else {
            emit error(-1, QCameraImageCapture::NotReadyError,
                       QLatin1String("Could not capture in stopped state"));
        }

        return -1;
    }

    void cancelCapture()
    {
        m_captureCanceled = true;
    }

private Q_SLOTS:
    void captured()
    {
        if (!m_captureCanceled) {
            emit imageCaptured(m_captureRequest, QImage());

            emit imageMetadataAvailable(m_captureRequest,
                                        QtMultimedia::FocalLengthIn35mmFilm,
                                        QVariant(50));

            emit imageMetadataAvailable(m_captureRequest,
                                        QtMultimedia::DateTimeOriginal,
                                        QVariant(QDateTime::currentDateTime()));

            emit imageMetadataAvailable(m_captureRequest,
                                        QLatin1String("Answer to the Ultimate Question of Life, the Universe, and Everything"),
                                        QVariant(42));
        }

        if (!m_ready)
        {
            emit readyForCaptureChanged(m_ready = true);
            emit imageExposed(m_captureRequest);
        }

        if (!m_captureCanceled)
            emit imageSaved(m_captureRequest, m_fileName);

        m_captureCanceled = false;
    }

private:
    MockCameraControl *m_cameraControl;
    QString m_fileName;
    int m_captureRequest;
    bool m_ready;
    bool m_captureCanceled;
};

#endif // MOCKCAMERACAPTURECONTROL_H
