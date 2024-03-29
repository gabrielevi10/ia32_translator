section .bss    
    string      resb 10       ; o maior int tem 10 digitos 
    aux_string  resb 10  
    cvstring    resb 10
    some_var    resb 10
section .data    
    soma dd 0
    newline db 0x0A
    overflow_msg db 'Overflow!', 0x0A
    overflow_msg_size EQU $-overflow_msg
section .text
global _start

; Params em stack = (string, len_string)
; eax = numero convertido
convert_int:
    enter 0, 0
    
    push ebx
    push ecx
    push edx
    
    mov eax, [ebp + 8]              ; eax = tamanho da string
    mov ebx, [ebp + 12]             ; ebx = *string

    mov dl, [ebx]
    cmp dl, 0x2D        ; vê se é negativo
    jne begin_conversion

    inc ebx             ; se for, ignore o caracter '-'
    add eax, -1         ; um num a menos no loop
begin_conversion:
    mov ecx, eax
    add ecx, -1         ; loop <- size-1    
    mov dl, [ebx]
    sub dl, 0x30        ; dl = digito

    movzx eax, dl       ; valor(eax) = digito
    cmp ecx,0
    je end_conversion

    lp:
        mov edx, 10         
        mul edx             ; valor*=10

        inc ebx
        mov dl,[ebx]
        sub dl, 0x30        ; Lê o prox digito em dl

        movzx edx, dl
        add eax, edx        ; valor += digito

        loop lp             ; repete

end_conversion:
    mov ebx,[ebp+12]         ; ao fim vê se é negativo o input
    mov dl, [ebx]
    cmp dl, 0x2D
    jne return

    neg eax                 ; se for, nega o retorno(eax)

return:
    pop edx
    pop ecx
    pop ebx

    leave 
    ret 8



; params = (endereco da string, valor a ser convertido)
convert_string:             
    enter 0, 0              ; inicia o frame de pilha

    push eax                ; salva todos os registradores utilizados
    push ebx
    push ecx
    push edx

    mov eax, [ebp + 8]      ; eax = Valor
    mov ebx, [ebp + 12]     ; ebx = str

    sub ecx, ecx            ; i = 0
    cmp eax, 0              
    jge do_while            ; se eax >= 0, pula para do
    mov byte [ebx], 0x2d    ; adiciona o caractere '-' se for negativo
    neg eax
    inc ecx 

    do_while:
        push ebx            ; salva o endereço de retorno
        cdq                 ; extende o sinal de eax em edx
        mov ebx, 10
        idiv ebx            ; Valor % 10 -> edx, Valor/10 -> eax
        add dl, 0x30        ; dl = (Valor % 10) + 0x30

        pop ebx
        mov byte [ebx + ecx], dl ; str[i] = dl
        
        cmp eax, 0
        jle eq_zero
        mov dl, [ebx]
        mov byte [ebx + ecx + 1], dl  ; str[i+1] = str[i]

        eq_zero:        
            inc ecx
    
    while: 
        cmp eax, 0
        jne do_while

    end_do_while:
        mov byte [ebx + ecx], 0x0

    push ebx
    push ecx
    push cvstring
    call reverse_string

    pop edx
    pop ecx
    pop ebx
    
    mov eax, ecx            ; return= tamanho da string de saida

    leave 
    ret 8

; params: endereco da string, tamanho, string de saida
reverse_string:                     
    enter 0, 0

    pusha

    mov esi, [ebp + 16]              ; esi = endereco da string de entrada
    mov ecx, [ebp + 12]             ; ecx = tamanho
    mov edi, [ebp + 8]              ; edi = endereco da string de saida             
    mov edx, 0                      ; indice da string de saida
    sub ecx, 1                      ; ecx = indice no final da string de entrada

    mov ebx, ecx

    cmp byte [esi], 0x2d
    jne while_rv
    mov byte [edi], 0x2d
    inc edx

    while_rv:
        cmp edx, ebx                     ; compara edx com o indice no final da string, para saber se ja chegou ao fim 
        jg end_while_rv             
    
        mov al, [esi + ecx]
        mov byte [edi + edx], al

        inc edx
        sub ecx, 1

        

        jmp while_rv 
    
    end_while_rv:
        mov byte [edi + ebx + 1], 0x0
        popa
    
    leave
    ret 12

; param = endereço do numero a se multiplicar pelo eax
multiply:    
    enter 0,0
    
    push ebx
    push edx

    mov ebx, [ebp + 8]
    mov ebx, [ebx]

    mov edx, 0
    
    imul ebx    

    cmp edx, 0          ; se edx tem alguma coisa é porque deu overflow
    je end_multiply
    cmp edx, 0xFFFFFFFF ; se edx tambem não for extensão de sinal é pq deu overflow
    je end_multiply
    
    jmp overflow

end_multiply:
    pop edx
    pop ebx

    leave
    ret 4

overflow:
    push overflow_msg
    push overflow_msg_size
    call PrintString

    mov eax, 1
    mov ebx, 0
    int 80h

; param = endereço do numero a se dividir pelo eax
divide:
    enter 0,0
    
    push ebx
    push edx

    mov ebx, [ebp + 8]
    mov ebx, [ebx]      ; ebx = inteiro(divisor)

    cdq                 ; extende o sinal de eax para edx

    idiv ebx            ; divide e retorna

    pop edx
    pop ebx

    leave
    ret 4

ReadInt:
    enter 0, 0

    call clear_string

    push ebx

    mov ebx, [ebp + 8]  ; ebx = endereco do int
    push ebx
    call ReadString
    push ebx
    push eax
    call convert_int

    pop ebx

    leave
    ret 4

PrintInt:
    enter 0, 0

    call clear_string

    push ebx
    mov ebx, [ebp + 8] 

    push aux_string
    push ebx
    call convert_string

    push cvstring
    push dword 10
    call PrintString

    pop ebx
    leave
    
    ret 4

ReadChar:
    enter 0,0

    push eax
    push ebx
    push ecx
    push edx

    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 8]   ;endereco do char
    mov edx, 1
    int 80h

    pop eax
    pop ebx
    pop ecx
    pop edx

    leave
    ret 4

PrintChar:
    enter 0,0

    push eax
    push ebx
    push ecx
    push edx

    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 8]     ; endereco da string a ser printada
    mov edx, 1             ; tamanho da string
    int 80h

    pop eax
    pop ebx
    pop ecx
    pop edx

    leave
    ret 4

; params  = (endereço da string)
; retorno = eax <- tamanho da string lida
ReadString:
    enter 0, 0

    push ebx
    push ecx
    push edx                        ; Salva os registradores não usados

    mov ecx, [ebp + 8]              ; ecx = *(string)
        
    mov  eax, 0          
    push eax                        ; Coloca o acumulador(tamanho da string) na pilha
    input_loop:        
        mov eax, 3
        mov ebx, 0
        mov edx, 1
        int 80h                     ; syscall de input(lê um digito)
        
        cmp byte [ecx], 0x0A
        je  end_input               ; Se não for \n continua lendo        

        inc ecx                     ; incrementa endereço da string
        pop ebx                     ; desempilha tamanho
        add ebx, 1                  
        push ebx                    ; incrementa tamanho e empilha de volta

        jmp input_loop              ; Volta pra loop

    end_input:
    pop eax                         ; Desempilha tamanho

    pop edx
    pop ecx
    pop ebx                         ; Resgata registradores não usados
    
    leave                        ; Retorna
    ret 4
    
; params = (endereco da string, tamanho)
PrintString:
    enter 0, 0              

    push ebx
    push ecx
    push edx

    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 12]     ; endereco da string a ser printada
    mov edx, [ebp + 8]      ; tamanho da string
    int 80h

    pop edx
    pop ecx
    pop ebx

    mov eax, [ebp + 8]      ; return= tamanho da string

    leave
    ret 8

clear_string:
    enter 0, 0
    
    push ecx
    sub ecx, ecx

    for1:
        cmp ecx, 10
        je endfor1

        mov byte [aux_string + ecx], 0x0
        mov byte [cvstring + ecx], 0x0
        mov byte [string + ecx], 0x0
        mov byte [some_var + ecx], 0x0

        inc ecx

        jmp for1

    endfor1:
    pop ecx
    leave
    ret 4
    

_start:
    push some_var
    call ReadInt

    ; push some_var
    ; push eax
    ; call convert_int
    
    ; mov dword [some_var], 10
    ; push some_var
    ; call multiply    

    ; mov dword [some_var], 2
    ; push some_var
    ; call divide    


    push eax
    call PrintInt
    

    push some_var
    call ReadInt

    push eax
    call PrintInt

    mov eax, 1
    mov ebx, 0
    int 80h    
    
error: