#include <stdio.h>
#include <stdlib.h>


void getImageSize(FILE *image_file);
void getUsageMessage();
int power(int base, int exponent);

int main(int argc, char const *argv[]) {
	/* code */

	FILE *input_image;
	input_image = fopen("cup.bmp", "rb");

	getImageSize(input_image);

	fclose(input_image);

	getUsageMessage();
	return 0;
}

void getImageSize(FILE *image_file) {
	int byte;
	int count = 0;
	int width[4];
	int height[4];


	if (image_file == NULL) {
		printf("Cannot open the image file.\n");
		exit(-1);
	}

	while (1) {
		byte = fgetc(image_file);
		// if (byte == EOF)
		// 	break;
		count++;
		if (count > 18 && count < 23) {
			width[count - 19] = byte;
		}
		if (count > 22 && count < 27) {
			height[count - 23] = byte;
		}
		if (count >= 27)
			break;
	}

	printf("Image width: %d\n", width[0] + width[1]*power(16,2) + width[2]*power(16,4) + width[3]*power(16,6));
	printf("Image height: %d\n", height[0] + height[1]*power(16,2) + height[2]*power(16,4) + height[3]*power(16,6));

	
}

void getUsageMessage() {
	printf("Usage: bmpedit [OPTIONS...] [input.bmp]\n");
	printf("\n");
	printf("DESCRIPTION:\n"
		"  This program does simple edits of BMP image files. When the program runs it first prints" 
		"out the width and the height of the input image within the BMP file.  Once this is done if" 
		"there is a filter (or sequence of filters) then they are applied to the image.  The resulting image is also"
		"stored using BMP format into an output file.  Without any filters only the width and height of the image is output.\n");
	printf("\n");
	printf("OPTIONS:\n"
		"  -o FILE      Sets the output file for modified images (default output file is \"out.bmp\").\n"
		"  -t 0.0-1.0   Apply a threshold filter to the image with a threshold the threshold value given.\n"
		"  -h           Displays this usage message.\n");
}

int power(int base, int exponent) {
	int i;
	int rst = 1;
	for (i = 0; i < exponent; i++) {
		rst *= base;
	}

	return rst;
}