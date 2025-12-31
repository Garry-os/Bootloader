org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

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
	jmp 0x0000:.reset_cs
.reset_cs:
	;; Reset segments & setup stack
	xor ax, ax
	mov es, ax
	mov ds, ax

	mov ss, ax
	mov sp, 0x7C00 ; Stack grows downward
	
	sti

	;; Save boot drive
	mov [boot_drive], dl ;; BIOS should set dl to boot disk
	
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
	; Begin to load stage2
	mov si, msg_load
	call print

	mov eax, STAGE2_LOAD_LBA
	mov cl, STAGE2_LOAD_COUNT
	mov dl, [boot_drive]
	
	; mov ax, STAGE2_LOAD_SEGMENT
	; mov es, ax
	mov bx, STAGE2_LOAD_OFFSET

	;; Perform disk read
	call disk_read

	;; Jump to stage2
	jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET

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
	mov [extension_dap.count], cl

	mov ah, 0x42 ; Disk read
	mov si, extension_dap
	
	;; Retry for 3 times
	mov di, 3
.retry:
	pusha
	stc
	int 0x13

	jnc .done ; Succeed

	; Failed, retry
	popa
	call disk_reset

	dec di
	test di, di
	jnz .retry

.fail:
	;; Disk read failed
	jmp disk_error

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

disk_reset:
	pusha
	mov ah, 0 ; Disk reset interrupts
	stc
	int 0x13
	jc disk_error
	popa
	ret

disk_error:
	mov si, msg_disk_error
	call print

	cli
	hlt

print:
	push si
	push ax
	push bx
.loop:
	lodsb ;; Load next character in al
	or al, al ; If next character == NULL?
	jz .done

	mov ah, 0x0E
	mov bh, 0
	int 0x10

	jmp .loop
.done:
	pop bx
	pop ax
	pop si
	ret

;; Extension data address packet structure
extension_dap:
	.size:    db 0x10
		      db 0 ; Unused
	.count:   dw 0
	.offset:  dw 0
	.segment: dw 0
	.lba:     dq 0

STAGE2_LOAD_LBA		equ 1
STAGE2_LOAD_COUNT	equ 32 ; 16 Kib
STAGE2_LOAD_SEGMENT equ 0x0000
STAGE2_LOAD_OFFSET  equ 0x8000

boot_drive: db 0

msg_disk_error: db "Disk error!", ENDL, 0
msg_load: db "Loading stage2", ENDL, 0

times 510 - ($ - $$) db 0
dw 0xAA55 ;; BIOS magic


