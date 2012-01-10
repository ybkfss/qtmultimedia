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

#ifndef QVIDEOWIDGET_H
#define QVIDEOWIDGET_H

#include <QtWidgets/qwidget.h>

#include <qtmultimediawidgetdefs.h>
#include <qmediabindableinterface.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Multimedia)


class QMediaObject;

class QVideoWidgetPrivate;
class Q_MULTIMEDIAWIDGETS_EXPORT QVideoWidget : public QWidget, public QMediaBindableInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaBindableInterface)
    Q_PROPERTY(QMediaObject* mediaObject READ mediaObject WRITE setMediaObject)
    Q_PROPERTY(bool fullScreen READ isFullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)

public:
    QVideoWidget(QWidget *parent = 0);
    ~QVideoWidget();   

    QMediaObject *mediaObject() const;    

#ifdef Q_QDOC
    bool isFullScreen() const;
#endif

    Qt::AspectRatioMode aspectRatioMode() const;

    int brightness() const;
    int contrast() const;
    int hue() const;
    int saturation() const;

    QSize sizeHint() const;

public Q_SLOTS:
    void setFullScreen(bool fullScreen);
    void setAspectRatioMode(Qt::AspectRatioMode mode);
    void setBrightness(int brightness);
    void setContrast(int contrast);
    void setHue(int hue);
    void setSaturation(int saturation);

Q_SIGNALS:
    void fullScreenChanged(bool fullScreen);
    void brightnessChanged(int brightness);
    void contrastChanged(int contrast);
    void hueChanged(int hue);
    void saturationChanged(int saturation);

protected:
    bool event(QEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void paintEvent(QPaintEvent *event);

    bool setMediaObject(QMediaObject *object);

#if defined(Q_WS_WIN)
    bool winEvent(MSG *message, long *result);
#endif

    QVideoWidget(QVideoWidgetPrivate &dd, QWidget *parent);
    QVideoWidgetPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QVideoWidget)
    Q_PRIVATE_SLOT(d_func(), void _q_serviceDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_brightnessChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_contrastChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_hueChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_saturationChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_fullScreenChanged(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_dimensionsChanged())
};

QT_END_NAMESPACE

QT_END_HEADER


#endif
