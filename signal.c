#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <png.h>

void write_png(const char *filename, int *data, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "Could not allocate write struct\n");
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, (png_infopp)NULL);
        fclose(fp);
        fprintf(stderr, "Could not allocate info struct\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        fprintf(stderr, "Error during init_io\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_bytep row = (png_bytep) malloc(4 * width);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            row[x] = (png_byte) (data[y * width + x]);
        }
        png_write_row(png, row);
    }

    free(row);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    SNDFILE *file;
    SF_INFO sfinfo;
    file = sf_open("signal.wav", SFM_READ, &sfinfo);
    
    if (!file) {
        fprintf(stderr, "Could not open signal.wav\n");
        return 1;
    }

    int *amp_arr = malloc(sfinfo.frames * sizeof(int));
    sf_readf_int(file, amp_arr, sfinfo.frames);
    sf_close(file);

    int width = sfinfo.frames / 2;  // Adjust width according to your logic
    int height = 1;  // For a 1D representation

    write_png("signal.png", amp_arr, width, height);
    free(amp_arr);
    return 0;
}
