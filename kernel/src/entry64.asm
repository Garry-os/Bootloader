bits 64

global long_mode_start
long_mode_start:
	; Setup data registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	;; Print something
	mov byte [0xb8000], 'A'
.halt:
	cli
	hlt
