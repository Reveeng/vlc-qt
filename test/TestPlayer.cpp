/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

#include "core/Common.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaPlayer.h"

#include "TestPlayer.h"
#include "ui_TestPlayer.h"

TestPlayer::TestPlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::TestPlayer),
      _media(0)
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidgetId(ui->video->widgetId());

    ui->video->setMediaPlayer(_player);
    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(_player);

    connect(ui->openLocal, SIGNAL(clicked()), this, SLOT(openLocal()));
    connect(ui->openUrl, SIGNAL(clicked()), this, SLOT(openUrl()));
    connect(ui->pause, SIGNAL(clicked()), _player, SLOT(pause()));
    connect(ui->stop, SIGNAL(clicked()), _player, SLOT(stop()));
}

TestPlayer::~TestPlayer()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}

void TestPlayer::openLocal()
{
    QString file =
        QFileDialog::getOpenFileName(this, tr("Open file"),
                        QDir::homePath(),
                        tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    _media = new VlcMedia(file, _instance);

    _player->open(_media);
}

void TestPlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(QUrl(url), _instance);

    _player->open(_media);
}
