#include <inc/lib.h>

// malloc()
//	This function use BEST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
int end[(USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE] = { 0 };
int vir[(USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE] = { 0 };
uint32 mallocsize;
void* malloc(uint32 size) {
	bool laena = 0;
	int counter = 0;
	mallocsize = size;
	int usersize = USER_HEAP_MAX - USER_HEAP_START;
	int numofpages = usersize / PAGE_SIZE;
	int temp = numofpages + 10;
	size = ROUNDUP(size, PAGE_SIZE);
	int needednumofpages = size / PAGE_SIZE;
	int alama = 0;
	for (int i = 0; i < numofpages; i++) {
		if (vir[i] == 0) {

			counter++;
		} else {
			if (counter >= needednumofpages) {
				if (temp > counter) {
					temp = counter;
					alama = i - counter;
					laena = 1;
				}
			}
			counter = 0;

		}
		if (i == numofpages - 1) {

			if (counter >= needednumofpages) {
				if (temp > counter) {
					temp = counter;
					alama = i - counter + 1;
					laena = 1;
				}
			}
			counter = 0;
		}

	}
	if (laena) {

		for (int i = 0; i < needednumofpages; i++) {
			vir[alama + i] = 1;
			end[alama + i] = needednumofpages;
		}
		uint32 viraddress = (alama * PAGE_SIZE) + USER_HEAP_START;

		sys_allocateMem(viraddress, size);
		return (void*) viraddress;
	} else {
		return NULL;
	}
}
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
	// Write your code here, remove the panic and write your code
	panic("smalloc() is not required...!!");

	// Steps:
	//	1) Implement BEST FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_createSharedObject(...) to invoke the Kernel for allocation of shared variable
	//		sys_createSharedObject(): if succeed, it returns the ID of the created variable. Else, it returns -ve
	//	4) If the Kernel successfully creates the shared variable, return its virtual address
	//	   Else, return NULL

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyBESTFIT() to check the current strategy

	//change this "return" according to your answer
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName) {
	// Write your code here, remove the panic and write your code
	panic("sget() is not required ...!!");

	// Steps:
	//	1) Get the size of the shared variable (use sys_getSizeOfSharedObject())
	//	2) If not exists, return NULL
	//	3) Implement BEST FIT strategy to search the heap for suitable space
	//		to share the variable (should be on 4 KB BOUNDARY)
	//	4) if no suitable space found, return NULL
	//	 Else,
	//	5) Call sys_getSharedObject(...) to invoke the Kernel for sharing this variable
	//		sys_getSharedObject(): if succeed, it returns the ID of the shared variable. Else, it returns -ve
	//	6) If the Kernel successfully share the variable, return its virtual address
	//	   Else, return NULL
	//

	//This function should find the space for sharing the variable
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyBESTFIT() to check the current strategy

	//change this "return" according to your answer
	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address) {
	int start = ((int) virtual_address - USER_HEAP_START) / PAGE_SIZE;
	int endd = end[start];
	int sizeee = endd  * PAGE_SIZE;
	sizeee = ROUNDUP(sizeee, PAGE_SIZE);
	for (int i = start; i < start + endd; i++) {

		vir[i] = 0;
		end[i] = 0;

	}
	uint32 viraddress = (start * PAGE_SIZE) + USER_HEAP_START;

	sys_freeMem((uint32) virtual_address, sizeee);
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=============
// [1] sfree():
//=============
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address) {
	// Write your code here, remove the panic and write your code
	panic("sfree() is not required ...!!");

	//	1) you should find the ID of the shared variable at the given address
	//	2) you need to call sys_freeSharedObject()

}

//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size) {
	//TODO: [PROJECT 2023 - MS2 - [4] Bonus1] realloc() [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");
}
