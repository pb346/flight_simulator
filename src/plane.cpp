#include <iostream>
#include "plane.h"

#include <cmath>

#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317675238467481846766940513200056812714526356082778577134275778960917363717872146844090122495343014654958537105079227968925892354201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859502445945534690830264252230825334468503526193118817101000313783875288658753320838142061717766914730359825349042875546873115956286388235378759375195778185778053217122680661300192787661119590921642019893809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151557485724245415069595082953311686172785588907509838175463746493931925506040092770167113900984882401285836160356370766010471018194295559619894676783744944825537977472684710404753464620804668425906949129331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992279678235478163600934172164121992458631503028618297455570674983850549458858692699569092721079750930295532116534498720275596023648066549911988183479775356636980742654252786255181841757467289097777279380008164706001614524919217321721477235014144197356854816136115735255213347574184946843852332390739414333454776241686251898356948556209921922218427255025425688767179049460165346680498862723279178608578438382796797668145410095388378636095068006422512520511739298489608412848862


Plane::Plane() {
    air_pressure = 1.0;
// scalars (all due to change in disaster scenarios)
	mass = 652.69995; // slugs
	wingspan = 32.8; // feet
	length = 49.3; // feet
	height = 16.7; // feet
	coefficient_of_lift = 0;
	coefficient_of_drag = 0;
	left_wing_surface_area = 0;
	right_wing_surface_area = 0;
// flaps/ailerons
	left_elevator_angle = 0;
	right_elevator_angle = 0;
	left_aileron_angle = 0;
	right_aileron_angle = 0;
	left_leading_edge_flap_angle = 0;
	right_leading_edge_flap_angle = 0;
	rudder_angle = 0;
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
// unit vector
	unit_vector_front.x = 1;
	unit_vector_front.y = 0;
	unit_vector_front.z = 0;
	unit_vector_left.x = 0;
	unit_vector_left.y = 1;
	unit_vector_left.z = 0;
	unit_vector_up.x = 0;
	unit_vector_up.y = 0;
	unit_vector_up.z = 1;
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
    //misc controls
    gears_Deployed = 1;
    afterburnerActive = 0;
    m_afterburner = 0;
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


void Plane::rotate_about_x(double degrees, double &x, double &y, double &z) {
	degrees = -degrees;
	double new_x = 1 * x + 0 * y + 0 * z;
	double new_y = 0 * x + cos(degrees*PI/180.0) * y - sin(degrees*PI/180.0) * z;
	double new_z = 0 * x + sin(degrees*PI/180.0) * y + cos(degrees*PI/180.0) * z;
	x = new_x;
	y = new_y;
	z = new_z;
	unitize(x, y, z);
}
void Plane::rotate_about_y(double degrees, double &x, double &y, double &z) {
	degrees = -degrees;
	double new_x = cos(degrees*PI/180.0) * x + 0 * y + sin(degrees*PI/180.0) * z;
	double new_y = 0 * x + 1 * y + 0 * z;
	double new_z = -sin(degrees*PI/180.0) * x + 0 * y + cos(degrees*PI/180.0) * z;
	x = new_x;
	y = new_y;
	z = new_z;
	unitize(x, y, z);
}
void Plane::rotate_about_z(double degrees, double &x, double &y, double &z) {
	degrees = -degrees;
	double new_x = cos(degrees*PI/180.0) * x - sin(degrees*PI/180.0) * y + 0 * z;
	double new_y = sin(degrees*PI/180.0) * x + cos(degrees*PI/180.0) * y + 0 * z;
	double new_z = 0 * x + 0 * y + 1 * z;
	x = new_x;
	y = new_y;
	z = new_z;
	unitize(x, y, z);
}
void Plane::rotate(double roll_degrees, double pitch_degrees, double yaw_degrees) {
	double x = 1;
	double y = 0;
	double z = 0;
	rotate_about_x(roll_degrees, x, y, z);
	rotate_about_y(pitch_degrees, x, y, z);
	rotate_about_z(yaw_degrees, x, y, z);
	unit_vector_front.x = x;
	unit_vector_front.y = y;
	unit_vector_front.z = z;

	x = 0;
	y = 1;
	z = 0;
	rotate_about_x(roll_degrees, x, y, z);
	rotate_about_y(pitch_degrees, x, y, z);
	rotate_about_z(yaw_degrees, x, y, z);
	unit_vector_left.x = x;
	unit_vector_left.y = y;
	unit_vector_left.z = z;

	x = 0;
	y = 0;
	z = 1;
	rotate_about_x(roll_degrees, x, y, z);
	rotate_about_y(pitch_degrees, x, y, z);
	rotate_about_z(yaw_degrees, x, y, z);
	unit_vector_up.x = x;
	unit_vector_up.y = y;
	unit_vector_up.z = z;
}
void Plane::unitize(double &x, double &y, double &z) {
	double x_new = x/sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	double y_new = y/sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	double z_new = z/sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	x = x_new;
	y = y_new;
	z = z_new;
}


void Plane::calculate_air_density() {
    air_pressure = 1.0 * exp(-z_position/22965.9);
}
void Plane::calculate_lift() { // (1/2) * d * v^2 * s * CL
	// lift = 9800*log(x+1)-0.015v^2+0.0000076x^3-10^(0.0022047877(x))
    if(m_velocity <= 220 && z_position <= 1000) {
        m_lift_right = 50 * m_velocity;
        m_lift_left = 50 * m_velocity;
    }
    else if(m_velocity > 220 && z_position <= 1000) {
        m_lift_right = 11000;
        m_lift_left = 11000;
    }
    else {
        m_lift_right = 10450;
        m_lift_left = 10450;
    }

    if(m_lift_right < 0) {
		m_lift_right = 0;
	}
	if(m_lift_left < 0) {
		m_lift_left = 0;
	}
	x_lift_left = m_lift_left * unit_vector_up.x;
	y_lift_left = m_lift_left * unit_vector_up.y;
	z_lift_left = m_lift_left * unit_vector_up.z;
	x_lift_right = m_lift_right * unit_vector_up.x;
	y_lift_right = m_lift_right * unit_vector_up.y;
	z_lift_right = m_lift_right * unit_vector_up.z;
}
void Plane::calculate_drag() { // Cd * (p * v^2)/2 * A
    m_drag_right = pow(m_velocity, 0.2) * 3218.15326;
    m_drag_left = pow(m_velocity, 0.2) * 3218.15326;
    if(m_drag_right > m_force/2) {
        m_drag_right = m_force/2;
    }
    if(m_drag_left > m_force/2) {
        m_drag_left = m_force/2;
    }
	if(m_drag_right < 0) {
		m_drag_right = 0;
	}
	if(m_drag_left < 0) {
		m_drag_left = 0;
	}
	x_drag_left = m_drag_left * -unit_vector_front.x;
	y_drag_left = m_drag_left * -unit_vector_front.y;
	z_drag_left = m_drag_left * -unit_vector_front.z;
	x_drag_right = m_drag_right * -unit_vector_front.x;
	y_drag_right = m_drag_right * -unit_vector_front.y;
	z_drag_right = m_drag_right * -unit_vector_front.z;
}
void Plane::calculate_thrust() {
	x_thrust = m_thrust * unit_vector_front.x;
	y_thrust = m_thrust * unit_vector_front.y;
	z_thrust = m_thrust * unit_vector_front.z;
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
	roll_torque = sqrt(m_velocity) * (m_lift_right * wingspan/4 + m_lift_left * -wingspan/4);
	pitch_torque = sqrt(m_velocity) * (length/2*sin(left_elevator_angle * PI/180) + length/2*sin(right_elevator_angle * PI/180));
	yaw_torque = sqrt(m_velocity) * (length/2*sin(rudder_angle * PI/180));
//	x_torque = sqrt(abs(x_velocity)) * (roll_torque * unit_vector_front.x + pitch_torque * unit_vector_left.x + yaw_torque * unit_vector_up.x);
//	y_torque = sqrt(abs(y_velocity)) * (roll_torque * unit_vector_front.y + pitch_torque * unit_vector_left.y + yaw_torque * unit_vector_up.y);
//	z_torque = sqrt(abs(z_velocity)) * (roll_torque * unit_vector_front.z + pitch_torque * unit_vector_left.z + yaw_torque * unit_vector_up.z);
	x_torque = (roll_torque * unit_vector_front.x + pitch_torque * unit_vector_left.x + yaw_torque * unit_vector_up.x);
	y_torque = (roll_torque * unit_vector_front.y + pitch_torque * unit_vector_left.y + yaw_torque * unit_vector_up.y);
	z_torque = (roll_torque * unit_vector_front.z + pitch_torque * unit_vector_left.z + yaw_torque * unit_vector_up.z);
}
void Plane::calculate_angular_accelerations() {
    pitch_angular_acceleration = pitch_angular_acceleration + pitch_torque / (mass);
    yaw_angular_acceleration = yaw_angular_acceleration + yaw_torque / (mass);
    roll_angular_acceleration = roll_angular_acceleration + roll_torque / (mass);
}
void Plane::calculate_angular_velocities() {
    pitch_angular_velocity = (pitch_angular_velocity + pitch_angular_acceleration)/ 10.0;
    yaw_angular_velocity = (yaw_angular_velocity + yaw_angular_acceleration) / 10.0;
    roll_angular_velocity = (roll_angular_velocity + roll_angular_acceleration) /10.0;
}
void Plane::calculate_angular_positions() {
	pitch_angle = pitch_angle + pitch_angular_velocity;
	yaw_angle = yaw_angle + yaw_angular_velocity;
	roll_angle = roll_angle + roll_angular_velocity;
}
void Plane::calculate_accelerations() {
    x_acceleration = x_force / (mass);//*100.0);
    y_acceleration = y_force / (mass);//*100.0);
    z_acceleration = z_force / (mass);//*100.0);
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
    x_position = x_position + x_velocity/10.0;
    y_position = y_position + y_velocity/10.0;
    z_position = z_position + z_velocity/10.0;
	m_position = calculate_magnitude(x_position, y_position, z_position);
	if(z_position < 0) {
		z_position = 0;
	}
}
void Plane::update_plane(double p_m_thrust, double p_left_elevator_angle, double p_right_elevator_angle, double p_left_aileron_angle, double p_right_aileron_angle, double p_rudder_angle, double p_m_afterburner) {

    m_thrust = 250.0 * p_m_thrust;//max 25,000
    m_afterburner = 50.0 * p_m_afterburner;//max 30,000
    m_thrust += m_afterburner;
	left_elevator_angle = p_left_elevator_angle;
	right_elevator_angle = p_right_elevator_angle;
    left_aileron_angle = p_left_aileron_angle;
    right_aileron_angle = p_right_aileron_angle;
    //left_leading_edge_flap_angle = p_left_leading_edge_flap_angle;
    //right_leading_edge_flap_angle = p_right_leading_edge_flap_angle;
	rudder_angle = p_rudder_angle;
    //calculate_air_density();
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
	rotate(roll_angle, pitch_angle, yaw_angle);
    calculate_accelerations();
    calculate_velocities();
	calculate_positions();
}

void Plane::update_model_parameters(){
//might not be needed
}

void Plane::process_joystick_input(PlaneModel* model, joystick_event* event, DebugValues** debug)
{
    DebugValues* localDebug = *debug;

/***************************AILERON*************************************/
    double local_right_aileron_angle = event->stick_x * model->maxAileronAngle;
    double local_left_aileron_angle = (-1.0)* event->stick_x * model->maxAileronAngle;
    if(( local_right_aileron_angle < 0 && (local_right_aileron_angle + .2) > 0 ) || (local_right_aileron_angle > 0 && (local_right_aileron_angle-.2 < 0)))
    {
        right_aileron_angle = 0;
        left_aileron_angle =  0;
        localDebug->aileronLeft = 0;
        localDebug->aileronRight = 0;
    }
    else
    {
        right_aileron_angle = local_right_aileron_angle;
        left_aileron_angle = local_left_aileron_angle;
        localDebug->aileronLeft = left_aileron_angle;
        localDebug->aileronRight = right_aileron_angle;
    }
    double tempThrottle;
    if(event->throttle > 0.0)
    {
        tempThrottle = 1.0 + event->throttle;
    }
    else
    {
        tempThrottle = 1.0 - (-1.0)*event->throttle;
    }
    if(tempThrottle > 1.7)
    {
        double afterburnerTemp = 2.0 - tempThrottle;
        double afterburnerVal = ((.3-afterburnerTemp) / .30 *100);
        localDebug->thrust = 100;
        afterburnerTemp = 2.0 - tempThrottle;
        if(afterburnerActive == 1)
        {
            afterburnerVal = ((.30-afterburnerTemp) / .30 *100);
            localDebug->afterburner = afterburnerVal;
        }
        else
        {
            localDebug->afterburner = 0;
        }
    }
    else
    {
        localDebug->thrust = tempThrottle /1.7 * 100;
        localDebug->afterburner = 0;
    }

    localDebug->rudder = event->stick_z *model->maxRudderAngle;
    rudder_angle = localDebug->rudder;

     if(( event->stick_y < 0 && (event->stick_y + .03) > 0 ) || (event->stick_y > 0 && (event->stick_y -.03 < 0)))
     {
         localDebug->elevator = 0;
         elevator_angle = 0;
     }
     else
     {
         localDebug->elevator = event->stick_y * model->maxElevPOS;
         elevator_angle = localDebug->elevator;
     }

    if(event->button[7] != 0)
    {
        localDebug->gears = 1;
    }
    else
    {
        localDebug->gears = 0;
    }
    if(event->button[1] != 0)
    {
        localDebug->afterburnerActive = 1;
        //this->afterburnerActive = 1;
    }
    else
    {
        localDebug->afterburnerActive = 0;
        //this->afterburnerActive = 0;
    }
    if(event->button[10] != 0)
    {
        localDebug->flapDown = 1;
    }
    else
    {
        localDebug->flapDown = 0;
    }
    if(event->button[11] != 0)
    {
        localDebug->flapUp = 1;
    }
    else
    {
        localDebug->flapUp = 0;
    }
    //slat
    //flap
    //thrust, lElev, rElev, lAil, rAil, slatl, slatr, rudder
}
