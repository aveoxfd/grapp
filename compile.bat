curl -L -o lib/libwinwindow.a "https://github.com/aveoxfd/winwindow/raw/main/libwinwindow.a"

g++ -O2  -DW_STATIC -static -static-libgcc -static-libstdc++ -o bin/grapp.exe eng.cpp wnd.cpp -Iheader -Llib -lwinwindow -lopengl32 -lgdi32 -luser32