
// #include <stdint.h>
// #include "mem.h"

// #define _(X) X "\n\t"

// void mmu_stop(void)
// {
//    asm volatile(
//        _("mrc p15, 0, r2, c1, c0, 0")
//            _("bic r2, #0x1000")
//                _("bic r2, #0x0004")
//                    _("bic r2, #0x0001")
//                        _("mcr p15, 0, r2, c1, c0, 0"));
// }

// void tlb_invalida(void)
// {
//    asm volatile(
//        _("mov r2, #0")
//            _("mcr p15, 0, r2, c8, c7, 0")
//                _("mcr p15, 0, r2, c7, c10, 4")::: "r2");
// }

// void mmu_start(ttb_l1_t *ttb)
// {
//    asm volatile(
//        _("mvn r2, #0")
//            _("bic r2, #0xC")
//                _("mcr p15, 0, r2, c3, c0, 0")
//                    _("mcr p15, 0, %0, c2, c0, 0")
//                        _("mcr p15, 0, %0, c2, c0, 1")
//                            _("mrc p15, 0, r2, c1, c0, 0")
//                                _("orr r2, r2, #0x05")
//                                    _("orr r2, r2, #0x1000")
//                                        _("mcr p15, 0, r2, c1, c0, 0")::"r"(ttb) : "r2");
// }

// void map_section(ttb_l1_t *ttb_l1, uint32_t virt, uint32_t fis, uint32_t flags)
// {
//    int indice = (virt & 0xfff00000) >> 20;          // índice do descritor, entre 0 e 4095 (20 bits do endereço virtual)
//    int descr = (fis & 0xfff00000) | 0xc002 | flags; // valor do descritor: 12 bits do endereço físico + flags.
//    ttb_l1->descriptor[indice] = descr;
// }

// void map_invalid(ttb_l1_t *ttb_l1, uint32_t virt)
// {
//    int indice = (virt & 0xfff00000) >> 20; // 0xf     // índice do descritor, entre 0 e 4095 (12 bits do endereço virtual)
//    ttb_l1->descriptor[indice] = 0;
// }

// void mmu_flat(ttb_l1_t *ttb_l1)
// {
//    int i;
//    for (i = 0; i < 4096; i++)
//    { // Iterar 4K vezes
//       map_section(ttb_l1, i << 20, i << 20, 0x0000);
//    }
// }
#include <stdint.h>
#include "mem.h"

#define _(x) x "\n\t"

void mmu_stop(void)
{
   asm volatile(
       _("mrc p15, 0, r2, c1, c0, 0")
           _("bic r2, #0x1000")
               _("bic r2, #0x0004")
                   _("bic r2, #0x0001")
                       _("mcr p15, 0, r2, c1, c0, 0")::: "r2");
}

void tlb_invalida(void)
{
   asm volatile(
       _("mov r2, #0")
           _("mcr p15, 0, r2, c8, c7, 0")
               _("mcr p15, 0, r2, c7, c10, 4")::: "r2");
}

void mmu_start(ttb_l1_t *ttb)
{
   asm volatile(
       _("mvn r2, #0") /* Domínios = manage */
       _("bic r2, #0xC")
           _("mcr p15, 0, r2, c3, c0, 0")
               _("mcr p15, 0, %0, c2, c0, 0") /* TTBR0 */
       _("mcr p15, 0, %0, c2, c0, 1")         /* TTBR1 */
       _("mrc p15, 0, r2, c1, c0, 0")
           _("orr r2, r2, #0x1000") /* I-cache enable */
       _("orr r2, r2, #0x0005")     /* MMU + alinhamento */
       _("mcr p15, 0, r2, c1, c0, 0")::"r"(ttb) : "r2");
}

void map_section(ttb_l1_t *ttb, uint32_t virt, uint32_t phys, uint32_t flags)
{
   const uint32_t idx = (virt & 0xFFF00000) >> 20;
   const uint32_t descr = (phys & 0xFFF00000) | 0xC002 | flags;
   ttb->descriptor[idx] = descr;
}

void map_invalid(ttb_l1_t *ttb, uint32_t virt)
{
   const uint32_t idx = (virt & 0xFFF00000) >> 20;
   ttb->descriptor[idx] = 0;
}

void mmu_flat(ttb_l1_t *ttb)
{
   for (uint32_t i = 0; i < 4096; ++i)
      map_section(ttb, i << 20, i << 20, 0);
}
