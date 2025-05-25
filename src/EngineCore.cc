
#include "EngineCore.hh"

namespace MGE {

    BufInfo::BufInfo() {}
    BufInfo::~BufInfo() {}

    EngineCore::EngineCore() {
        print("{}: Starting Up\n", eng_name);
        if (!init_sdl()) {
            print("{}\n", SDL_GetError());
            shutdown = true;
        }

        cam.kbdPtr = &kbd;
        cam.setProjMat(fov, window_width, window_height, near_z, far_z);
    }

    EngineCore::~EngineCore() {
        print("{}: Shutting Down\n", eng_name);
        quit_sdl();
    }

    void EngineCore::run() {
        calc_graphics_pipeline();
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
        obj_list.push_back(make_pair(o, get_buf_info(o)));
        //update graphics_pipeline
    }

    void EngineCore::add_objs(Vector<Obj> const& o_vec) {
        for (Obj const& o : o_vec) {
            obj_list.push_back(make_pair(o, get_buf_info(o)));
        }
        //calc_graphics_pipeline();
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
                    if (newTri[i][2] < near_z) {
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
                    {(newTri[0][0] + 1.0f) * (window_width/2), (newTri[0][1] + 1.0f) * (window_height/2)},
                    {(newTri[1][0] + 1.0f) * (window_width/2), (newTri[1][1] + 1.0f) * (window_height/2)},
                    {(newTri[2][0] + 1.0f) * (window_width/2), (newTri[2][1] + 1.0f) * (window_height/2)},
                    {(newTri[0][0] + 1.0f) * (window_width/2), (newTri[0][1] + 1.0f) * (window_height/2)},
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

        ifntn (graphics_pipeline) {} else return;

        Mat<4,4> const vpMat = cam.getVPMat();

        SDL_GPUCommandBuffer* comm_buf = SDL_AcquireGPUCommandBuffer(gpu_device);  //check if fail

        SDL_GPUCopyPass* gpu_copy_pass = SDL_BeginGPUCopyPass(comm_buf);

        for (auto & [obj, buf_info] : obj_list) {
            Vertex* data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(gpu_device, buf_info.transfer_buf, false));
            if (data == NULL) {
                print("ERROR: Mapping Transfer Buffer Failed.\n"); return;
            }
            Size index = obj.mesh.size()-1, num_outside = 0;
            for (Triangle<3> const& triangle : obj.mesh) {
                for (Vec<3> const& point : triangle) {
                    Vec<4> screenPoint = vpMat * Vec<4>{ point.x, point.y, point.z, 1.0f };
                    if (screenPoint.z < near_z) {
                        num_outside++;
                        break; //triangle is outside clipping volume
                    }
                    if (screenPoint.w != 0.0f) { //if (std::fabs(screenPoint.w) >= 0.0625f) {
                        screenPoint.x /= screenPoint.w;
                        screenPoint.y /= screenPoint.w;
                    }

                    data[index] = Vertex {
                        screenPoint.x, screenPoint.y, screenPoint.z,
                        1.0f, 1.0f, 1.0f, 1.0f,
                    };
                    index--;
                }
            }
            SDL_UnmapGPUTransferBuffer(gpu_device, buf_info.transfer_buf);

            SDL_GPUTransferBufferLocation tr_buf_loc {
                .transfer_buffer = buf_info.transfer_buf,
                .offset = 0,
            };
            SDL_GPUBufferRegion buf_region {
                .buffer = buf_info.vertex_buf,
                .offset = 0,
                .size = buf_info.size,
            };
            SDL_UploadToGPUBuffer(gpu_copy_pass, &tr_buf_loc, &buf_region, false);
        }
        
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
            .clear_color = {0.0f, 0.0f, 0.0f, 1.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
        };

        /* SDL_GPUDepthStencilTargetInfo* asdf = {

        }; */

        SDL_GPURenderPass* gpu_render_pass = SDL_BeginGPURenderPass(comm_buf, &colorTargetInfo, 1, NULL);
        
        SDL_BindGPUGraphicsPipeline(gpu_render_pass, graphics_pipeline);

        Uint num_triangles = 0;
        Vector<SDL_GPUBufferBinding> buffer_bindings;

        for (auto const& [obj, buf_info] : obj_list) {
            num_triangles += obj.mesh.size();
            buffer_bindings.push_back(SDL_GPUBufferBinding{
                .buffer = buf_info.vertex_buf,
                .offset = 0,
            });
        }
        SDL_BindGPUVertexBuffers(gpu_render_pass, 0, buffer_bindings.data(), buffer_bindings.size());

        timeUniform.time = SDL_GetTicksNS() / 1e9f; // sent the time since the app started in seconds
        SDL_PushGPUFragmentUniformData(comm_buf, 0, &timeUniform, sizeof(UniformBuffer));

        SDL_DrawGPUPrimitives(gpu_render_pass, num_triangles * 3, 1, 0, 0);

        SDL_EndGPURenderPass(gpu_render_pass);
        SDL_SubmitGPUCommandBuffer(comm_buf); //check if fail

    }

    /* Vec<3> EngineCore::mapVertex(Vec<3> vertex) {
        //TODO
    } */

    bool EngineCore::init_sdl() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(eng_name.data(), window_width, window_height, NULL);
        if (window == NULL) return false;
        gpu_device = SDL_CreateGPUDevice(shader_fmt, true, NULL);
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

        return true;
    }
    
    void EngineCore::quit_sdl() {
        SDL_ReleaseGPUShader(gpu_device, vert_shader);
        SDL_ReleaseGPUShader(gpu_device, frag_shader);

        for (auto & [_, buf_info] : obj_list) {
            ifntn (buf_info.transfer_buf) SDL_ReleaseGPUTransferBuffer (gpu_device, buf_info.transfer_buf);
            ifntn (buf_info.vertex_buf)   SDL_ReleaseGPUBuffer         (gpu_device, buf_info.vertex_buf);
        }
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
        info.format = shader_fmt;
        SDL_GPUShader* shader = SDL_CreateGPUShader(gpu_device, &info);  //check if fail?
        SDL_free(code);
        return shader;
    }

    BufInfo EngineCore::get_buf_info(Obj const& o) {
        BufInfo buf_info;

        //7 = # of members of a Vertex (x,y,z,r,g,b,a)
        buf_info.size = sizeof(Float) * 7 * o.mesh.size();

        SDL_GPUBufferCreateInfo buffer_info {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = buf_info.size,
        };
        buf_info.vertex_buf = SDL_CreateGPUBuffer(gpu_device, &buffer_info);
        if (buf_info.vertex_buf == NULL) return BufInfo{};

        SDL_GPUTransferBufferCreateInfo transfer_info {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = buf_info.size,
        };
        buf_info.transfer_buf = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);
        if (buf_info.transfer_buf == NULL) return BufInfo{};

        return buf_info;
    }

    void EngineCore::calc_graphics_pipeline() {
        ifntn (graphics_pipeline) SDL_ReleaseGPUGraphicsPipeline(gpu_device, graphics_pipeline);

        Vector<SDL_GPUVertexBufferDescription> vert_buf_descs;
        Vector<SDL_GPUVertexAttribute> vert_attributes;

        for (Uint i = 0; i < obj_list.size(); i++) {
            vert_buf_descs.push_back(SDL_GPUVertexBufferDescription{
                .slot = i,
                .pitch = sizeof(Vertex),
                .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0,
            });
            vert_attributes.push_back(SDL_GPUVertexAttribute{
                .location = 0,
                .buffer_slot = i,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset = 0,
            });
            vert_attributes.push_back(SDL_GPUVertexAttribute{
                .location = 1,
                .buffer_slot = i,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                .offset = sizeof(Float) * 3,
            });
        }

        SDL_GPUColorTargetDescription color_target_desc {
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

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {
            .vertex_shader = vert_shader,
            .fragment_shader = frag_shader,
            .vertex_input_state = {
                .vertex_buffer_descriptions = vert_buf_descs.data(),
                .num_vertex_buffers = static_cast<Uint>(vert_buf_descs.size()),
                .vertex_attributes = vert_attributes.data(),
                .num_vertex_attributes = static_cast<Uint>(vert_attributes.size()),
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .target_info = {
                .color_target_descriptions = &color_target_desc,
                .num_color_targets = 1,
            }
        };
        if (!obj_list.empty()) {
            graphics_pipeline = SDL_CreateGPUGraphicsPipeline(gpu_device, &pipelineInfo);
            if (graphics_pipeline == NULL) return; //raise error?
        }
    }

}