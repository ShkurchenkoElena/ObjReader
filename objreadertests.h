#ifndef OBJREADERTESTS_H
#define OBJREADERTESTS_H

#include "objreader.h"
#include <QObject>
#include <QTest>

class objreadertests: public QObject
{ Q_OBJECT
public:
    objreadertests(QObject *parent = nullptr);
private slots:
    void testParseVector3D();
    void testParseVector2D();
    void testParseInt();
    void testParsePolygonBlock();
    void testParsePolygon01();
    void testParsePolygon02();
    void testParsePolygon03();
    void testParsePolygon04();
    void testParsePolygon05();
    void testParsePolygon06();
    void testParsePolygon07();
    void testParsePolygon08();
    void testReadTokenString();
    void testReadTokenAndBody();
    void testReadObj01();
    void testReadObj02();
    void testReadObj03();

};

#endif // OBJREADERTESTS_H
