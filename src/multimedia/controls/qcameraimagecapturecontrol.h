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

#ifndef QCAMERAIMAGECAPTURECONTROL_H
#define QCAMERAIMAGECAPTURECONTROL_H

#include <qmediacontrol.h>
#include <qcameraimagecapture.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Multimedia)

class QImage;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class Q_MULTIMEDIA_EXPORT QCameraImageCaptureControl : public QMediaControl
{
    Q_OBJECT

public:
    ~QCameraImageCaptureControl();

    virtual bool isReadyForCapture() const = 0;

    virtual QCameraImageCapture::DriveMode driveMode() const = 0;
    virtual void setDriveMode(QCameraImageCapture::DriveMode mode) = 0;

    virtual int capture(const QString &fileName) = 0;
    virtual void cancelCapture() = 0;

Q_SIGNALS:
    void readyForCaptureChanged(bool);

    void imageExposed(int id);
    void imageCaptured(int id, const QImage &preview);
    void imageMetadataAvailable(int id, QtMultimedia::MetaData key, const QVariant &value);
    void imageMetadataAvailable(int id, const QString &key, const QVariant &value);
    void imageAvailable(int id, const QVideoFrame &buffer);
    void imageSaved(int id, const QString &fileName);

    void error(int id, int error, const QString &errorString);

protected:
    QCameraImageCaptureControl(QObject* parent = 0);
};

#define QCameraImageCaptureControl_iid "com.nokia.Qt.QCameraImageCaptureControl/1.0"
Q_MEDIA_DECLARE_CONTROL(QCameraImageCaptureControl, QCameraImageCaptureControl_iid)

QT_END_NAMESPACE

QT_END_HEADER


#endif  // QCAMERAIMAGECAPTURECONTROL_H

