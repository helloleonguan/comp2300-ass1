/*
Author: Liyang(Leon) Guan
Uni ID: u5684206
Declaration: The following program is all written by meself.
*/


#include <stdio.h>
#include <stdlib.h>

/*
Signatures for all functions used in this program. 
*/
void getImageSize(FILE *image_file);
void applyThresfoldFilter(FILE *image_file, float threshold_value, char output_file_name[]);
void cropIamge(FILE *image_file, int xPos, int yPos, int width, int height, char output_file_name[]);
void getUsageMessage();
int * intToBytes(int number);
int power(int base, int exponent);

/*
Global variables declaration. 
*/
int image_width = 0;
int image_height = 0;
int oFlag = 0;
int tFlag = 0;
int cFlag = 0;
int hFlag = 0;

/*
The main function simply takes in all the arguments, parses them and perfroms relative oprations on the image file. 
*/
int main(int argc, char *argv[]) {
	
	int i;
	int pos;
	char output_file_name[1000];
	float threshold_value;
	FILE *input_image;

	int originX, originY, outWidth, outHeight;

	//parse the main arguments and call related functions.
	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        switch (argv[i][1]) {
        case 'o': 
        	for ( pos = 0; argv[i+1][pos] != '\0'; pos++) {
        		output_file_name[pos] = argv[i+1][pos];
        	}
        	output_file_name[pos] = '\0';
        	oFlag = 1;
        	i++; 
        	break;
        case 't':
        	threshold_value = (float) atof(argv[i+1]); 
        	tFlag = 1;
        	i++; 
        	break;
        case 'h':
        	hFlag = 1;
        	break;
        case 'c':
        	cFlag = 1;
        	originX = (int) atoi(argv[i+1]);
        	originY = (int) atoi(argv[i+2]);
        	outWidth = (int) atoi(argv[i+3]);
        	outHeight = (int) atoi(argv[i+4]);
        	i += 4;
        	break;
        default:
        	printf("Invalid option -%c.\n", argv[i][1]);
        	exit(-1);
        }
    }   

    //Show the help message OR perform operations on the image.
	if (hFlag) {
		printf("\n");
		getUsageMessage();
	} else {
		input_image = fopen(argv[argc - 1], "rb"); 
		if (input_image == NULL) {
			printf("Cannot open the image file.\n");
			exit(-1);
		}

		getImageSize(input_image);
		rewind(input_image);

		if (tFlag) {
			if (oFlag) {
				applyThresfoldFilter(input_image, threshold_value, output_file_name);
			} else {
				applyThresfoldFilter(input_image, threshold_value, "out.bmp");
			}
			rewind(input_image);
		}

		if (cFlag) {
			if (oFlag) {
				cropIamge(input_image, originX, originY, outWidth, outHeight, output_file_name);
			} else {
				cropIamge(input_image, originX, originY, outWidth, outHeight, "out.bmp");
			}
			rewind(input_image);
		}
		fclose(input_image);
	}
	return 0;
}

/*
The function takes in the original image file and extracts the width and height of the file. 
It prints them of onto the console. 
*/
void getImageSize(FILE *image_file) {
	int byte;
	int counter = 0;
	int width[4];
	int height[4];

	// Get the values from the file. 
	while (1) {
		byte = fgetc(image_file);
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

	// Convert them to decimal values.
	image_width = width[0] + width[1]*power(16,2) + width[2]*power(16,4) + width[3]*power(16,6);
	image_height = height[0] + height[1]*power(16,2) + height[2]*power(16,4) + height[3]*power(16,6);

	printf("Image width: %d\n", image_width);
	printf("Image height: %d\n", image_height);
	
	return;
}

/*
The function takes in the original image file, a thredhold value and the output file name 
	to apply a threshold filter.
It outputs an image file. 
*/
void applyThresfoldFilter(FILE *image_file, float threshold_value, char output_file_name[]){
	int byte;
	int red, green, blue;
	int counter = 0;
	int padding_size = (4 - ((image_width * 3) % 4)) % 4; // computing the size of padding
	int row_length = image_width * 3 + padding_size ;
	FILE *output_image;
	output_image = fopen(output_file_name, "wb");

	while(1){
		byte = fgetc(image_file);
		if (byte == EOF){
			break;
		}

		// apply the threshold filter to RGB values and copy over the rest of the file. 
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

/*
The function takes in the original image file, 4 parameters and output file name 
	to crop the image based on those 4 values (in number of pixels).
It outputs an image file. 
*/
void cropIamge(FILE *image_file, int xPos, int yPos, int width, int height, char output_file_name[]){
	// Check whether the requested output image is out of the original size. 
	if (xPos + width > image_width || yPos + height > image_height || xPos < 0 || yPos < 0 || width <= 0 || height <= 0) {
		printf("The size of the output image is out of the original image.\n");
		exit(-1);
	}

	int byte;
	int i;
	int counter = 0;
	int output_image_pos;
	int padding_size = (4 - ((image_width * 3) % 4)) % 4; // computing the size of padding for the original image.
	int row_length = image_width * 3 + padding_size;
	int output_padding_size = (4 - ((width * 3) % 4)) % 4; // computing the size of padding for the output image. 
	int output_row_length = width * 3 + output_padding_size;
	FILE *output_image;
	output_image = fopen(output_file_name, "wb");

	// Change the size of the image in the header of the file and copy over the RGB values in the requested area.
	while(1) {
		byte = fgetc(image_file);
		output_image_pos = ftell(output_image);
		if (counter >= 54 && (output_image_pos - 54) % output_row_length == 3 * width) {
			for(i = 0; i < output_padding_size; i++) {
				fputc(0, output_image);
			}
		}

		if (byte == EOF){
			break;
		}

		// Change the size of the BMP file.
		if (counter == 2) {
			int size_of_output_file = 54 + output_row_length * height;
			int *size_in_byte = intToBytes(size_of_output_file);

			for (i = 0; i < 4; i++) {
				fputc(* (size_in_byte + i), output_image);
			}

			counter += 4;
			fseek(image_file, 3, SEEK_CUR);

			// Change the values for the width and the height of the image in pixels. 
		} else if (counter == 18) {
			int *width_in_byte = intToBytes(width);
			for (i = 0; i < 4; i++) {
				fputc(* (width_in_byte + i), output_image);
			}
			int *height_in_byte = intToBytes(height);
			for (i = 0; i < 4; i++) {
				fputc(* (height_in_byte + i), output_image);
			}

			counter += 8;
			fseek(image_file, 7, SEEK_CUR);

			// Chnage the size of the raw bitmap data (including padding). 
		} else if (counter == 34) {
			int *color_array_size_in_byte = intToBytes(output_row_length * height);

			for (i = 0; i < 4; i++) {
				fputc(* (color_array_size_in_byte + i), output_image);
			}

			counter += 4;
			fseek(image_file, 3, SEEK_CUR);

			// Copy over the RGB values in the requested area (including padding). 
		} else if (counter >= 54) {
			int currentX = ((counter - 54) % row_length) / 3;
			int currentY = (counter - 54) / row_length;

			if (currentX >= xPos && currentX < xPos + width && currentY >= yPos && currentY < yPos + height) {
				fputc(byte, output_image);
			}
			counter++;
		} else {
			counter++;
		    fputc(byte, output_image);
		}

	}

	fclose(output_image);
	return;
}

/*
The function prints out the help message onto the console. 
*/
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
		"  -h           Displays this usage message.\n"
		"  -c [originX] [originY] [width] [height]   Crop the image from the origin(left-bottom corner).\n");
}

/*
This function converts an integer to 4 integers stading for the encoding of that integer in 4 bytes.
It returns a pointer pointing to the head of the int array. 
*/
int * intToBytes(int number) {
	static int rst[4];

	rst[3] = number / power(16,6);
	number -= rst[3] * power(16,6);
	rst[2] = number / power(16,4);
	number -= rst[2] * power(16,4);
	rst[1] = number / power(16,2);
	number -= rst[1] * power(16,2);
	rst[0] = number;

	return rst;

}

/*
This function takes two integers (a, b) and return the result of a to the power of b. 
*/
int power(int base, int exponent) {
	int i;
	int rst = 1;
	for (i = 0; i < exponent; i++) {
		rst *= base;
	}

	return rst;
}