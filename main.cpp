
#include <QCoreApplication>
#include "objreader.h"
#include "objreadertests.h"
#include <QDebug>
#include<iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    objreadertests tests;
    QTest::qExec(&tests);

    ObjReadingTools::ObjData objData;
    QString errorMsg;
    if (!ObjReadingTools::read("fileName", objData, errorMsg))//file to read from
    {
        qDebug() << errorMsg;
        return -1;
    }
    QFile file("newFileName"); //file to save
    ObjReadingTools::save(file,objData,errorMsg);

    return 0;
}
