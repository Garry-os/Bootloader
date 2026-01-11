bits 64

section .text

extern main

global long_mode_start
long_mode_start:
	; Setup data registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	;; Call the main C function
	call main

.halt:
	cli
	hlt
