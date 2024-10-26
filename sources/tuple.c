#include "../includes/minirt.h"


// Function to create a tuple
t_tuple tuple(double x, double y, double z, double w)
{
	t_tuple t;
	t.x = x;
	t.y = y;
	t.z = z;
	t.w = w;
	return (t);
}

// Function to multiply a tuple by a scalar
t_tuple tuple_multiply(t_tuple t, double scalar)
{
	return (tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w));
}

// Function to divide a tuple by a scalar, same as multiplying with a fraction lets say 0.5
t_tuple tuple_divide(t_tuple t, double scalar)
{
	if (scalar == 0)
	{
		// Handle division by zero if necessary
		// Here we'll just return a tuple with NaN components as a placeholder
		return tuple(NAN, NAN, NAN, NAN);
	}
	return tuple(t.x / scalar, t.y / scalar, t.z / scalar, t.w / scalar);
}

// Function to add two tuples
t_tuple tuple_add(t_tuple t1, t_tuple t2)
{
	return tuple(t1.x + t2.x, t1.y + t2.y, t1.z + t2.z, t1.w + t2.w);
}

// Function to subtract two tuples
t_tuple tuple_subtract(t_tuple t1, t_tuple t2)
{
	return tuple(t1.x - t2.x, t1.y - t2.y, t1.z - t2.z, t1.w - t2.w);
}

// Function to create a point (w = 1.0)
t_tuple point(double x, double y, double z)
{
	return tuple(x, y, z, 1.0);
}

// Function to create a vector (w = 0.0)
t_tuple vector(double x, double y, double z)
{
	return tuple(x, y, z, 0.0);
}
