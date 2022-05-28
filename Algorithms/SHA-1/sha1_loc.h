#ifndef __SHA1_LOC_H__
#define __SHA1_LOC_H__

namespace sha1 {
    inline unsigned int cyclic_left_rotate(unsigned int data, unsigned int shift_bits) {
        return (data << shift_bits) | (data >> (32 - shift_bits));
    }

    inline void make_big_endian_uint32( unsigned char* byte, unsigned int num ) {
        byte[0] = (unsigned char)(num >> 24);
        byte[1] = (unsigned char)(num >> 16);
        byte[2] = (unsigned char)(num >> 8);
        byte[3] = (unsigned char)num;
    }

    const char* HEX_STRING = "0123456789abcdef";
}
#endif
