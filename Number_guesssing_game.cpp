#include <bits/stdc++.h>
using namespace std;
 //NUMBER GUESSING GAME 
int main()
{
    cout << "Welcome to The Number guessing game" << endl;
    cout << "You have to guess a number between 1 and 50. "
            "You'll have limited tries based on the level you choose."<< endl;
 
    while (true) {
        cout << "Enter the difficulty level:  ";
        cout << "1 for easy!\t";
        cout << "2 for medium!\t";
        cout << "3 for difficult!\t";
        cout << "0 for ending the game" << endl;
      
        int difficultyChoice;
        cout << "Enter your choice: ";
        cin >> difficultyChoice;

        srand(time(0));
        int secretNumber = 1 + (rand() % 50);
        int playerChoice;
        if (difficultyChoice == 1) {
            cout << "You have 10 choices for finding the secret number between 1 and 50.";
            int choicesLeft = 10;
            for (int i = 1; i <= 10; i++) {
                cout << "Enter the number: ";
                cin >> playerChoice;
                if (playerChoice == secretNumber) {
                    cout << "CORRECT ANSWER YOU WON !!!!!"<< endl;
                    cout << "Thanks for playing" << endl;
                    break;
                }
                else {
                    cout << "wrong answer " << playerChoice
                         << " is not the right number\n";
                    if (playerChoice > secretNumber) {
                        cout << "The secret number is smaller than the number you have chosen"<< endl;
                    }
                    else {
                        cout << "The secret number is greater than the number you have chosen"<< endl;
                    }
                    choicesLeft--;
                    cout << choicesLeft << " tries left. "<< endl;
                    if (choicesLeft == 0) {
                        cout << "You couldn't guess the  number, it was "<< secretNumber<<endl;
                    }
                }
            }
        }
        else if (difficultyChoice == 2) {
            cout << "You have 5 choices for finding the secret number between 1 and 50.";
            int choicesLeft = 5;
            for (int i = 1; i <= 5; i++) {
                cout << "Enter the number: ";
                cin >> playerChoice;
                if (playerChoice == secretNumber) {
                    cout << "CORRECT ANSWER YOU WON !!!!!"<< endl;
                    cout << " Thanks for playing" << endl;
                    break;
                }
                else {
                   cout << "wrong answer " << playerChoice
                         << " is not the right number\n";
                    if (playerChoice > secretNumber) {
                        cout << "The secret number is smaller than the number you have chosen"<< endl;
                    }
                    else {
                        cout << "The secret number is greater than the number you have chosen"<< endl;
                    }
                    choicesLeft--;
                    cout << choicesLeft << " tries left. "<< endl;
                    if (choicesLeft == 0) {
                        cout << "You couldn't guess the  number, it was "<< secretNumber<<endl;
                    
                    }
                }
            }
        }
        else if (difficultyChoice == 3) {
            cout << "You have 3 choices for finding the secret number between 1 and 50.";
            int choicesLeft = 3;
            for (int i = 1; i <= 3; i++) {
                cout << "Enter the number: ";
                cin >> playerChoice;
                if (playerChoice == secretNumber) {
                   cout << "CORRECT ANSWER YOU WON !!!!!"<< endl;
                    cout << " Thanks for playing" << endl;
                    break;
                }
                else {
                    cout << "wrong answer " << playerChoice
                         << " is not the right number\n";
                    if (playerChoice > secretNumber) {
                        cout << "The secret number is smaller than the number you have chosen"<< endl;
                    }
                    else {
                        cout << "The secret number is greater than the number you have chosen"<< endl;
                    }
                    choicesLeft--;
                    cout << choicesLeft << " tries left. "<< endl;
                    if (choicesLeft == 0) {
                        cout << "You couldn't guess the  number, it was "<< secretNumber<<endl;
                  
                    }
                }
            }
        }
     
        else if (difficultyChoice == 0) {
            exit(0);
        }
        else {
            cout << "Wrong choice, Enter valid choice to play the game! (0,1,2,3)"<< endl;
        }
    }
    return 0;
}
