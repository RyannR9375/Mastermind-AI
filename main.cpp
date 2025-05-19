/*
    Dr. Mark E. Lehr
    CSC 7 Template for Mastermind AI
    May 11th, 2021
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

//Function Prototypes
string AI(char,char);
bool eval(string,string,char &,char &);
string set();
void setDigit(string&, int);
void setDigitExcept(string&, int, char, vector<int>&, bool);
void printHelp(vector<char>);
char get(vector<char>);
std::string step(string, int, bool, char&, char);

int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare variables
    string code,guess;  //code to break, and current guess
    char rr,rw;         //right digit in right place vs. wrong place
    int nGuess;         //number of guesses
    
    //Initialize Values
    nGuess=0;
    code=set();
    rr=rw=0;
    
    //Loop until solved and count to find solution
    do{
       nGuess++;
       guess=AI(rr,rw);
    }while(eval(code,guess,rr,rw));
    //Check evaluation
    cout<<nGuess<<" "<<code<<" "<<guess<<endl;
    guess=AI(rr,rw);
    cout<<code<<endl;
    /*for(int i=0;i<10000;i++){
        guess=AI(0,0);
        eval(code,guess,rr,rw);
        cout<<setw(5)<<code<<setw(5)<<guess
                <<setw(2)<<static_cast<int>(rr)
                <<setw(2)<<static_cast<int>(rw)<<endl;
    }*/
    
    //Exit the program
    return 0;
}

string AI(char rr,char rw){
    //Define helper functions here
    void (*print)(string [],char [],char [],int,int)=
        [] (string g[],char r[],char w[],int nb,int ne){ 
            for(int i=nb;i<=ne;i++){
                cout<<g[i]<<" "
                    <<static_cast<int>(r[i])<<" "
                    <<static_cast<int>(w[i])<<endl;
            }
    };

    
    //Save the historical values of guesses and results
    static const int SIZE=10000;//How many guesses to save
    static string aGuess[SIZE]; //Save the guesses
    static char grr[SIZE];      //Save right guess in right spot
    static char grw[SIZE];      //Save right guess in wrong spot
    static int guess=0;         //Increment the guess number
    string sGuess = "0000";       //Size the guest string
    
    
    static vector<char> rightDigits;
    static vector<char> digitsNotIn;
    const int step1Guesses = 9;
    const int code_length  = 4;
    
    //Store the results from the last guess
    grr[guess]=rr;
    grw[guess]=rw;
    
    //Test the helper function
    if(rr==4)print(aGuess,grr,grw,
            guess-10>0?guess-10:0,guess);
    
    //Calculate the guess
    
//STEP 1 - GATHER  THE 4 DIGITS
    static std::map<char,int> count;
    //CHECK TO SEE IF YOU ALREADY HAVE THE 4 DIGITS,
    if(rightDigits.size() < code_length){
        //IF A DIGIT WAS FOUND AT THAT NUMBER,
        if(grr[guess] != 0){
            //ADD HOW MANY TIMES IT WAS FOUND
            for(int i = 0; i < grr[guess]; i++){
                rightDigits.push_back(char(aGuess[guess][0]));
                count[aGuess[guess][0]]++;
            }
        }
        else{
            digitsNotIn.push_back(aGuess[guess][0]);
        }
        //ACTUAL ITERATE CHECKER
        setDigit(sGuess, guess);
    }
    
    //IF YOU STILL DONT HAVE 4rr BY GUESS 8, THEN FILL THE REMAINING WITH 9's
    if(guess >= step1Guesses && int(rightDigits.size()) < 4){
        int diff = 4 - int(rightDigits.size());
        
        for(int i = 0; i < diff; i++){
            count['9']++;
            rightDigits.push_back('9');
        }
    } 
    
//STEP 2 - FIND POSITION OF FIRST DIGIT
    static char cachedRR;
    if(guess >= step1Guesses || rightDigits.size() >= code_length){ 
        char safeChar = get(digitsNotIn);
        sGuess = "";
        for(int i = 0; i < int(rightDigits.size()); i++){
            sGuess += rightDigits[i];
        }
        
        static int digitOn = 0;
        static int numbersPassed = 0;
        
        //IN THIS USE CASE, THE INDEX OF THE VECTOR IS THE CORRECT POSITION OF THE DIGIT IN THAT INDEX
        //i.e vec(0) = 4 MEANS THAT THE FIRST CHARACTER OF THE STRING IS 4.
        //i.e vec(1) = 2 MEANS THAT THE SECOND CHARACTER OF THE STRING IS 4.
        static char dontChange[4] = {'r','r','r','r'};
        
        //CHECK IF YOU GOT IT
        if(std::find(std::begin(dontChange), std::end(dontChange), 'r') == std::end(dontChange)){
            sGuess[0] = dontChange[0];
            sGuess[1] = dontChange[1];
            sGuess[2] = dontChange[2];
            sGuess[3] = dontChange[3];
            
            aGuess[++guess]=sGuess;
            return sGuess;
        }
                
        static bool buffer = true;
       if(!buffer){
            if(grr[guess] == 0){
                numbersPassed++;
            }
            else{ //IF THE LAST GUESS WAS RIGHT,
                dontChange[numbersPassed] = int(rightDigits[digitOn]);
                count[sGuess[digitOn]]--;      
                
                if(count[sGuess[digitOn]] <= 0 || numbersPassed == 3){
                    numbersPassed = 0;
                }
                else{
                    numbersPassed++;
                }
                digitOn++; //MOVE ON TO THE NEXT DIGIT
            }
        }else
            buffer = false;
        
        //IN HERE SOMEWHERE
        
        if(digitOn <= 3){
            for(int i = 0; i < sGuess.length(); i++){
                if(i != numbersPassed){
                    sGuess[i] = int(safeChar);
                }else{
                    sGuess[i] = int(rightDigits[digitOn]);
                }
            }
        }
    }
            
    aGuess[++guess]=sGuess;
    
    //Return the result
    return sGuess;
}

char get(vector<char> vec){
    char temp;
    for(char chars: vec)
        temp = chars;
    return temp;
}

void setDigit(string& str, int num){
    if(num == 0){
        str = "0000"; 
        return;
    }
    else{
        str = "";
        
        for(int i = 0; i < 4; i++){
            str += num + '0';
        }
    }
    
    //cout << str << '\n';
}

bool eval(string code,string guess,char &rr,char &rw){
    //cout << "CODE IS: " << code << '\n';
    string check="    ";
    rr=0,rw=0;
    //Check how many are right place
    for(int i=0;i<code.length();i++){
        if(code[i]==guess[i]){
            rr++;
            check[i]='x';
            guess[i]='x';
        }
    }
    //Check how many are wrong place
    for(int j=0;j<code.length();j++){
        for(int i=0;i<code.length();i++){
            if((i!=j)&&(code[i]==guess[j])&&(check[i]==' ')){
                rw++;
                check[i]='x';
                break;
            }
        }
    }
    
    //Found or not
    if(rr==4)return false;
    return true;
}

string set(){
    string code="0000";
    for(int i=0;i<code.length();i++){
        code[i]=rand()%10+'0';
    }
    return code;
}