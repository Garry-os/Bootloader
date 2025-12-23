org 0x8000
bits 16

entry:
	mov ah, 0x0E
	mov al, 'e'
	int 0x10
.halt:
	cli
	hlt

