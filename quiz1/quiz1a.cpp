#include <iostream>
using std::cin;
using std::cout;

int main()
{
    int sum = 0;
    int number;
    cout << "Give me a number: ";
    cin >> number;
    while(number > 0)
    {
        sum += number*number;
        number--;
    }
    cout << "Sum: " << sum << "\n";
    return 0;
}