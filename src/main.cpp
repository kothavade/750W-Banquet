#include "main.h"

//ENV Variables:
//ports
int FRONT_LEFT_MOTOR = 15;
int FRONT_RIGHT_MOTOR = 16;
int BACK_LEFT_MOTOR = 17;
int BACK_RIGHT_MOTOR = 18;
int FRONT_CLAW_LEFT = 12;
int FRONT_CLAW_RIGHT = 13;
int BACK_CLAW = 11;
//normal/turbo speeds
int SPEED_SLOW = 75;
int SPEED_FAST = 127;
int SPEED = SPEED_SLOW;
bool turbo = false;

//Object Definitions:
//controller
Controller master(E_CONTROLLER_MASTER);
//drive motors
Motor front_left (FRONT_LEFT_MOTOR, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor back_left (BACK_LEFT_MOTOR, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_right (FRONT_RIGHT_MOTOR, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES); //right motors reversed based on mounting position
Motor back_right (BACK_RIGHT_MOTOR, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
//claw motors
Motor back_claw (BACK_CLAW, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_claw_left (FRONT_CLAW_LEFT, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_claw_right (FRONT_CLAW_RIGHT, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES); //right motor reversed for same reason as above

void initialize() {
	//lcd
	lcd::initialize();
	lcd::set_text(1, "750W Banquet Code");
	lcd::set_text(2, "by: Ved K.");
	lcd::set_text(3, "v1.0");
	lcd::set_text(4, "Currently: Initializing...");
	delay(1000);
}

//not necessary
void disabled() {}

//not necessary
void competition_initialize() {}

void autonomous() {
	lcd::set_text(4, "Currently: In Autonomous Mode.");
	//TODO: Autonomous Code
	//Current plan: Right side, take middle mobile goal back to alliance side
}

//driver control
void opcontrol() {
	lcd::set_text(4, "Currently: In Operator Mode.");
	//this is so that the claw motor holds it position once raised
	back_claw.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	//runs in a loop, polls inputs from controller every 20ms
	while (true) {
		//raw inputs from controller
		int left_y_raw = master.get_analog(ANALOG_LEFT_Y);
		int right_x_raw = master.get_analog(ANALOG_RIGHT_X);
		//use quadratic curve to smooth out inputs
		int left = (0.0085 * pow(left_y_raw,2)) * (left_y_raw / std::abs(left_y_raw));
		int right = (0.0085 * pow(right_x_raw,2)) * (right_x_raw / std::abs(right_x_raw));
		//combine smooth inputs for power/turn
		int left_motors = left + right;
		int right_motors = left - right;
		//move motors
		front_left.move(left_motors);
		back_left.move(left_motors);
		front_right.move(right_motors);
		back_right.move(right_motors);
		//back claw controlled by R1/R2
		if (master.get_digital(DIGITAL_R2)) {
			back_claw.move(SPEED);
		} else if (master.get_digital(DIGITAL_R1)) {
			back_claw.move(-SPEED);
		} else {
			back_claw.move(0);
		}
		//front claw controlled by L1/L2
		if (master.get_digital(DIGITAL_L2)) {
			front_claw_left.move(SPEED);
			front_claw_right.move(SPEED);
		} else if (master.get_digital(DIGITAL_L1)) {
			front_claw_left.move(-SPEED);
			front_claw_right.move(-SPEED);
		} else {
			front_claw_left.move(0);
			front_claw_right.move(0);
		}
		//toggle turbo mode for claw(s) with B
		if (master.get_digital(DIGITAL_B)) {
			if (!turbo) {
				turbo = true;
				SPEED = SPEED_FAST;
			}
			else {
				turbo = false;
				SPEED = SPEED_SLOW;
			}
		}
		delay(20); //polling rate
	}
}
