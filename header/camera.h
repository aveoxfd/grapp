#ifndef CAMERA_H
#define CAMERA_H
#include "position.h"
#include "object.h"
#include "wnd.h"
#include <math.h>
#include "vectorpacket.h"

class Camera{
    private:
    position pos;
    double yaw = 0, pitch = 0, roll = 0; //alpha, bheta, ghamma 
    double fov = 90;
    typedef void(*Render_function)(const Camera&, const Object&);

    Render_function render_function;
    vector_packet forward = {cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw)}; //vector of view {x, y, z}; start = +z
    vector_packet right = {cos(yaw), 0, -sin(yaw)};                                 //vector right {1, 0, 0}
    vector_packet up = {-sin(yaw)*sin(pitch), cos(pitch), -cos(yaw)*sin(pitch)};    //vector up {0, 1, 0}

    int width, height;; // pixels

    Wnd *window;

    double speed_rotation = 0.1;

    static void default_render_function(const Camera& cam, const Object& obj){
        position *nodes = obj.get_nodes();
        double fov = (double)cam.get_fov() * M_PI / 180.0;   //!!!
        double scale = 1.0 / tan(fov / 2.0);                 //!!!

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

            delta_x = (delta_x/delta_z)*scale*cam.window->getw()+cam.window->getw()/2;
            delta_y = (delta_y/delta_z)*scale*cam.window->geth()+cam.window->geth()/2;

            if(delta_x >= 0 && delta_x < cam.window->getw() && delta_y >= 0 && delta_y < cam.window->geth()){
                cam.window->putpixel(delta_x, delta_y, 0xffffffff);
            }
        }
    }
 
    public:
    void render_shapes(const Object& object){
        if(!object.get_edges())return;

        for(int i = 0; i<object.get_edge_count(); i++){
            position start = object.get_real_position(object.get_edges()[i][0]);
            position end = object.get_real_position(object.get_edges()[i][1]);
            double diff_x = end.x - start.x;
            double diff_y = end.y - start.y;
            double diff_z = end.z - start.z;

            double length = sqrt(diff_x*diff_x + diff_y*diff_y + diff_z*diff_z);
            if(length < 0.001) continue;
            int steps = (int)(length * 2000); //how much steps
            if(steps < 2) steps = 2;

            for(int j = 0; j <= steps; j++){
                double t = (double)j / steps;
                
                double ix = start.x + diff_x * t;
                double iy = start.y + diff_y * t;
                double iz = start.z + diff_z * t;
                
                Object pixel((position){ix, iy, iz}, 1);
                
                this->render_object(pixel);
            }
        }
    }
    Camera(position pos, int width, int height, Wnd *window): pos(pos), width(width), height(height), window(window), render_function(nullptr){
        set_render_function(default_render_function);
    }
    Camera(position pos, Wnd *window): pos(pos), width(0), height(0), window(window), render_function(nullptr), fov(90){
        render_function = default_render_function;
    };
    ~Camera(){
    }

    void set_window(Wnd window){
        this->window = &window;
        return;
    }

    void set_render_function(Render_function function){
        render_function = function;
        return;
    }

    void render_object(Object &object){
        render_function(*this, object);
        return;
    }
    void support_render(void){
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
};

#endif