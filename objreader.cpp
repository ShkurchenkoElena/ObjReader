#include "objreader.h"

#include<QFile>
#include<QTextStream>
#include <QDebug>
ObjReadingTools::ObjData::ObjData()
{
}

bool ObjReadingTools::ObjData::operator==(const ObjReadingTools::ObjData &other) const
{
    return (m_vertices == other.m_vertices)
        && (m_texCoords == other.m_texCoords)
        && (m_normals == other.m_normals)
        && (m_polygonVertexIndices == other.m_polygonVertexIndices)
        && (m_polygonTexCoordIndices == other.m_polygonTexCoordIndices)
        && (m_polygonNormalIndices == other.m_polygonNormalIndices);
}

bool ObjReadingTools::read(QString fileName, ObjData &objData, QString &errorMsg)
{
    QFile file(fileName);
    if (!file.exists(fileName)) {
        errorMsg = QString("File \"%1\" doesn\'t exist").arg(fileName);
        return false;
    }
    if (!file.open(QFile::ReadOnly)) {
        errorMsg = QString("Failed to read file \"%1\"").arg(fileName);
        return false;
    }
    QTextStream stream(&file);
    return read(stream, objData, errorMsg);
}

bool ObjReadingTools::save(QFile &file,ObjData objData, QString &errorMsg){

if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
    QTextStream writeStream(&file);

    for(int i=0;i<objData.m_vertices.size();i++)
    {   QString line="v ";
        line+=vector3DtoLine(objData.m_vertices[i]);
         writeStream<<line<<"\n";

    }
    writeStream<<"\n";
    for(int i=0;i<objData.m_texCoords.size();i++)
    {   QString line="vt ";
        line+=vector2DtoLine(objData.m_texCoords[i]);
        writeStream<<line<<"\n";
    }
    writeStream<<"\n";

    for(int i=0;i<objData.m_normals.size();i++)
    {   QString line="vn ";
        line+=vector3DtoLine(objData.m_normals[i]);
        writeStream<<line<<"\n";
    }
    writeStream<<"\n";

    for(int i=0;i<objData.m_polygonVertexIndices.size();i++)
    {   QString line="f ";
        line+=faceToLine(objData.m_polygonVertexIndices[i],objData.m_polygonTexCoordIndices[i],objData.m_polygonNormalIndices[i]);
        writeStream<<line<<"\n";
    }
    writeStream<<"\n";
}
else{
    errorMsg="File is not open";
    return false;
}

    file.close();
    return true;
}


QString ObjReadingTools::vector3DtoLine(QVector3D v){
    QString line;
    QString buf;
    buf.setNum(v.x());
    line+=buf;
    line+=' ';
    buf.setNum(v.y());
    line+=buf;
    line+=' ';
    buf.setNum(v.z());
    line+=buf;
    line+=' ';
    return line;
}
QString ObjReadingTools::vector2DtoLine(QVector2D v){
    QString line;
    QString buf;
    buf.setNum(v.x());
    line+=buf;
    line+=' ';
    buf.setNum(v.y());
    line+=buf;
    line+=' ';
    return line;
}
QString ObjReadingTools::faceToLine(QVector<int> v1,QVector<int> v2, QVector<int> v3){
    QString line;
    QString buf;
    for(int i=0;i<v1.size();i++){
        buf.setNum(v1[i]+1);
        line+=buf;
        line+='/';
        buf.setNum(v2[i]+1);
        line+=buf;
        line+='/';
        buf.setNum(v3[i]+1);
        line+=buf;
        line+=' ';
    }
    return line;
}
bool ObjReadingTools::read(QTextStream &stream, ObjReadingTools::ObjData &objData, QString &errorMsg)
{
    ObjData data;
    int lineInd = -1;
    while (!stream.atEnd()) {
        lineInd++;
        QString line = stream.readLine();
        ObjToken token;
        QString body;
        if (!readTokenAndBody(line, token, body))
            continue;
        if (token == ObjToken::Vertex) {
            QVector3D vertex;
            if (!parseVector3D(body, vertex, errorMsg)) {
                errorMsg = QString("Error in line %1. %2").arg(lineInd).arg(errorMsg);
                return false;
            }
            data.m_vertices.append(vertex);
            continue;
        }
        if (token == ObjToken::TexCoord) {
            QVector2D texCoord;
            if (!parseVector2D(body, texCoord, errorMsg)) {
                errorMsg = QString("Error in line %1. %2").arg(lineInd).arg(errorMsg);
                return false;
            }
            data.m_texCoords.append(texCoord);
            continue;
        }
        if (token == ObjToken::Normal) {
            QVector3D normal;
            if (!parseVector3D(body, normal, errorMsg)) {
                errorMsg = QString("Error in line %1. %2").arg(lineInd).arg(errorMsg);
                return false;
            }
            data.m_normals.append(normal);
            continue;
        }
        if (token == ObjToken::Face) {
            QVector<int> vertexIndices;
            QVector<int> texCoordIndices;
            QVector<int> normalIndices;
            if (!parsePolygon(body, vertexIndices, texCoordIndices, normalIndices, errorMsg)) {
                errorMsg = QString("Error in line %1. %2").arg(lineInd).arg(errorMsg);
                return false;
            }
            data.m_polygonVertexIndices.append(vertexIndices);
            if (!texCoordIndices.isEmpty())
                data.m_polygonTexCoordIndices.append(texCoordIndices);
            if (!normalIndices.isEmpty())
                data.m_polygonNormalIndices.append(normalIndices);
            continue;
        }
    }
    if (!checkPolygonIndices(data.m_polygonVertexIndices, data.m_polygonTexCoordIndices, data.m_polygonNormalIndices, errorMsg))
        return false;
    if (!checkPolygonVertexIndexRange(data.m_polygonVertexIndices, data.m_vertices.size() - 1, errorMsg)
            || !checkPolygonVertexIndexRange(data.m_polygonTexCoordIndices, data.m_texCoords.size() - 1, errorMsg)
            || !checkPolygonVertexIndexRange(data.m_polygonNormalIndices, data.m_normals.size() - 1, errorMsg))
        return false;
    objData = data;
    return true;
}

bool ObjReadingTools::parseVector3D(QString string, QVector3D &vertex, QString &errorMsg)
{
    if (string.isEmpty()) {
        errorMsg = "Empty body";
        return false;
    }
    QStringList blocks = string.split(" ", Qt::SkipEmptyParts);
    if (blocks.size() != 3) {
        errorMsg = "Number of block is not equal to 3";
        return false;
    }
    bool isFloat = false;
    float components[3];
    for (int componentInd = 0; componentInd < 3; ++componentInd) {
        components[componentInd] = blocks[componentInd].toFloat(&isFloat);
        if (!isFloat) {
            errorMsg = "Vectory component is not a float value";
            return false;
        }
    }
    vertex = QVector3D(components[0], components[1], components[2]);
    return true;
}

bool ObjReadingTools::parsePolygon(QString string, QVector<int> &vertexIndices, QVector<int> &texCoordIndices, QVector<int> &normalIndices, QString &errorMsg)
{
    if (string.isEmpty()) {
        errorMsg = "Polygon body is empty";
        return false;
    }
    QStringList blocks = string.split(" ", Qt::SkipEmptyParts);
    int nVertices = blocks.size();
    if (nVertices < 3) {
        errorMsg = "Polygon contains less than 3 vertices";
        return false;
    }
    for (int blockInd = 0; blockInd < nVertices; ++blockInd) {
        bool hasVertex = false;
        int vertexInd = -1;
        bool hasTexCoord = false;
        int texCoordInd = -1;
        bool hasNormal = false;
        int normalInd = -1;
        if (!parsePolygonBlock(blocks[blockInd], hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg)) {
            errorMsg = "Failed to parse polygon. " + errorMsg;
            return false;
        }
        if (!hasVertex) {
            errorMsg = "Polygon block doesn\'t contain a vertex index";
            return false;
        }
        vertexIndices.append(vertexInd);
        if (hasTexCoord)
            texCoordIndices.append(texCoordInd);
        if (hasNormal)
            normalIndices.append(normalInd);
    }
    if ((texCoordIndices.size() > 0) && (texCoordIndices.size() < nVertices)) {
        errorMsg = "Tex coordnate indices were not specified for all polygon vertices";
        return false;
    }
    if ((normalIndices.size() > 0) && (normalIndices.size() < nVertices)) {
        errorMsg = "Normal indices were not specified for all polygon vertices";
        return false;
    }
    return true;
}

bool ObjReadingTools::parsePolygonBlock(QString string, bool &hasVertex, int &vertexInd, bool &hasTexCoord, int &texCoordInd, bool &hasNormal, int &normalInd, QString &errorMsg)
{
    if (string.isEmpty()) {
        errorMsg = "Polygon block is empty";
        return false;
    }
    QStringList blocks = string.split("/");
    if (blocks.size() > 3) {
        errorMsg = "More than 3 polygon blocks";
        return false;
    }
    hasVertex = !blocks[0].isEmpty();
    if (hasVertex && !parseInt(blocks[0], vertexInd)) {
        errorMsg = "Vertex index is not an int";
        return false;
    }
    vertexInd--;
    hasTexCoord = (blocks.size() > 1) && (!blocks[1].isEmpty());
    if (hasTexCoord && !parseInt(blocks[1], texCoordInd)) {
        errorMsg = "Tex coord index is not an int";
        return false;
    }
    texCoordInd--;
    hasNormal = (blocks.size() > 2) && (!blocks[2].isEmpty());
    if (hasNormal && !parseInt(blocks[2], normalInd)) {
        errorMsg = "Normal index is not an int";
        return false;
    }
    normalInd--;
    return true;
}

bool ObjReadingTools::parseInt(QString string, int &value)
{
    bool isInt = false;
    int m_value = string.toInt(&isInt);
    if (!isInt)
        return false;
    value = m_value;
    return true;
}

bool ObjReadingTools::readTokenString(QString string, QString &token)
{
    int firstSpaceInd = string.indexOf(' ');
    bool entireStringIsToken = (firstSpaceInd == -1);
    if (entireStringIsToken) {
        token = string;
        return true;
    }
    bool startsWithSpace = (firstSpaceInd == 0);
    if (startsWithSpace)
        return false;
    token = string.left(firstSpaceInd);
    return true;
}

bool ObjReadingTools::readTokenAndBody(QString string, ObjToken &token, QString &body)
{
    QString tokenString;
    if (!readTokenString(string, tokenString))
        return false;
    int tokenWithSpaceLength = tokenString.size() + 1;
    const bool isEmptyBody = (tokenWithSpaceLength >= string.size());
    if (isEmptyBody)
        body = QString();
    else
        body = string.mid(tokenWithSpaceLength);

    const int nTokens = 4;
    ObjToken tokens[nTokens] = { ObjToken::Vertex, ObjToken::Normal, ObjToken::TexCoord, ObjToken::Face };
    QString tokenStrings[nTokens] = { "v", "vn", "vt", "f" };
    for (int tokenInd = 0; tokenInd < nTokens; ++tokenInd)
        if (tokenString == tokenStrings[tokenInd]) {
            token = tokens[tokenInd];
            return true;
        }
    token = ObjToken::Unknown;
    return true;
}

bool ObjReadingTools::parseVector2D(QString string, QVector2D &vertex, QString &errorMsg)
{
    if (string.isEmpty()) {
        errorMsg = "Empty body";
        return false;
    }
    QStringList blocks = string.split(" ", Qt::SkipEmptyParts);
    if (blocks.size() < 2) {
        errorMsg = "Number of blocks is less than 2";
        return false;
    }
    bool isFloat = false;
    float components[2];
    for (int componentInd = 0; componentInd < 2; ++componentInd) {
        components[componentInd] = blocks[componentInd].toFloat(&isFloat);
        if (!isFloat) {
            errorMsg = "Vector component is not a float value";
            return false;
        }
    }
    vertex = QVector2D(components[0], components[1]);
    return true;
}

bool ObjReadingTools::checkPolygonIndices(QVector<QVector<int> > polygonVertexIndices, QVector<QVector<int> > polygonTexCoordIndices, QVector<QVector<int> > polygonNormalIndices, QString &errorMsg)
{
    int nPolygons = polygonVertexIndices.size();
    bool noneOrAllTexCoordIndicesSpecified = polygonTexCoordIndices.isEmpty() || (polygonTexCoordIndices.size() == nPolygons);
    if (!noneOrAllTexCoordIndicesSpecified) {
        errorMsg = "Tex coord indices were not specified for all the polygons";
        return false;
    }
    bool noneOrAllNormalIndicesSpecified = polygonNormalIndices.isEmpty() || (polygonNormalIndices.size() == nPolygons);
    if (!noneOrAllNormalIndicesSpecified) {
        errorMsg = "Normal indices were not specified for all the polygons";
        return false;
    }
    return true;
}

bool ObjReadingTools::checkPolygonVertexIndexRange(QVector<QVector<int> > polygonVertexIndices, int maxIndex, QString &errorMsg)
{
    for (int polygonInd = 0; polygonInd < polygonVertexIndices.size(); ++polygonInd) {
        int nVerticesInPolygon = polygonVertexIndices[polygonInd].size();
        const QVector<int> &vertexIndices = polygonVertexIndices[polygonInd];
        for (int polygonVertexInd = 0; polygonVertexInd < nVerticesInPolygon; ++polygonVertexInd) {
            int vertexInd = vertexIndices[polygonVertexInd];
            if ((vertexInd < 0) || (vertexInd > maxIndex)) {
                errorMsg = QString("Polygon %1 refers to non-existing vertex %2").arg(polygonInd).arg(vertexInd);
                return false;
            }
        }
    }
    return true;
}
