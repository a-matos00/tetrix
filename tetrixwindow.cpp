/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "tetrixboard.h"
#include "tetrixwindow.h"
#include "tetrixpiece.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>

//! [0]
TetrixWindow::TetrixWindow(QWidget *parent)
    : QWidget(parent), board(new TetrixBoard)
{
//! [0]
    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    board->setNextPieceLabel(nextPieceLabel);
//! [1]
    scoreLcd = new QLCDNumber(5);
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);
//! [1]
    levelLcd = new QLCDNumber(2);
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    linesLcd = new QLCDNumber(5);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);

//! [2]
    startButton = new QPushButton(tr("START"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("EXIT"));
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("Pause"));

    //NEW
    leftButton = new QPushButton(tr("LEFT"));
    leftButton->setFocusPolicy(Qt::NoFocus);
    rightButton = new QPushButton(tr("RIGHT"));
    rightButton->setFocusPolicy(Qt::NoFocus);
    dropButton = new QPushButton(tr("DROP"));
    dropButton->setFocusPolicy(Qt::NoFocus);

    rightButton->setStyleSheet("QPushButton{font-size: 50px;font-family: Arial;}");
    leftButton->setStyleSheet("QPushButton{font-size: 50px;font-family: Arial;}");
    startButton->setStyleSheet("QPushButton{font-size: 30px;font-family: Arial;}");
    quitButton->setStyleSheet("QPushButton{font-size: 30px;font-family: Arial;}");
    pauseButton->setStyleSheet("QPushButton{font-size: 30px;font-family: Arial;}");
    dropButton->setStyleSheet("QPushButton{font-size: 30px;font-family: Arial;}");
    //rotateButton = new QPushButton(tr("&Rotate"));
    //rotateButton->setFocusPolicy(Qt::NoFocus);
//! [2] //! [3]
    pauseButton->setFocusPolicy(Qt::NoFocus);
//! [3] //! [4]

    connect(startButton, &QPushButton::clicked, board, &TetrixBoard::start);
    connect(leftButton, &QPushButton::clicked, board, &TetrixBoard::leftPress);
    connect(rightButton, &QPushButton::clicked, board, &TetrixBoard::rightPress);
    connect(dropButton, &QPushButton::clicked, board, &TetrixBoard::dropDown);
//! [4] //! [5]
    connect(quitButton , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(pauseButton, &QPushButton::clicked, board, &TetrixBoard::pause);
#if __cplusplus >= 201402L
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, qOverload<int>(&QLCDNumber::display));
#else
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, QOverload<int>::of(&QLCDNumber::display));
#endif
//! [5]
//! [6]
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("NEXT")), 1, 0);
    layout->addWidget(nextPieceLabel, 2, 0,2,1);
    //layout->addWidget(createLabel(tr("LEVEL")), 2, 0);
    //layout->addWidget(levelLcd, 3, 0);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(createLabel(tr("SCORE")), 3, 2);
    layout->addWidget(scoreLcd, 2, 2,2,1);
    //layout->addWidget(createLabel(tr("LINES REMOVED")), 2, 2);
    //layout->addWidget(linesLcd, 3, 2);
    layout->addWidget(quitButton, 4, 2);
    layout->addWidget(pauseButton, 5, 2);
    layout->addWidget(leftButton, 0, 0);
    layout->addWidget(rightButton, 0, 2);
    layout->addWidget(dropButton, 1, 2);
    //layout->addWidget(rotateButton, 1, 2);
    setLayout(layout);

    setWindowTitle(tr("Tetrix"));
    resize(550, 370);
    //background color
    this->setStyleSheet("TetrixWindow{background-color:#eaeaea}");
}
//! [6]

//! [7]
QLabel *TetrixWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}
//! [7]

