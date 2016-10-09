#include <iostream>
#include "plane.h"
#include <cmath>

#define PI 3.14159265358979323846264338327950

Plane::Plane() {
// scalars (all due to change in disaster scenarios)
	mass = 652.69995; // slugs
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
	m_force = 0;
	m_torque = 0;
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
	m_thrust = 0;
	x_thrust = 0;
	y_thrust = 0;
	z_thrust = 0;
	// force of gravity (z_gravity == m_gravity)
	m_gravity = 0;
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
	return sqrt(pow(x_unit_vector,2) + pow(y_unit_vector,2) + pow(z_unit_vector,2));
}
double Plane::calculate_unit_vector(double magnitude, double angle_to_axis) {
	return magnitude * cos(angle_to_axis*PI/180.0);
}
double Plane::calculate_angle_to_axis(double magnitude, double axis_unit_vector) {
	return cos(axis_unit_vector*PI/180.0) / magnitude;
}

void Plane::calculate_air_density() {

}
void Plane::calculate_lift() { // (1/2) * d * v^2 * s * CL
	// lift = 9800*log(x+1)-0.015v^2+0.0000076x^3-10^(0.0022047877(x))
	m_lift_right = (9800 * log10(m_velocity + 1) - 0.015*pow(m_velocity,2) + 0.0000076*pow(m_velocity, 3) - pow(10, 0.0022047877*m_velocity))/2;
	m_lift_left = (9800 * log10(m_velocity + 1) - 0.015*pow(m_velocity,2) + 0.0000076*pow(m_velocity, 3) - pow(10, 0.0022047877*m_velocity))/2;
	if(m_lift_right < 0) {
		m_lift_right = 0;
	}
	if(m_lift_left < 0) {
		m_lift_left = 0;
	}
	x_lift_left = m_lift_left * cos((alpha_angle - 90) * PI/180);
	y_lift_left = m_lift_left * cos((beta_angle) * PI/180);
	z_lift_left = m_lift_left * cos((gamma_angle - 90) * PI/180);
	x_lift_right = m_lift_right * cos((alpha_angle - 90) * PI/180);
	y_lift_right = m_lift_right * cos((beta_angle) * PI/180);
	z_lift_right = m_lift_right * cos((gamma_angle - 90) * PI/180);
}
void Plane::calculate_drag() { // Cd * (p * v^2)/2 * A
	m_drag_right = 774.5966692*pow(m_velocity, 0.5)/2;
	m_drag_left = 774.5966692*pow(m_velocity, 0.5)/2;
	if(m_drag_right < 0) {
		m_drag_right = 0;
	}
	if(m_drag_left < 0) {
		m_drag_left = 0;
	}
	x_drag_left = m_drag_left * cos((alpha_angle + 180) * PI/180);
	y_drag_left = m_drag_left * cos((beta_angle + 180) * PI/180);
	z_drag_left = m_drag_left * cos((gamma_angle + 180) * PI/180);
	x_drag_right = m_drag_right * cos((alpha_angle + 180) * PI/180);
	y_drag_right = m_drag_right * cos((beta_angle + 180) * PI/180);
	z_drag_right = m_drag_right * cos((gamma_angle + 180) * PI/180);
}
void Plane::calculate_thrust() {
	x_thrust = cos(alpha_angle * PI/180) * m_thrust;
	y_thrust = cos(beta_angle * PI/180) * m_thrust;
	z_thrust = cos(gamma_angle * PI/180) * m_thrust;
}
void Plane::calculate_gravitational_force() {
	m_gravity = mass * 32.174; // in feet not meters
}
void Plane::calculate_normal_force() {
	m_normal = 0;
}
void Plane::calculate_resultant_force() {
	// thrust, drag, lift, and weight + normal (z only)
	x_force = x_thrust + x_lift_left + x_lift_right + x_drag_left + x_drag_right;
	y_force = y_thrust + y_lift_left + y_lift_right + y_drag_left + y_drag_right;
	z_force = z_thrust + z_lift_left + z_lift_right + z_drag_left + z_drag_right + m_normal - m_gravity;
	if(z_force < 0.05 && z_force > -0.05) {
		z_force = 0;
	}
	if(z_position <= 0 && z_force < 0) {
		z_force = 0;
	}
	m_force = calculate_magnitude(x_force, y_force, z_force);
}
void Plane::calculate_torque() {

}
void Plane::calculate_angular_accelerations() {
	pitch_angular_acceleration = pitch_angular_acceleration + pitch_torque / mass;
	yaw_angular_acceleration = yaw_angular_acceleration + yaw_torque / mass;
	roll_angular_acceleration = roll_angular_acceleration + roll_torque / mass;
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
	x_acceleration = x_force / mass;
	y_acceleration = y_force / mass;
	z_acceleration = z_force / mass;
	if(z_position <= 0 && z_acceleration < 0) {
		z_acceleration = 0;
	}
	m_acceleration = calculate_magnitude(x_acceleration,y_acceleration,z_acceleration);
}
void Plane::calculate_velocities() {
	x_velocity = x_velocity + x_acceleration;
	y_velocity = y_velocity + y_acceleration;
	z_velocity = z_velocity + z_acceleration;
	if(z_position <= 0 && z_velocity < 0) {
		z_velocity = 0;
	}
	m_velocity = calculate_magnitude(x_velocity, y_velocity, z_velocity);
	if(m_velocity < 0.0) {
		m_velocity = 0;
		x_velocity = 0;
		y_velocity = 0;
		z_velocity = 0;
	}
}
void Plane::calculate_positions() {
	x_position = x_position + x_velocity;
	y_position = y_position + y_velocity;
	z_position = z_position + z_velocity;
	m_position = calculate_magnitude(x_position, y_position, z_position);
	if(z_position < 0) {
		z_position = 0;
	}
}
void Plane::update_plane() {
	calculate_air_density();
	calculate_lift(); // (1/2) * d * v^2 * s * CL
	calculate_drag(); // Cd * (p * v^2)/2 * A
	calculate_thrust();
	calculate_gravitational_force();
	calculate_normal_force();
	calculate_resultant_force();
	calculate_torque();

	calculate_angular_accelerations();
	calculate_angular_velocities();
	calculate_angular_positions();
	calculate_accelerations();
	calculate_velocities();
	calculate_positions();
}
