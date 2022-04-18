#include "objreadertests.h"
#include <QDebug>

using namespace ObjReadingTools;

objreadertests::objreadertests(QObject *parent):
    QObject(parent)
{
}
    void objreadertests::testParseVector3D()
    {
        QVector3D vertex;
        QString errorMsg;

        QVERIFY(parseVector3D("1 2 3", vertex, errorMsg));
        QCOMPARE(vertex, QVector3D(1, 2, 3));

        QVERIFY(parseVector3D("-1 -2 -3", vertex, errorMsg));
        QCOMPARE(vertex, QVector3D(-1, -2, -3));

        QVERIFY(parseVector3D("1e-3 2 3", vertex, errorMsg));
        QCOMPARE(vertex, QVector3D(1e-3, 2, 3));

        QVERIFY(parseVector3D("   1  2 3", vertex, errorMsg));
        QCOMPARE(vertex, QVector3D(1, 2, 3));

        QVERIFY(!parseVector3D("", vertex, errorMsg));

        QVERIFY(!parseVector3D("1 2", vertex, errorMsg));

        QVERIFY(!parseVector3D("1 2 3 4", vertex, errorMsg));

        QVERIFY(!parseVector3D("1 2 abc", vertex, errorMsg));
    }

    void objreadertests::testParseVector2D()
    {
        QVector2D vertex;
        QString errorMsg;

        QVERIFY(parseVector2D("1 2", vertex, errorMsg));
        QCOMPARE(vertex, QVector2D(1, 2));

        QVERIFY(parseVector2D("-1 -2", vertex, errorMsg));
        QCOMPARE(vertex, QVector2D(-1, -2));

        QVERIFY(parseVector2D("1e-3 2", vertex, errorMsg));
        QCOMPARE(vertex, QVector2D(1e-3, 2));

        QVERIFY(parseVector2D("   1  2", vertex, errorMsg));
        QCOMPARE(vertex, QVector2D(1, 2));

        QVERIFY(parseVector2D("1 2 3", vertex, errorMsg));
        QCOMPARE(vertex, QVector2D(1, 2));

        QVERIFY(!parseVector2D("", vertex, errorMsg));

        QVERIFY(!parseVector2D("1", vertex, errorMsg));

        QVERIFY(!parseVector2D("1 abc", vertex, errorMsg));
    }

    void objreadertests::testParseInt()
    {
        int value = -1;

        QVERIFY(parseInt("123", value));
        QCOMPARE(value, 123);

        QVERIFY(parseInt("-1", value));
        QCOMPARE(value, -1);

        QVERIFY(parseInt("0001", value));
        QCOMPARE(value, 1);

        QVERIFY(!parseInt("-1e-3", value));

        QVERIFY(!parseInt("", value));

        QVERIFY(!parseInt("abc", value));
    }

    void objreadertests::testParsePolygonBlock()
    {
        bool hasVertex;
        bool hasTexCoord;
        bool hasNormal;
        int vertexInd;
        int texCoordInd;
        int normalInd;
        QString errorMsg;

        QVERIFY(parsePolygonBlock("1/2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(hasTexCoord);
        QVERIFY(hasNormal);
        QCOMPARE(vertexInd, 0);
        QCOMPARE(texCoordInd, 1);
        QCOMPARE(normalInd, 2);

        QVERIFY(parsePolygonBlock("1", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(!hasNormal);
        QCOMPARE(vertexInd, 0);

        QVERIFY(parsePolygonBlock("1/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(!hasNormal);
        QCOMPARE(vertexInd, 0);

        QVERIFY(parsePolygonBlock("1//", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(!hasNormal);
        QCOMPARE(vertexInd, 0);

        QVERIFY(parsePolygonBlock("1//3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(hasNormal);
        QCOMPARE(vertexInd, 0);
        QCOMPARE(normalInd, 2);

        QVERIFY(parsePolygonBlock("1/2", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(hasTexCoord);
        QVERIFY(!hasNormal);
        QCOMPARE(vertexInd, 0);
        QCOMPARE(texCoordInd, 1);

        QVERIFY(parsePolygonBlock("1/2/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(hasVertex);
        QVERIFY(hasTexCoord);
        QVERIFY(!hasNormal);
        QCOMPARE(vertexInd, 0);
        QCOMPARE(texCoordInd, 1);

        QVERIFY(parsePolygonBlock("/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(!hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(!hasNormal);

        QVERIFY(parsePolygonBlock("//", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(!hasVertex);
        QVERIFY(!hasTexCoord);
        QVERIFY(!hasNormal);

        QVERIFY(parsePolygonBlock("/2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
        QVERIFY(!hasVertex);
        QVERIFY(hasTexCoord);
        QVERIFY(hasNormal);
        QCOMPARE(texCoordInd, 1);
        QCOMPARE(normalInd, 2);

        QVERIFY(!parsePolygonBlock("", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

        QVERIFY(!parsePolygonBlock(" /2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

        QVERIFY(!parsePolygonBlock("1/2/3/4", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

        QVERIFY(!parsePolygonBlock("1/2/a", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    }

    void objreadertests::testParsePolygon01()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(parsePolygon("1/2/3 4/5/6 7/8/9 10/11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
        QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
        QCOMPARE(texCoordIndices, QVector<int>({1, 4, 7, 10}));
        QCOMPARE(normalIndices, QVector<int>({2, 5, 8, 11}));
    }

    void objreadertests::testParsePolygon02()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(!parsePolygon("", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    }

    void objreadertests::testParsePolygon03()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(!parsePolygon("1/2/3", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    }

    void objreadertests::testParsePolygon04()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(parsePolygon("1//3 4//6 7//9 10//12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
        QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
        QCOMPARE(texCoordIndices, QVector<int>());
        QCOMPARE(normalIndices, QVector<int>({2, 5, 8, 11}));
    }

    void objreadertests::testParsePolygon05()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(parsePolygon("1/2/ 4/5/ 7/8/ 10/11/", vertexIndices, texCoordIndices, normalIndices, errorMsg));
        QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
        QCOMPARE(texCoordIndices, QVector<int>({1, 4, 7, 10}));
        QCOMPARE(normalIndices, QVector<int>());
    }

    void objreadertests::testParsePolygon06()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(!parsePolygon("/2/3 /5/6 /8/9 /11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    }

    void objreadertests::testParsePolygon07()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(!parsePolygon("1/2/3 4/5/6 7/8/9 10//12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    }

    void objreadertests::testParsePolygon08()
    {
        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;
        QString errorMsg;
        QVERIFY(!parsePolygon("1/2/3 4/5/6 7/8 10/11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    }

    void objreadertests::testReadTokenString()
    {
        QString token;

        QVERIFY(readTokenString("v 1 2 3", token));
        QCOMPARE(token, QString("v"));

        QVERIFY(readTokenString("vt 1 0.5", token));
        QCOMPARE(token, QString("vt"));

        QVERIFY(readTokenString("# ", token));
        QCOMPARE(token, QString("#"));

        QVERIFY(readTokenString("#", token));
        QCOMPARE(token, QString("#"));

        QVERIFY(!readTokenString(" #", token));
    }

    void objreadertests::testReadTokenAndBody()
    {
        ObjToken token;
        QString body;

        QVERIFY(readTokenAndBody("v 1 2 3", token, body));
        QCOMPARE(token, ObjToken::Vertex);
        QCOMPARE(body, QString("1 2 3"));

        QVERIFY(readTokenAndBody("vt 1 0.5", token, body));
        QCOMPARE(token, ObjToken::TexCoord);
        QCOMPARE(body, QString("1 0.5"));

        QVERIFY(readTokenAndBody("vn 1 2 3", token, body));
        QCOMPARE(token, ObjToken::Normal);
        QCOMPARE(body, QString("1 2 3"));

        QVERIFY(readTokenAndBody("# ", token, body));
        QCOMPARE(token, ObjToken::Unknown);
        QVERIFY(body.isEmpty());

        QVERIFY(readTokenAndBody("# comment", token, body));
        QCOMPARE(token, ObjToken::Unknown);
        QCOMPARE(body, QString("comment"));

        QVERIFY(readTokenAndBody("#", token, body));
        QCOMPARE(token, ObjToken::Unknown);
        QVERIFY(body.isEmpty());

        QVERIFY(!readTokenAndBody(" #", token, body));
    }

    void objreadertests::testReadObj01()
    {
        QString objString =
                "v 0 0 0\n"
                "v 1 0 0\n"
                "v 2 0 0\n"
                "v 2 1 0\n"
                "v 1 1 0\n"
                "f 1 2 5\n"
                "f 2 3 4 5\n";
        ObjData objData;
        QString errorMsg;
        QTextStream stream(&objString);
        QVERIFY(read(stream, objData, errorMsg));
        ObjData expectedData;
        expectedData.m_vertices = {
            { 0, 0, 0 },
            { 1, 0, 0 },
            { 2, 0, 0 },
            { 2, 1, 0 },
            { 1, 1, 0}
        };
        expectedData.m_polygonVertexIndices = {
            { 0, 1, 4 },
            { 1, 2, 3, 4 }
        };
        QCOMPARE(objData, expectedData);
    }

    void objreadertests::testReadObj02()
    {
        QString objString =
                "v 0 0 0\n"
                "v 1 0 0\n"
                "v 2 0 0\n"
                "v 2 1 0\n"
                "v 1 1 0\n"
                "f 1 2 5\n"
                "f 2 3 4 6\n";
        ObjData objData;
        QString errorMsg;
        QTextStream stream(&objString);
        QVERIFY(!read(stream, objData, errorMsg));
    }

    void objreadertests::testReadObj03()
    {
        QString objString =
                "v 0 0 0\n"
                "v 1 0 0\n"
                "v 1 1 0\n"
                "vn 0 0 -1\n"
                "vn 0 0 -1\n"
                "vn 0 0 -1\n"
                "f 1//1 2//2 3//3\n"
                "f 1/ 2/ 3/\n";
        ObjData objData;
        QString errorMsg;
        QTextStream stream(&objString);
        QVERIFY(!read(stream, objData, errorMsg));
}

