#include "initializer.hpp"

using namespace std;

void initialize_file(ofstream &output_file) {
    output_file << "section .bss" << endl
                << "    string resb 10" << endl
                << "    aux_string resb 10" << endl
                << "    cvstring resb 10" << endl
                << "    some_var resb 10" << endl
                << "section .data" << endl
                << "    soma dd 0" << endl
                << "    newline db 0x0a" << endl
                << "    overflow_msg db 'Overflow!', 0x0A" << endl
                << "    overflow_msg_size EQU $-overflow_msg" << endl
                << "section .text" << endl
                << "global _start" << endl
                << endl
                << "ReadInt:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    call clear_string" << endl
                << endl
                << "    push ebx" << endl
                << endl
                << "    mov ebx, [ebp + 8]  ;ebx = endereco do int" << endl
                << endl
                << "    push ebx" << endl
                << "    call ReadString" << endl
                << endl
                << "    push ebx" << endl
                << "    push eax" << endl
                << "    call convert_int" << endl
                << endl
                << "    pop ebx" << endl
                << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "PrintInt:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    call clear_string" << endl
                << endl
                << "    push ebx" << endl
                << "    mov ebx, [ebp + 8]" << endl
                << endl
                << "    push aux_string" << endl
                << "    push ebx" << endl
                << "    call convert_string" << endl
                << endl
                << "    push cvstring" << endl
                << "    push dword 10" << endl
                << "    call PrintString" << endl
                << endl
                << "    pop ebx" << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "ReadChar:" << endl
                << "    enter 0,0" << endl
                << endl
                << "    push eax" << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx" << endl
                << endl
                << "    mov eax, 3" << endl
                << "    mov ebx, 0" << endl
                << "    mov ecx, [ebp + 8]   ;endereco do char" << endl
                << "    mov edx, 1" << endl
                << "    int 80h" << endl
                << endl
                << "    pop eax" << endl
                << "    pop ebx" << endl
                << "    pop ecx" << endl
                << "    pop edx" << endl
                << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "PrintChar:" << endl
                << "    enter 0,0" << endl
                << endl
                << "    push eax" << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx" << endl
                << endl
                << "    mov eax, 4" << endl
                << "    mov ebx, 1" << endl
                << "    mov ecx, [ebp + 8]     ; endereco da string a ser printada" << endl
                << "    mov edx, 1             ; tamanho da string" << endl
                << "    int 80h" << endl
                << endl
                << "    pop eax" << endl
                << "    pop ebx" << endl
                << "    pop ecx" << endl
                << "    pop edx" << endl
                << endl
                << "    call printNewLine" << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "; params  = (endereço da string)" << endl
                << "; retorno = eax <- tamanho da string lida" << endl
                << "ReadString:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx                        ; Salva os registradores não usados" << endl
                << endl
                << "    mov ecx, [ebp + 8]              ; ecx = *(string)" << endl
                << endl
                << "    mov  eax, 0" << endl
                << "    push eax                        ; Coloca o acumulador(tamanho da string) na pilha" << endl
                << "    input_loop:" << endl
                << "        mov eax, 3" << endl
                << "        mov ebx, 0" << endl
                << "        mov edx, 1" << endl
                << "        int 80h                     ; syscall de input(lê um digito)" << endl
                << endl
                << "        cmp byte [ecx], 0x0A" << endl
                << "        je  end_input               ; Se não for newline continua lendo" << endl
                << endl
                << "        inc ecx                     ; incrementa endereço da string" << endl
                << "        pop ebx                     ; desempilha tamanho" << endl
                << "        add ebx, 1" << endl
                << "        push ebx                    ; incrementa tamanho e empilha de volta" << endl
                << endl
                << "        jmp input_loop              ; Volta pra loop" << endl
                << endl
                << "    end_input:" << endl
                << "    pop eax                         ; Desempilha tamanho" << endl
                << endl
                << "    pop edx" << endl
                << "    pop ecx" << endl
                << "    pop ebx                         ; Resgata registradores não usados" << endl
                << endl
                << "    leave                        ; Retorna" << endl
                << "    ret 4" << endl
                << endl
                << endl
                << "; params = (endereco da string, tamanho)" << endl
                << "PrintString:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx" << endl
                << endl
                << "    mov eax, 4" << endl
                << "    mov ebx, 1" << endl
                << "    mov ecx, [ebp + 12]     ; endereco da string a ser printada" << endl
                << "    mov edx, [ebp + 8]      ; tamanho da string" << endl
                << "    int 80h" << endl
                << endl
                << "    pop edx" << endl
                << "    pop ecx" << endl
                << "    pop ebx" << endl
                << endl
                << "    mov eax, [ebp + 8]      ; return= tamanho da string" << endl
                << endl
                << "    call printNewLine" << endl
                << "    leave" << endl
                << "    ret 8" << endl
                << endl
                << "convert_int:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx" << endl
                << endl
                << "    mov eax, [ebp + 8]              ; eax = tamanho da string" << endl
                << "    mov ebx, [ebp + 12]             ; ebx = *string" << endl
                << endl
                << "    mov dl, [ebx]" << endl
                << "    cmp dl, 0x2D        ; vê se é negativo" << endl
                << "    jne begin_conversion" << endl
                << endl
                << "    inc ebx             ; se for, ignore o caracter '-'" << endl
                << "    add eax, -1         ; um num a menos no loop" << endl
                << "    begin_conversion:" << endl
                << "        mov ecx, eax" << endl
                << "        add ecx, -1         ; loop <- size-1" << endl
                << "        mov dl, [ebx]" << endl
                << "        sub dl, 0x30        ; dl = digito" << endl
                << endl
                << "        movzx eax, dl       ; valor(eax) = digito" << endl
                << "        cmp ecx,0" << endl
                << "        je end_conversion" << endl
                << endl
                << "        lp:" << endl
                << "            mov edx, 10" << endl
                << "            mul edx             ; valor*=10" << endl
                << endl
                << "            inc ebx" << endl
                << "            mov dl,[ebx]" << endl
                << "            sub dl, 0x30        ; Lê o prox digito em dl" << endl
                << endl
                << "            movzx edx, dl" << endl
                << "            add eax, edx        ; valor += digito" << endl
                << endl
                << "            loop lp             ; repete" << endl
                << endl
                << "    end_conversion:" << endl
                << "    mov ebx,[ebp+12]         ; ao fim vê se é negativo o input" << endl
                << "    mov dl, [ebx]" << endl
                << "    cmp dl, 0x2D" << endl
                << "    jne return" << endl
                << endl
                << "    neg eax                 ; se for, nega o retorno(eax)" << endl
                << endl
                << "    return:" << endl
                << "        pop edx" << endl
                << "        pop ecx" << endl
                << "        pop ebx" << endl
                << endl
                << "    leave" << endl
                << "    ret 8" << endl
                << endl
                << "; params = (endereco da string, valor a ser convertido)" << endl
                << "convert_string:" << endl
                << "    enter 0, 0              ; inicia o frame de pilha" << endl
                << endl
                << "    push eax                ; salva todos os registradores utilizados" << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push edx" << endl
                << endl
                << "    mov eax, [ebp + 8]      ; eax = Valor" << endl
                << "    mov ebx, [ebp + 12]     ; ebx = str" << endl
                << endl
                << "    sub ecx, ecx            ; i = 0" << endl
                << "    cmp eax, 0" << endl
                << "    jge do_while            ; se eax >= 0, pula para do" << endl
                << "    mov byte [ebx], 0x2d    ; adiciona o caractere '-' se for negativo" << endl
                << "    neg eax" << endl
                << "    inc ecx" << endl
                << endl
                << "    do_while:" << endl
                << "        push ebx            ; salva o endereço de retorno" << endl
                << "        cdq                 ; extende o sinal de eax em edx" << endl
                << "        mov ebx, 10" << endl
                << "        idiv ebx            ; Valor % 10 -> edx, Valor/10 -> eax" << endl
                << "        add dl, 0x30        ; dl = (Valor % 10) + 0x30" << endl
                << endl
                << "        pop ebx" << endl
                << "        mov byte [ebx + ecx], dl ; str[i] = dl" << endl
                << endl
                << "        cmp eax, 0" << endl
                << "        jle eq_zero" << endl
                << "        mov dl, [ebx]" << endl
                << "        mov byte [ebx + ecx + 1], dl  ; str[i+1] = str[i]" << endl
                << endl
                << "        eq_zero:" << endl
                << "            inc ecx" << endl
                << endl
                << "    while:" << endl
                << "        cmp eax, 0" << endl
                << "        jne do_while" << endl
                << endl
                << "    end_do_while:" << endl
                << "        mov byte [ebx + ecx], 0x0" << endl
                << endl
                << "    push ebx" << endl
                << "    push ecx" << endl
                << "    push cvstring" << endl
                << "    call reverse_string" << endl
                << endl
                << "    pop edx" << endl
                << "    pop ecx" << endl
                << "    pop ebx" << endl
                << endl
                << "    mov eax, ecx            ; return= tamanho da string de saida" << endl
                << endl
                << "    leave" << endl
                << "    ret 8" << endl
                << endl
                << "; params: endereco da string, tamanho, string de saida" << endl
                << "reverse_string:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    pusha" << endl
                << endl
                << "    mov esi, [ebp + 16]              ; esi = endereco da string de entrada" << endl
                << "    mov ecx, [ebp + 12]             ; ecx = tamanho" << endl
                << "    mov edi, [ebp + 8]              ; edi = endereco da string de saida" << endl
                << "    mov edx, 0                      ; indice da string de saida" << endl
                << "    sub ecx, 1                      ; ecx = indice no final da string de entrada" << endl
                << endl
                << "    mov ebx, ecx" << endl
                << endl
                << "    cmp byte [esi], 0x2d" << endl
                << "    jne while_rv" << endl
                << "    mov byte [edi], 0x2d" << endl
                << "    inc edx" << endl
                << endl
                << "    while_rv:" << endl
                << "        cmp edx, ebx                     ; compara edx com o indice no final da string, para saber se ja chegou ao fim" << endl
                << "        jg end_while_rv" << endl
                << endl
                << "        mov al, [esi + ecx]" << endl
                << "        mov byte [edi + edx], al" << endl
                << endl
                << "        inc edx" << endl
                << "        sub ecx, 1" << endl
                << endl
                << "        jmp while_rv" << endl
                << endl
                << "    end_while_rv:" << endl
                << "        mov byte [edi + ebx + 1], 0x0" << endl
                << "        popa" << endl
                << endl
                << "    leave" << endl
                << "    ret 12" << endl
                << endl
                << "; param = endereço do numero a se multiplicar pelo eax" << endl
                << "multiply:" << endl
                << "    enter 0,0" << endl
                << endl
                << "    push ebx" << endl
                << "    push edx" << endl
                << endl
                << "    mov ebx, [ebp + 8]" << endl
                << "    mov ebx, [ebx]" << endl
                << endl
                << "    mov edx, 0" << endl
                << endl
                << "    imul ebx" << endl
                << endl
                << "    cmp edx, 0          ; se edx tem alguma coisa é porque deu overflow" << endl
                << "    je end_multiply" << endl
                << "    cmp edx, 0xFFFFFFFF ; se edx tambem não for extensão de sinal é pq deu overflow" << endl
                << "    je end_multiply" << endl
                << endl
                << "    jmp overflow" << endl
                << endl
                << "    end_multiply:" << endl
                << "    pop edx" << endl
                << "    pop ebx" << endl
                << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "    overflow:" << endl
                << "        push overflow_msg" << endl
                << "        push overflow_msg_size" << endl
                << "        call PrintString" << endl
                << endl
                << "        mov eax, 1" << endl
                << "        mov ebx, 0" << endl
                << "        int 80h" << endl
                << endl
                << "; param = endereço do numero a se dividir pelo eax" << endl
                << "divide:" << endl
                << "    enter 0,0" << endl
                << endl
                << "    push ebx" << endl
                << "    push edx" << endl
                << endl
                << "    mov ebx, [ebp + 8]" << endl
                << "    mov ebx, [ebx]      ; ebx = inteiro(divisor)" << endl
                << endl
                << "    cdq                 ; extende o sinal de eax para edx" << endl
                << endl
                << "    idiv ebx            ; divide e retorna" << endl
                << endl
                << "    pop edx" << endl
                << "    pop ebx" << endl
                << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "clear_string:" << endl
                << "    enter 0, 0" << endl
                << endl
                << "    push ecx" << endl
                << "    sub ecx, ecx" << endl
                << endl
                << "    for1:" << endl
                << "        cmp ecx, 10" << endl
                << "        je endfor1" << endl
                << endl
                << "        mov byte [aux_string + ecx], 0x0" << endl
                << "        mov byte [cvstring + ecx], 0x0" << endl
                << "        mov byte [string + ecx], 0x0" << endl
                << "        mov byte [some_var + ecx], 0x0" << endl
                << endl
                << "        inc ecx" << endl
                << endl
                << "    jmp for1" << endl
                << endl
                << "    endfor1:" << endl
                << "    pop ecx" << endl
                << "    leave" << endl
                << "    ret 4" << endl
                << endl
                << "printNewLine:" << endl
                << "    enter 0,0" << endl
                << "    pusha" << endl
                << "    mov eax, 4" << endl
                << "    mov ebx, 1" << endl
                << "    mov ecx, newline" << endl
                << "    mov edx, 1" << endl
                << "    int 80h" << endl
                << "    popa" << endl
                << "    leave" << endl
                << "    ret" << endl
                << endl
                << "_start:" << endl;
}
