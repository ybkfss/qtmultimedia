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

#ifndef QSIMULATORVIDEORENDERERCONTROL_H
#define QSIMULATORVIDEORENDERERCONTROL_H

#include <qvideorenderercontrol.h>
#include <QtGui/QImage>


class SimulatorCameraSession;
/*
 * Control for QGraphicsVideoItem. Viewfinder frames are streamed to a surface
 * which is drawn to the display by the Qt Graphics Vide Framework.
 */
class SimulatorVideoRendererControl : public QVideoRendererControl
{
    Q_OBJECT

public:
    SimulatorVideoRendererControl(SimulatorCameraSession *session, QObject *parent = 0);
    virtual ~SimulatorVideoRendererControl();

    QAbstractVideoSurface *surface() const;
    void setSurface(QAbstractVideoSurface *surface);

    void setImagePath(const QString &imagePath);
    const QImage *image() const;

public slots:
    void showImage();

private slots:
    void stop();

private:
    SimulatorCameraSession *mSession;
    QAbstractVideoSurface *mSurface;
    QImage mImage;
    QImage mShownImage;
    bool mRunning;
};

#endif // QSIMULATORVIDEORENDERERCONTROL_H
