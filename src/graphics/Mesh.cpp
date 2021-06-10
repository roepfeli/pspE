#include <Mesh.hpp>
#include <fstream>
#include <iostream>
#include <map>

enum LineType
{
    VERTEX_POS,
    TEXTURE_COORD,
    VERTEX_NORMAL,
    FACE,
    UNDEFINED
};

struct vertexPos
{
    float x, y, z;
};

struct vertexNormal
{
    float x, y, z;
};

struct texturePos
{
    float u, v;
};

struct vertexIndexData
{
    size_t posIndex;
    size_t texIndex;
    size_t normIndex;
};

static std::vector<std::string> splitString(std::string& str, char denominator)
{
    std::vector<std::string> splits;

    std::string tmp;
    for (char i : str)
    {
        if (i == denominator)
        {
            if (tmp.size() > 0)
            {
                splits.push_back(tmp);
            }
            tmp = "";
        }

        else
        {
            tmp.push_back(i);
        }
    }

    splits.push_back(tmp);

    return splits;
}

static LineType getLineType(std::string& line)
{
    if (line.size() >= 3)
    {
        // v
        if (line[0] == 'v' && line[1] == ' ')
        {
            return VERTEX_POS;
        }

        // vt
        if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            return TEXTURE_COORD;
        }

        // vn
        if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            return VERTEX_NORMAL;
        }

        // f
        if (line[0] == 'f' && line[1] == ' ')
        {
            return FACE;
        }
    }

    return UNDEFINED;
}

static vertexPos processVertexPos(std::string& line)
{
    std::vector<std::string> numbers = splitString(line, ' ');
    vertexPos position;

    position.x = atof(numbers[1].c_str());
    position.y = atof(numbers[2].c_str());
    position.z = atof(numbers[3].c_str());

    return position;
}

static texturePos processTexturePos(std::string& line)
{
    std::vector<std::string> numbers = splitString(line, ' ');

    texturePos position;

    position.u = atof(numbers[1].c_str());
    position.v = atof(numbers[2].c_str());

    return position;
}

static vertexNormal processVertexNormal(std::string& line)
{
    std::vector<std::string> numbers = splitString(line, ' ');
    vertexNormal normal;

    normal.x = atof(numbers[1].c_str());
    normal.y = atof(numbers[2].c_str());
    normal.z = atof(numbers[3].c_str());

    return normal;
}

static vertexIndexData processFaceVertexData(std::string& vertexStr)
{
    vertexIndexData vid;

    std::vector<std::string> vertexAttribs = splitString(vertexStr, '/');
    vid.posIndex = atoi(vertexAttribs[0].c_str()) - 1;
    vid.texIndex = atoi(vertexAttribs[1].c_str()) - 1;
    vid.normIndex = atoi(vertexAttribs[2].c_str()) - 1;

    return vid;
}

namespace graphics
{

Mesh::Mesh(const std::string& pathToObj)
{
    m_pathToObj = pathToObj;

    // TODO: put mesh-loading in Util.cpp...
    std::ifstream inputStream(pathToObj.c_str());

    std::string line;

    std::vector<vertexPos> vertexPositions;
    std::vector<texturePos> texturePositions;
    std::vector<vertexNormal> normalPositions;

    // TODO: this is retarded and needs optimaziaitnaont...
    // for now no index-buffers...

    vertexIndexData vid;
    std::map<std::string, ssize_t> vertexIndexMap;
    std::vector<std::string> vertexStrings;
    Vertex3D vert;

    while (std::getline(inputStream, line))
    {
        switch (getLineType(line))
        {
        case VERTEX_POS:
            vertexPositions.push_back(processVertexPos(line));
            break;
        case TEXTURE_COORD:
            texturePositions.push_back(processTexturePos(line));
            break;
        case VERTEX_NORMAL:
            normalPositions.push_back(processVertexNormal(line));
            break;
        case FACE:

            vertexStrings = splitString(line, ' ');

            for (size_t i = 1; i <= 3; ++i)
            {
                if (vertexIndexMap.count(vertexStrings[i]))
                {
                    // is in:
                    m_indices.push_back(vertexIndexMap[vertexStrings[i]]);
                }
                else
                {
                    // is not in:
                    vid = processFaceVertexData(vertexStrings[i]);

                    vert.x = vertexPositions[vid.posIndex].x;
                    vert.y = vertexPositions[vid.posIndex].y;
                    vert.z = vertexPositions[vid.posIndex].z;

                    vert.u = texturePositions[vid.texIndex].u;
                    vert.v = texturePositions[vid.texIndex].v;

                    vert.nx = normalPositions[vid.normIndex].x;
                    vert.ny = normalPositions[vid.normIndex].y;
                    vert.nz = normalPositions[vid.normIndex].z;

                    vertexIndexMap[vertexStrings[i]] = m_verticies.size();
                    m_indices.push_back(m_verticies.size());

                    m_verticies.push_back(vert);
                }
            }

            break;
        case UNDEFINED:
            break;
        }
    }

    inputStream.close();

    // create vertex/index-buffer
}

Mesh::~Mesh() { }

void* Mesh::getVerticiesAddr() const
{
    return (void*)m_verticies.data();
}

void* Mesh::getIndiciesAddr() const
{
    return (void*)m_indices.data();
}

uint16_t Mesh::getNumOfDrawVertices() const
{
    return (uint16_t)m_indices.size();
}

} // namespace graphics