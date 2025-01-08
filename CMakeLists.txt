cmake_minimum_required(VERSION 3.30)
project(space_invaders C)

set(CMAKE_C_STANDARD 11)

# Crée des variables avec les fichiers à compiler
set(SRCS
        src/vessel.c
        src/input.c
        src/projectile.c
        src/star.c
        src/gui.c
        src/player.c
        src/image.c
        src/gameplay.c
        src/game.c
        src/enemy.c
        src/main.c
        src/global.c
)

set(HEADERS
        inc/globals.h
        inc/vessel.h
        inc/input.h
        inc/projectile.h
        inc/star.h
        inc/gui.h
        inc/player.h
        inc/image.h
        inc/gameplay.h
        inc/game.h
        inc/enemy.h
)

add_executable(space_invaders ${SRCS} ${HEADERS})
