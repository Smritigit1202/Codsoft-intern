#include <iostream>
using namespace std;
 //A SIMPLE CALCULATOR
int main()
{
    cout<<"WELCOME TO SMRITI'S SIMPLE CALCULATOR"<<endl<<"THE OPERATIONS THAT ARE VALID ARE:"<<"1)Addition "<<endl<<"2)Subtraction "<<endl<<"3)Multiplication "<<endl<<"4)Division ";
    char op;
    float num1, num2;
    cout<<"Enter the operation you want to perform through it's symbol:  ";
    cin >> op;
    cout<<"Enter two numbers you want to perform operation on respectively:  ";
    cin >> num1 >> num2;
 
    switch (op) {
    case '+':
        cout <<"Your ans is "<< num1 + num2;
        break;
    case '-':
        cout<<"Your ans is " << num1 - num2;
        break;
    case '*':
        cout<<"Your ans is " << num1 * num2;
        break;
    case '/':
        cout<<"Your ans is "<< num1 / num2;
        break;
 
    default:
        cout << "Error! operator is not correct";
    }

 
    return 0;
}
