#include <stdio.h>
#include <stdlib.h>


void getImageSize(FILE *image_file);
void applyThresfoldFilter(FILE *image_file, float threshold_value, char output_file_name[]);
void getUsageMessage();
int power(int base, int exponent);

int image_width = 0;
int image_height = 0;

int main(int argc, char const *argv[]) {
	/* code */

	//parse the main arguments and call related functions.

	FILE *input_image;
	input_image = fopen("cup.bmp", "rb"); 
	if (input_image == NULL) {
		printf("Cannot open the image file.\n");
		exit(-1);
	}

	getImageSize(input_image);
	rewind(input_image);

	char output[] = "out.bmp";
	applyThresfoldFilter(input_image, 0.5, output);
	rewind(input_image);

	fclose(input_image);

	getUsageMessage();
	return 0;
}

void getImageSize(FILE *image_file) {
	int byte;
	int counter = 0;
	int width[4];
	int height[4];

	while (1) {
		byte = fgetc(image_file);
		// if (byte == EOF)
		// 	break;
		counter++;
		if (counter > 18 && counter < 23) {
			width[counter - 19] = byte;
		}
		if (counter > 22 && counter < 27) {
			height[counter - 23] = byte;
		}
		if (counter >= 27) 
			break;
	}

	image_width = width[0] + width[1]*power(16,2) + width[2]*power(16,4) + width[3]*power(16,6);
	image_height = height[0] + height[1]*power(16,2) + height[2]*power(16,4) + height[3]*power(16,6);

	printf("Image width: %d\n", image_width);
	printf("Image height: %d\n", image_height);
	
	return;
}

void applyThresfoldFilter(FILE *image_file, float threshold_value, char output_file_name[]){
	int byte;
	int red, green, blue;
	int counter = 0;
	int padding_size = (4 - ((image_width * 3) % 4)) % 4;
	int row_length = image_width * 3 + padding_size ;
	FILE *output_image;
	output_image = fopen(output_file_name, "wb");

	while(1){
		byte = fgetc(image_file);
		if (byte == EOF){
			// fputc(byte, output_image); 
			break;
		}
		if (counter >= 54 && ((counter - 54) % row_length) < 3 * image_width) {
			blue = byte;
			green = fgetc(image_file);
			red = fgetc(image_file);
			counter += 3;

			float average = ((float) (red + green + blue))/3.0f;

			if (average > threshold_value * 255) {
				red = 255;
				green = 255;
				blue = 255;
			} else {
				red = 0;
				green = 0;
				blue = 0;
			}

			fputc(blue, output_image);
			fputc(green, output_image);
			fputc(red, output_image);

		} else {
			counter++;
		    fputc(byte, output_image);
		}
	}

	fclose(output_image);

	return;
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