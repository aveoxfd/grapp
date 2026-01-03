#ifndef OBJECT_H
#define OBJECT_H

#include "position.h"
#include <math.h>

class Object{
    private:
    position pos;                               //center
    position *edges_positions = nullptr;        //edges
    int **edges = nullptr;
    position *nodes = nullptr;
    int node_count, edge_count;
    int scale;
    double yaw, pitch, roll;
    vector_packet forward = {cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw)}; 
    vector_packet right = {cos(yaw), 0, -sin(yaw)};
    vector_packet up = {-sin(yaw)*sin(pitch), cos(pitch), -cos(yaw)*sin(pitch)};

    public:
    template<unsigned long long N>
    Object(position pos, position(&nodes_array)[N], int scale)
    : pos(pos), scale(scale), node_count(N){
        nodes = new position[N];
        for(int i = 0; i<N; i++){
            nodes[i] = nodes_array[i];
        }
    }
    template<unsigned long long N, unsigned long long E>
    Object(position pos, position(&nodes_array)[N], int(&edges_array)[E][2], int scale)
    : pos(pos), scale(scale), node_count(N), edge_count(E){
        nodes = new position[N];
        for(int i = 0; i<N; i++){
            nodes[i] = nodes_array[i];
        }

        edges = new int*[E];
        for (unsigned long long i = 0; i<E; i++){
            edges[i] = new int[2];
            edges[i][0] = edges_array[i][0];
            edges[i][1] = edges_array[i][1];
        }
        update();
    }
    Object(position pos, int scale):scale(scale), pos(pos), node_count(1){
        nodes = new position[1];
        nodes[0] = {0, 0, 0};
    }

    position* get_nodes(void)const{
        return nodes;
    }
    int** get_edges(void)const{
        return edges;
    }
    void update(){//x, y, z
        forward = {cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw)};
        right = {cos(yaw), 0, -sin(yaw)};
        up = {-sin(yaw)*sin(pitch), cos(pitch), -cos(yaw)*sin(pitch)};  
        return;
    }
    position get_real_position(int num) const{
        if (num < 0 || num >= node_count) {
            return position{0, 0, 0};
        }
        if(nodes)return (position){
            ((nodes[num].x*right.x+nodes[num].y*up.x+nodes[num].z*forward.x)*scale)+pos.x, 
            ((nodes[num].x*right.y+nodes[num].y*up.y+nodes[num].z*forward.y)*scale)+pos.y,
            ((nodes[num].x*right.z+nodes[num].y*up.z+nodes[num].z*forward.z)*scale)+pos.z,
        };
        else return {0, 0, 0};
    }
    void rotate_roll(double dr){
        roll+=dr;
        update();
        return;
    }
    void rotate_yaw(double dy) {
        yaw += dy;
        update();
    }
    void rotate_pitch(double dp) {
        pitch += dp;
        update();
    }
    int get_node_count()const{
        return node_count;
    }
    int get_edge_count()const{
        return edge_count;
    }
    ~Object(){
        delete[] nodes;
        if(edges){
            for (unsigned long long i = 0; i<edge_count; i++){
                delete[] edges[i];
            }
        }
        delete[] edges;
        delete[] edges_positions;
    }
};

#endif