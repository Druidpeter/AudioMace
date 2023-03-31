#include <stdlib.h>
#include <unistd.h>

#include <sndfile.h>

#include "yxml.h"

void analyze_data(SNDFILE *data, SF_INFO data_info, int len)
{
    int buffer_len = data_info.channels * len;
    int buffer[buffer_len];

    int nfr = sf_read_int(data, buffer, buffer_len);

    if (nfr > 0) {
        for(int i = 0; i < nfr; ++i){
            printf("Frame Output: 0x%08x \n", buffer[i]);
        };
    } else {
        printf("No frame data available.\n");
    };
}

typedef struct bin_params BIN_PARAMS;

struct bin_params{
    int request_length;
};

void parse_params(int argc, char *argv[], BIN_PARAMS *params)
{
    const char *optstring = "n:";
    char opt;

    // First, fill out default values
    params->request_length = 10;

    do {
        opt = getopt(argc, argv, optstring);

        if(opt == 'n'){
            params->request_length = atoi(optarg);
        } else if(opt == '?'){
            break;
        }
    } while (opt != -1);
}

const char *getManifest()
{
    FILE *fd = fopen("manifest.xml", "rb");

    if(!fd){
        return NULL;
    }

    fseek(fd, 0, SEEK_END);
    long length = ftell(fd);
    rewind(fd);

    char *buffer = malloc(length);

    if(!buffer){
        fclose(fd);
        return NULL;
    }

    fread(buffer, 1, length, fd);
    return buffer;
}

/*
 * Convert an UTF-8 encoded char * array into the correct
 * sequence of sound-files for speech synthesis.
 */

void convert(const char *dialogue, SNDFILE *buffer)
{
    // Ok, so UTF-8 is a an absolute BITCH. So this is
    // how we're going to do this. We start at the beginning
    // of the string. We look for FF FE or FE FF byte order.

    // That will tell us how to interpret our bytes moving
    // forward.

    // Next, we seek through until we find a char that has
    // 00, 110, 1110, or 1111 0X. We then copy the value of
    // each subsequent bytes into a 32 bit integer, makng
    // sure to lshift/rshift appropriately so that we can
    // just sum them together.

    // We then use a switch statement that compares the
    // resulting 32-bit integer with our predetermined
    // hexadecimal literals. When we find a match, we
    // we load the audio data from the correct sound file.

    // Once we have our loaded audio data, we need to take
    // care to blend it with the end of our current working
    // buffer before appending it to the buffer and seeking
    // for the next codepoint.
}

/*
 * Attach some sound-file data to the end of a sound-file
 * buffer, blending the data together using opts.
 */

void blendBuffer(SNDFILE *buffer, SNDFILE* data, uint32_t opts)
{
    // opts is a bitfield. We check the bits to determine
    // how, or if, to blend the data.

    // The most basic way to do this would be to just replace
    // each sample in time with the average of the two overlayed
    // samples. But we'll see.
}

int main(int argc, char *argv[])
{
    SF_INFO data_info = {.format = 0};

    BIN_PARAMS params;
    parse_params(argc, argv, &params);

    // Sample rate = 44100
    // channels = 2
    // format = SF_FORMAT_WAV

    // Test dialogue for now. Eventually this will be supplied by file or
    // command line option.
    const char *dialogue = "ɤʒɑəø"
}
