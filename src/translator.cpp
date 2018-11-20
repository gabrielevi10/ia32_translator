#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "PreProcessor.hpp"
#include "helpers.hpp"
#include "initializer.hpp"

using namespace std;

void translate(string f) {
    string file_line;
    ifstream file(f + ".pre");
    ofstream converted_file(f + ".s");
    Line line;
    string label;
    string opcode;
    vector<string> operands;
    int count = 0;
    
    initialize_file(converted_file);

    while(getline(file, file_line)) {
        line = token_separator(file_line);
        label = line.get_label();
        opcode = line.get_opcode();
        operands = line.get_operands();

        if (label.size() > 0 && opcode != "space" && opcode != "const") {
            converted_file << label << ":" << endl;
        }
        if (opcode == "add") {
            converted_file << "    add eax, [" << operands[0] << "]" << endl << endl; 
        }
        else if (opcode == "sub") {
            converted_file << "    sub eax, [" << operands[0] << "]" << endl << endl;
        }
        else if (opcode == "mult") {
            converted_file << "    push " << operands[0] << endl
                           << "    call multiply" << endl << endl;
        }
        else if (opcode == "div") {
            converted_file << "    push " << operands[0] << endl
                           << "    call divide" << endl << endl;
        }
        else if (opcode == "jmp") {
            converted_file << "    jmp" << operands[0] << endl << endl;
        }
        else if (opcode == "jmpn") {
            converted_file << "    cmp eax, 0" << endl
                           << "    jl " << operands[0] << endl << endl;
        }
        else if (opcode == "jmpp") {
            converted_file << "    cmp eax, 0" << endl
                           << "    jg " << operands[0] << endl << endl;
        }
        else if (opcode == "jmpz") {
            converted_file << "    cmp eax, 0" << endl
                           << "    je " << operands[0] << endl << endl;
        }
        else if (opcode == "copy") {
            converted_file << "    mov ebx, [" << operands[0] << "]" << endl
                           << "    mov dword [" << operands[1] << "], ebx" << endl << endl;
        }
        else if (opcode == "load") {
            converted_file << "    mov eax, [" << operands[0] << "]" << endl << endl;
        }
        else if (opcode == "store") {
            converted_file << "    mov dword [" << operands[0] << "], eax" << endl << endl;
        }
        else if (opcode == "input") {
            converted_file << "    push eax" << endl
                           << "    push some_var" << endl
                           << "    call ReadInt"  << endl
                           << "    mov dword [" << operands[0] << "], eax" << endl
                           << "    pop eax" << endl << endl; 
        }
        else if (opcode == "output") {
            converted_file << "    push eax" << endl
                           << "    push dword [" << operands[0] << "]" << endl
                           << "    call PrintInt" << endl
                           << "    pop eax" << endl << endl;
        }
        else if (opcode == "c_input") {
            converted_file << "    push eax" << endl
                           << "    push " << operands[0] << endl
                           << "    call ReadChar" << endl
                           << "    pop eax" << endl << endl;
        }
        else if (opcode == "c_output") {
            converted_file << "    push eax" << endl
                           << "    push "<< operands[0] << endl
                           << "    call PrintChar" << endl
                           << "    pop eax" << endl << endl;
        }
        else if (opcode == "s_input") {
            converted_file << "    push eax" << endl
                           << "    push " << operands[0] << endl
                           << "    call ReadString" << endl
                           << "    mov dword [" << operands[1] << "], eax" << endl
                           << "    pop eax" << endl << endl;
        }
        else if (opcode == "s_output") {
            converted_file << "    push eax" << endl
                           << "    push " << operands[0] << endl
                           << "    push " << operands[1] << endl
                           << "    call PrintString" << endl
                           << "    pop eax" << endl << endl;
        }
        else if (opcode == "stop") {
            converted_file << "    mov eax, 1" << endl
                           << "    mov ebx, 0" << endl
                           << "    int 80h" << endl << endl;
        }
        else if (opcode == "section") {
            if (operands[0] == "text") {
                if (count > 0) 
                    converted_file << "section .text" << endl;
                count++;
            }
            else if (operands[0] == "bss")
                converted_file << "section .bss" << endl;
            else if (operands[0] == "data")
                converted_file << "section .data" << endl;
        }
        else if (opcode == "const") {
            converted_file << label << " dd " << operands[0] << endl;
        }
        else if (opcode == "space") {
            if (operands.size() > 0)
                converted_file << label << " resd " << operands[0] << endl;
            else
                converted_file << label << " resd " << 1 << endl;
        }
    }
    file.close();
    converted_file.close();

    remove((f + ".pre").c_str());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Passe um arquivo para traduzir." << endl;
    }
    else if (argc > 2) {
        cout << "Passe apenas um arquivo para traduzir.";
    }
    else {
        PreProcessor pre;
        pre.passage_zero(argv[1]);
        translate(argv[1]);
    }
    
    return 0;
}