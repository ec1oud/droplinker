/****************************************************************************
**
** Copyright (C) 2020 Shawn Rutledge
**
** This file is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation
** and appearing in the file LICENSE included in the packaging
** of this file.
**
** This code is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMimeData>

MainWindow::MainWindow(QString linkDir, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_destinationDir(linkDir)
{
    ui->setupUi(this);
    updateTitlebar();
    setAcceptDrops(true);
    setAutoFillBackground(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionChoose_destination_triggered()
{
    m_destinationDir.setPath(QFileDialog::getExistingDirectory(this, tr("Choose destination link folder")));
    updateTitlebar();
}

void MainWindow::updateTitlebar()
{
    m_destinationDir.makeAbsolute();
    QDir::setCurrent(m_destinationDir.path());
    setWindowTitle(tr("drop to create links in %1").arg(m_destinationDir.path()));
}

void MainWindow::makeLink(const QString &filename)
{
    QFileInfo fi(filename);
    QString linkDest = ui->actionRelative->isChecked() ?
                m_destinationDir.relativeFilePath(fi.absoluteFilePath()) :
                m_destinationDir.filePath(filename);
    QFile f(linkDest);
    if (f.link(fi.fileName()))
        ui->plainTextEdit->appendPlainText(linkDest);
    else
        ui->plainTextEdit->appendPlainText(tr("failed to link %1").arg(linkDest));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && event->proposedAction() != Qt::IgnoreAction) {
        setBackgroundRole(QPalette::Highlight);
        event->acceptProposedAction();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event)
    setBackgroundRole(QPalette::Window);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        for (QUrl url : mimeData->urls()) {
            if (url.isLocalFile())
                makeLink(url.toLocalFile());
        }
        event->acceptProposedAction();
    }
    setBackgroundRole(QPalette::Window);
}
