#ifndef WND_H
#define WND_H

#include "window.h"
class Wnd{
    private:
    Window* window;
    void Create(int, int);
    
    public:
    Wnd(const int w, const int h);
    void Clear(unsigned int);
    void putpixel(int x, int y, unsigned int color);
    void Update(void);
    void Sleep(int);
    unsigned int argb(unsigned char, unsigned char, unsigned char, unsigned char);
    void Destroy(void);
    int getw(void);
    int geth(void);
    void WndSetKeyCallback(void(*func)(Window*, int, int));
    void WndSetMouseButtonCallback(void(*func)(Window*, int, int));
    void WndSetMouseMoveCallback(void(*func)(Window*, int, int));

    ~Wnd();
};
#endif