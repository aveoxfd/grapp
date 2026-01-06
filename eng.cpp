#include "wnd.h"
#include "camera.h"
#include <iostream>

//smart edges finder

Camera *global_camera = nullptr;
static int hold = 0;
#define _W_HOLD 0b000000000000000001
#define _A_HOLD 0b000000000000000010
#define _S_HOLD 0b000000000000000100
#define _D_HOLD 0b000000000000001000
#define _I_HOLD 0b000000000000010000
#define _K_HOLD 0b000000000000100000
#define _J_HOLD 0b000000000001000000
#define _L_HOLD 0b000000000010000000
#define _U_HOLD 0b000000000100000000
#define _O_HOLD 0b000000001000000000

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
    position p[] = {
        {-1, -1, -1},
        {1, -1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {0, 1, 0}
    };
    int p_edges[][2] = {
        {0, 1},
        {2, 3},
        {3, 1},
        {2, 0},
        {1, 4},
        {0, 4},
        {2, 4},
        {3, 4},
    };

    position flr[] = {
        {-pow(10, 5), (double)0, pow(10, 5)},
        {pow(10, 5), (double)0, pow(10, 5)},
        {pow(10, 5), (double)0, -pow(10, 5)},
        {-pow(10, 5), (double)0, -pow(10, 5)},
    };
    Object cube((position){0, 0, 10}, cb, cb_edges, 1);
    Object cube2({3, 0, 10}, cb, cb_edges, 1);
    Object floor({0,0,0}, flr, 1);
    Object pr({0, 0, 0}, p, p_edges, 1);
    Wnd window1(1080, 720);
    window1.Set_name("3DTest");
    Camera camera((position){0, 0, 0}, window1.getw(), window1.geth(), &window1);

    global_camera = &camera;

    window1.WndSetKeyCallback([](Window* wnd, int key, int pressed){
        if (key == 'A') {
            if (pressed) hold |= _A_HOLD;
            else hold &= ~_A_HOLD;
        }
        else if (key == 'D') {
            if (pressed) hold |= _D_HOLD;
            else hold &= ~_D_HOLD;
        }
        else if (key == 'W') {
            if (pressed) hold |= _W_HOLD;
            else hold &= ~_W_HOLD;
        }
        else if (key == 'S') {
            if (pressed) hold |= _S_HOLD;
            else hold &= ~_S_HOLD;
        }
        if(key == 'I'){
            if(pressed) hold |= _I_HOLD;
            else hold &= ~_I_HOLD;
        }
        if(key == 'K'){
            if(pressed) hold |= _K_HOLD;
            else hold &= ~_K_HOLD;
        }
        if(key == 'J'){
            if(pressed) hold |= _J_HOLD;
            else hold &= ~_J_HOLD;
        }
        if(key == 'L'){
            if(pressed) hold |= _L_HOLD;
            else hold &= ~_L_HOLD;
        }
        if(key == 'O'){
            if(pressed) hold |= _O_HOLD;
            else hold &= ~_O_HOLD;
        }
        if(key == 'U'){
            if(pressed) hold |= _U_HOLD;
            else hold &= ~_U_HOLD;
        }
        return;
    });

    camera.add_render_funtion(
        [](const Camera& cam, const Object& obj) -> void{
            if(!obj.get_edges())return;
            
            for(int i = 0; i<obj.get_edge_count(); i++){
                position start = obj.get_real_position(obj.get_edges()[i][0]);
                position end = obj.get_real_position(obj.get_edges()[i][1]);
                double diff_x = end.x - start.x;
                double diff_y = end.y - start.y;
                double diff_z = end.z - start.z;
                
                double length = sqrt(diff_x*diff_x + diff_y*diff_y + diff_z*diff_z);
                if(length < 0.001) continue;
                int steps = (int)(length * cam.get_render_scale()); //how much steps
                if(steps < 2) steps = 2;
                
                for(int j = 0; j <= steps; j++){
                    double t = (double)j / steps;

                    double ix = start.x + diff_x * t;
                    double iy = start.y + diff_y * t;
                    double iz = start.z + diff_z * t;
                    
                    pixel pix;
                    pix.pos.x = ix;
                    pix.pos.y = iy;
                    pix.pos.z = iz;
                    
                    cam.draw_pixel(pix, 0xffffffff);
                }
            }
        }
    );
    while(WindowProcessMessage()){

        if (hold & _A_HOLD) {
            global_camera->move({0, 0, -0.1});
        }
        if (hold & _D_HOLD) {
            global_camera->move({0, 0, 0.1});
        }
        if (hold & _W_HOLD) {
            global_camera->move({0.1, 0, 0});
        }
        if (hold & _S_HOLD) {
            global_camera->move({-0.1, 0, 0});
        }
        if (hold & _I_HOLD) {
            global_camera->rotate_pitch(-0.01);
        }
        if (hold & _K_HOLD) {
            global_camera->rotate_pitch(0.01);
        }
        if (hold & _J_HOLD) {
            global_camera->rotate_yaw(-0.01);
        }
        if (hold & _L_HOLD) {
            global_camera->rotate_yaw(0.01);
        }
        if (hold & _O_HOLD) {
            global_camera->move(vector_packet{0,-0.1,0});
        }
        if (hold & _U_HOLD) {
            global_camera->move(vector_packet{0,0.1,0});
        }

        window1.Update();

        cube.rotate_yaw(0.01);
        cube2.rotate_pitch(0.01);
        cube2.rotate_yaw(0.01);

        cube.update();
        cube2.update();

        camera.render(cube);
        camera.render(pr);
        camera.render(cube2, 0);

        camera.render(cube, 1);

        camera.update();

        WSleep(16);
    }
    return 0;
}