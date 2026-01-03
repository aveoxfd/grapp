#include "header/wnd.h"

Wnd::Wnd(const int w, const int h){
    Create(w, h);
}
void Wnd::putpixel(int x, int y, unsigned int color){
    PutPixel(window, x, y, color);
}
void Wnd::Create(int w, int h){
    window = WindowCreate(w, h);
}
//int Wnd::WndProcessMessage(void){
//    return WindowProcessMessage();
//}
void Wnd::Update(){
    ClearWindow(window, 0);
    WindowUpdate(window);
}
void Wnd::Sleep(int t){
    WSleep(t);
}
unsigned int Wnd::argb(unsigned char a, unsigned char r, unsigned char g, unsigned char b){
    return ARGB(a, r, g, b);
}
void Wnd::Destroy(){
    WindowDestroy(window);
}
Wnd::~Wnd(){
    Destroy();
}
int Wnd::getw(){
    return get_width(window);
}
int Wnd::geth(){
    return get_height(window);
}
void Wnd::WndSetKeyCallback(void(*func)(Window*, int, int)){
    WindowSetKeyCallback(window, func);
}
void Wnd::WndSetMouseButtonCallback(void(*func)(Window*, int, int)){
    WindowSetMouseButtonCallback(window, func);
}
void Wnd::WndSetMouseMoveCallback(void(*func)(Window*, int, int)){
    WindowSetMouseMoveCallback(window, func);
}