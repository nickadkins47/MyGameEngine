/**
 *   @file: Chunk.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "Chunk.hh"

MyChunk::MyChunk() {}

CubeID ref MyChunk::at(int x, int y, int z)
{
    return _chunk[x][y][z];
}

void MyChunk::generate(int cx, int cy)
{
    for (int x = 0; x < x_dim; x++) { //local x
        int const gx = x + (cx * x_dim); //global x
        for (int y = 0; y < y_dim; y++) { //local y
            int const gy = y + (cy * y_dim); //global y
            int const z = cast<int>(perlin2d(gx*0.03125, gy*0.03125, 16, 0, 24));
            _chunk[x][y][z] = CubeID::GRASS;
            for (int uz = z - 1; uz >= 0; uz--) {
                _chunk[x][y][uz] = CubeID::DIRT;
            }
        }
    }
}

void MyChunk::register_cube(int x, int y, int z, six<Texture> cref textures, six<bool> cref open_sides)
{
    const float gx = qs * x; //graphical/simulation location x
    const float gy = qs * y; //etc
    const float gz = qs * z; //etc

    array<glm::vec3, 8> const v {
        glm::vec3{   gx,    gy,    gz},
        glm::vec3{   gx,    gy, gz+qs},
        glm::vec3{   gx, gy+qs,    gz},
        glm::vec3{   gx, gy+qs, gz+qs},
        glm::vec3{gx+qs,    gy,    gz},
        glm::vec3{gx+qs,    gy, gz+qs},
        glm::vec3{gx+qs, gy+qs,    gz},
        glm::vec3{gx+qs, gy+qs, gz+qs},
    };

    if (open_sides[0]) vertices.append_range(get_from_v_at_indices(v, 0,1,3,2));
    if (open_sides[1]) vertices.append_range(get_from_v_at_indices(v, 4,5,7,6));
    if (open_sides[2]) vertices.append_range(get_from_v_at_indices(v, 0,1,5,4));
    if (open_sides[3]) vertices.append_range(get_from_v_at_indices(v, 2,3,7,6));
    if (open_sides[4]) vertices.append_range(get_from_v_at_indices(v, 0,2,6,4));
    if (open_sides[5]) vertices.append_range(get_from_v_at_indices(v, 1,3,7,5));
}

void MyChunk::make_model(int cx, int cy)
{
    model->attributes = {{3, GL_FLOAT}, {2, GL_FLOAT}};
    model->gen_optimal_v_i(vertices);
    model->shader = this->shader;
    model->process();
    model->textures.push_back(Texture("Textures/grass_side.png", GL_RGBA));
}

double MyChunk::perlin2d(double x, double y, double scale, double shift, int32_t octaves, double persistence)
{
    //siv::PerlinNoise static const perlin(seed);
    return scale * 1;//perlin.octave2D_01(double(x), double(y), octaves, persistence) + shift;
}

constexpr array<float, 20> MyChunk::get_from_v_at_indices(array<glm::vec3, 8> cref v, int i1, int i2, int i3, int i4)
{
    return {
        v[i1].x, v[i1].y, v[i1].z, 0.0f, 0.0f,
        v[i2].x, v[i2].y, v[i2].z, 0.0f, 1.0f,
        v[i3].x, v[i3].y, v[i3].z, 1.0f, 1.0f,
        v[i4].x, v[i4].y, v[i4].z, 1.0f, 0.0f,
    };
}