	.file	"asgn1.c"			# source file name
	.text
	.section	.rodata			# read-only data section
	.align 8					# align with 8-byte boundary
.LC0:							# Label of f-string-1st printf
	.string	"Enter the string (all lowrer case): "
.LC1:							#  Label of f-string scanf
	.string	"%s"
.LC2:							# Label of f-string - 2nd printf
	.string	"Length of the string: %d\n"
	.align 8					# align with 8-byte boundary
.LC3:							# Label of f-string - 3rd printf
	.string	"The string in descending order: %s\n"
	.text						# Code starts
	.globl	main				# main is a global name
	.type	main, @function		# main is a function
main:							# main: starts
.LFB0:
	.cfi_startproc				# Call Frame Information
	endbr64
	pushq	%rbp				# Save old base pointer
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp			# rbp <-- rsp stack pointer is new base pointer
	.cfi_def_cfa_register 6
	subq	$80, %rsp			# Create space for local array and variables of main function
	movq	%fs:40, %rax		# allocating memory of 40 bytes for str and dest
	movq	%rax, -8(%rbp)		# rbp-8 <-- rax
	xorl	%eax, %eax			# eax <-- 0
	leaq	.LC0(%rip), %rdi	# first argument to the printf function which is lable LC0, which is string	"Enter the string (all lowrer case): "
	movl	$0, %eax			# eax <-- 0
	call	printf@PLT			# calling prinf function
	leaq	-64(%rbp), %rax		# rax <-- rbp-64 setting rax to str
	movq	%rax, %rsi			# rsi <-- rax (2nd argument <-- str)
	leaq	.LC1(%rip), %rdi	# rdi <-- starting of the format string LC1
	movl	$0, %eax			# eax <-- 0
	call	__isoc99_scanf@PLT	# call scanf
	leaq	-64(%rbp), %rax		# setting rax to str
	movq	%rax, %rdi			# rdi <-- starting of the format string str
	call	length				# calling length function
	movl	%eax, -68(%rbp)		# rbp-68 <-- eax
	movl	-68(%rbp), %eax		# eax <-- rbp-68
	movl	%eax, %esi			# storing eax (return value = len) to esi (2nd param)
	leaq	.LC2(%rip), %rdi	# rdi <-- starting of the format string LC2
	movl	$0, %eax			# eax <-- 0
	call	printf@PLT			# call printf function
	leaq	-32(%rbp), %rdx		# rdx <-- dest
	movl	-68(%rbp), %ecx		# ecx <-- rbp-68 (4th argument)		?
	leaq	-64(%rbp), %rax		
	movl	%ecx, %esi			# esi <-- ecx (2nd argument esi = len)
	movq	%rax, %rdi			# rdi <-- rax (first argument rdi = str, we are storing return value in it)
	call	sort				# call sort function
	leaq	-32(%rbp), %rax		# rax <-- rbp-32 (rax = dest)
	movq	%rax, %rsi			# rsi <-- dest 
	leaq	.LC3(%rip), %rdi	# rdi <-- LC3
	movl	$0, %eax			# eax <-- 0
	call	printf@PLT			# call printf
	movl	$0, %eax			# eax <-- 0
	movq	-8(%rbp), %rcx		# rcx <-- rbp-8
	xorq	%fs:40, %rcx		# dellocating memory of 40 bytes for str and dest
	je	.L3						# jump to L3
	call	__stack_chk_fail@PLT
.L3:
	leave						# leave or return from the stack frame of main function
	.cfi_def_cfa 7, 8			
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	length				# length is a global name
	.type	length, @function	# length is a function
length:							# length starts
.LFB1:
	.cfi_startproc				# Call Frame Information
	endbr64
	pushq	%rbp				# push base pointer into the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp			# rbp <-- rsp (means we are assigning the top of th stack to base pointer, i.e. top==base)
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)		# rbp-24 <-- rdi (str) (assigning memory for str+i)
	movl	$0, -4(%rbp)		# rbp-4 (i) <-- 0
	jmp	.L5						# jump to L5
.L6:
	addl	$1, -4(%rbp)		# rbp-4 += 1 (i.e. we are adding 1 in i, i++)
.L5:
	movl	-4(%rbp), %eax		# eax <-- rbp-4 (i.e. 32-bit return value = i)
	movslq	%eax, %rdx			# rdx <-- eax (i.e. 3rd parameter = eax = i)
	movq	-24(%rbp), %rax		# rax <-- rbp-24 (i.e. 64-bit return value = str)
	addq	%rdx, %rax			# going at the ith index str
	movzbl	(%rax), %eax		# loading the index value of str at ith index
	testb	%al, %al			# here it is taking AND between al and al itself and If al & al == 0, the zero flag will be set to 1.
	jne	.L6						#  jump to the address of L6, if the zero flag is 0. Otherwise, the jne will do nothing.
	movl	-4(%rbp), %eax		# eax <-- rbp-4 (i.e. eax (32-bit return value = i)), provided if we don't jump on L6
	popq	%rbp				# pop base pointer from the stack
	.cfi_def_cfa 7, 8
	ret							# return from the function 
	.cfi_endproc
.LFE1:
	.size	length, .-length
	.globl	sort
	.type	sort, @function
sort:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp				# push base pointer into stack		
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp			# rbp <-- rsp
	.cfi_def_cfa_register 6
	subq	$48, %rsp			# Create space for local array and variables of sort function
	movq	%rdi, -24(%rbp)		# rbp-24 <-- rdi (assigning memory for str)
	movl	%esi, -28(%rbp)		# rbp-28 <-- esi (assigning memory for len integer)
	movq	%rdx, -40(%rbp)		# rbp-40 <-- rdx (assigning memory for dest)
	movl	$0, -8(%rbp)		# rbp-8 <-- 0 (assigning i=0)
	jmp	.L9						# jump on L9
.L13:
	movl	$0, -4(%rbp)		#
	jmp	.L10
.L12:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %edx
	movl	-4(%rbp), %eax
	movslq	%eax, %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	jge	.L11
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -9(%rbp)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rcx
	movq	-24(%rbp), %rdx
	addq	%rcx, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-9(%rbp), %eax
	movb	%al, (%rdx)
.L11:
	addl	$1, -4(%rbp)
.L10:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L12
	addl	$1, -8(%rbp)
.L9:
	movl	-8(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L13
	movq	-40(%rbp), %rdx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	reverse
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	sort, .-sort
	.globl	reverse
	.type	reverse, @function
reverse:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L15
.L20:
	movl	-28(%rbp), %eax
	subl	-8(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	nop
	movl	-28(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	cmpl	%eax, -4(%rbp)
	jl	.L18
	movl	-8(%rbp), %eax
	cmpl	-4(%rbp), %eax
	je	.L23
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -9(%rbp)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rcx
	movq	-24(%rbp), %rdx
	addq	%rcx, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-9(%rbp), %eax
	movb	%al, (%rdx)
	jmp	.L18
.L23:
	nop
.L18:
	addl	$1, -8(%rbp)
.L15:
	movl	-28(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	cmpl	%eax, -8(%rbp)
	jl	.L20
	movl	$0, -8(%rbp)
	jmp	.L21
.L22:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rcx
	movq	-40(%rbp), %rdx
	addq	%rcx, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	addl	$1, -8(%rbp)
.L21:
	movl	-8(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L22
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	reverse, .-reverse
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
