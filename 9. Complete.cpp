#include <iostream>
using namespace std;

int main() {
    int i = 0;
    int j = 0;
    float v = 0.0f;
    float x = 0.0f;
    float a[100]; // Array of size 100

    while (true) {
        // Increment i until we find an element greater than or equal to v
        do {
            i = i + 1;
        } while (a[i] < v);

        // Decrement j until we find an element less than or equal to v
        do {
            j = j - 1;
        } while (a[j] > v);

        // If indices have crossed, exit the loop
        if (i >= j) break;

        // Swap elements at positions i and j
        x = a[i];
        a[i] = a[j];
        a[j] = x;
    }

    return 0;
}
// Intermediate Code Produced by the Front End
L1: L3: i = i + 1;            // Increment i
L5: t1 = i * 8;              // Calculate memory address for a[i]
t2 = a[t1];                 // Load the value at a[i] into t2
if (t2 < v) goto L3;        // If a[i] < v, continue the loop

L4: j = j - 1;              // Decrement j
L7: t3 = j * 8;              // Calculate memory address for a[j]
t4 = a[t3];                 // Load the value at a[j] into t4
if (t4 > v) goto L4;        // If a[j] > v, continue the loop

L6: if (i >= j) goto L2;    // If i >= j, break the loop

t5 = i * 8;                 // Calculate memory address for a[i]
x = a[t5];                  // Store the value of a[i] in x
t6 = j * 8;                 // Calculate memory address for a[j]
a[t5] = a[t6];              // Swap a[i] and a[j]
a[t6] = x;                  // Store x in a[j]

goto L1;                    // Repeat the loop

L2: // Exit point of the loop
