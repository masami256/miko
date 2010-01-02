ENTRY(_start)
SECTIONS
{
       . = 0x100000;
       .text :{
               *(.entry);
               *(.text);
       }

       .data ALIGN (0x1000) : {
               *(.data)
       }

       .rodata ALIGN (0x1000) : {
           *(.rodata)
       }

       .bss :{
               *(.bss);
       }
       end = .;
}

