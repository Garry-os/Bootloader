org 0x7C00
bits 16

main:
	;; Reset segments & setup stack
	xor ax, ax
	mov es, ax
	mov ds, ax

	mov ss, ax
	mov sp, 0x7C00 ; Stack grows downward

.halt:
	cli
	hlt

times 510 - ($ - $$) db 0
dw 0xAA55 ;; BIOS magic


