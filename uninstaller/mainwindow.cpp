/*
Copyright (c) 2013, cen (imbacen@gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 
   
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registry.h"
#include "QFile"
#include "QProcess"
#include "QDir"
#include "winutils.h"
#include "QStringList"
#include "QProcess"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//uninstall
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);

    Registry reg;
    //prepare paths
    QString w3path = reg.getW3dir();
    QString europath = reg.getEuroPath();

    //first make sure that europath contains Eurobattle.net so we are not deleting system32 or some shit..
    if (!europath.endsWith("Eurobattle.net")){
        sendLine("FATAL ERROR! Installation path from registry is invalid. I don't know where the program is installed.");
        return;
    }

    //remove Uninstall reg entry
    bool r = reg.removeInstallationEntries();
    if (!r){
        sendLine("Unable to remove registry entry for Add/Remove programs. Uninstall failed");
        return;
    }


    if (!QFile::remove(w3path+"\\w3l.exe")) {
        sendLine("Could not remove "+w3path+"\\w3l.exe"+". Try to remove it manually.");
    }
    if (!QFile::remove(w3path+"\\w3lh.dll")) {
        sendLine("Could not remove "+w3path+"\\w3lh.dll"+". Try to remove it manually.");
    }
    QDir d(Winutils::getAppData()+"\\Eurobattle.net");
    d.removeRecursively();

    //copy war3patch
    QFile w3p(europath+"\\uninstall\\War3Patch.mpq");
    if (w3p.exists()){
        QFile::remove(w3path+"\\War3Patch.mpq");
        bool b=w3p.copy(w3path+"\\War3Patch.mpq");
        if (b) sendLine("Restored War3Patch.mpq to original state");
        else sendLine("Unable to restore War3Patch.mpq to original state. To remove cutom icons reinstall W3");
    }
    //delete desktop shortcut
    QFile::remove(Winutils::getDesktop()+"\\Eurobattle.net Client.lnk");

    //delete program files directory
    if (Winutils::IsWow64()) {
        QStringList a;
        a << "/c";
        a << "rd";
        a << "/s/q";
        a << europath;

        QProcess::startDetached(Winutils::getSystem32()+"\\cmd.exe", a);
        QApplication::quit();
    }
    else {
        QStringList a;
        a << "/c";
        a << "rd";
        a << "/s/q";
        a << europath;

        QProcess::startDetached(Winutils::getSystem32()+"\\cmd.exe", a);
        QApplication::quit();
    }



    /*QString command = "\"taskkill /f /im uninstaller.exe && rd /s/q \""+europath+"\"\r\n\"";
    std::string s = command.toStdString();
    QProcess cmd;
    cmd.startDetached(Winutils::getSystem32()+"\\cmd.exe");
    cmd.waitForStarted();
    cmd.write(s.c_str());
    cmd.write(command.toLocal8Bit());*/
}

void MainWindow::sendLine(QString line)
{
    ui->textBrowser->append(line);
}
