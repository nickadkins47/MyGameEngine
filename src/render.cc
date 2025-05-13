
#include "render.hh"

void renderQuad(glm::vec3 position, QuadOrientation orientation) {
    switch (orientation) {
        case QuadOrientation::XY: {
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{0.0f, 1.0f, 0.0f}, position + glm::vec3{1.0f, 1.0f, 0.0f}
            });
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{1.0f, 1.0f, 0.0f}, position + glm::vec3{1.0f, 0.0f, 0.0f}
            });
            break;
        }
        case QuadOrientation::XZ: {
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{0.0f, 0.0f, 1.0f}, position + glm::vec3{1.0f, 0.0f, 1.0f}
            });
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{1.0f, 0.0f, 1.0f}, position + glm::vec3{1.0f, 0.0f, 0.0f}
            });
            break;
        }
        case QuadOrientation::YZ: {
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{0.0f, 1.0f, 0.0f}, position + glm::vec3{0.0f, 1.0f, 1.0f}
            });
            triangles.push_back({
                position + glm::vec3{0.0f, 0.0f, 0.0f}, position + glm::vec3{0.0f, 1.0f, 1.0f}, position + glm::vec3{0.0f, 0.0f, 1.0f}
            });
            break;
        }
    }
}

void render(SDL_Window* window, SDL_Renderer* renderer, glm::mat4& vpMat) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //TODO:
    //import my stuff from AftrBurner to here (IE MyKeyboard, MyCam, etc)

    vector<array<glm::vec4, 3>> newTriangles {};

    for (auto const& tri : triangles) {
        array<glm::vec4, 3> newTri {glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0}};
        bool withinClippingVolume = true;
        for (int i = 0; i < tri.size(); i++) {
            newTri[i] = vpMat * glm::vec4 { tri[i][0], tri[i][1], tri[i][2], 1.0f };
            if (newTri[i][2] < zNear) withinClippingVolume = false;
            if (std::fabs(newTri[i][3]) >= 0.0625f) {
                newTri[i][0] /= newTri[i][3];
                newTri[i][1] /= newTri[i][3];
            }
        }
        if (withinClippingVolume) newTriangles.push_back(newTri);
    }

    //TODO: implement proper clipping: https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html

    for (int j = 0; j < newTriangles.size(); j++) {
        auto& newTri = newTriangles[j];

        SDL_FPoint fpoints[] = {
            {(newTri[0][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[0][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[1][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[1][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[2][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[2][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[0][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[0][1] + 1.0f) * (WINDOW_HEIGHT/2)},
        };
        SDL_RenderLines(renderer, fpoints, 4);

        SDL_Vertex vertices[] = {
            {fpoints[0], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
            {fpoints[1], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
            {fpoints[2], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
            {fpoints[3], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
        };
        int indices[] = { 0,1,2, 2,3,0 };
        SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);

    printf("%s",SDL_GetError());
}
