/* ctors and dtors arrays -- to be used by runtime system */
/*   to call static constructors and destructors          */
/*                                                        */
/* NOTE: Use a C compiler to compile this file. If you    */
/*       are using GNU C++, be sure to use compile this   */
/*       file using a GNU compiler with the               */
/*       -fdollars-in-identifiers flag.                   */


#ifdef __DCC__
asm("     	.text");
asm("     	.section        .init$00,4,C");
asm("	.text");
asm("	.section        .fini$00,4,C");
asm("	.section	.frame_info$00,4,r");
asm("_frame_info_start:");
asm("	.section	.frame_info$99");
asm("_frame_info_end:");
#endif

#ifdef __GNUC__
#error Diab C++ code must be munched and linked using the Diab toolchain
#endif

extern unsigned long _frame_info_start[];
extern unsigned long _frame_info_end[];

extern void __frameinfo_add(void *, void *);
extern void __frameinfo_del(void *, void *);

static void _STI__15_ctors()
{
	__frameinfo_add(_frame_info_start, _frame_info_end);
}

static void _STD__15_ctors()
{
	__frameinfo_del(_frame_info_start, _frame_info_end);
}

char __dso_handle = 0;
extern void __cxa_finalize(void *);
static void _STD__cxa_finalize()
{
	__cxa_finalize(&__dso_handle);
}


void _STI___14_TCP_Server_cpp_7f75c8d8();

void _STI___17_Telnet_Server_cpp_7f75c8d8();

void _STI___12_diaTimer_cpp_7f75c8d8();

void _STI___8_main_cpp_7f75c8d8();

void _STI___16_stateMachine_cpp_7f75c8d8();

void _STI___14_stateTable_cpp_7f75c8d8();

void _STI___17_systemManager_cpp_7f75c8d8();

extern void (*_ctors[])();
void (*_ctors[])() =
    {
    _STI__15_ctors,
    _STI___14_TCP_Server_cpp_7f75c8d8,
    _STI___17_Telnet_Server_cpp_7f75c8d8,
    _STI___12_diaTimer_cpp_7f75c8d8,
    _STI___8_main_cpp_7f75c8d8,
    _STI___16_stateMachine_cpp_7f75c8d8,
    _STI___14_stateTable_cpp_7f75c8d8,
    _STI___17_systemManager_cpp_7f75c8d8,
    0
    };

extern void (*_dtors[])();
void (*_dtors[])() =
    {
    _STD__15_ctors,
    _STD__cxa_finalize,
    0
    };

/* build variables */
#ifdef __GNUC__
__asm("	.section \".wrs_build_vars\",\"a\"");
#endif
#ifdef __DCC__
__asm("	.section \".wrs_build_vars\",\"r\"");
#endif
__asm(" .ascii \"tag SMP 0\"");
__asm(" .byte 0");
__asm("	.ascii \"end\"");
__asm("	.byte 0");
__asm("	.previous");
