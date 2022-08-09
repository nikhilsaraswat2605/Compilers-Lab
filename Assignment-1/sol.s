	.file	"asgn1.c"						# source file name
	.text									# code starts here
	.section	.rodata						# read only data
	.align 8								# align to 8 byte boundary
.LC0:										# label for string "Enter the string (all lowrer case): " (1st printf statement)
	.string	"Enter the string (all lowrer case): "		
.LC1:										# label for string "%s"	(scanf statement)
	.string	"%s"
.LC2:										# label for string "Length of the string: %d\n" (2nd printf statement)
	.string	"Length of the string: %d\n"
	.align 8								# align to 8 byte boundary
.LC3:										# label for string "The string in descending order: %s\n" (3rd printf statement)
	.string	"The string in descending order: %s\n"
	.text									# start code
	.globl	main							# main is a global name
	.type	main, @function					# main is a function
main:										# start of main						
.LFB0:										
	.cfi_startproc							# initialize internal structures and emit initial CFI for entry in .eh_frame			
	endbr64									
	pushq	%rbp							# push rbp onto stack
	.cfi_def_cfa_offset 16					# set CFA at an offset of 16 bytes from the current stack pointer
	.cfi_offset 6, -16						# set offset of 6th register at -16 bytes from the current stack pointer
	movq	%rsp, %rbp						# rbp <-- rsp, (set base pointer to current stack pointer)
	.cfi_def_cfa_register 6					# use register 6 for computing CFA
	subq	$80, %rsp						# allocate space for local variables on the stack (str, dest, len)
# asgn1.c:6: {
	movq	%fs:40, %rax					# get canary from stack
	movq	%rax, -8(%rbp)					# M[rbp-8] <-- rax, (save canary in stack)
	xorl	%eax, %eax						# erase canary

# asgn1.c:9:     printf("Enter the string (all lowrer case): ");
	leaq	.LC0(%rip), %rdi				# rdi <-- LC0, set rdi to label for string "Enter the string (all lowrer case): "
	movl	$0, %eax						# eax <-- 0, set eax to 0
	call	printf@PLT						# call printf

# asgn1.c:10:     scanf("%s", str);
	leaq	-64(%rbp), %rax					# rax <-- M[rbp-64], set rax to address of str
	movq	%rax, %rsi						# rsi <-- rax, set rsi to address of str
	leaq	.LC1(%rip), %rdi				# rdi <-- LC1, set rdi to label for string "%s"
	movl	$0, %eax						# eax <-- 0, set eax to 0
	call	__isoc99_scanf@PLT				# call scanf

# asgn1.c:11:     len = length(str); // calling length function
	leaq	-64(%rbp), %rax					# rax <-- M[rbp-64], set rax to address of str
	movq	%rax, %rdi						# rdi <-- rax, set rdi to address of str
	call	length							# call length
	movl	%eax, -68(%rbp)					# M[rbp-68] <-- eax, save length of string in stack

# asgn1.c:12:     printf("Length of the string: %d\n", len);
	movl	-68(%rbp), %eax					# eax <-- M[rbp-68], set eax to length of string
	movl	%eax, %esi						# esi <-- eax, set esi to length of string
	leaq	.LC2(%rip), %rdi				# rdi <-- LC2, set rdi to label for string "Length of the string: %d\n"
	movl	$0, %eax						# eax <-- 0, set eax to 0
	call	printf@PLT						# call printf

# asgn1.c:13:     sort(str, len, dest); // calling sorting function
	leaq	-32(%rbp), %rdx					# rdx <-- M[rbp-32], set rdx to address of dest
	movl	-68(%rbp), %ecx					# ecx <-- M[rbp-68], set ecx to length of string
	leaq	-64(%rbp), %rax					# rax <-- M[rbp-64], set rax to address of str
	movl	%ecx, %esi						# esi <-- ecx, set esi to length of string
	movq	%rax, %rdi						# rdi <-- rax, set rdi to address of str
	call	sort							# call sort

# asgn1.c:14:     printf("The string in descending order: %s\n", dest);
	leaq	-32(%rbp), %rax					# rax <-- M[rbp-32], set rax to address of dest
	movq	%rax, %rsi						# rsi <-- rax, set rsi to address of dest
	leaq	.LC3(%rip), %rdi				# rdi <-- LC3, set rdi to label for string "The string in descending order: %s\n"
	movl	$0, %eax						# eax <-- 0, set eax to 0
	call	printf@PLT						# call printf

# asgn1.c:15:     return 0;
	movl	$0, %eax						# eax <-- 0, set eax to 0

# asgn1.c:16: }
	movq	-8(%rbp), %rcx					# rcx <-- M[rbp-8], set rcx to canary
	xorq	%fs:40, %rcx					# take xor, and check if we get 0 or not, if we get 0 then that means there is no stack overflow, and everything is fine					
	je	.L3									# if we get 0 in previous step, then jump to .L3
	call	__stack_chk_fail@PLT			# if we reach here that indicates a stack overflow, hence the execution is terminated
.L3:
	leave									# set rsp to rbp, and pop top of stack into rbp
	.cfi_def_cfa 7, 8						# for computing CFA, take address from register 7 and add an offset of 8 to it
	ret										# pop return address from stack and transfer control back to the return address
	.cfi_endproc							# close the unwind entry previously opened by .cfi_startproc. and emit it to .eh_frame
.LFE0:
	.size	main, .-main					# set size of main function to .-main						
	.globl	length							# length is a global name
	.type	length, @function				# length is a function
length:
.LFB1:
	.cfi_startproc							# initialize internal structures and emit initial CFI for entry in .eh_frame
	endbr64									# endbr64 is used to mark the end of the function, and is used to generate unwind information for the function.
	pushq	%rbp							# push rbp onto stack
	.cfi_def_cfa_offset 16					# set CFA at an offset of 16 bytes from the current stack pointer
	.cfi_offset 6, -16						# set offset of 6th register at -16 bytes from the current stack pointer
	movq	%rsp, %rbp						# rbp <-- rsp, set base pointer to current stack pointer
	.cfi_def_cfa_register 6					# use register 6 for computing CFA
	movq	%rdi, -24(%rbp)					# M[rbp-24] <-- rdi, set M[rbp-24] to address of str

# asgn1.c:20:     for (i = 0; str[i] != '\0'; i++) // computing length of the string
	movl	$0, -4(%rbp)					# M[rbp-4] <-- 0, set i to 0

# asgn1.c:20:     for (i = 0; str[i] != '\0'; i++) // computing length of the string
	jmp	.L5									# jump to .L5
.L6:
# asgn1.c:20:     for (i = 0; str[i] != '\0'; i++) // computing length of the string
	addl	$1, -4(%rbp)					# M[rbp-4]+=1, set i to i+1
.L5:
# asgn1.c:20:     for (i = 0; str[i] != '\0'; i++) // computing length of the string
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to i
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to i
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rdx, %rax						# rax <-- rax+rdx, set rax to address of str+i
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to value at address of str+i

# asgn1.c:20:     for (i = 0; str[i] != '\0'; i++) // computing length of the string
	testb	%al, %al						# test if al is 0 or not, if it is 0 then jump to .L6
	jne	.L6									# if al is not 0, then jump to .L6

# asgn1.c:24:     return i;
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to i

# asgn1.c:25: }
	popq	%rbp							# pop rbp from stack
	.cfi_def_cfa 7, 8						# for computing CFA, take address from register 7 and add an offset of 8 to it
	ret										# pop return address from stack and transfer control back to the return address
	.cfi_endproc							# close the unwind entry previously opened by .cfi_startproc. and emit it to .eh_frame
.LFE1:
	.size	length, .-length				# size of length 
	.globl	sort							# sort is a global name
	.type	sort, @function					# sort is a function
sort:
.LFB2:
	.cfi_startproc							# initialize internal structures and emit initial CFI for entry in .eh_frame
	endbr64									# endbr64 is used to mark the end of the function, and is used to generate unwind information for the function.
	pushq	%rbp							# push rbp onto stack
	.cfi_def_cfa_offset 16					# set CFA at an offset of 16 bytes from the current stack pointer
	.cfi_offset 6, -16						# set offset of 6th register at -16 bytes from the current stack pointer
	movq	%rsp, %rbp						# rbp <-- rsp, set base pointer to current stack pointer
	.cfi_def_cfa_register 6					# use register 6 for computing CFA
	subq	$48, %rsp						# allocate space for local variables on the stack (len, dest, len, i,j, temp)
	movq	%rdi, -24(%rbp)					# M[rbp-24] <-- rdi, set M[rbp-24] to address of str
	movl	%esi, -28(%rbp)					# M[rbp-28] <-- esi, set M[rbp-28] to address of dest
	movq	%rdx, -40(%rbp)					# M[rbp-40] <-- rdx, set M[rbp-40] to address of len

# asgn1.c:30:     for (i = 0; i < len; i++)
	movl	$0, -8(%rbp)					# M[rbp-8] <-- 0, set i to 0

# asgn1.c:30:     for (i = 0; i < len; i++)
	jmp	.L9									# jump to .L9
.L13:
# asgn1.c:32:         for (j = 0; j < len; j++)
	movl	$0, -4(%rbp)					# M[rbp-4] <-- 0, set j to 0
# asgn1.c:32:         for (j = 0; j < len; j++)
	jmp	.L10								# jump to .L10
.L12:
# asgn1.c:34:             if (str[i] < str[j]) // sorting in ascending order
	movl	-8(%rbp), %eax					# eax <-- M[rbp-8], set eax to i
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to i
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rdx, %rax						# rax <-- rax+rdx, set rax to address of str+i
	movzbl	(%rax), %edx					# edx <-- M[rax], set edx to value at address of str+i

# asgn1.c:34:             if (str[i] < str[j]) // sorting in ascending order
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	movslq	%eax, %rcx						# rcx <-- eax, set rcx to j
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rcx, %rax						# rax <-- rax+rcx, set rax to address of str+j
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to value at address of str+j

# asgn1.c:34:             if (str[i] < str[j]) // sorting in ascending order
	cmpb	%al, %dl						# compare al and dl	
	jge	.L11								# if al is greater than or equal to dl, then jump to .L11

# asgn1.c:36:                 temp = str[i];
	movl	-8(%rbp), %eax					# eax <-- M[rbp-8], set eax to i
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to i
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rdx, %rax						# rax <-- rax+rdx, set rax to address of str+i

# asgn1.c:36:                 temp = str[i];
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to value at address of str+i
	movb	%al, -9(%rbp)					# M[rbp-9] <-- al, set M[rbp-9] to al

# asgn1.c:37:                 str[i] = str[j];
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to j
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rdx, %rax						# rax <-- rax+rdx, set rax to address of str+j

# asgn1.c:37:                 str[i] = str[j];
	movl	-8(%rbp), %edx					# edx <-- M[rbp-8], set edx to i
	movslq	%edx, %rcx						# rcx <-- edx, set rcx to i
	movq	-24(%rbp), %rdx					# rdx <-- M[rbp-24], set rdx to address of str
	addq	%rcx, %rdx						# rdx <-- rdx+rcx, set rdx to address of str+i

# asgn1.c:37:                 str[i] = str[j];
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to value at address of str+i

# asgn1.c:37:                 str[i] = str[j];

# asgn1.c:38:                 str[j] = temp;
	movb	%al, (%rdx)						# M[rdx] <-- al, set M[rdx] to al
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to j
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rax, %rdx						# rdx <-- rax+rdx, set rdx to address of str+j

# asgn1.c:38:                 str[j] = temp;
	movzbl	-9(%rbp), %eax					# eax <-- M[rbp-9], set eax to value at address of str+j
	movb	%al, (%rdx)						# M[rdx] <-- al, set M[rdx] to al
.L11:
# asgn1.c:32:         for (j = 0; j < len; j++)
	addl	$1, -4(%rbp)					# M[rbp-4] <-- M[rbp-4]+1, set j to j+1
.L10:
# asgn1.c:32:         for (j = 0; j < len; j++)
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	cmpl	-28(%rbp), %eax					# compare eax to len, if eax < len then jump to .L12
	jl	.L12		
# asgn1.c:30:     for (i = 0; i < len; i++)						# if eax < len, then jump to .L12
	addl	$1, -8(%rbp)					# add 1 to i, set i to i+1
.L9:
# asgn1.c:30:     for (i = 0; i < len; i++)
	movl	-8(%rbp), %eax					# eax <-- M[rbp-8], set eax to i
	cmpl	-28(%rbp), %eax					# compare i to len

# asgn1.c:43:     reverse(str, len, dest);
	jl	.L13								# if i < len, jump to .L13
	movq	-40(%rbp), %rdx					# rdx <-- M[rbp-40], set rdx to len
	movl	-28(%rbp), %ecx					# ecx <-- M[rbp-28], set ecx to dest
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to str
	movl	%ecx, %esi						# esi <-- ecx, set esi to dest
	movq	%rax, %rdi						# rdi <-- rax, set rdi to str
	call	reverse							# call reverse function

# asgn1.c:44: }
	nop										# nop instruction
	leave									# set rsp to rbp, and pop top of stack into rbp
	.cfi_def_cfa 7, 8						# set CFA to an offset of 8 bytes from the current stack pointer, and an offset of 7 bytes from the current frame pointer
	ret										# pop return address from stack and transfer control back to the return address
	.cfi_endproc							# terminate CFI for entry in .eh_frame
.LFE2:
	.size	sort, .-sort					# .size sort, .-sort is the size of the function
	.globl	reverse							# reverse is a global name
	.type	reverse, @function				# reverse is a function
reverse:
.LFB3:
	.cfi_startproc							# initialize internal structures and emit initial CFI for entry in .eh_frame			
	endbr64									# endbr64 is used to mark the end of the function, and is used to generate unwind information for the function.
	pushq	%rbp							# push rbp onto the stack
	.cfi_def_cfa_offset 16					# set CFA to an offset of 16 bytes from the current stack pointer
	.cfi_offset 6, -16						# set the offset of the 6th register to -16 bytes from the current stack pointer
	movq	%rsp, %rbp						# set rbp to the current stack pointer
	.cfi_def_cfa_register 6					# set CFA to the 6th register
	movq	%rdi, -24(%rbp)					# M[rbp-24] <-- rdi, set M[rbp-24] to address of str
	movl	%esi, -28(%rbp)					# M[rbp-28] <-- esi, set M[rbp-28] to address of dest
	movq	%rdx, -40(%rbp)					# M[rbp-40] <-- rdx, set M[rbp-40] to len

# asgn1.c:49:     for (i = 0; i < len / 2; i++)
	movl	$0, -8(%rbp)					# M[rbp-8] <-- 0, set i to 0

# asgn1.c:49:     for (i = 0; i < len / 2; i++)
	jmp	.L15								# jump to .L15
.L20:

# asgn1.c:51:         for (j = len - i - 1; j >= len / 2; j--) // reversing the string
	movl	-28(%rbp), %eax					# eax <-- M[rbp-28], set eax to address of dest
	subl	-8(%rbp), %eax					# eax <-- eax-8, set eax to address of dest-i

# asgn1.c:51:         for (j = len - i - 1; j >= len / 2; j--) // reversing the string
	subl	$1, %eax						# eax <-- eax-1, set eax to address of dest-i-1
	movl	%eax, -4(%rbp)					# M[rbp-4] <-- eax, set M[rbp-4] to address of dest-i-1

# asgn1.c:51:         for (j = len - i - 1; j >= len / 2; j--) // reversing the string
	nop
# asgn1.c:51:         for (j = len - i - 1; j >= len / 2; j--) // reversing the string
	movl	-28(%rbp), %eax					# eax <-- M[rbp-28], set eax to address of dest
	movl	%eax, %edx						# edx <-- eax, set edx to address of dest
	shrl	$31, %edx						# edx <-- edx >> 31, Shifts the value in edx to the right by 31 bits and store in edx. Since edx is a 32 bit register this effectively reads the sign bit.
	addl	%edx, %eax						# eax <-- eax + edx, Since the previous operation stored the sign bit in edx, this adds 1 to the original value if eax is negative, otherwise it does nothing.				
	sarl	%eax							# eax <-- eax >> 1, Shift eax to the right by 1 and preserves the sign bit. Basically a division by 2.

# asgn1.c:51:         for (j = len - i - 1; j >= len / 2; j--) // reversing the string
	cmpl	%eax, -4(%rbp)					# compare eax to M[rbp-4], set flags if eax is less than M[rbp-4]			
	jl	.L18								# jump to .L18 if eax is less than M[rbp-4]	

# asgn1.c:53:             if (i == j)
	movl	-8(%rbp), %eax					# eax <-- M[rbp-8], set eax to i
	cmpl	-4(%rbp), %eax					# compare eax to M[rbp-4], set flags if i is less than j
	je	.L23								# jump to .L23 if i is equal to j

# asgn1.c:57:                 temp = str[i];
	movl	-8(%rbp), %eax					# eax <-- M[rbp-8], set eax to i
	movslq	%eax, %rdx						# edx <-- eax, set edx to i
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str	
	addq	%rdx, %rax						# rax <-- rax+edx, set rax to address of str+i

# asgn1.c:57:                 temp = str[i];
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to str[i]
	movb	%al, -9(%rbp)					# M[rbp-9] <-- al, set M[rbp-9] to str[i]

# asgn1.c:58:                 str[i] = str[j];
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to j
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rdx, %rax						# rax <-- rax+rdx, set rax to address of str+j

# asgn1.c:58:                 str[i] = str[j];
	movl	-8(%rbp), %edx					# edx <-- M[rbp-8], set edx to i
	movslq	%edx, %rcx						# rcx <-- edx, set rcx to i
	movq	-24(%rbp), %rdx					# rdx <-- M[rbp-24], set rdx to address of str
	addq	%rcx, %rdx						# rdx <-- rdx+rcx, set rdx to address of str+i

# asgn1.c:58:                 str[i] = str[j];
	movzbl	(%rax), %eax					# eax <-- M[rax], set eax to str[j]
# asgn1.c:58:                 str[i] = str[j];
	movb	%al, (%rdx)						# M[rdx] <-- al, set M[rdx] to str[j]
# asgn1.c:59:                 str[j] = temp;
	movl	-4(%rbp), %eax					# eax <-- M[rbp-4], set eax to j
	movslq	%eax, %rdx						# rdx <-- eax, set rdx to j
	movq	-24(%rbp), %rax					# rax <-- M[rbp-24], set rax to address of str
	addq	%rax, %rdx						# rdx <-- rdx+rax, set rdx to address of str+j

# asgn1.c:59:                 str[j] = temp;
	movzbl	-9(%rbp), %eax					# eax <-- M[rbp-9], set eax to temp
	movb	%al, (%rdx)						# M[rdx] <-- al, set M[rdx] to temp

# asgn1.c:60:                 break;
	jmp	.L18								# jump to .L18
.L23:

# asgn1.c:54:                 break;
	nop										# nop
.L18:

# asgn1.c:49:     for (i = 0; i < len / 2; i++)
	addl	$1, -8(%rbp)						# M[rbp-8] <-- M[rbp-8] + 1, set i to i+1
.L15:
# asgn1.c:49:     for (i = 0; i < len / 2; i++)
	movl	-28(%rbp), %eax						# eax <-- M[rbp-28], set eax to len
	movl	%eax, %edx							# edx <-- eax, set edx to len
	shrl	$31, %edx							# edx <-- edx >> 31, Shifts the value in edx to the right by 31 bits and store in edx. Since edx is a 32 bit register this effectively reads the sign bit.
	addl	%edx, %eax							# eax <-- eax + edx, Since the previous operation stored the sign bit in edx, this adds 1 to the original value if eax is negative, otherwise it does nothing.				
	sarl	%eax								# eax <-- eax >> 1, Shift eax to the right by 1 and preserves the sign bit. Basically a division by 2.

# asgn1.c:49:     for (i = 0; i < len / 2; i++)
	cmpl	%eax, -8(%rbp)						# compare eax to i, if eax is less than i, jump to .L20
	jl	.L20
										# if eax is less than i, jump to .L20
# asgn1.c:64:     for (i = 0; i < len; i++)
	movl	$0, -8(%rbp)						# set i to 0

# asgn1.c:64:     for (i = 0; i < len; i++)
	jmp	.L21									# jump to .L21
.L22:
# asgn1.c:65:         dest[i] = str[i];
	movl	-8(%rbp), %eax						# eax <-- i, set eax to i
	movslq	%eax, %rdx							# rdx <-- eax, set rdx to i
	movq	-24(%rbp), %rax						# set rax to str
	addq	%rdx, %rax							# rax <-- rax+rdx, set rax to str+i
# asgn1.c:65:         dest[i] = str[i];
	movl	-8(%rbp), %edx						# edx <-- i, set edx to i# asgn1.c:65:         dest[i] = str[i];

	movslq	%edx, %rcx							# rcx <-- edx, set rcx to i
	movq	-40(%rbp), %rdx						# set rdx to dest
	addq	%rcx, %rdx							# rdx <-- rdx+rcx, set rdx to dest+i
# asgn1.c:65:         dest[i] = str[i];
	movzbl	(%rax), %eax						# eax <-- M[rax], set eax to str[i]
# asgn1.c:65:         dest[i] = str[i];
	movb	%al, (%rdx)							# M[rdx] <-- al, set M[rdx] to str[i]
# asgn1.c:64:     for (i = 0; i < len; i++)
	addl	$1, -8(%rbp)						# M[rbp-8] <-- M[rbp-8] + 1, set i to i+1
.L21:
# asgn1.c:64:     for (i = 0; i < len; i++)
	movl	-8(%rbp), %eax						# eax <-- i, set eax to i
	cmpl	-28(%rbp), %eax						# compare eax to len, if eax is less than len, jump to .L22
	jl	.L22									# if eax is less than len, jump to .L22
# asgn1.c:66: }
	nop											# nop
	nop											# nop
	popq	%rbp								# restore rbp
	.cfi_def_cfa 7, 8							# set CFA to the 7th register + 8 bytes
	ret											# pop return address from stack and transfer control back to the return address
	.cfi_endproc								# close the unwind entry previously opened by .cfi_startproc. and emit it to .eh_frame						
.LFE3:
	.size	reverse, .-reverse					# size of reverse		
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
