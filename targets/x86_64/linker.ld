ENTRY(start)

SECTIONS
{
    . = 1M;

    .boot :
    {
        KEEP(*(.multiboot_header))
    }

    .text :
    {
        *(.text)
    }

    /* Define the memory pool section with 6 GB */
    __memory_pool_start = .;
    .memory_pool :
    {
        . = . + 100M;
    }
    __memory_pool_end = .;
}