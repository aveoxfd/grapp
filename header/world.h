#ifndef WORLD_H
#define WORLD_H

#include "position.h"
#include "object.h"
#include "camera.h"
#include <iostream>

class World{
    private:
    Camera *camera = nullptr;
    int camera_count = 0;
    Object *object = nullptr;
    int object_count = 0;

    public:
    World(Camera *_camera): camera(_camera){

    }
    ~World(){
        std::cout<<"The world has been destroyed.\n";
    }
    void update(){
        for (int i = 0; i < object_count; i++){
            object[i].update();
            camera->render(object[i], PIPELINE_MODE);
        }
        return;
    }
    void spawn_object(Object &_object){
        if (object == nullptr){
            object = new Object[1];
            object[0] = _object;
            object_count++;
        }
        else{
            Object *temp = new Object[object_count+1];
            for (int i = 0; i < object_count; i++){
                temp[i] = object[i];
            }
            temp[object_count] = _object;
            delete[] object;
            object = temp;
        }
    }
    void spawn_camera(Camera &_camera){
        return;
    }
};

#endif