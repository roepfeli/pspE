#pragma once

#include <Vertex.h>

#include <string>
#include <vector>

namespace graphics
{
class Mesh
{
private:
    std::vector<Vertex3D> m_verticies;
    std::vector<uint16_t> m_indices;

    std::string m_pathToObj;

public:
    Mesh(const std::string& pathToFile);
    ~Mesh();
    void* getVerticiesAddr() const;
    void* getIndiciesAddr() const;
    uint16_t getNumOfDrawVertices() const;
};

} // namespace graphics