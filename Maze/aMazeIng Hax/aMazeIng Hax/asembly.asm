.code
extern sendEmojiBackAddress:QWORD
extern sendEmojiValue:QWORD
EmojiHook proc
	mov rdx, [sendEmojiValue]
	push rdi
	sub rsp,50
	jmp [sendEmojiBackAddress]
EmojiHook endp
end