//
// Created by Paul Baudinot on 16/04/2024.
//
#ifndef DOOMLIKE_CONSTANTE_H
#define DOOMLIKE_CONSTANTE_H
#define PI 3.14159265358979323846
#include <SDL2/SDL.h>

//const int width = 1000;
//const int height= 1000;

const int width = 3024;
const int height= 1964;
const int size_map = 30;
const int rapportPlayerMaps = 8;
const int MiniMap = 5;

const int nb_case_w = 30;
const int nb_case_h = 30;


const int playerWidth = (width / nb_case_w) / rapportPlayerMaps;
const int playerHeight = (height / nb_case_h) / rapportPlayerMaps;

const int vitesse = width/size_map/rapportPlayerMaps/(width/2500);
const int v_angle = 5;
const int fov = 60;
const float precision_angle = 0.02;
const int NbRayon = fov/precision_angle;

const int dfps = 10000;

const int sizeCursor = 20;

const float sensitivity = 0.2;
#endif //DOOMLIKE_CONSTANTE_H
