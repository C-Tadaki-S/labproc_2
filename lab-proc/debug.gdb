#####################################################################
#  debug.gdb – arranca, carrega o ELF, define breakpoints & macros
#####################################################################

target remote :1234
set architecture arm
load 

# ---------- macro utilitária ---------------------------------------
define showmap
    set $desc = *(unsigned int*)(0x3EFFC000 + 4)
    printf "\n=== slice =========================================\n"
    printf "tid              = %d\n", tid
    printf "descritor L1[1]  = 0x%08x\n", $desc
    printf "a (@0x%p)       = %d\n", &a, *(&a)
end

# ---------- breakpoint em schedule() (entrada) ------------------
break schedule
commands
    silent
    finish
    set $desc = *(unsigned int*)(0x3EFFC000+4)
    printf "\n=== slice =====================\n"
    printf "tid = %d,  desc = 0x%08x,  a = %d\n", tid, $desc, a
    continue
end



# breakpoint B (tbreak) herda da regra acima:
# quando disparar, só existirá 'lr' no topo da pilha, portanto estamos
# de volta ao chamador de schedule() – hora de imprimir.
commands
    silent
    showmap
    continue
end

echo "GDB pronto!  digite 'c' para começar.\n"