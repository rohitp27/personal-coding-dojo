// Problem: Basic console I/O.
// 1. Print "Hello, World!" on its own line.
// 2. Prompt for the user's name, read a full line, then print "Hello, <name>!".
// 3. Prompt for an integer age, read it, then print "You are <age> years old.".
//
// Run with: make run FILE=practice/cpp/io/hello_and_echo.cpp

#include <iostream>
#include <string>

using namespace std;

int main() {
    // TODO: print "Hello, World!"
    std::string helloWorld = "Hello World!";
    printf("%s: 1\n", helloWorld.c_str());
    cout<<helloWorld<<": 2"<<endl;

    // TODO: prompt "Enter your name: ", read a full line into a std::string,
    string new_thing;
    // print "Hello, <name>!"

    int age = -1;
    cout<<"Enter your age"<<endl;
    scanf("%d", &age);
    printf("Your age is : %d\n", age);

    // reusing int to drain
    while( (age = getchar()) != '\n' && age != EOF){

    }

    printf("Enter your name: ");
    char name[25];
    fgets(name, sizeof(name), stdin);

    size_t nameLen = strlen(name);
    if ( nameLen > 0 && name[nameLen-1] == '\n' ) {
        name[nameLen-1] = '\0';
    }
    printf("Your name is : %s, hi!\n", name);
    // TODO: prompt "Enter your age: ", read an int, print "You are <age> years old."

    return 0;
}
