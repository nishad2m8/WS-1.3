#ifndef _WS_QMI8658_H_
#define _WS_QMI8658_H_
#include <Arduino.h>
#include <Wire.h>
#include "SensorQMI8658.hpp"                    

void QMI8658_Init();
void QMI8658_Loop();

String QMI8658_get_A_x();
String QMI8658_get_A_y();

float QMI8658_get_A_fx();
float QMI8658_get_A_fy();

#endif
