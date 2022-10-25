#include <stdio.h>
#include <stdlib.h>
#include <opus/opus_multistream.h>
#include <opus/opusfile.h>

void printHelp();
OggOpusFile* openFile(char* filename);
OpusPictureTag* extractPictureTag(OggOpusFile* f);
void writePicureTagToFile(OpusPictureTag* tag, char* filename);

int main(int argc, char **argv) {
    // opusart 
    //printHelp();

    OggOpusFile *f = openFile("Brothers of Metal - Hel.opus");
    OpusPictureTag* pt = extractPictureTag(f);
    writePicureTagToFile(pt, "test.jpg");


    op_free(f);
    free(pt);

    (void) argc;
    (void) argv;
}

OggOpusFile* openFile(char* filename) {
    int error = 0;
    OggOpusFile *f = op_open_file(filename, &error);
    if (error != 0) {
        printf("Error opening file: %d", error);
        exit(EXIT_FAILURE);
    }
    return f;
}

OpusPictureTag* extractPictureTag(OggOpusFile* f) {
    const OpusTags* t = op_tags(f, 0);
    OpusPictureTag* pt = malloc(sizeof(OpusPictureTag));
    int error = opus_picture_tag_parse(pt, opus_tags_query(t, "METADATA_BLOCK_PICTURE", 0));
    if (error != 0) {
        printf("Error parsing picture tag: %d", error);
        exit(EXIT_FAILURE);
    }
    return pt;
}

void writePicureTagToFile(OpusPictureTag* tag, char* filename) {
    FILE *img = fopen(filename, "w");
    fwrite(tag->data, tag->data_length, 1, img);
    fclose(img);
}


void printHelp() {
    printf("Usage: opusart [options] opus_file image_file\n\n"
        "Write and read opus cover art.\n\n"
        "opus_file can be:\n"
        "filename.opus\tfile\n"
        "image_file can be:\n"
        "filename.jpg\tfile\n"
        "-\t\tstdin/stdout\n\n"
        "Options:\n"
        "-h\t\tprint this help\n"
        "-v\t\tprint version\n"
        "-w\t\twrite cover art\n"
        "-r\t\tread cover art\n");
    exit(0);
}