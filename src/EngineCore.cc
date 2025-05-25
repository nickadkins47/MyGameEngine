
#include "EngineCore.hh"

namespace MGE {

    EngineCore::EngineCore() {
        print("{}: Starting Up\n", engName);
        if (!init_sdl()) {
            print("{}\n", SDL_GetError());
            shutdown = true;
        }

        cam.kbdPtr = &kbd;
        cam.setProjMat(fov, windowWidth, windowHeight, zNear, zFar);
    }

    EngineCore::~EngineCore() {
        print("{}: Shutting Down\n", engName);
        quit_sdl();
    }

    void EngineCore::run() {
        while (!shutdown) update();
    }

    void EngineCore::update() {

        //handle any inputs from either mouse or keyboard
        if (SDL_Event e; SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    shutdown = true; return;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    SDL_SetWindowRelativeMouseMode(window, true); break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    SDL_SetWindowRelativeMouseMode(window, false); break;
                case SDL_EVENT_MOUSE_MOTION:
                    if (SDL_GetWindowRelativeMouseMode(window)) {
                        cam.updateCamAngle(e.motion.xrel, e.motion.yrel);
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    kbd.keyDown(e.key.key); break;
                case SDL_EVENT_KEY_UP:
                    kbd.keyUp(e.key.key); break;
                default: break;
            }
        }

        cam.update();

        render();
    }

    void EngineCore::add_obj(Obj const& o) {
        obj_list.push_back(o);
    }

    void EngineCore::add_objs(Vector<Obj> const& o_vec) {
        obj_list.append_range(o_vec);
    }

    void EngineCore::asdf() {
        vertices[0].x = 0.25f;
    }

    void EngineCore::render() {
        /* Mat<4,4> const vpMat = cam.getVPMat();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (Obj const& o : obj_list) {
            Vector<Triangle<4>> newMesh {};

            for (Triangle<3> const& tri : o.mesh) {
                Triangle<4> newTri {Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}};
                bool withinClippingVolume = true;
                for (Int i = 0; i < tri.size(); i++) {
                    newTri[i] = vpMat * Vec<4>{ tri[i][0], tri[i][1], tri[i][2], 1.0f };
                    if (newTri[i][2] < zNear) {
                        withinClippingVolume = false;
                        break;
                    }
                    if (newTri[i][3] != 0.0f) { //if (std::fabs(newTri[i][3]) >= 0.0625f) {
                        newTri[i][0] /= newTri[i][3];
                        newTri[i][1] /= newTri[i][3];
                    }
                }
                if (withinClippingVolume) newMesh.push_back(newTri);
            }

            //TODO: implement proper clipping: https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html

            for (Int j = 0; j < newMesh.size(); j++) {
                Triangle<4>& newTri = newMesh[j];

                SDL_FPoint fpoints[] = {
                    {(newTri[0][0] + 1.0f) * (windowWidth/2), (newTri[0][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[1][0] + 1.0f) * (windowWidth/2), (newTri[1][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[2][0] + 1.0f) * (windowWidth/2), (newTri[2][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[0][0] + 1.0f) * (windowWidth/2), (newTri[0][1] + 1.0f) * (windowHeight/2)},
                };
                SDL_RenderLines(renderer, fpoints, 4);

                //print("Z's: {}, {}, {}\n", newTri[0][2], newTri[1][2], newTri[2][2]);

                SDL_Vertex vertices[] = {
                    //{fpoints[0], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    //{fpoints[1], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    //{fpoints[2], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    {fpoints[0], {0.7f, 0.3f, 0.3f, 1.0f}, {0,0}},
                    {fpoints[1], {0.3f, 0.7f, 0.3f, 1.0f}, {0,0}},
                    {fpoints[2], {0.3f, 0.3f, 0.7f, 1.0f}, {0,0}},
                };
                SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
                
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
        */

        Vertex* data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(gpu_device, transfer_buf, false));
        if (data == NULL) {
            print("ERROR: Mapping Transfer Buffer Failed.\n"); return;
        }
        for (int i = 0; i < vertices_len; i++) {
            data[i] = vertices[i];
        }
        SDL_UnmapGPUTransferBuffer(gpu_device, transfer_buf);

        SDL_GPUCommandBuffer* comm_buf = SDL_AcquireGPUCommandBuffer(gpu_device);  //check if fail

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
        SDL_UploadToGPUBuffer(gpu_copy_pass, &tr_buf_loc, &buf_region, false);

        SDL_EndGPUCopyPass(gpu_copy_pass);
    
        SDL_GPUTexture* s_texture; //swapchain texture
        Uint32 s_width, s_height; //swapchain width/height
        SDL_WaitAndAcquireGPUSwapchainTexture(comm_buf, window, &s_texture, &s_width, &s_height);  //check if fail
        if (s_texture == NULL) {
            SDL_SubmitGPUCommandBuffer(comm_buf); //must always submit the command buffer  //check if fail
            return;
        }

        SDL_GPUColorTargetInfo colorTargetInfo {
            .texture = s_texture,
            .clear_color = {0.0f, 0.0f, 0.0f, 0.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
        };

        /* SDL_GPUDepthStencilTargetInfo* asdf = {

        }; */

        SDL_GPURenderPass* gpu_render_pass = SDL_BeginGPURenderPass(comm_buf, &colorTargetInfo, 1, NULL);
        
        SDL_BindGPUGraphicsPipeline(gpu_render_pass, graphics_pipeline);

        SDL_GPUBufferBinding bufferBindings[1] {
            {
                .buffer = vertex_buf,
                .offset = 0,
            }
        };
        SDL_BindGPUVertexBuffers(gpu_render_pass, 0, bufferBindings, 1);

        timeUniform.time = SDL_GetTicksNS() / 1e9f; // sent the time since the app started in seconds
        SDL_PushGPUFragmentUniformData(comm_buf, 0, &timeUniform, sizeof(UniformBuffer));

        SDL_DrawGPUPrimitives(gpu_render_pass, vertices_len, 1, 0, 0);

        SDL_EndGPURenderPass(gpu_render_pass);
        SDL_SubmitGPUCommandBuffer(comm_buf); //check if fail

    }

    /* Vec<3> EngineCore::mapVertex(Vec<3> vertex) {
        //TODO
    } */

    bool EngineCore::init_sdl() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(engName.data(), windowWidth, windowHeight, NULL);
        if (window == NULL) return false;
        gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
        if (gpu_device == NULL) return false;
        if (!SDL_ClaimWindowForGPUDevice(gpu_device, window)) return false;

        vert_shader = get_shader("build/shaders/Vertex.vert.spv", {
            .entrypoint = "main",
            .stage = SDL_GPU_SHADERSTAGE_VERTEX,
            .num_samplers = 0,
            .num_storage_textures = 0,
            .num_storage_buffers = 0,
            .num_uniform_buffers = 0,
        });
        if (vert_shader == NULL) return false;

        frag_shader = get_shader("build/shaders/Fragment.frag.spv", {
            .entrypoint = "main",
            .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
            .num_samplers = 0,
            .num_storage_textures = 0,
            .num_storage_buffers = 0,
            .num_uniform_buffers = 1,
        });
        if (frag_shader == NULL) return false;

        SDL_GPUVertexBufferDescription vertexBufferDesriptions[1] {
            {
                .slot = 0,
                .pitch = sizeof(Vertex),
                .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0,
            }
        };

        SDL_GPUVertexAttribute vertexAttributes[2] {
            {
                .location = 0,
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset = 0,
            }, {
                .location = 1,
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                .offset = sizeof(float) * 3,
            }
        };

        SDL_GPUColorTargetDescription colorTargetDescriptions[1] {
            {
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
            }
        };

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {
            .vertex_shader = vert_shader,
            .fragment_shader = frag_shader,
            .vertex_input_state = {
                .vertex_buffer_descriptions = vertexBufferDesriptions,
                .num_vertex_buffers = 1,
                .vertex_attributes = vertexAttributes,
                .num_vertex_attributes = 2,
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .target_info = {
                .color_target_descriptions = colorTargetDescriptions,
                .num_color_targets = 1,
            }
        };
        graphics_pipeline = SDL_CreateGPUGraphicsPipeline(gpu_device, &pipelineInfo);
        if (graphics_pipeline == NULL) return false;

        SDL_GPUBufferCreateInfo buffer_info {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = sizeof(vertices),
        };
        vertex_buf = SDL_CreateGPUBuffer(gpu_device, &buffer_info);
        if (vertex_buf == NULL) return false;

        SDL_GPUTransferBufferCreateInfo transfer_info {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = sizeof(vertices),
        };
        transfer_buf = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);
        if (transfer_buf == NULL) return false;

        return true;
    }
    
    void EngineCore::quit_sdl() {
        SDL_ReleaseGPUShader(gpu_device, vert_shader);
        SDL_ReleaseGPUShader(gpu_device, frag_shader);
        ifntn (transfer_buf)      SDL_ReleaseGPUTransferBuffer  (gpu_device, transfer_buf);
        ifntn (vertex_buf)        SDL_ReleaseGPUBuffer          (gpu_device, vertex_buf);
        ifntn (graphics_pipeline) SDL_ReleaseGPUGraphicsPipeline(gpu_device, graphics_pipeline);
        ifntn (gpu_device)        SDL_DestroyGPUDevice          (gpu_device);
        ifntn (window)            SDL_DestroyWindow             (window);
        SDL_Quit();
    }

    SDL_GPUShader* EngineCore::get_shader(String const& path, SDL_GPUShaderCreateInfo info) {
        auto [code, code_size] = MGE::load_file_as_code(path);
        if (code_size == 0) return NULL;
        info.code = (Uint8*)code;
        info.code_size = code_size;
        info.format = shaderFmt;
        SDL_GPUShader* shader = SDL_CreateGPUShader(gpu_device, &info);  //check if fail?
        SDL_free(code);
        return shader;
    }

}