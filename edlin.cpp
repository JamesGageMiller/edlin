//Author:James Miller
//purpose: to emulate the msdos text editor
#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

using namespace std;

int main(){
    string fileName;
    // used for the input to decide which command to run out of the options avalible
    string userInput;
    //string array, each string in the array is a line, yes this is innefficent but all you have to do to resize it is change the array size and the i < statement in the this file
    string loadedFile[100];
    // keeps track of how many strings in the array are in use
    int numStrings = 0;
    // due to the fact the input is seperated by a space, i am using this to seperate the inputs
    char *const menvp[] = {
				(char *) "PATH=/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", 0
    };
    cout <<  "\n\n\tHERE\n\n";
    // sets all of thenstrings equal to null so that when there is a comparison to null it properly works, should work anyway but just a precaution
    // for(int i=0; i < 99; i++){
    //     loadedFile[i] = nullptr;
    // }

    cout <<  "\n\n\tHERE\n\n";
    // not q is the exit condition because just in case the return 0 from q doesnt work, this will exit the loop and return 0
    while(userInput[0] != 'q'){
        // the prompt to ask for a input, uses getline due to the fact its a string
        cout << "edlin>" << endl;
        getline(cin, userInput);
        // exit condition, prints goodbye message and returns 0;
        if (userInput.at(0) == 'q' || userInput.at(0) == 'Q'){
            cout << "edlin> Goodbye" << endl;
            return 0;
        }
        // outputs everything loaded into the string array
        else if(userInput.at(0) == 'p' || userInput.at(0) == 'P'){
            // makes sure anything exist in the array and gives a error if theres nothing in it
            if (loadedFile->size() == 0){
                cout << "edlin> Oops, no file loaded in";
            }
            // goes line by line and prints everything in the array up to the number of strings
            else{
                for (int i=0; i < numStrings; i++){
                    cout << loadedFile[i] << endl;
                }
            }
        }
        // loads in from a file
        else if(userInput.at(0) == 'l' || userInput.at(0) == 'L'){
            // clears the current array of strings
            for (int i = 0; i < 100; i++){
                loadedFile[i] = nullptr;

            }
            //sets numstrings to 0 since we cleared the array
            numStrings = 0;
            // gets the inputed file from the user input
            ifstream inputedFile;
        
            inputedFile.open(userInput.substr(2,userInput.size()));
            // checks if open then loads everything into the array and iterates num strings up
            if(inputedFile.is_open()){
                
                while (getline(inputedFile,loadedFile[numStrings])){
                    numStrings++;
                }
                
            }
            // error message for if the file does not open
            else{
                cout << "edlin> Failed to open file" << endl ;
            }
            inputedFile.close();
            fileName = userInput.substr(2,userInput.size());

        }
        // outputs everything to a file
        else if(userInput.at(0) == 's' || userInput.at(0) == 'S'){
            // gets file name from the input
            ofstream userFile;
            userFile.open(userInput.substr(2,userInput.size()));
            // checks if open then outputs everything line by line, then closes file
            if(userFile.is_open()){
                for(int i=0; i<numStrings; i++){
                    userFile << loadedFile[i] << endl;
                }
                userFile.close();
            }
            // fail message for if the file does not open
            else{
                cout << "edlin> File failed to open" << endl;
            }

        }
        //gets the line number 
        else if(userInput.at(0) == 'e' || userInput.at(0)== 'E'){
            int lineNumber = stoi((userInput.substr(2, userInput.substr(2, userInput.size()).find_first_of(" "))));
        }
        else if(userInput.at(0) == 'c' || userInput.at(0) == 'C'){
            //check for filename existing
            if (fileName.empty()){
                cout << "edlin>Please input a file name";
                getline(cin,fileName);
            }
            //fork to start the process of compiling while keeping this running
                pid_t pid = fork();
                // fail message
                if(pid == -1){
                    cout << "Fork has failed";
                }
                // creates the command to run it in peices
                else if(pid == 0){
                    // filename is converted to a cstring, no idea why but didnt work if it wasnt one
                    const char* command = fileName.c_str();
                    // name of executable
                    const char* executable = (fileName + ".exe").c_str();
                    // the exact thing im sending for compilation, basically just making the console command
                    char* arguments[] = {(char*)"/bin/sh", "-Wall", (char*)command,(char*)"-o", (char*)executable, nullptr};
                    // compiling it if possible
                    execvpe("usr/bin/g++", arguments, menvp);
                }
            //parent function return statements for if the fork actually went through and what it did
                else{
                    int processStatus;
                    if(waitpid(pid, &processStatus,0) < 0){
                        cout << pid << " error" << endl;
                    }
                    else{
                        cout << pid << " complete" << endl;
                    }
                }
            }

    
        else if(userInput.at(0) == 'r' || userInput.at(0) == 'R'){
            // fork to keep program running while doing this
            pid_t pid = fork();
            // fail message
            if (pid == -1){
                cout << "Fork Failure" << endl;
            }
            // assembles the execute command one by one
            else if (pid == 0){
                // the exact command
            const char* execute = ("./" +  fileName + ".exe").c_str();
            // puts it in a character array for it to work
            char* arguments[] = {(char*)execute, nullptr};
            
            execvpe(execute,arguments,menvp);
            }
            //parent function return statements for if the fork actually went through and what it did
            else{
                int processStatus;
                if(waitpid(pid, &processStatus,0) < 0){
                    cout << pid << "error" << std::endl;
                }
                else{
                    cout << pid << "complete" << endl;
                }

            }

        }
        // error message for bad input
        else{
            cout << "edlin> Invalid input" << endl;
        }
    
    }
    return 0;

}