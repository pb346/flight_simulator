#include <iostream>

class Plane {
private:
// environmental variables
	const double gravity = 9.81;
	double air_density;
// scalars (all due to change in disaster scenarios)
	double mass;
	double coefficient_of_lift;
	double coefficient_of_drag;
	double left_wing_surface_area;
	double right_wing_surface_area;
// position variables
	double x_position;
	double y_position;
	double z_position;
	double x_velocity;
	double y_velocity;
	double z_velocity;
	double x_acceleration;
	double y_acceleration;
	double z_acceleration;
// angles
	// angle to axis variables
	double alpha_angle;
	double beta_angle;
	double gamma_angle;
	double alpha_velocity;
	double beta_velocity;
	double gamma_velocity;
	double alpha_acceleration;
	double beta_acceleration;
	double gamma_acceleration;
	// angle to about axis variables
	double pitch_angle;
	double pitch_angular_velocity;
	double pitch_angular_acceleration;
	double yaw_angle;
	double yaw_angular_velocity;
	double yaw_angular_acceleration;
	double roll_angle;
	double roll_angular_velocity;
	double roll_angular_acceleration;
// resultant momentum and resultant forces
	// magnitude
	double m_momentum;
	double m_angular_momentum;
	// unit vectors
	double x_momentum;
	double y_momentum;
	double z_momentum;
	double x_angular_momentum;
	double y_angular_momentum;
	double z_angular_momentum;
	double x_force;
	double y_force;
	double z_force;
	double x_torque;
	double y_torque;
	double z_torque;
// forces
	double thrust_mag;
	double x_thrust;
	double y_thrust;
	double z_thrust;
	// force of gravity (z_gravity == m_gravity)
	double z_gravity;
	// normal force
	double m_normal;
	double z_normal;
	double y_normal;
	double x_normal;
	// left wing
	double m_drag_left;
	double m_lift_left;
	double x_drag_left;
	double y_drag_left;
	double z_drag_left;
	double x_lift_left;
	double y_lift_left;
	double z_lift_left;
	// right wing
	double m_drag_right;
	double m_lift_right;
	double x_drag_right;
	double y_drag_right;
	double z_drag_right;
	double x_lift_right;
	double y_lift_right;
	double z_lift_right;

	double * cross_product(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);
	double dot_product(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);
	double calculate_magnitude(double x_unit_vector, double y_unit_vector, double z_unit_vector);
	double calculate_unit_vector(double magnitude, double angle_to_axis);
	double calculate_angle_to_axis(double magnitude, double axis_unit_vector);

	void calculate_air_density();
	void calculate_coefficient_of_lift();
	void calculate_lift(); // (1/2) * d * v^2 * s * CL
	void calculate_coefficient_of_drag();
	void calcualte_drag(); // Cd * (p * v^2)/2 * A
	void calculate_thrust();
	void calculate_gravitational_force();
	void calculate_normal_force();
	void calculate_torque();

	void calculate_angular_accelerations();
	void calculate_angular_velocities();
	void calculate_angular_positions();
	void calculate_accelerations();
	void calculate_velocities();
	void calculate_positions();
public:
	Plane();
	void update_plane();
};