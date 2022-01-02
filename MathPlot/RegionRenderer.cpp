#include "RegionRenderer.h"

RegionRenderer::RegionRenderer():
    shader(R"(.\shaders\region.vert)", R"(.\shaders\region.frag)")
{
    double quadVertices[] = {
    -1.0,  1.0,  0.0, 1.0,
    -1.0, -1.0,  0.0, 0.0,
     1.0, -1.0,  1.0, 0.0,
    -1.0,  1.0,  0.0, 1.0,
     1.0, -1.0,  1.0, 0.0,
     1.0,  1.0,  1.0, 1.0
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void*)(2 * sizeof(double)));

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


}

void RegionRenderer::draw()
{
    shader.use();
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RegionRenderer::updateData(const std::vector<std::vector<double>>& data)
{
    if (data.size() == 0 || data.front().size() == 0)
    {
        return;
    }

    std::vector<unsigned char> flattenedData;
    flattenedData.reserve(data.size() * data.front().size());
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data.front().size(); j++)
        {
            flattenedData.push_back(static_cast<unsigned char>(data[i][j] * 255));
        }

        // 4-byte alignment
        while (flattenedData.size() % 4 != 0)
        {
            flattenedData.push_back(0);
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.front().size(), data.size(), 0, GL_RED, GL_UNSIGNED_BYTE, &flattenedData[0]);
}
