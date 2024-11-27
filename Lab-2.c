//lab2

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;  // Defining a type alias for unsigned char as uchar

// Function prototypes
int readPGMText(const char* filename, int* width, int* height, int* maxval, uchar** pixels);
int writePGMText(const char* filename, int width, int height, int maxval, uchar* pixels);
int writePGMBinary(const char* filename, int width, int height, int maxval, uchar* pixels);
void embedLSB(int width, int height, uchar* coverPixels, uchar* secretPixels, uchar** stegoPixels);
void extractLSB(int width, int height, uchar* stegoPixels, uchar** outputPixels);

int main() {
    int width, height, maxval;
    uchar *coverPixels = NULL, *secretPixels = NULL, *stegoPixels = NULL, *outputPixels = NULL;

    // Reading the cover image
    if (readPGMText("C:/Users/misle/Downloads/Lab4_Q2/baboon.pgm", &width, &height, &maxval, &coverPixels) != 0) {
        printf("Failed to read cover image.\n");
        return 1;
    }

    // Reading the secret image
    if (readPGMText("C:/Users/misle/Downloads/Lab4_Q2/farm.pgm", &width, &height, &maxval, &secretPixels) != 0) {
        printf("Failed to read secret image.\n");
        free(coverPixels);  // Freeing memory for cover image in case of failure
        return 1;
    }

    // Ensuring that the dimensions match for both cover and secret images
    if (width != height) {
        printf("Error: Image dimensions do not match.\n");
        free(coverPixels);  // Freeing memory in case of error
        free(secretPixels); 
        return 1;
    }

    // Embedding the secret image into the cover image using LSB steganography
    embedLSB(width, height, coverPixels, secretPixels, &stegoPixels);

    // Writing the stego image (image with embedded secret) to a binary PGM file
    if (writePGMBinary("C:/Users/misle/Downloads/Lab4_Q2/stego_image_bin.pgm", width, height, maxval, stegoPixels) != 0) {
        printf("Failed to write stego image.\n");
        free(coverPixels);
        free(secretPixels);
        free(stegoPixels);  // Freeing memory in case of failure
        return 1;
    }

    // Extracting the secret image from the stego image
    extractLSB(width, height, stegoPixels, &outputPixels);

    // Writing the extracted secret image to a text PGM file
    if (writePGMText("C:/Users/misle/Downloads/Lab4_Q2/extracted_secret.pgm", width, height, maxval, outputPixels) != 0) {
        printf("Failed to write extracted secret image.\n");
    }

    // Freeing all allocated memory after processing
    free(coverPixels);
    free(secretPixels);
    free(stegoPixels);
    free(outputPixels);

    return 0;
}

// Function to read a PGM image in text format (P2)
int readPGMText(const char* filename, int* width, int* height, int* maxval, uchar** pixels) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return 1;
    }

    // Read the PGM header
    char buffer[16];
    if (!fgets(buffer, sizeof(buffer), file)) {
        fclose(file);
        return 1;
    }

    // Checking if the format is P2 (text-based PGM format)
    if (buffer[0] != 'P' || buffer[1] != '2') {
        printf("Error: invalid file format (must be 'P2').\n");
        fclose(file);
        return 1;
    }

    // Skip over comment lines
    do {
        fgets(buffer, sizeof(buffer), file);
    } while (buffer[0] == '#');

    // Read the image width, height, and maximum pixel value (maxval)
    sscanf(buffer, "%d %d", width, height);
    fscanf(file, "%d", maxval);

    // Allocate memory for the pixel data
    int size = (*width) * (*height);
    *pixels = (uchar*) malloc(size * sizeof(uchar));
    if (*pixels == NULL) {
        printf("Error: failed to allocate memory.\n");
        fclose(file);
        return 1;
    }

    // Read pixel values from the file
    for (int i = 0; i < size; i++) {
        fscanf(file, "%hhu", (*pixels) + i);  // Reading each pixel as an unsigned char
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in text format (P2)
int writePGMText(const char* filename, int width, int height, int maxval, uchar* pixels) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return 1;
    }

    // Write the PGM header
    fprintf(file, "P2\n%d %d\n%d\n", width, height, maxval);

    // Write pixel data to the file
    int size = width * height;
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", pixels[i]);
        if (i % 17 == 0) fprintf(file, "\n");  // Adding newlines for readability
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in binary format (P5)
int writePGMBinary(const char* filename, int width, int height, int maxval, uchar* pixels) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return 1;
    }

    // Write the PGM header
    fprintf(file, "P5\n%d %d\n%d\n", width, height, maxval);

    // Write pixel data in binary format
    int size = width * height;
    fwrite(pixels, sizeof(uchar), size, file);

    fclose(file);
    return 0;
}

// Function to embed the secret image into the cover image using Least Significant Bit (LSB) steganography
void embedLSB(int width, int height, uchar* coverPixels, uchar* secretPixels, uchar** stegoPixels) {
    int size = width * height;
    *stegoPixels = (uchar*) malloc(size * sizeof(uchar));  // Allocate memory for the stego image
    if (*stegoPixels == NULL) {
        printf("Error: failed to allocate memory for stego image.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        // Extract the 4 most significant bits (MSBs) from the cover image pixel
        uchar coverMSB = coverPixels[i] & 0xF0;  
        // Extract the 4 MSBs from the secret image pixel and shift them to the 4 least significant bits
        uchar secretMSB = (secretPixels[i] & 0xF0) >> 4;  
        // Combine the cover and secret MSBs into the stego pixel
        (*stegoPixels)[i] = coverMSB | secretMSB;
    }
}

// Function to extract the secret image from the stego image using LSB extraction
void extractLSB(int width, int height, uchar* stegoPixels, uchar** outputPixels) {
    int size = width * height;
    *outputPixels = (uchar*) malloc(size * sizeof(uchar));  // Allocate memory for the extracted secret image
    if (*outputPixels == NULL) {
        printf("Error: failed to allocate memory for extracted secret image.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        // Extract the 4 least significant bits (LSBs) from the stego pixel and shift them to the most significant bits
        uchar secretMSB = (stegoPixels[i] & 0x0F) << 4;  
        (*outputPixels)[i] = secretMSB;  // Store the secret MSBs as the extracted secret pixel
    }
}
