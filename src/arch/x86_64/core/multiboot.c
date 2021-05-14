#include "multiboot.h"
#include <core/printf.h>

void* find_multiboot_tag(multiboot_info_t* mbi, uint16_t type)
{
    multiboot_tag_t* tag = NULL;

    for (tag = (multiboot_tag_t*)mbi->tags; tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == type)
        {
            printf("Found tage for type=%d\n", type);
            return tag;
        }
    }
    return NULL;
}

reserved_areas_t find_reserved_areas(multiboot_info_t* mbi)
{
    multiboot_tag_t* tag = NULL;
    reserved_areas_t reserved = { .kernel_start = 0,
                                .kernel_end = 0,
                                .multiboot_start = 0,
                                .multiboot_end = 0,
                                .start = 0,
                                .end = 0};
    
    printf("Announced MBI size %#x\n", mbi->size);

    for (tag = (multiboot_tag_t*)mbi->tags; tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                printf("Command line=%s\n",
                    ((multiboot_tag_string_t*)tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                printf("Bootloader name=%s\n",
                    ((multiboot_tag_string_t*)tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                printf("Module at %p-%p with command line=%s\n",
                    ((multiboot_tag_module_t*)tag)->mod_start,
                    ((multiboot_tag_module_t*)tag)->mod_end,
                    ((multiboot_tag_module_t*)tag)->cmdline);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                printf("Lower Memory = %dKB\nUpper Mem = %dKB\n",
                    ((multiboot_tag_basic_meminfo_t*)tag)->mem_lower,
                    ((multiboot_tag_basic_meminfo_t*)tag)->mem_upper);
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                printf("Boot device %#x, %u, %u\n",
                    ((multiboot_tag_bootdev_t*)tag)->biosdev,
                    ((multiboot_tag_bootdev_t*)tag)->slice,
                    ((multiboot_tag_bootdev_t*)tag)->part);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: 
            {
                multiboot_mmap_entry_t* mmap = NULL;

                for (mmap = ((multiboot_tag_mmap_t*)tag)->entries;
                    (uint8_t*)mmap < (uint8_t*)tag + tag->size;
                    mmap = (multiboot_mmap_entry_t*)((uint64_t)mmap +
                    ((multiboot_tag_mmap_t*)tag)->entry_size))
                {
                    printf("mmap base address = %p, length = %#x, type = %#x\n",
                        mmap->addr,
                        mmap->len,
                        mmap->type);
                }
                break;
            }

            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                printf("%s\n", "framebuffer");
                break;

            case MULTIBOOT_TAG_TYPE_APM:
                printf("%s\n", "apm");
                break;

            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                printf("%s\n", "acpi old");
                break;
            
            case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                printf("%s\n", "acpi new");
                break;
            
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
            {
                 multiboot_elf_sections_entry_t* elf = NULL;

        uint32_t i = 0;

        for (
          i = 0, elf = ((multiboot_tag_elf_sections_t*)tag)->sections;
          i < ((multiboot_tag_elf_sections_t*)tag)->num;
          elf =
            (multiboot_elf_sections_entry_t*)((uint64_t)elf +
                                              ((multiboot_tag_elf_sections_t*)
                                                 tag)
                                                ->section_size),
         i++) {
          printf("elf section #%02d addr = %p, type = %#x, size = %#x, "
                     "flags = %#x\n",
                     i,
                     elf->addr,
                     elf->type,
                     elf->size,
                     elf->flags);

          if (elf->type == MULTIBOOT_ELF_SECTION_TYPE_NULL) {
            continue;
          }

          if (elf->addr < reserved.kernel_start || reserved.kernel_start == 0) {
            reserved.kernel_start = elf->addr;
          }

          if ((elf->addr + elf->size) > reserved.kernel_end) {
            reserved.kernel_end = elf->addr + elf->size;
          }
        }
        break;
      }

    case MULTIBOOT_TAG_TYPE_NETWORK:
        printf("%s\n", "network");
        break;

    case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
        printf("%s\n", "load base addr");
        break;

    default:
        printf("tag %#x, size %#x\n", tag->type, tag->size);
        }
    }
    tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    reserved.multiboot_end = (uint64_t)tag;

    printf("Total MBI size %#x\n", (uint64_t)tag - (uint64_t)mbi);

    reserved.start = reserved.kernel_start;

    printf("Kernel start = %#x, Kernel End=%#X, Multiboot Start=%#x, "
            "Multiboot End=%#x, Reserved: Start=%#x, End=%#x\n",
             reserved.kernel_start,
             reserved.kernel_end,
             reserved.multiboot_start,
             reserved.multiboot_end,
             reserved.start,
             reserved.end);

    return reserved;
}