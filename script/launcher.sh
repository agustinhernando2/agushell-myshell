#!/bin/bash

if [ "$1" = "debug" ]; then
    # Debug ex
    gnome-terminal --title="gushgush - gdb" -- bash -c "gdb ./build/gushgush-myshell -ex 'br main.c:36' -ex 'br external.c:43' -ex 'run'; exec bash" &
elif [ "$1" = "valgrind" ]; then
    # Valgrind ex
    gnome-terminal --title="gushgush - valgrind" -- bash -c "valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/gushgush-myshell; exec bash" &
else
    # Normal ex
    gnome-terminal --title="gushgush" -- ./build/gushgush-myshell
fi
