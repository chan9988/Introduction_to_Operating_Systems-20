#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "page_table_driver.h"

int fd;

// Obtain my cr3 value (a.k.a. PML4 table physical address)
uint64_t get_cr3_value()
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_CR3;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Given a physical address, return the value
uint64_t read_physical_address(uint64_t physical_address)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_READ;
	cmd.request[1] = physical_address;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Write value to a physical address
void write_physical_address(uint64_t physical_address, uint64_t value)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_WRITE;
	cmd.request[1] = physical_address;
	cmd.request[2] = value;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
}

int main()
{
	char *x = (char*)aligned_alloc(4096, 4096) + 0x123;
	char *y = (char*)aligned_alloc(4096, 4096) + 0x123;
	strcpy(x, "This is a simple HW.");
	strcpy(y, "You have to modify my page table.");

	fd = open("/dev/os", O_RDONLY);
	if(fd < 0) 
	{
		printf("Cannot open device!\n");
		return 0;
	}

	printf("Before\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 1 */
	// ------------------------------------------------
	// Modify page table entry of y
	// Let y point to x's physical address
	uint64_t offset1[4];
	uint64_t offset2[4];
	offset1[0]=((uint64_t)x>>39&0x1ff)<<3;
	offset1[1]=((uint64_t)x>>30&0x1ff)<<3;
	offset1[2]=((uint64_t)x>>21&0x1ff)<<3;
	offset1[3]=((uint64_t)x>>12&0x1ff)<<3;
	offset2[0]=((uint64_t)y>>39&0x1ff)<<3;
	offset2[1]=((uint64_t)y>>30&0x1ff)<<3;
	offset2[2]=((uint64_t)y>>21&0x1ff)<<3;
	offset2[3]=((uint64_t)y>>12&0x1ff)<<3;
	/*
	for (int i=0;i<4;i++){
		printf("%lx\n",offset1[i]);
	}
	for (int i=0;i<4;i++){
		printf("%lx\n",offset2[i]);
	}
	*/
	uint64_t pml4,pdpt,pd,x_pte,y_pte,y_pte_o;
	pml4=get_cr3_value()+offset1[0];
	pdpt=(read_physical_address(pml4)&0xfffffffffffff000)+offset1[1];
	pd=(read_physical_address(pdpt)&0xfffffffffffff000)+offset1[2];
	x_pte=(read_physical_address(pd)&0xfffffffffffff000)+offset1[3];
	pml4=get_cr3_value()+offset2[0];
	pdpt=(read_physical_address(pml4)&0xfffffffffffff000)+offset2[1];
	pd=(read_physical_address(pdpt)&0xfffffffffffff000)+offset2[2];
	y_pte=(read_physical_address(pd)&0xfffffffffffff000)+offset2[3];
	
	y_pte_o=read_physical_address(y_pte);
	write_physical_address(y_pte,read_physical_address(x_pte));
	// ------------------------------------------------

	getchar();

	printf("After modifying page table\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	getchar();

	strcpy(y, "When you modify y, x is modified actually.");
	printf("After modifying string y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 2 */
	// ------------------------------------------------
	// Recover page table entry of y
	// Let y point to its original address
	// You may need to store y's original address at previous step
	write_physical_address(y_pte,y_pte_o);
	// ------------------------------------------------

	getchar();

	printf("After recovering page table of y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	close(fd);
	return 0;
}

