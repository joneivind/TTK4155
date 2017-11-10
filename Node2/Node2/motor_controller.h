/*
 * motor_controller.h
 *
 * Created: 10.11.2017 17:59:44
 *  Author: jonestr
 */ 


#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_

void pid_init(float _kp, float _ki, float _kd);
float pid_generate(float r, float y, float dt);
uint16_t pid_find_max_encoder_value(void);


#endif /* MOTOR_CONTROLLER_H_ */