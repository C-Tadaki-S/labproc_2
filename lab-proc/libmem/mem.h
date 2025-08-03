
// #include <stdint.h>

// #define _(X) X "\n\t"

// /**
//  * Tabela de tradução de endereços (L1).
//  */
// typedef struct {
//    uint32_t descriptor[4096];
// } ttb_l1_t;

// /**
//  * Desabilita o gerenciador de memória.
//  */
// void mmu_stop(void);

// /**
//  * Invalida o TLB atual.
//  */
// void tlb_invalida(void);

// /**
//  * Ativa o gerenciador de memória.
//  * @param ttb Endereço para o primeiro nível da tabela de tradução de páginas.
//  */
// void mmu_start(ttb_l1_t *ttb);

// /**
//  * Cria uma entrada de seção na tabela de nível 1.
//  * @param virt Endereço virtual a definir.
//  * @param fis Endereço físico correspondente.
//  * @param flags Flags adicionais.
//  */
// void map_section(ttb_l1_t* ttb_l1, uint32_t virt, uint32_t fis, uint32_t flags);

// /**
//  * Cria uma entrada de seção inválida (não mapeada) na tabela de nível 1.
//  * @param virt Endereço virtual da seção.
//  */
// void map_invalid(ttb_l1_t* ttb_l1, uint32_t virt);

// /**
//  * Cria uma tabela de tradução "plana" (endereços físicos e virtuais são iguais).
//  */
// void mmu_flat(ttb_l1_t* ttb_l1);

#ifndef MEM_H
#define MEM_H
#include <stdint.h>

/* tabela L1 – 4096 descritores × 4 B = 16 KiB (alinhados a 16 KiB) */
typedef struct
{
   uint32_t descriptor[4096];
} __attribute__((aligned(16384))) ttb_l1_t;

/* protótipos */
void mmu_stop(void);
void mmu_start(ttb_l1_t *ttb);
void mmu_flat(ttb_l1_t *ttb);
void map_section(ttb_l1_t *ttb, uint32_t virt, uint32_t phys, uint32_t flags);
void map_invalid(ttb_l1_t *ttb, uint32_t virt);
void tlb_invalida(void);
#endif
