bits 32

section .text

entry:
	mov [0xb8000], 'a'

	cli
	hlt

