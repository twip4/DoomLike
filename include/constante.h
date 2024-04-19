//
// Created by Paul Baudinot on 16/04/2024.
//
#ifndef DOOMLIKE_CONSTANTE_H
#define DOOMLIKE_CONSTANTE_H
#define PI 3.14159265358979323846

const int width = 1000;
const int height = 1000;
const int size_map = 20;
const int rapportPlayerMaps = 8;
const int MiniMap = 5;

const int nb_case_w = 20;
const int nb_case_h = 20;

const int playerWidth = (width / nb_case_w) / rapportPlayerMaps;
const int playerHeight = (height / nb_case_h) / rapportPlayerMaps;

const int vitesse = width/size_map/rapportPlayerMaps;
const int v_angle = 5;
const int fov = 60;
const float precision_angle = 0.06;
const int NbRayon = fov/precision_angle;

const int fps = 120;

const int sizeCursoe = 20;
#endif //DOOMLIKE_CONSTANTE_H
