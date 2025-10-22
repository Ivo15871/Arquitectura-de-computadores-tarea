.text
.globl __start
__start:
    # Mostrar cadena original
    la $a0, prm1
    li $v0, 4
    syscall
    la $a0, orig
    li $v0, 4
    syscall

    # Inicialización
    la $s0, orig          # Dirección de la cadena
    li $t0, 0             # Contador de posición
    li $t2, 1             # Flag: 1 = próximo carácter debe verificarse
    li $t6, 0x20          # ASCII space
    li $t7, 0x61          # ASCII 'a'
    li $t8, 0x7a          # ASCII 'z'
    add $t0, $t0, $s0     # Posición actual en cadena

loop:
    lb $t1, 0($t0)        # Cargar byte actual
    beq $t1, $zero, endLoop # Fin si carácter nulo
    
    # Verificar si es primer carácter o después de espacio
    beq $t2, $zero, check_space
    
    # Verificar si es minúscula (a-z o á,é,í,ó,ú,ñ)
    # Primero, rango a-z
    slt $t3, $t1, $t7     # t3=1 si t1 < 'a'
    slt $t4, $t8, $t1     # t4=1 si t1 > 'z'
    or $t3, $t3, $t4      # t3=1 si fuera de a-z
    beq $t3, $zero, is_lower  # Si está en a-z, convertir
    
    # Verificar caracteres con tildes y ñ
    li $t5, 0xE1          # á
    beq $t1, $t5, is_accent
    li $t5, 0xE9          # é
    beq $t1, $t5, is_accent
    li $t5, 0xED          # í
    beq $t1, $t5, is_accent
    li $t5, 0xF3          # ó
    beq $t1, $t5, is_accent
    li $t5, 0xFA          # ú
    beq $t1, $t5, is_accent
    li $t5, 0xF1          # ñ
    beq $t1, $t5, is_accent
    j not_lower           # No es minúscula ni acentuada

is_accent:
is_lower:
    addi $t1, $t1, -32    # Convertir a mayúscula (válido para a-z y á,é,í,ó,ú,ñ)
    sb $t1, 0($t0)        # Guardar carácter modificado
    
not_lower:
    li $t2, 0             # Resetear flag
    
check_space:
    bne $t1, $t6, not_space
    li $t2, 1             # Marcar que próximo carácter debe verificarse
    
not_space:
    addi $t0, $t0, 1      # Siguiente carácter
    j loop

endLoop:
    # Mostrar resultado
    la $a0, prm2
    li $v0, 4
    syscall
    la $a0, orig
    li $v0, 4
    syscall
    
    # Terminar programa
    li $v0, 10
    syscall

.data
orig: .asciiz "la vida es una sola, la traición es total, quiero café"  # Cadena con tildes
prm1: .asciiz "Original: "
prm2: .asciiz "\nUpcased : "