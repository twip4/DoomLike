//
// Created by Paul Baudinot on 16/04/2024.
//
#ifndef DOOMLIKE_CONSTANTE_H
#define DOOMLIKE_CONSTANTE_H
#define PI 3.14159265358979323846
#include <SDL2/SDL.h>

extern int width;
extern int height;

const int size_map = 30;
const int rapportPlayerMaps = 8;
const int MiniMap = 5;


const int nb_case_w = 30;
const int nb_case_h = 30;


const int playerWidth = (width / nb_case_w) / rapportPlayerMaps;
const int playerHeight = (height / nb_case_h) / rapportPlayerMaps;

const int vitesse = size_map/rapportPlayerMaps * 5;
const int fov = 60;
const float precision_angle = 0.03333333333;
const int NbRayon = fov/precision_angle;

const int sizeCursor = 10;


const float sensitivity = 0.1;
#endif //DOOMLIKE_CONSTANTE_H
