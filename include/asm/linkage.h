#ifndef LINKAGE_H
#define LINKAGE_H

#define __ASSEMBLY__

#define __ALIGN         .align 4,0x90
#define __ALIGN_STR     ".align 4,0x90"

#ifdef __ASSEMBLY__
#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#ifndef ENTRY
#define ENTRY(name) \
   .globl name	; \
   ALIGN		; \
   name:
#endif // ENTRY

#endif // __ASSEMBLY__

#endif // LINKAGE_H 
