﻿# CMakeList.txt : projet CMake pour asterisk, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (asterisk "asterisk.cpp" "asterisk.h")

# TODO: Ajoutez des tests et installez des cibles si nécessaire.