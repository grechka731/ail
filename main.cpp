#include "lexer.h"
#include <iostream>
#include "string"
#include <vector>

using namespace std;

int main(){
    string input;
    getline(cin, input);
    Lexer l(input);

    l.readAllTokens();

    std::vector<Token>& array=l.getArray();

    for(int i=0;i<array.size();i++){
        cout << array[i].type << " " << array[i].word <<endl;
    }

    cout << "Успешно разобрано токенов: " << array.size() << endl;
    
}
