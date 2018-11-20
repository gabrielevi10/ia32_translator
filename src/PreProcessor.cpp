#include "PreProcessor.hpp"

using namespace std;

PreProcessor::PreProcessor() {}

PreProcessor::~PreProcessor() {}

void add_to_equ_map(string line, map<string, int> &map) {
    string label;
    int equ_value;

    // Pega o rótulo
    label = line.substr(0,line.find(':'));
    // Pega o valor do equate
    equ_value = stoi(line.substr(line.find("equ ")+4,line.length()));
    
    // Adiciona ao mapa
    map[label] = equ_value;
}

// Pega o valor do equ correspondente ao 'if'
int get_correspondant_equ(string line, map<string, int> &map) {
    // Pega o rótulo
    string label = line.substr(line.find("if ")+ 3, line.length());
    return map[label];
}

// Passagem zero do montador
void PreProcessor::passage_zero(string program_name) {
    // Conta as linhas originais
    int original_line_counter = 1;
    // Conta as linhas do arquivo pre-processado
    int pre_line_counter = 1;
    string line;
    ifstream myfile (program_name + ".asm");    
    ofstream pre_processed;
    // Map contendo os rótulos dos equs e seus respectivos valores
    map<string, int> equ_map;    

    if (myfile.is_open()) {
        // Abre o arquivo de pre-processamento
        pre_processed.open(program_name + ".pre");
        while (getline (myfile,line)) {            
            // Formata a linha
            string clear_line = format_line(line);

            // Se ela for do tipo equate adiciona ao mapa
            if (clear_line.find("equ") != string::npos) {
                add_to_equ_map(clear_line,equ_map);                        
                // lines_relations[pre_line_counter] = original_line_counter;
            }
            // Se for do tipo 'if', veja o valor do equate 
            else if (clear_line.find("if") != string::npos) {
                if (!get_correspondant_equ(clear_line,equ_map)) {
                    // Quando equ = 0 jogue fora a próxima linha
                    getline (myfile,line);
                    original_line_counter++;
                }
            }
            // Troca as ocorrencias do equ nas seções const
            else if (clear_line.find("const") != string::npos) {                
                for(auto i : equ_map) {
                    int occurrence = clear_line.find(i.first);
                    if(occurrence != string::npos) {
                        // Substituição devida(valor do equ)
                        string subs = to_string(i.second);
                        // Troca
                        clear_line.replace(occurrence, i.first.length(), subs);                        
                    }
                }
                pre_processed << clear_line << endl;

                // lines_relations[pre_line_counter] = original_line_counter;                
                pre_line_counter++;
            }
            else if (clear_line != "") {
                // Para as outras instruções só copiar a linha formatada para o arquivo novo
                // lines_relations[pre_line_counter] = original_line_counter;
                // pre_processed << original_line_counter << ' ' << pre_line_counter << ' ' << clear_line << endl;
                pre_processed << clear_line << endl;
                pre_line_counter++;
            }            

            original_line_counter++;
        }
        myfile.close();
        pre_processed.close();        
    }

    else {
        cout << "Arquivo \'" << program_name + ".asm\'" << " não existe em memória\n"; 
        exit(0);
    }
}