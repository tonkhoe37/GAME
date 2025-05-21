all: clean build

build:
	g++ main.cpp \
    src/Button.cpp \
    src/Effect.cpp \
    src/Enemy.cpp \
    src/Entity.cpp \
    src/Game.cpp \
    src/Player.cpp \
    src/ManagerRender.cpp \
    src/ManagerResour.cpp \
    src/common.cpp \
    src/GameUI.cpp \
    -Iinclude \
    -Iinclude/header \
    -Iinclude/SDL2 \
    -Llib \
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    -o main.exe

run:
	./main.exe

clean:
	del main.exe
