.section .note.GNU-stack
.section .data
heap_start:
    .quad 0            # Global variable to store the start of the heap
errno:
    .int 0
.section .text
.globl _brk

# Custom _brk system call implementation
_brk:
    mov $12, %rax       # System call number for brk (12 for x86_64 Linux)
    syscall             # Invoke system call
    cmp $-4096, %rax    # Check if syscall returned -4096 (ENOMEM)
    jae _brk_error      # Jump to _brk_error if syscall error (rax >= -4096)
    ret                 # Return from _brk function

_brk_error:
    neg %rax            # Convert syscall error to positive value
    mov %rax, errno(%rip)  # Store errno value (syscall error code)
    mov $-1, %rax       # Return -1 on error
    ret                 # Return from _brk function

.globl brk

# Custom brk library function implementation
brk:
    mov %rdi, %rax      # Move the argument (new end address) to %rax for system call
    call _brk           # Call our custom _brk system call
    ret                 # Return from brk function

.globl _sbrk

# Custom _sbrk function implementation
_sbrk:
    mov heap_start(%rip), %rax   # Load current program break (start of heap)
    add %rdi, %rax               # Increment program break by the requested amount in %rdi
    mov %rax, heap_start(%rip)   # Update the start of heap to the new program break
    ret                          # Return the updated program break
