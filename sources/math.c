#include "../includes/minirt.h"



// Function to normalize a vector
t_tuple normalize(t_tuple v)
{
	double mag = magnitude(v);
	if (mag == 0) {
		// Handling the case where magnitude is zero (though it's not expected for valid vectors)
		return vector(0, 0, 0);
	}
	return (vector(v.x / mag, v.y / mag, v.z / mag));
}

// Function to compute the dot product of two vectors
float dot(t_tuple a, t_tuple b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Function to compute the cross product of two vectors in a left-handed system
t_tuple cross(t_tuple a, t_tuple b)
{
	return vector(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z, // Swap terms for left-handed system
		a.x * b.y - a.y * b.x
	);
}

// Function to calculate the magnitude of a vector
float magnitude(t_tuple v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}
