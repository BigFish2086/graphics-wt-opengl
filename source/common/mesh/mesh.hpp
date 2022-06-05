#pragma once

#include "vertex.hpp"
#include <glad/gl.h>

namespace our {

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

class Mesh {
    // Here, we store the object names of the 3 main components of a mesh:
    // A vertex array object, A vertex buffer and an element buffer
    unsigned int VBO, EBO;
    unsigned int VAO;
    // We need to remember the number of elements that
    // will be draw by glDrawElements
    GLsizei elementCount;


public:
    float minVertexX, maxVertexX;
    // The constructor takes two vectors:
    // - vertices which contain the vertex data.
    // - elements which contain the indices of the vertices out of which
    // each rectangle will be constructed. The mesh class does not keep a
    // these data on the RAM. Instead, it should create a vertex buffer to
    // store the vertex data on the VRAM, an element buffer to store the
    // element data on the VRAM, a vertex array object to define how to read
    // the vertex & element buffer during rendering
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements) {
        // TODO: (Req 1) Write this function
        //  remember to store the number of elements in "elementCount"
        //  since you will need it for drawing For the attribute
        //  locations, use the constants defined above:
        //  ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

        minVertexX = 1000000;
        maxVertexX = -1000000;
        for(int i = 0 ; i < vertices.size() ; i++){
            minVertexX = glm::min(minVertexX, vertices[i].position.x);
            maxVertexX = glm::max(maxVertexX, vertices[i].position.x);
        }

        elementCount = elements.size();

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // size = 3, since position in vertex is of type vec3
        // stride = sizeof(Vertex), since this would be the begin of
        // each next vertex pointer = (void*)0, to start the next vertex
        // from its first component which is understood by default when it's 0
        glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
        glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex),
                              (void*)offsetof(Vertex, position));

        // size = 4, since color in vertex is of type vec4
        glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
        glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex),
                              (void*)offsetof(Vertex, color));

        // size = 2, since tex_coord is a 2D vec
        glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
        glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex),
                              (void*)offsetof(Vertex, tex_coord));

        // size = 3, since normal is of type vec3
        glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
        glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);
    }

    // this function should render the mesh
    void draw() {
        // TODO: (Req 1) Write this function
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);
    }

    // this function should delete the vertex & element buffers and the
    // vertex array object
    ~Mesh() {
        // TODO: (Req 1) Write this function
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    Mesh(Mesh const &) = delete;
    Mesh &operator=(Mesh const &) = delete;
};

} // namespace our
