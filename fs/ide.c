/*
 * Minimal PIO-based (non-interrupt-driven) IDE driver code.
 * For information about what all this IDE/ATA magic means,
 * see for example "The Guide to ATA/ATAPI documentation" at:
 *	http://www.stanford.edu/~csapuntz/ide.html
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
//      read data from IDE disk. First issue a read request through
//      disk register and then copy data from disk buffer
//      (512 bytes, a sector) to destination array.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      dst: destination for data read from IDE disk.
//      nsecs: the number of sectors to read.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
    // 0x200: the size of a sector: 512 bytes.
    int begin = secno * 512;
    int end = begin + nsecs * 512;
    int offset = begin;
    int zero = 0, ret;

    while (offset < end) {
        // copy data from disk buffer(512 bytes, a sector) to destination array.
        syscall_write_dev(&diskno, 0x13000010, 4); //sizeof(u_int)
        syscall_write_dev(&offset, 0x13000000, 4);
        syscall_write_dev(&zero, 0x13000020, 4);
        syscall_read_dev(&ret, 0x13000030, 4);
        if (ret != 0) {
            syscall_read_dev(dst + (offset - begin), 0x13004000, 512);
            offset += 512;
        } else {
            // error occurred, then panic.
            user_panic("disk I/O error");
        }
    }
}

// Overview:
//      write data to IDE disk.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      src: the source data to write into IDE disk.
//      nsecs: the number of sectors to write.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
    int begin = secno * 512;
    int end = begin + nsecs * 512;
    int offset = begin;
    int one = 1, ret;

    writef("diskno: %d\n", diskno);
    while (offset < end) {
        // copy data from source array to disk buffer.
        syscall_write_dev(src + (offset - begin), 0x13004000, 512);
        syscall_write_dev(&diskno, 0x13000010, 4); //sizeof(u_int)
        syscall_write_dev(&offset, 0x13000000, 4);
        syscall_write_dev(&one, 0x13000020, 4);
        syscall_read_dev(&ret, 0x13000030, 4);
        if (ret != 0) {
            offset += 512;
        } else {
            // if error occur, then panic.
            user_panic("disk I/O error");
        }
    }
}
