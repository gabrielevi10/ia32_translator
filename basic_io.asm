; Lê uma string e printa ela na tela

section .bss    
    string    resb 10       ; o maior int tem 10 digitos        
section .data    
    soma dd 0
    hw db 'HELLO MY FREN!', 0x0A
    hw_size EQU $-hw
section .text
global _start

;params em stack = ( string, len(string) )
print:
    pop ebp         ; salva o retorno em ebp    

    pop edx         ; primeiro pop no tamanho dela
    pop ecx         ; depois   pop a string

    push eax        ; salva o eax
    push ebx        ; salva o ebx

    mov eax, 4      ; system call:
    mov ebx, 1    
    int 80h

    pop ebx         ; resgata eax
    pop eax         ; resgata ebx

    push ebp        ; devolve o retorno à pilha
    ret


; params  = (endereço da string)
; retorno = eax <- tamanho da string lida
input_string:
    pop ebp
    pop eax                         ; eax = *(string)

    push ebx
    push ecx
    push edx                        ; Salva os registradores não usados

    mov ecx, eax                    ; ecx = *(string)
        
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
    
    push ebp                        ; Retorna
    ret

; Params em stack = (string, len_string)
; eax = numero convertido
convert_int:
    pop ebp
    
    pop eax             ; eax = tamanho da string
    pop esi             ; ebx = *string

    push ebx
    push ecx
    push edx

    mov ebx, esi

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

    lp:
        mov edx, 10         
        mul edx             ; valor*=10

        inc ebx
        mov dl,[ebx]
        sub dl, 0x30        ; Lê o prox digito em dl

        movzx edx, dl
        add eax, edx        ; valor += digito

        loop lp             ; repete

    mov ebx, string         ; ao fim vê se é negativo o input
    mov dl, [string]
    cmp dl, 0x2D
    jne end_conversion

    neg eax                 ; se for, nega o retorno(eax)

end_conversion:
    pop edx
    pop ecx
    pop ebx

    push ebp
    ret

_start:
    push string
    call input_string        

    mov ebx, 4
    mov ecx, 5
    mov edx, 6

    push string
    push eax
    call convert_int

    cmp eax,32769
    jne error 

    mov eax, 1
    mov ebx, 0
    int 80h    

    error: