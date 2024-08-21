#include <stdio.h>
#include <math.h>

// compile with gcc vectors_test.c -lm

// Define the struct for a 3D vector
// This struct holds three float values: x, y, and z, representing the components of a 3D vector.
typedef struct {
    float x, y, z;
} Vector3D;

// Function to add two vectors
// This function takes two Vector3D structs as input and returns a new Vector3D
// representing the sum of the two vectors. The components (x, y, z) of both vectors 
// are added together.
Vector3D vector_add(Vector3D v1, Vector3D v2) {
    Vector3D result; // Create a new Vector3D to store the result
    result.x = v1.x + v2.x; // Add the x components
    result.y = v1.y + v2.y; // Add the y components
    result.z = v1.z + v2.z; // Add the z components
    return result; // Return the resulting vector
}

// Function to subtract two vectors
// This function takes two Vector3D structs as input and returns a new Vector3D
// representing the difference between the two vectors. The components (x, y, z)
// of the second vector are subtracted from the components of the first vector.
Vector3D vector_subtract(Vector3D v1, Vector3D v2) {
    Vector3D result; // Create a new Vector3D to store the result
    result.x = v1.x - v2.x; // Subtract the x components
    result.y = v1.y - v2.y; // Subtract the y components
    result.z = v1.z - v2.z; // Subtract the z components
    return result; // Return the resulting vector
}

// Function to calculate the magnitude of a vector
// The magnitude (or length) of a vector is calculated using the formula:
// magnitude = sqrt(x^2 + y^2 + z^2). This function returns the magnitude as a float.
float vector_magnitude(Vector3D v) {
    // Calculate the square root of the sum of the squares of the components
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Function to calculate the dot product of two vectors
// The dot product of two vectors is calculated using the formula:
// dot_product = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z).
// This function returns the dot product as a float.
float vector_dot_product(Vector3D v1, Vector3D v2) {
    // Multiply the corresponding components of the two vectors and sum them
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Function to normalize a vector
// Normalization scales a vector so that its magnitude becomes 1 (unit vector).
// This is done by dividing each component of the vector by its magnitude.
// If the magnitude is 0 (zero vector), a message is printed and the original vector is returned.
Vector3D vector_normalize(Vector3D v) {
    float mag = vector_magnitude(v); // Calculate the magnitude of the vector
    if (mag == 0) {
        printf("Cannot normalize a zero vector.\n"); // Handle zero vector case
        return v; // Return the original vector unchanged
    }
    Vector3D result; // Create a new Vector3D to store the result
    result.x = v.x / mag; // Divide the x component by the magnitude
    result.y = v.y / mag; // Divide the y component by the magnitude
    result.z = v.z / mag; // Divide the z component by the magnitude
    return result; // Return the normalized vector
}

// Function to multiply a vector by a scalar
// Scalar multiplication scales the vector by multiplying each component (x, y, z)
// by a scalar value. This function returns the scaled vector as a new Vector3D.
Vector3D vector_scalar_multiply(Vector3D v, float scalar) {
    Vector3D result; // Create a new Vector3D to store the result
    result.x = v.x * scalar; // Multiply the x component by the scalar
    result.y = v.y * scalar; // Multiply the y component by the scalar
    result.z = v.z * scalar; // Multiply the z component by the scalar
    return result; // Return the scaled vector
}

// Function to print a vector (for convenience)
// This function prints the components of the vector in the format:
// Vector3D(x, y, z).
void print_vector(Vector3D v) {
    // Print the components of the vector
    printf("Vector3D(%f, %f, %f)\n", v.x, v.y, v.z);
}

// Main function to demonstrate the operations
int main() {
    // Initialize two vectors v1 and v2
    Vector3D v1 = {1.0, 2.0, 3.0}; // Vector (1, 2, 3)
    Vector3D v2 = {4.0, 5.0, 6.0}; // Vector (4, 5, 6)

    // Vector addition
    // Add v1 and v2 and store the result in v3
    Vector3D v3 = vector_add(v1, v2);
    printf("v1 + v2 = ");
    print_vector(v3);  // Output: Vector3D(5.000000, 7.000000, 9.000000)

    // Vector subtraction
    // Subtract v2 from v1 and store the result in v4
    Vector3D v4 = vector_subtract(v1, v2);
    printf("v1 - v2 = ");
    print_vector(v4);  // Output: Vector3D(-3.000000, -3.000000, -3.000000)

    // Magnitude of v1
    // Calculate the magnitude of v1 and print it
    float magnitude = vector_magnitude(v1);
    printf("Magnitude of v1 = %f\n", magnitude);  // Output: 3.741657

    // Dot product of v1 and v2
    // Calculate the dot product of v1 and v2 and print it
    float dot = vector_dot_product(v1, v2);
    printf("Dot product of v1 and v2 = %f\n", dot);  // Output: 32.000000

    // Normalizing v1
    // Normalize v1 and store the result in v1_normalized
    Vector3D v1_normalized = vector_normalize(v1);
    printf("Normalized v1 = ");
    print_vector(v1_normalized);  // Output: Vector3D(0.267261, 0.534522, 0.801784)

    // Scalar multiplication of v1 by 2
    // Multiply v1 by 2 and store the result in v5
    Vector3D v5 = vector_scalar_multiply(v1, 2.0);
    printf("v1 * 2 = ");
    print_vector(v5);  // Output: Vector3D(2.000000, 4.000000, 6.000000)

    return 0; // End of the program
}