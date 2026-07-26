#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include "compress.h"

//compressing a file using raw deflate 
//may not be thoughout using stackoverflow for help
int compress_file_raw(FILE *source, FILE *dest)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK_SIZE];  // Input buffer for reading raw file bytes
    unsigned char out[CHUNK_SIZE]; // Output buffer for holding compressed bytes
    // Initialize zlib's internal stream management pointers to default/null
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    // Initialize the compression state using deflateInit2:
    // - Z_DEFAULT_COMPRESSION: Balanced setting for speed vs. size
    // - Z_DEFLATED: Standard compression method
    // - -15: Negative window bits disables zlib/gzip headers and footers (Raw DEFLATE)
    // - 8: Default memory level (internal state allocation)
    // - Z_DEFAULT_STRATEGY: Default tuning for typical data
    ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK)
        return ret;

    // Outer loop: Read and process the source file block by block until completion
    do
    {
        // Read a chunk of raw data from the source file into the input buffer
        strm.avail_in = fread(in, 1, CHUNK_SIZE, source);

        // Check for read errors on the file stream
        if (ferror(source))
        {
            deflateEnd(&strm); // Clean up zlib state before exiting
            return Z_ERRNO;
        }
        // If we reached the end of the file, signal zlib to finish up (Z_FINISH).
        // Otherwise, continue processing normally (Z_NO_FLUSH).
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in; // Point zlib to the start of our input buffer

        // Inner loop: Feed the input chunk into zlib's compressor and flush output chunks
        do
        {
            strm.avail_out = CHUNK_SIZE; // Set available space in output buffer
            strm.next_out = out;         // Point zlib to the start of our output buffer
            // Execute compression on the current chunk slice
            ret = deflate(&strm, flush);
            // Calculate how many compressed bytes were actually produced
            have = CHUNK_SIZE - strm.avail_out;
            // Write the compressed data block to the destination file
            if (fwrite(out, 1, have, dest) != have || ferror(dest))
            {
                deflateEnd(&strm); // Clean up zlib state on write failure
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0); // Repeat if the output buffer filled up completely
    } while (flush != Z_FINISH); // Continue until the final flush state is processed
    // Free internal zlib allocation states to prevent memory leaks
    deflateEnd(&strm);
    return Z_OK;
}