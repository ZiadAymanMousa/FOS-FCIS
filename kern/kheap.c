#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

uint32 num_free_pages = ((KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE);
uint32 free_page_table[(KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE] ={ 0 };
int end[(KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE]={0} ;

void* kmalloc(unsigned int size) {

	int num_pages = ((ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE));
	int start = -1;
	int block_size = 0;

	for (int i = 0; i < num_free_pages; i++) {
		if (free_page_table[i] == 0) {
			if (block_size == 0 && (i + num_pages) < num_free_pages) {
				start = i;
			}
			block_size++;
			if (block_size == num_pages) {
				end[start]=num_pages;
				break;
			}

		} else {
			start = -1;
			block_size = 0;
		}
	}
	if (start == -1) {
		return NULL;
	}
	void*va = (void*) (KERNEL_HEAP_START + (start * PAGE_SIZE));
	struct Frame_Info *frame_info = NULL;
	for (int i = 0; i < num_pages; i++) {
		int r = allocate_frame(&frame_info);
		if (r == E_NO_MEM) {
			return NULL;
		}
		map_frame(ptr_page_directory, frame_info, (void*) va, PERM_WRITEABLE);
		frame_info->va=(uint32)va;
		va += PAGE_SIZE;
		free_page_table[start + i] = 1;

	}
	if (start != -1) {
		void * virtual_address = (void*) (KERNEL_HEAP_START + (start * PAGE_SIZE));
		return virtual_address;
	} else
		return NULL;
}

void kfree(void* virtual_address) {
	int start_index = (int)(virtual_address - KERNEL_HEAP_START) / PAGE_SIZE;
	int end_index=end[start_index];

	for (int i = start_index; i < start_index+end_index; i++) {
		unmap_frame(ptr_page_directory, virtual_address);
		virtual_address += PAGE_SIZE;
		free_page_table[i] = 0;
		end[i] = 0;

	}
}

unsigned int kheap_virtual_address(unsigned int physical_address) {

	struct Frame_Info *frame_info=to_frame_info(physical_address);
	uint32 virtual_address=(uint32)frame_info->va;

return virtual_address;
}

unsigned int kheap_physical_address(unsigned int virtual_address) {

	uint32 dir_index=PDX(virtual_address);
	uint32 page_index=PTX(virtual_address);
	uint32 *ptr_page_table=NULL;

	get_page_table(ptr_page_directory,(void*)virtual_address,&ptr_page_table);

	if(ptr_page_table!=NULL){
		uint32 entry=ptr_page_table[page_index];
		uint32 physical=entry>>12;


		return physical*PAGE_SIZE;
	}

	return 0;
}

//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void* krealloc(void *virtual_address, uint32 new_size) {
    panic("krealloc() is not implemented yet...!!");
    return NULL;
}
