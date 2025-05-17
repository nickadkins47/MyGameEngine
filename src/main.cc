
//#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "EngineCore.hh"
#include "Obj/Quad.hh"

//main directory, relative to GetBasePath, which is exe's directory, aka build/windows/x64/release
std::string static const srcDir = std::format("{}../../../..", SDL_GetBasePath());

// the vertex input layout
struct Vertex {
    float x, y, z;      //vec3 position
    float r, g, b, a;   //vec4 color
};

// a list of vertices
static Vertex vertices[] {
    {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},     // top vertex
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}     // bottom right vertex
};
static int vertices_len = 3;

int main(int argc, char** argv) {
    /* MGE::EngineCore engine;
    
    engine.add_obj(MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::XY));

    engine.add_objs({
        MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::XZ),
        MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::YZ),
        MGE::ObjQuad({0.0f, 0.0f, 1.0f}, MGE::QuadOrientation::XY),
        MGE::ObjQuad({0.0f, 1.0f, 0.0f}, MGE::QuadOrientation::XZ),
        MGE::ObjQuad({1.0f, 0.0f, 0.0f}, MGE::QuadOrientation::YZ),
    });
    
    engine.run(); */

    const int width = 1200;
    const int height = 800;

    //test
    void* asdf = SDL_LoadFile(std::format("{}/src/Camera.cc", srcDir).c_str(), NULL);
    if (asdf == NULL) {
        std::print("ERROR: (asdf) returned NULL\n{}\n", SDL_GetError());
        return -1;
    }

    //INIT
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("TEST", width, height, NULL);
    SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
    SDL_ClaimWindowForGPUDevice(gpu_device, window);

    size_t vertexCodeSize;
    
    void* vertexCode = SDL_LoadFile(std::format("{}/build/Shaders/vertex.vert.spv", srcDir).c_str(), &vertexCodeSize);
    if (vertexCode == NULL) {
        std::print("ERROR: vertexCode returned NULL\n");
        return -1;
    }
    SDL_GPUShaderCreateInfo vertex_info {
        .code_size = vertexCodeSize,
        .code = (Uint8*)vertexCode,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers = 0,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 0,
    };
    SDL_GPUShader* vertexShader = SDL_CreateGPUShader(gpu_device, &vertex_info);
    SDL_free(vertexCode);

    size_t fragmentCodeSize;
    void* fragmentCode = SDL_LoadFile(std::format("{}/build/Shaders/fragment.frag.spv", srcDir).c_str(), &fragmentCodeSize);
    if (fragmentCode == NULL) {
        std::print("ERROR: fragmentCode returned NULL\n");
        return -1;
    }
    SDL_GPUShaderCreateInfo fragment_info {
        .code_size = fragmentCodeSize,
        .code = (Uint8*)fragmentCode,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = 0,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 0,
    };
    SDL_GPUShader* fragmentShader = SDL_CreateGPUShader(gpu_device, &fragment_info);
    SDL_free(fragmentCode);
    
    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    };

    SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
    vertexBufferDesctiptions[0] = {
        .slot = 0,
        .pitch = sizeof(Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };
    pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
    pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

    SDL_GPUVertexAttribute vertexAttributes[2];
    vertexAttributes[0] = {
        .location = 0,
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        .offset = 0,
    };
    vertexAttributes[1] = {
        .location = 1,
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
        .offset = sizeof(float) * 3,
    };
    pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
    pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

    SDL_GPUColorTargetDescription colorTargetDescriptions[1];
    colorTargetDescriptions[0] = {
        .format = SDL_GetGPUSwapchainTextureFormat(gpu_device, window),
        .blend_state = {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .enable_blend = true,
        },
    };
    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

    SDL_GPUGraphicsPipeline* graphicsPipeline = SDL_CreateGPUGraphicsPipeline(gpu_device, &pipelineInfo);

    SDL_ReleaseGPUShader(gpu_device, vertexShader);
    SDL_ReleaseGPUShader(gpu_device, fragmentShader);

    SDL_GPUBufferCreateInfo buffer_info {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(vertices),
    };
    SDL_GPUBuffer* vertex_buf = SDL_CreateGPUBuffer(gpu_device, &buffer_info);

    SDL_GPUTransferBufferCreateInfo transfer_info {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(vertices),
    };
    SDL_GPUTransferBuffer* transfer_buf = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(gpu_device, transfer_buf, false);
    SDL_memcpy(data, (void*)vertices, sizeof(vertices));
    SDL_UnmapGPUTransferBuffer(gpu_device, transfer_buf);

    //ITERATE
    while (true) {

        SDL_GPUCommandBuffer* comm_buf = SDL_AcquireGPUCommandBuffer(gpu_device);

        SDL_GPUCopyPass* gpu_copy_pass = SDL_BeginGPUCopyPass(comm_buf);
    
        SDL_GPUTransferBufferLocation tr_buf_loc {
            .transfer_buffer = transfer_buf,
            .offset = 0,
        };
        SDL_GPUBufferRegion buf_region {
            .buffer = vertex_buf,
            .offset = 0,
            .size = sizeof(vertices),
        };
        SDL_UploadToGPUBuffer(gpu_copy_pass, &tr_buf_loc, &buf_region, true);

        SDL_EndGPUCopyPass(gpu_copy_pass);
    
        SDL_GPUTexture* s_texture; //swapchain texture
        Uint32 s_width, s_height; //swapchain width/height
        SDL_WaitAndAcquireGPUSwapchainTexture(comm_buf, window, &s_texture, &s_width, &s_height);

        if (s_texture == NULL) {
            SDL_SubmitGPUCommandBuffer(comm_buf); //must always submit the command buffer
            continue;
        }

        SDL_GPUColorTargetInfo colorTargetInfo {
            .texture = s_texture,
            .clear_color = {255/255.0f, 219/255.0f, 187/255.0f, 255/255.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
        };

        SDL_GPURenderPass* gpu_render_pass = SDL_BeginGPURenderPass(comm_buf, &colorTargetInfo, 1, NULL);
        
        SDL_BindGPUGraphicsPipeline(gpu_render_pass, graphicsPipeline);

        SDL_GPUBufferBinding bufferBindings[1];
        bufferBindings[0] = {
            .buffer = vertex_buf,
            .offset = 0,
        };
        SDL_BindGPUVertexBuffers(gpu_render_pass, 0, bufferBindings, 1);

        SDL_DrawGPUPrimitives(gpu_render_pass, 3, 1, 0, 0);

        SDL_EndGPURenderPass(gpu_render_pass);
        SDL_SubmitGPUCommandBuffer(comm_buf);

        if (SDL_Event e; SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) break;
            if (e.key.key == SDLK_Z) break;
        }
    }

    //QUIT
    SDL_ReleaseGPUBuffer(gpu_device, vertex_buf);
    SDL_ReleaseGPUTransferBuffer(gpu_device, transfer_buf);

    SDL_ReleaseGPUGraphicsPipeline(gpu_device, graphicsPipeline);

    SDL_DestroyGPUDevice(gpu_device);
    SDL_DestroyWindow(window);
    return 0;
}
