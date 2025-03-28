SECTION .DATA
gdtr DW 0 ; Limit
     DD 0 ; Base
 
SECTION .TEXT
global gdt_flush

; Parameters: Address of the GDT
gdt_flush:
  mov eax, [esp+4]
  lgdt [eax]

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  jmp 0x08:.flush
.flush:
  ret