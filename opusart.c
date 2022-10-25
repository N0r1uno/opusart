#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <opus/opusfile.h>


void printHelp();
OggOpusFile *openFile(const char *filename);
OpusPictureTag *extractPictureTag(OggOpusFile *f, const int type);
void writePicureTagToFile(OpusPictureTag *tag, const char *filename);
void writePicureTagToStdout(OpusPictureTag *tag);
void writePictureTagInfo(OpusPictureTag *tag, FILE* to);
const char *pictureTypeToString(const int type);

int main(int argc, char **argv)
{
    if (argc < 3 || argc > 4) {
        printHelp();
        return 1;
    }

    const int type = (argc == 4) ? atoi(argv[1]) : 3;
    const char *in = argv[argc - 2];
    const char *out = argv[argc - 1];

    if (type < 0 || type > 20) {
        fprintf(stderr, "Invalid picture type: %d\n", type);
        return 1;
    }
    
    OggOpusFile *f = openFile(in);
    OpusPictureTag *pt = extractPictureTag(f, type);
    if (strcmp(out, "-") == 0) {
        writePictureTagInfo(pt, stderr);
        writePicureTagToStdout(pt);
    } else {
        writePictureTagInfo(pt, stdout);
        writePicureTagToFile(pt, out);
    }
    op_free(f);
    opus_picture_tag_clear(pt);
}

OggOpusFile *openFile(const char *filename)
{
    int error = 0;
    OggOpusFile *f = op_open_file(filename, &error);
    if (error != 0)
    {
        fprintf(stderr, "Error opening file %s: %d\n", filename, error);
        exit(EXIT_FAILURE);
    }
    return f;
}

OpusPictureTag *extractPictureTag(OggOpusFile *f, const int type)
{
    const OpusTags* t = op_tags(f, 0);
    OpusPictureTag *pt = malloc(sizeof(OpusPictureTag));

    int n = opus_tags_query_count(t, "METADATA_BLOCK_PICTURE");
    for (int i = 0; i < n; i++) {
        int error = opus_picture_tag_parse(pt, opus_tags_query(t, "METADATA_BLOCK_PICTURE", 0));
        if (error == 0) {
            if (pt->type == type) {
                return pt;
            }
        } else {
            fprintf(stderr, "Error parsing picture tag: %d\n", error);
        }
    }
    opus_picture_tag_clear(pt);
    fprintf(stderr, "No picture tag of type %d [%s] found!\n", type, pictureTypeToString(type));
    exit(EXIT_FAILURE);
}

void writePictureTagInfo(OpusPictureTag *tag, FILE* to)
{
    fprintf(to, "Info:\n"
           "  Type: %d [%s]\n"
           "  MIME: %s\n"
           "  Description: %s\n"
           "  Width: %d\n"
           "  Height: %d\n"
           "  Depth: %d\n"
           "  Colors (GIF): %d\n"
           "  Picture data length: %d\n",
           tag->type,
           pictureTypeToString(tag->type),
           tag->mime_type,
           tag->description,
           tag->width,
           tag->height,
           tag->depth,
           tag->colors,
           tag->data_length);
}

const char *pictureTypeToString(const int type)
{
    switch (type)
    {
    case 0:
        return "Other";
    case 1:
        return "32x32 pixels 'file icon' (PNG only)";
    case 2:
        return "Other file icon";
    case 3:
        return "Cover (front)";
    case 4:
        return "Cover (back)";
    case 5:
        return "Leaflet page";
    case 6:
        return "Media (e.g. label side of CD)";
    case 7:
        return "Lead artist/lead performer/soloist";
    case 8:
        return "Artist/performer";
    case 9:
        return "Conductor";
    case 10:
        return "Band/Orchestra";
    case 11:
        return "Composer";
    case 12:
        return "Lyricist/text writer";
    case 13:
        return "Recording Location";
    case 14:
        return "During recording";
    case 15:
        return "During performance";
    case 16:
        return "Movie/video screen capture";
    case 17:
        return "A bright colored fish";
    case 18:
        return "Illustration";
    case 19:
        return "Band/artist logotype";
    case 20:
        return "Publisher/Studio logotype";
    default:
        return "Unknown";
    }
}

void writePicureTagToFile(OpusPictureTag *tag, const char *filename)
{
    FILE *img = fopen(filename, "w");
    fwrite(tag->data, tag->data_length, 1, img);
    fclose(img);
}

void writePicureTagToStdout(OpusPictureTag *tag) {
    fwrite(tag->data, tag->data_length, 1, stdout);
}

void printHelp()
{
    printf("Usage: opusart [type] opus_file image_file\n\n"
           "Read opus cover art.\n\n"
           "opus_file can be:\n"
           "filename\tfile\n\n"
           "image_file can be:\n"
           "filename\tfile\n"
           "\t-\t\tstdout\n\n"
           "type can be:\n"
           "0\t\tOther\n"
           "1\t\t32x32 pixels 'file icon' (PNG only)\n"
           "2\t\tOther file icon\n"
           "3\t\tCover (front) [default]\n"
           "4\t\tCover (back)\n"
           "5\t\tLeaflet page\n"
           "6\t\tMedia (e.g. label side of CD)\n"
           "7\t\tLead artist/lead performer/soloist\n"
           "8\t\tArtist/performer\n"
           "9\t\tConductor\n"
           "10\t\tBand/Orchestra\n"
           "11\t\tComposer\n"
           "12\t\tLyricist/text writer\n"
           "13\t\tRecording Location\n"
           "14\t\tDuring recording\n"
           "15\t\tDuring performance\n"
           "16\t\tMovie/video screen capture\n"
           "17\t\tA bright colored fish\n"
           "18\t\tIllustration\n"
           "19\t\tBand/artist logotype\n"
           "20\t\tPublisher/Studio logotype\n\n");
}