#include "Heightmap.h"

#include <math.h>

#define LERP(a, b, x) (a + (b - a) * x)

Heightmap::Heightmap(unsigned int width, unsigned int length) : m_width(width), m_length(length), m_chunks(width * length)
{
    for (size_t i = 0; i < m_chunks.size(); i++) {
        m_chunks[i] = new Chunk<CHUNK_SIZE>();
    }
}

Heightmap::~Heightmap()
{
    for (size_t i = 0; i < m_chunks.size(); i++) {
        delete m_chunks[i];
    }
}

float Heightmap::getHeightAt(unsigned int x, unsigned int z) const
{
    const Chunk<CHUNK_SIZE>* chunk = getChunkAt(x / m_width, z / m_length);
    return chunk->getHeightAt(x % CHUNK_SIZE, z % CHUNK_SIZE);
}

float Heightmap::getHeightInterpolated(float x, float z) const
{
    if (x < 0.0f || z < 0.0f || x > m_width - 1 || z > m_length - 1) {
        return 0.0f;
    }
    float h00 = getHeightAt(static_cast<unsigned int>(floor(x)), static_cast<unsigned int>(floor(z)));
    float h01 = getHeightAt(static_cast<unsigned int>(floor(x)), static_cast<unsigned int>(ceil(z)));
    float h10 = getHeightAt(static_cast<unsigned int>(ceil(x)), static_cast<unsigned int>(floor(z)));
    float h11 = getHeightAt(static_cast<unsigned int>(ceil(x)), static_cast<unsigned int>(ceil(z)));
    float fx = x - floor(x);
    float fz = z - floor(z);
    return LERP(LERP(h00, h10, fx), LERP(h01, h11, fx), fz);
}