#include "wnd.h"
#include "camera.h"
#include <iostream>

Camera *global_camera = nullptr;

int main(){
    position cb[]= {
        {-0.5, -0.5, -0.5},
        {-0.5, 0.5, -0.5},
        {0.5, -0.5, -0.5},
        {0.5, 0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, 0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, 0.5},
    };
    int cb_edges[][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0},
        {4, 5}, {5, 7}, {7, 6}, {6, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7} 
    };
    Object cube((position){0, 0, 10}, cb, cb_edges, 1);
    Object cube2({3, 0, 10}, cb, cb_edges, 1);
    Wnd window1(1080, 720);
    Camera camera((position){2, 0, 0}, window1.getw(), window1.geth(), &window1);

    global_camera = &camera;

    window1.WndSetKeyCallback([](Window* wnd, int key, int pressed){
        if(pressed && key == 'A'){
            global_camera->move({0, 0, -0.1});;
        }
        if(pressed && key == 'D'){
            global_camera->move({0, 0, 0.1});
        }
        if(pressed && key == 'W'){
            global_camera->move(vector_packet{0.1,0,0});
        }
        if(pressed && key == 'S'){
            global_camera->move({-0.1, 0, 0});;
        }
        if(pressed && key == 'I'){
            global_camera->rotate_pitch(-0.01);
        }
        if(pressed && key == 'K'){
            global_camera->rotate_pitch(0.01);
        }
        if(pressed && key == 'J'){
            global_camera->rotate_yaw(-0.01);
        }
        if(pressed && key == 'L'){
            global_camera->rotate_yaw(0.01);
        }
        if(pressed && key == 'O'){
            global_camera->move(vector_packet{0,-0.1,0});
        }
        if(pressed && key == 'U'){
            global_camera->move(vector_packet{0,0.1,0});
        }
        return;
    });

    while(WindowProcessMessage()){

        window1.Update();
        cube.rotate_yaw(0.01);
        cube2.rotate_pitch(0.01);
        cube2.rotate_yaw(0.01);
        cube.update();
        cube2.update();
        camera.render(cube);
        camera.render(cube2);

        camera.render_shapes(cube);
        camera.render_shapes(cube2);
        Object pixel(cube.get_real_position(0), 1);
        camera.update();

        WSleep(16);
    }
    return 0;
}