#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    unsigned int j = 0;

    for (float i = 0; i <= 2 * M_PI; i += 0.0001f) {
        vertices.push_back(VertexFormat(glm::vec3((sin(i) * radius), (cos(i) * radius), 0), color));
        indices.push_back(j);
        j++;
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

  
    circle->InitFromData(vertices, indices);
    return circle;
}
Mesh* object2D::CreateShuriken(
    const std::string& name, 
    glm::vec3 leftBottomCorner, 
    float edgeLength, 
    glm::vec3 color, 
    bool fill) {
    
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(edgeLength, 0, 0), color),
        VertexFormat(corner + glm::vec3(edgeLength, edgeLength, 0), color),
        VertexFormat(corner + glm::vec3(0, edgeLength, 0), color),
        VertexFormat(corner + glm::vec3(-edgeLength, edgeLength, 0), color),
        VertexFormat(corner + glm::vec3(-edgeLength, 0, 0), color),
        VertexFormat(corner + glm::vec3(-edgeLength, -edgeLength, 0), color),
        VertexFormat(corner + glm::vec3(0, -edgeLength, 0), color),
        VertexFormat(corner + glm::vec3(edgeLength, -edgeLength, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8 };
    Mesh* shuriken = new Mesh(name);
    shuriken->SetDrawMode(GL_TRIANGLES);
    shuriken->InitFromData(vertices, indices);

    return shuriken;

}

Mesh* object2D::CreateHorizontalRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(4 *length, 0, 0), color),
        VertexFormat(corner + glm::vec3(4 *length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateVerticalRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, 4 * length, 0), color),
        VertexFormat(corner + glm::vec3(0, 4 * length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


