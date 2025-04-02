extern exception_handler
extern irq_handler

; ----------------------------------------
; Exception Stubs (0 - 31)
; ----------------------------------------

%macro isr_err_stub 1
isr_stub_%+%1:
  call exception_handler
  iret
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
  call exception_handler
  iret
%endmacro

; Generate exception stubs
%assign i 0
%rep 32
  %if i = 8 || i = 10 || i = 11 || i = 12 || i = 13 || i = 14 || i = 17 || i = 30
    isr_err_stub i
  %else
    isr_no_err_stub i
  %endif
%assign i i+1
%endrep

; ----------------------------------------
; IRQ Stubs (32 - 47)
; ----------------------------------------

%macro irq_stub 1
isr_stub_%+%1:
  push dword %1      ; Pass IRQ number as argument
  call irq_handler
  add esp, 4         ; Clean up pushed argument
  iret
%endmacro

; Generate IRQ stubs
%assign i 32
%rep 16
  irq_stub i
%assign i i+1
%endrep

; ----------------------------------------
; Stub Table (0 - 47)
; ----------------------------------------

global isr_stub_table
isr_stub_table:
%assign i 0
%rep 48
  dd isr_stub_%+i
%assign i i+1
%endrep

; ----------------------------------------
; Load IDT function
; ----------------------------------------

global idt_load
idt_load:
  mov edx, [esp + 4]
  lidt [edx]
  sti
  ret
