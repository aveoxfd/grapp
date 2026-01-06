#ifndef CAMERA_H
#define CAMERA_H
#include "position.h"
#include "object.h"
#include "wnd.h"
#include <math.h>
#include "vectorpacket.h"
#include "pixel.h"

class Camera{
    private:
    position pos;
    double yaw = 0, pitch = 0, roll = 0; //alpha, bheta, ghamma 
    double fov = 90 * M_PI / 180.0;
    double scale = 1.0 / tan(fov / 2.0);
    int rander_scale = 4000;
    int max_distant_view = 1000;
    typedef void(*Render_function)(const Camera&, const Object&); //render object funtion type

    Render_function* array_render_functions = nullptr;
    int render_function_count = 0;

    //Render_function render_function;
    vector_packet forward = {cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw)}; //vector of view {x, y, z}; start = +z
    vector_packet right = {cos(yaw), 0, -sin(yaw)};                                 //vector right {1, 0, 0}
    vector_packet up = {-sin(yaw)*sin(pitch), cos(pitch), -cos(yaw)*sin(pitch)};    //vector up {0, 1, 0}

    int width, height;; // pixels

    Wnd *window;

    double speed_rotation = 0.1;

    //for example render function
    static void default_render_function(const Camera& cam, const Object& obj){
        position *nodes = obj.get_nodes();

        for (int i = 0; i<obj.get_node_count(); i++){ //main cycle of rendering
            position real_node_position = obj.get_real_position(i);
            position normalize_pos = (position){real_node_position.x-cam.get_position().x, real_node_position.y-cam.get_position().y, real_node_position.z-cam.get_position().z};
            double delta_z = normalize_pos.x*cam.get_vector_coeff_forward().x + normalize_pos.y*cam.get_vector_coeff_forward().y + normalize_pos.z*cam.get_vector_coeff_forward().z;
            if(delta_z<0.1)continue;
            double delta_x = normalize_pos.x*cam.get_vector_coeff_right().x + normalize_pos.y*cam.get_vector_coeff_right().y + normalize_pos.z*cam.get_vector_coeff_right().z;
            double delta_y = normalize_pos.x*cam.get_vector_coeff_up().x + normalize_pos.y*cam.get_vector_coeff_up().y + normalize_pos.z*cam.get_vector_coeff_up().z;

            //double orig_delta_x = delta_x; rolling
            //double orig_delta_y = delta_y;
            //delta_x = orig_delta_x*cos(cam.get_roll()) + orig_delta_y*sin(cam.get_roll());
            //delta_y = -orig_delta_x*sin(cam.get_roll()) + orig_delta_y*cos(cam.get_roll());

            delta_x = (delta_x/delta_z)*cam.scale*cam.window->getw()+cam.window->getw()/2;
            delta_y = (delta_y/delta_z)*cam.scale*cam.window->geth()+cam.window->geth()/2;

            if(delta_x >= 0 && delta_x < cam.window->getw() && delta_y >= 0 && delta_y < cam.window->geth()){
                if(delta_z < cam.max_distant_view)cam.window->putpixel(delta_x, delta_y, 0xffffffff);
            }
        }
    }
 
    public:
    Camera(position pos, int width, int height, Wnd *window): pos(pos), width(width), height(height), window(window){
        set_render_function(default_render_function);
    }
    Camera(position pos, Wnd *window): pos(pos), width(0), height(0), window(window), fov(90){
        set_render_function(default_render_function);
    };
    ~Camera(){
        window->Destroy();
    }

    void set_window(Wnd window){
        this->window = &window;
        return;
    }

    void set_render_function(Render_function function, int index = -1){
        if (index >= 0 && index < render_function_count){
            array_render_functions[index] = function;
            return;
        }
        if (!array_render_functions){
            array_render_functions = new Render_function[1];
            array_render_functions[0] = function;
            render_function_count = 1;
            return;
        }
    }
    void add_render_funtion(Render_function function){
        Render_function* new_array = new Render_function[render_function_count + 1];
        for (int i = 0; i < render_function_count; i++){
            new_array[i] = array_render_functions[i];
        }
        new_array[render_function_count] = function;
        delete[] array_render_functions;
        array_render_functions = new_array;
        render_function_count++;
        return;
    }
    void delete_render_functions(const int n){
        if (array_render_functions && n >= 0 && n < render_function_count){
            Render_function* new_array = new Render_function[render_function_count - 1];
            for (int i = 0, j = 0; i < render_function_count; i++){
                if (i != n) {
                    new_array[j++] = array_render_functions[i];
                }
            }
            delete[] array_render_functions;
            array_render_functions = new_array;
            render_function_count--;
        }
        return;
    }

    void render(Object &object, int render_mode = 0){
        if(render_mode < render_function_count && array_render_functions[render_mode] != nullptr){
            array_render_functions[render_mode](*this, object);
        }
        return;
    }

    position get_position(void)const{
        return pos;
    }
    void update(){
        forward = {cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw)};
        right = {cos(yaw), 0, -sin(yaw)};
        up = {-sin(yaw)*sin(pitch), cos(pitch), -cos(yaw)*sin(pitch)};
        return;
    }
    vector_packet get_vector_coeff_forward()const{
        return forward;
    }
    vector_packet get_vector_coeff_right()const{
        return right;
    }
    vector_packet get_vector_coeff_up()const{
        return up;
    }
    double get_roll()const{
        return roll;
    }
    double get_fov()const{
        return fov;
    }
    void rotate_roll(double dr){
        roll+=dr;
        return;
    }
    void rotate_yaw(double dy){
        yaw+=dy;
        return;
    }
    void rotate_pitch(double dp){
        pitch+=dp;
        return;
    }
    void move(vector_packet vector){ //{dx, dy, dz}
        pos.x+=forward.x*vector.x+up.x*vector.y+right.x*vector.z; //vector x
        pos.y+=forward.y*vector.x+up.y*vector.y+right.y*vector.z; //vector y
        pos.z+=forward.z*vector.x+up.z*vector.y+right.z*vector.z; //vector z
        return;
    }
    void set_fov(int new_fov){
        fov=new_fov;
        return;
    }
    void draw_pixel(pixel pix, int color)const{
        position normalized_pos = (position){pix.pos.x - pos.x, pix.pos.y - pos.y, pix.pos.z - pos.z};
        double delta_z = normalized_pos.x*forward.x + normalized_pos.y*forward.y + normalized_pos.z*forward.z;
        if (delta_z<0.01)return;
        double delta_x = normalized_pos.x*right.x + normalized_pos.y*right.y + normalized_pos.z*right.z;
        double delta_y = normalized_pos.x*up.x + normalized_pos.y*up.y + normalized_pos.z*up.z;

        delta_x = (delta_x/delta_z) * scale * window->getw() + window->getw()/2;
        delta_y = (delta_y/delta_z) * scale * window->geth() + window->geth()/2;

        if (delta_x >= 0 && delta_x <= window->getw() && delta_y >= 0 && delta_y <= window->geth()){
            if(delta_z < max_distant_view)window->putpixel(delta_x, delta_y, color);
        }
        return;
    }
    Wnd* get_window(){
        return window;
    }
    int get_render_scale()const{
        return rander_scale;
    }
    int get_max_distant_view()const{
        return max_distant_view;
    }
};

#endif