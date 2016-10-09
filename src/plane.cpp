#include <iostream>
#include "plane.h"
#include <cmath>
Plane::Plane() {
// scalars (all due to change in disaster scenarios)
	mass = 0;
	coefficient_of_lift = 0;
	coefficient_of_drag = 0;
	left_wing_surface_area = 0;
	right_wing_surface_area = 0;
// position variables
	x_position = 0;
	y_position = 0;
	z_position = 0;
	x_velocity = 0;
	y_velocity = 0;
	z_velocity = 0;
	x_acceleration = 0;
	y_acceleration = 0;
	z_acceleration = 0;
// angles
	// angle to axis variables
	alpha_angle = 0;
	beta_angle = 0;
	gamma_angle = 0;
	alpha_velocity = 0;
	beta_velocity = 0;
	gamma_velocity = 0;
	alpha_acceleration = 0;
	beta_acceleration = 0;
	gamma_acceleration = 0;
	// angle to about axis variables
	pitch_angle = 0;
	pitch_angular_velocity = 0;
	pitch_angular_acceleration = 0;
	yaw_angle = 0;
	yaw_angular_velocity = 0;
	yaw_angular_acceleration = 0;
	roll_angle = 0;
	roll_angular_velocity = 0;
	roll_angular_acceleration = 0;
// resultant momentum and resultant forces
	// magnitude
	m_momentum = 0;
	m_angular_momentum = 0;
	// unit vectors
	x_momentum = 0;
	y_momentum = 0;
	z_momentum = 0;
	x_angular_momentum = 0;
	y_angular_momentum = 0;
	z_angular_momentum = 0;
	x_force = 0;
	y_force = 0;
	z_force = 0;
	x_torque = 0;
	y_torque = 0;
	z_torque = 0;
// forces
	thrust_mag = 0;
	x_thrust = 0;
	y_thrust = 0;
	z_thrust = 0;
	// force of gravity (z_gravity == m_gravity)
	z_gravity = 0;
	// normal force
	m_normal = 0;
	z_normal = 0;
	y_normal = 0;
	x_normal = 0;
	// left wing
	m_drag_left = 0;
	m_lift_left = 0;
	x_drag_left = 0;
	y_drag_left = 0;
	z_drag_left = 0;
	x_lift_left = 0;
	y_lift_left = 0;
	z_lift_left = 0;
	// right wing
	m_drag_right = 0;
	m_lift_right = 0;
	x_drag_right = 0;
	y_drag_right = 0;
	z_drag_right = 0;
	x_lift_right = 0;
	y_lift_right = 0;
	z_lift_right = 0;
}


double * Plane::cross_product(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z) {
	double crossproduct[3] = {a_y * b_z - a_z * b_y, a_x * b_z - b_x * a_z, a_x * b_y - a_y * b_x};
	return crossproduct;
}
double Plane::dot_product(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z) {
	return a_x * b_x + a_y + b_y + a_z + b_z;
}
double Plane::calculate_magnitude(double x_unit_vector, double y_unit_vector, double z_unit_vector) {
	return sqrt(x_unit_vector * x_unit_vector + y_unit_vector * y_unit_vector + z_unit_vector * z_unit_vector);
}
double Plane::calculate_unit_vector(double magnitude, double angle_to_axis) {
	return magnitude * cos(angle_to_axis*3.14159265/180.0);
}
double Plane::calculate_angle_to_axis(double magnitude, double axis_unit_vector) {
	return cos(axis_unit_vector*3.14159265/180.0) / magnitude;
}

void Plane::calculate_air_density() {

}
void Plane::calculate_coefficient_of_lift() {

}
void Plane::calculate_lift() { // (1/2) * d * v^2 * s * CL

}
void Plane::calculate_coefficient_of_drag() {

}
void Plane::calcualte_drag() { // Cd * (p * v^2)/2 * A

}
void Plane::calculate_thrust() {

}
void Plane::calculate_gravitational_force() {

}
void Plane::calculate_normal_force() {

}
void Plane::calculate_torque() {

}
void Plane::calculate_angular_accelerations() {
	pitch_angular_acceleration = pitch_angular_acceleration + z_torque / mass;
	yaw_angular_acceleration = yaw_angular_acceleration + y_torque / mass;
	roll_angular_acceleration = roll_angular_acceleration + x_torque / mass;
}
void Plane::calculate_angular_velocities() {
	pitch_angular_velocity = pitch_angular_velocity + pitch_angular_acceleration;
	yaw_angular_velocity = yaw_angular_velocity + yaw_angular_acceleration;
	roll_angular_velocity = roll_angular_velocity + roll_angular_acceleration;
}
void Plane::calculate_angular_positions() {
	pitch_angle = pitch_angle + pitch_angular_velocity;
	yaw_angle = yaw_angle + yaw_angular_velocity;
	roll_angle = roll_angle + roll_angular_velocity;
}
void Plane::calculate_accelerations() {
	x_acceleration = x_acceleration + x_force / mass;
	y_acceleration = y_acceleration + y_force / mass;
	z_acceleration = z_acceleration + z_force / mass;
}
void Plane::calculate_velocities() {
	x_velocity = x_velocity + x_acceleration;
	y_velocity = y_velocity + y_acceleration;
	z_velocity = z_velocity + z_acceleration;
}
void Plane::calculate_positions() {
	x_position = x_position + x_velocity;
	y_position = y_position + y_velocity;
	z_position = z_position + z_velocity;
}
void Plane::update_plane() {
	calculate_air_density();
	calculate_coefficient_of_lift();
	calculate_lift(); // (1/2) * d * v^2 * s * CL
	calculate_coefficient_of_drag();
	calcualte_drag(); // Cd * (p * v^2)/2 * A
	calculate_thrust();
	calculate_gravitational_force();
	calculate_normal_force();
	calculate_torque();

	calculate_angular_accelerations();
	calculate_angular_velocities();
	calculate_angular_positions();
	calculate_accelerations();
	calculate_velocities();
	calculate_positions();
}

void Plane::update_model_parameters(){

}
