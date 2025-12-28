#include <stdint.h>
#include "memory.h" 

#define PAGE_SIZE    0x1000
#define PT_ENTRIES   1024
#define PD_ENTRIES   1024

// Alinhamento do Page Directory
uint32 page_directory[PD_ENTRIES] __attribute__((aligned(4096)));
uint8 pmm_bitmap[BITMAP_SIZE];
// Início da alocação das tabelas de página (após o kernel, em 4MB)
// Certifique-se de que seu kernel não é maior que 3MB.
uint32 next_free_phys_addr = 0x400000;

// Local helpers
static inline uint32 align_down(uint32 a, uint32 b) { return a & ~(b - 1); }
static inline uint32 align_up(uint32 a, uint32 b)   { return (a + b - 1) & ~(b - 1); }

int pmm_initialized = 0;
// Alocador "Bump" simples para criar novas Page Tables
uint32 allocate_pt_phys() {
    // FASE 1: Se o PMM ainda não foi iniciado, use o Bump Allocator (seguro para boot)
    if (!pmm_initialized) {
        uint32 addr = next_free_phys_addr;
        next_free_phys_addr += PAGE_SIZE;
        
        // Limpa a memória recém alocada
        uint32* ptr = (uint32*)addr;
        for (int i=0; i<1024; i++) ptr[i] = 0;
        
        return addr;
    }

    // FASE 2: PMM Real (só entra aqui depois do pmm_init)
    uint32 frame_phys = pmm_alloc_frame();
    if (frame_phys == 0xFFFFFFFF) {
        // OOM (Out of Memory) - Aqui você deveria entrar em pânico (kernel panic)
        return 0; 
    }

    uint32 *ptr = (uint32*)frame_phys;
    for (int i = 0; i < 1024; ++i) ptr[i] = 0;
    return frame_phys;
}


void map_page(uint32 virt, uint32 phys) {
    uint32 pdi = (virt >> 22) & 0x3FF;
    uint32 pti = (virt >> 12) & 0x3FF;

    // Se a Page Table não existe, aloque uma
    if (!(page_directory[pdi] & 1)) {
        uint32 pt_phys = allocate_pt_phys();
        page_directory[pdi] = pt_phys | 0x3; // Present + RW
    }

    // Endereço físico da Page Table
    uint32* pt = (uint32*)(page_directory[pdi] & 0xFFFFF000);

    pt[pti] = (phys & 0xFFFFF000) | 0x3; // RW + Present
}

// Faz identity mapping para [phys_addr, phys_addr + size)
void map_identity(uint32 phys_addr, uint32 size) {
    if (size == 0) return;

    uint32 start = align_down(phys_addr, PAGE_SIZE);
    uint32 end   = align_up(phys_addr + size, PAGE_SIZE);

    for (uint32 addr = start; addr < end; addr += PAGE_SIZE) {
        uint32 pdi = (addr >> 22) & 0x3FF;
        uint32 pti = (addr >> 12) & 0x3FF;

        // Se a PDE não existe (bit 0 = 0), aloca nova Page Table
        if (!(page_directory[pdi] & 0x1)) {
            uint32 pt_phys = allocate_pt_phys();
            page_directory[pdi] = pt_phys | 0x3; // Present + RW
        }

        // Pega o endereço físico da PT
        uint32 *pt = (uint32*) (page_directory[pdi] & 0xFFFFF000);

        // Mapeia a página
        pt[pti] = (addr & 0xFFFFF000) | 0x3; // Present + RW
    }
}

// Ativa o paging
void enable_paging(uint32 page_directory_physical_addr) {
    asm volatile(
        "movl %0, %%cr3\n\t"
        "movl %%cr0, %%eax\n\t"
        "orl $0x80000001, %%eax\n\t" // PG + PE
        "movl %%eax, %%cr0\n\t"
        :
        : "r"(page_directory_physical_addr)
        : "eax"
    );
}

void map_range(uint32 virt, uint32 phys, uint32 size) {
    for (uint32 offset = 0; offset < size; offset += 0x1000) {
        map_page(virt + offset, phys + offset);
    }
}

// Função principal de setup de memória
void map_all_physical_memory(uint32 e820_map_addr, uint32 e820_entry_count) {
    e820_entry_t *map = (e820_entry_t*) e820_map_addr;

    // 1. Limpar diretório completamente
    for (int i = 0; i < PD_ENTRIES; ++i) page_directory[i] = 0;

    // 2. IMPORTANTE: Mapear VGA Text Mode explicitamente (0xB8000)
    // O E820 ignora isso, e se não mapear, o printf falha.
    map_identity(0xB8000, 0x1000); // 4KB para vídeo

    // 3. Opcional: Garantir que o Kernel (0x100000) e a pilha estejam mapeados
    // Isso evita problemas caso o mapa E820 tenha "buracos" estranhos no início.
    map_identity(0x0, 0x400000); // Mapeia os primeiros 4MB cegamente

    // 4. Mapear RAM baseada no E820
    for (uint32 i = 0; i < e820_entry_count; ++i) {
        if (map[i].type != 1) continue; 

        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;

        // Filtra endereços > 4GB (32-bit)
        if (base >= 0xFFFFFFFF) continue;
        if (base + len > 0xFFFFFFFF) len = 0xFFFFFFFF - base;

        map_identity((uint32)base, (uint32)len);
    }
    
    // Nota: O loop E820 vai re-mapear áreas que fizemos no passo 3.
    // Isso não tem problema, o map_identity apenas sobrescreve a entrada com o mesmo valor.
}


void bitmap_set(uint32 frame) {
    if (frame >= MAX_FRAMES) return; // <--- PROTEÇÃO CRÍTICA
    pmm_bitmap[frame / 8] |= (1 << (frame % 8));
}

void bitmap_clear(uint32 frame) {
    if (frame >= MAX_FRAMES) return; // <--- PROTEÇÃO CRÍTICA
    pmm_bitmap[frame / 8] &= ~(1 << (frame % 8));
}

uint32 bitmap_test(uint32 frame) {
    if (frame >= MAX_FRAMES) return 0; // Se tá fora, assume ocupado/inexistente
    return pmm_bitmap[frame / 8] & (1 << (frame % 8));
}

uint32 pmm_alloc_frame() {
    for (uint32 i = 0; i < MAX_FRAMES; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return i * PAGE_SIZE; // endereço físico
        }
    }
    return 0xFFFFFFFF; // OUT OF MEMORY (sentinel)
}

void pmm_free_frame(uint32 phys_addr) {
    uint32 frame = phys_addr / 4096;
    bitmap_clear(frame);
}

void pmm_init(e820_entry_t* map, uint32 count) {
    /* marca tudo como usado por padrão */
    memset(pmm_bitmap, 0xFF, BITMAP_SIZE);

    /* para cada área usable (type == 1) marque frames como livres */
    for (uint32 i = 0; i < count; ++i) {
        if (map[i].type != 1) continue;

        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;
        uint64_t end  = base + len;

        /* clip para 32-bit se você só suporta 32-bit */
        if (base >= 0x100000000ULL) continue;
        if (end > 0x100000000ULL) end = 0x100000000ULL;

        for (uint64_t addr = base; addr < end; addr += PAGE_SIZE) {
            uint32 frame = (uint32)(addr / PAGE_SIZE);
            bitmap_clear(frame); /* agora livre */
        }
    }

    /* marque áreas críticas como usadas: kernel, page tables, VGA (0xB8000), etc. */
    /* Exemplo: marcar 0..0x400000 como usados (kernel + identidade que você mapeou) */
    for (uint32 addr = 0; addr < 0x400000; addr += PAGE_SIZE) {
        uint32 frame = addr / PAGE_SIZE;
        bitmap_set(frame);
    }

    /* marcar VGA */
    uint32 vga_frame = 0xB8000 / PAGE_SIZE;
    bitmap_set(vga_frame);
}

void *memset(void *s, int c, uint32 n) {
    uint8 *p = (uint8*)s;
    while (n--) *p++ = (uint8)c;
    return s;
}