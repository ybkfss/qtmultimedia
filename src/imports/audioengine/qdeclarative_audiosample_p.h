/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the plugins of the Qt Toolkit.
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

#ifndef QDECLARATIVEAUDIOSAMPLE_P_H
#define QDECLARATIVEAUDIOSAMPLE_P_H

#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/qdeclarativecomponent.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QSoundBuffer;

class QDeclarativeAudioSample : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_PROPERTY(bool preloaded READ isPreloaded WRITE setPreloaded)
    Q_PROPERTY(bool streaming READ isStreaming WRITE setStreaming)
    Q_PROPERTY(bool loaded READ isLoaded NOTIFY loadedChanged)

public:
    QDeclarativeAudioSample(QObject *parent = 0);
    ~QDeclarativeAudioSample();

    void classBegin();
    void componentComplete();

    QString name() const;
    void setName(const QString& name);

    QUrl source() const;
    void setSource(const QUrl& url);

    bool isStreaming() const;
    void setStreaming(bool streaming);

    bool isPreloaded() const;
    void setPreloaded(bool preloaded);

    bool isLoaded() const;

    QSoundBuffer* soundBuffer() const;

    //called by QDeclarativeAudioEngine
    void init();

Q_SIGNALS:
    void loadedChanged();

public Q_SLOTS:
    void load();

private:
    Q_DISABLE_COPY(QDeclarativeAudioSample);
    bool m_complete;
    QString m_name;
    QUrl m_url;
    bool m_streaming;
    bool m_preloaded;

    QSoundBuffer *m_soundBuffer;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
