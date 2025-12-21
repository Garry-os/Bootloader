org 0x7C00
bits 16

jmp start
nop

;; FAT BPB
;; ============ BPB bytes ===========
;;
times 138-($-$$) db 0

start:
	;; Resetting cs by performing a jump
	cli
	cld
	jmp 0x000:.reset_cs
.reset_cs:
	;; Reset segments & setup stack
	xor ax, ax
	mov es, ax
	mov ds, ax

	mov ss, ax
	mov sp, 0x7C00 ; Stack grows downward
	
	sti
	
	;; Check disk extension
	mov ah, 0x41
	mov bx, 0x55AA
	stc
	int 0x13

	jc .no_disk_extension

	;; Verify
	cmp bx, 0xAA55
	jne .no_disk_extension
	
	jmp .after
.no_disk_extension:
	;; TODO: Print a message
	jmp .halt
.after:
	; Test disk read
	mov eax, 1
	mov cl, 1
	;; BIOS already set dl
	mov bx, 0x1000
	call disk_read

	; A test
	mov al, 'h'
	mov ah, 0x0E
	int 0x10

.halt:
	cli
	hlt

;; Parameters:
;;; EAX = LBA
;;; CL = sectors count
;;; DL: drive number
;;; ES:BX: buffer location 
disk_read:
	;; Save registers
	push eax
	push bx
	push cx
	push dx
	push si
	push di

	;; Setup extension DAP
	mov [extension_dap.lba], eax
	mov [extension_dap.segment], es
	mov [extension_dap.offset], bx
	mov [extension_dap.size], cl

	mov ah, 0x42 ; Disk read
	mov si, extension_dap
.read:
	pusha ; Save all registers
	stc
	int 0x13
	jnc .done ; Succeed

.fail:
	;; Disk read failed
	;; TODO: Print message
	hlt
	cli

.done:
	;; Restore registers
	popa

	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop eax

	ret


;; Extension data address packet structure
extension_dap:
	.size:    db 10
		      db 0 ; Unused
	.count:   dw 0
	.offset:  dw 0
	.segment: dw 0
	.lba:     dq 0

times 510 - ($ - $$) db 0
dw 0xAA55 ;; BIOS magic


