/*
 * CSc103 Project 2: prime numbers.
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:
 * https://www.geeksforgeeks.org/bool-data-type-in-c/
 * https://www.programiz.com/cpp-programming/library-function/cmath/sqrt
 */

#include <iostream>
using namespace std;

int main()
{
    int num = 2;
    while (num > 1)
    {
        cin >> num;
        int i = 2;
        bool prime = true;
        if (num <= 1)
        {
            prime = false;
            i = num;
        }
        while (i < num)
        {
            int check = num % i;
            i++;
            if (check == 0)
            {
                prime = false;
            }
        }
        cout << prime << endl;
    }
	return 0;
}
