#include "main.h"

Controller master(E_CONTROLLER_MASTER);
//optionally define a non-standard brake behavior
int brake_behavior = E_MOTOR_BRAKE_COAST;
int FRONT_LEFT_MOTOR = 1;
int FRONT_RIGHT_MOTOR = 2;
int BACK_LEFT_MOTOR = 3;
int BACK_RIGHT_MOTOR = 4;
int BACK_CLAW = 5;
int FRONT_CLAW = 6;
int FRONT_LIFT = 7;

//temp for testing on another bot:
int MIDDLE_LEFT_MOTOR = 8;
int MIDDLE_RIGHT_MOTOR = 9;

Motor front_left (FRONT_LEFT_MOTOR, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor back_left (BACK_LEFT_MOTOR, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_right (FRONT_RIGHT_MOTOR, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor back_right (BACK_RIGHT_MOTOR, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

//temp for testing on another bot:
Motor middle_left (MIDDLE_LEFT_MOTOR, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor middle_right (MIDDLE_RIGHT_MOTOR, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

Motor back_claw (BACK_CLAW, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_claw (FRONT_CLAW, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor front_lift (FRONT_LIFT, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	lcd::set_text(1, "750W Banquet Code");
	lcd::set_text(2, "by: Ved K.");

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {


}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		int left_y = master.get_analog(ANALOG_LEFT_Y);
		int right_x = master.get_analog(ANALOG_RIGHT_X);
		int left_y_smooth =  (0.01 * pow(left_y,2)) * (left_y / std::abs(left_y));
		int right_x_smooth =  (0.01 * pow(right_x,2)) * (right_x / std::abs(right_x));
		int left_motors = left_y_smooth + right_x_smooth;
		int right_motors = left_y_smooth - right_x_smooth;
		front_left.move(left_motors);
		back_left.move(left_motors);
		front_right.move(right_motors);
		back_right.move(right_motors);
		//temp for testing on another bot:
		middle_left.move(left_motors);
		middle_right.move(right_motors);

		if (master.get_digital(DIGITAL_R1)) {
			front_claw.move(127);
		} else if (master.get_digital(DIGITAL_R2)) {
			front_claw.move(-127);
		} else {
			front_claw.move(0);
		}
		if (master.get_digital(DIGITAL_L1)) {
			back_claw.move(127);
		} else if (master.get_digital(DIGITAL_L2)) {
			back_claw.move(-127);
		} else {
			back_claw.move(0);
		}
		if (master.get_digital(DIGITAL_A)) {
			front_lift.move(127);
		} else if (master.get_digital(DIGITAL_B)) {
			front_lift.move(-127);
		} else {
			front_lift.move(0);
		}
		delay(20);
	}
}
