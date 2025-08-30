/**
 *   @file: Chunk.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "Chunk.hh"
#include "../Engine.hh"

MyChunk::MyChunk() {}

CubeID ref MyChunk::at(int x, int y, int z)
{
    return _chunk[x][y][z];
}

void MyChunk::generate(int cx, int cy)
{
    for (int lx = 0; lx < x_dim; lx++) { //local x
        int const x = lx + (cx * x_dim); //global x
        for (int ly = 0; ly < y_dim; ly++) { //local y
            int const y = ly + (cy * y_dim); //global y
            //int const z = 1;
            int const z = perlin(x, y, 0.1f, 2.0f, 4.0f);
            _chunk[lx][ly][z] = CubeID::GRASS;
            for (int uz = z - 1; uz >= 0; uz--) {
                _chunk[lx][ly][uz] = CubeID::DIRT;
            }
        }
    }
    is_gen = true;
}

void MyChunk::register_cube(int lx, int x, int ly, int y, int z, six<bool> cref open_sides)
{
    float const gx = qs * x; //graphical/simulation location x
    float const gy = qs * y; //etc
    float const gz = qs * z; //etc
    glm::vec3 const gpos {gx,gy,gz};

    six<Texture> cref textures = cube_txts[at(lx,ly,z)];

    for (size_t i = 0; i < 6; i++)
    {
        if (open_sides[i])
        {
            Obj obj;
            obj.model = new Model(*quad_models[i]);
            obj.model->process();
            obj.model->textures.push_back(textures[i]);
            obj.move_position(gpos);
            engine->objs.push_back(obj);
        }
    }

    /* array<glm::vec3, 8> const v {
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
    if (open_sides[5]) vertices.append_range(get_from_v_at_indices(v, 1,3,7,5)); */
}

/* void MyChunk::make_model()
{
    //TODO CHECK: am i assigning this all to a nullptr or not? is model ever created?
    model->attributes = {{3, GL_FLOAT}, {2, GL_FLOAT}};
    model->gen_optimal_v_i(vertices);
    model->shader = this->shader;
    model->process();
    model->textures.emplace_back("Textures/grass_side.png");
    is_ren = true;
} */

int MyChunk::perlin(int x, int y, float in_scale, float out_shift, float out_scale)
{
    float res = stb_perlin_noise3_seed(
        cast<float>(x) * in_scale + 0.5f,
        cast<float>(y) * in_scale + 0.5f,
        0.5f,
        0, 0, 0,
        seed
    );
    res = out_scale * (res + out_shift);
    //print("{}\n", res);
    return res;
}

/* constexpr array<float, 20> MyChunk::get_from_v_at_indices(array<glm::vec3, 8> cref v, int i1, int i2, int i3, int i4)
{
    return {
        v[i1].x, v[i1].y, v[i1].z, 0.0f, 0.0f,
        v[i2].x, v[i2].y, v[i2].z, 0.0f, 1.0f,
        v[i3].x, v[i3].y, v[i3].z, 1.0f, 1.0f,
        v[i4].x, v[i4].y, v[i4].z, 1.0f, 0.0f,
    };
} */