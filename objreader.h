#ifndef OBJREADER_H
#define OBJREADER_H

#include<QString>
#include<QVector>
#include<QVector3D>
#include<QVector2D>
#include<QTextStream>
#include<QFile>

namespace ObjReadingTools{

class ObjData
{
public:
    ObjData();
    bool operator==(const ObjData &other) const;
    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_texCoords;
    QVector<QVector3D> m_normals;
    QVector<QVector<int> > m_polygonVertexIndices;
    QVector<QVector<int> > m_polygonTexCoordIndices;
    QVector<QVector<int> > m_polygonNormalIndices;
};

enum class ObjToken {
    Vertex,
    TexCoord,
    Normal,
    Face,
    Unknown
};

bool save(/*QString fileName,*/QFile &file,ObjData objData, QString &errorMsg);
QString vector3DtoLine(QVector3D v);
QString vector2DtoLine(QVector2D v);
QString faceToLine(QVector<int> v1,QVector<int> v2, QVector<int> v3);
bool read(QTextStream &stream, ObjData &objData, QString &errorMsg);
bool read(QString fileName, ObjData &objData, QString &errorMsg);
bool parseVector3D(QString string, QVector3D &vertex, QString &errorMsg);
bool parseVector2D(QString string, QVector2D &vertex, QString &errorMsg);
bool parsePolygon(QString string, QVector<int> &vertexIndices, QVector<int> &texCoordIndices, QVector<int> &normalIndices, QString &errorMsg);
bool parsePolygonBlock(QString string, bool &hasVertex, int &vertexInd, bool &hasTexCoord, int &texCoordInd, bool &hasNormal, int &normalInd, QString &errorMsg);
bool parseInt(QString string, int &value);
bool readTokenString(QString string, QString &token);
bool readTokenAndBody(QString string, ObjToken &token, QString &body);
bool checkPolygonIndices(QVector<QVector<int> > polygonVertexIndices, QVector<QVector<int> > polygonTexCoordIndices, QVector<QVector<int> > polygonNormalIndices, QString &errorMsg);
bool checkPolygonVertexIndexRange(QVector<QVector<int> > polygonIndices, int maxIndex, QString &errorMsg);

}
#endif // OBJREADER_H
