#ifndef SHA1_HEADER
#define SHA1_HEADER

const unsigned int SHA1_SIZE = (5 * sizeof(unsigned int));  /* 20 */
const unsigned int SHA1_STRING_SIZE = 2 * SHA1_SIZE + 1;    /* 41 */

namespace sha1 {
    const unsigned int BLOCK_SIZE = 64;

    class sha1_t {
        public:
            sha1_t();
            sha1_t(const void* input, const unsigned int input_length, void* signature_);

            void process(const void* input, int input_length);
            void finish(void* signature_ = NULL);
            void get_sig(void* signature_);
            void get_string(void* str_);

        private:
            void initialise();

            void storeBigEndianUint32(unsigned char*, unsigned int);
            void process_block(const unsigned char*);

            unsigned int H0;
            unsigned int H1;
            unsigned int H2;
            unsigned int H3;
            unsigned int H4;

            unsigned char stored[sha1::BLOCK_SIZE * 2];
            unsigned int stored_size;
            unsigned int message_length[2];

            unsigned char signature[SHA1_SIZE]; 
            char str[SHA1_STRING_SIZE];          

            bool finished;
    };

    extern void sig_to_string(const void* signature, char* str);
    extern void sig_from_string(void* signature, const char* str);
} 

#endif
