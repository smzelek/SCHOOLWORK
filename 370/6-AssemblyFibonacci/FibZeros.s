/*
  Filename   : FibZeros.s
  Author     : Steve Zelek
  Course     : CSCI 370
  Description: Compute the fibonacci number for abs(n)...
*/

/**********************************************************/
// Data
.data
prompt:
	.string "n ==> "

formatSpec:
	.string "%d"

output:
	.string "fib(|%d|) - zeros(%d) = %d - %d = %d\n"
	
/**********************************************************/
// Code
	.text
	.align	4	
	.globl	main

main:
	pushl	%ebp
	movl	%esp, %ebp

	# Save callee save register ebx
	#  and move esp down past it
	pushl 	%ebx
	subl 	$4, %esp
	
	pushl	$prompt
	call	printf

	# Compute and push address of x
	leal	-8(%ebp), %edx
	pushl	%edx
	pushl	$formatSpec
	call	scanf

	#do computation, have result in eax

	pushl 	-8(%ebp)
	call 	zeros
	movl 	%eax, %ebx

	pushl 	-8(%ebp)
	call 	fibonacci
	movl 	%eax, %ecx

	movl 	%ecx, %edx
	subl 	%ebx, %edx
	# Output result
	# Need to use address of x again, scanf overwrites...
	
	pushl	%edx
	pushl 	%ebx
	pushl 	%ecx
	pushl 	-8(%ebp)
	pushl 	-8(%ebp)
	pushl	$output
	call	printf

	# Return 0 from main
	movl	$0, %eax

	# Restore ebx
	movl 	-4(%ebp), %ebx
	movl	%ebp, %esp
	popl	%ebp

	ret

fibonacci:
	# Compute the fibonacci value for a given number
	pushl %ebp
	movl %esp, %ebp

	pushl %ebx
	subl $4, %esp
	#save ebx
	
	movl 8(%ebp), %ebx
	
	#check if number is non-negative
	cmpl $0, %ebx
	jge beginF
	
	#otherwise, fall through and negate
	negl %ebx
beginF:
	movl $0, %edx #a
	movl $1, %eax #b
	movl $0, %ecx #c
	
	#check if number is less than 2
	cmpl $2, %ebx
	jl doneF
loopF:	
	movl %edx, %ecx
	addl %eax, %ecx
	
	movl %eax, %edx
	movl %ecx, %eax
	
	#use ecx, eax, edx because they are caller save
	decl %ebx
	cmpl $0, %ebx
	jg loopF
doneF:
	#restore ebx
	movl 	-4(%ebp), %ebx
	leave
	ret
	
	
zeros:
	# Compute the number of 0s in the binary representation
	#  of a given number

	pushl 	%ebp
	movl 	%esp, %ebp
	pushl 	%ebx
	subl 	$4, %esp 
	
	# get variable passed in
	movl	8(%ebp), %ebx

	#shift by 1, 31 times
	#store accumulation in eax

	movl 	$32, %eax
	movl 	$0, %edx
loopZ:
	cmpl 	$32, %edx
	jge 	doneZ

	movl 	%ebx, %ecx
	
	and 	$0x1, %ecx
	subl	%ecx, %eax
	
	shr 	$1, %ebx
	inc 	%edx
	jmp 	loopZ
doneZ:
	#return will be eax
	#restore callee save registers
	movl 	-4(%ebp), %ebx
	
	leave
	ret
