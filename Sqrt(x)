#include <iostream>
#include <vector>
using namespace std;

int main() {
    double x = 5;
    double left = 0, right = x;
    double middle = (right + left) / 2;
    while (true) {
        if (abs(middle * middle - x) < 0.00001) {
            break;
        }
        if (middle * middle > x) {
            right = middle;
            middle = (right + left) / 2;
        }
        if (middle * middle < x) {
            left = middle;
            middle = (right + left) / 2;
        }
        cout << middle << endl;
    }
    cout << middle << endl;
    return 0;
}
