#include <stdio.h>
#include <stdlib.h>

int power(int base, int exponent);
void getImageSize();

int main(int argc, char const *argv[]) {
	/* code */
	int byte;
	int count = 0;
	int width[4];
	int height[4];

	FILE *input_image;
	input_image = fopen("cup.bmp", "rb");
	if (input_image == NULL) {
		printf("Cannot open the image file.\n");
		exit(-1);
	}

	while (1) {
		byte = fgetc(input_image);
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
	return 0;
}

int power(int base, int exponent) {
	int i;
	int rst = 1;
	for (i = 0; i < exponent; i++) {
		rst *= base;
	}

	return rst;
}