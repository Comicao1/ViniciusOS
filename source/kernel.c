#include "headers/idt/idt.h"
#include "headers/memory/memory.h"
#include "headers/text/fonts.h"
void kernel_main(uint32 e820_map_addr, uint32 e820_entry_count) {
    e820_entry_t *map = (e820_entry_t*)e820_map_addr;

    // 1. Limpa Page Directory
    for (int i = 0; i < 1024; ++i) page_directory[i] = 0;

    // 2. Mapeamento Crítico (Identity Map dos primeiros 4MB)
    // Isso garante que o Kernel (0x100000), a Pilha e o código rodando agora não sumam.
    // Como pmm_initialized é 0, ele vai usar o next_free_phys_addr (0x400000+)
    // para criar as tabelas, sem corromper a memória baixa.
    map_identity(0x0, 0x400000); 

    // 3. Mapeia o resto da RAM detectada (E820)
    for (uint32 i = 0; i < e820_entry_count; ++i) {
        if (map[i].type != 1) continue; 
        uint64_t base = map[i].base;
        uint64_t len  = map[i].length;
        
        if (base >= 0xFFFFFFFF) continue;
        if (base + len > 0xFFFFFFFF) len = 0xFFFFFFFF - base;

        // map_identity verifica se a página já existe, então não tem problema re-mapear
        map_identity((uint32)base, (uint32)len);
    }

    // 4. Mapeia VGA
    map_identity(0xB8000, 0x1000);
    draw_text(100, 100, "EXCECAO DE DIVISAO POR 0", 1);
    // 5. Ativa Paging
    enable_paging((uint32)page_directory);

    // 6. INICIALIZA O PMM
    // Agora que a memória virtual funciona, configuramos o físico.
    pmm_init(map, e820_entry_count);
    
    // IMPORTANTE: Precisamos dizer ao PMM que a memória usada pelo 
    // Bump Allocator (0x400000 até onde ele parou) está ocupada!
    // next_free_phys_addr é onde o bump parou.
    uint32 used_by_tables = next_free_phys_addr;
    for (uint32 addr = 0x400000; addr < used_by_tables; addr += PAGE_SIZE) {
        uint32 frame = addr / PAGE_SIZE;
        bitmap_set(frame); // Marca como usado no bitmap
    }
    
    // Agora podemos liberar o alocador real
    pmm_initialized = 1; 

    // 7. Configura Interrupções
    pic_remap();
    idt_init();

    asm volatile("sti"); 

    for(;;) {
        asm volatile("hlt");
    }
}