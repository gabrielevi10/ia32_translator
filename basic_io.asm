; Lê uma string e printa ela na tela

section .bss    
    string    resb 10       ; o maior int tem 10 digitos    
section .data    
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

_start:
    push string
    call input_string    

    ; Add um no tamanho pra printar com o \n
    ; por default eax considera a string sem o \n
    ; porem o \n é lido tambem caso seja util para print
    add eax,1

    push string
    push eax
    call print

    mov eax, 1
    mov ebx, 0
    int 80h    