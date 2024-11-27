#include <iostream>
#include <iomanip> // For fixed, setprecision, setw
#include <cstdlib> 
using namespace std;

// Function declarations
void print_real(float number, int fieldspace, int precision);

// Placeholder for update_scale function
template <typename T>
void update_scale(T &a, T &b) {
    // Example functionality: Scale a and b by dividing both by 2
    a /= 2;
    b /= 2;
}

int main(void) {
    float a, b;

    // Prompt the user for input
    cout << "Please input two real numbers: ";
    cin >> a >> b;

    // Print the numbers before calling update_scale
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    // Call the update_scale function
    update_scale(a, b);

    // Print the numbers after calling update_scale
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
