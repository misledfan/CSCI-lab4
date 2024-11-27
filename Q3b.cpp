// ok

#include <iostream>
#include <iomanip> // For fixed, setprecision, setw
#include <cstdlib>
using namespace std;

// Function declarations
void print_real(float number, int fieldspace, int precision);

// Template function to update and scale values
template <typename T>
void update_scale(T &m1, T &m2, T m3 = 10) {
    T original_m1 = m1; // Store original m1 value
    T original_m2 = m2; // Store original m2 value

    // Update m1 with the sum of m1 and m2, then multiply by m3
    m1 = (original_m1 + original_m2) * m3;

    // Update m2 with the difference of m1 and m2, then multiply by m3
    m2 = (original_m1 - original_m2) * m3;
}

int main(void) {
    float a, b;

    // Prompt user for input
    cout << "Please input two real numbers: ";
    cin >> a >> b;

    // Print initial values
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    // Call update_scale function
    update_scale(a, b);

    // Print updated values
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    return 0;
}

// Function to print a floating-point number with specific formatting
void print_real(float number, int fieldspace, int precision) {
    cout << fixed              // Use fixed-point notation
         << setprecision(precision) // Set the number of decimal places
         << setw(fieldspace)       // Set the output field width
         << number                 // Print the number
         << endl;                  // Move to the next line
}

