/*
 * func.h
 *
 *  Created on: Jan 18, 2022
 *      Author: slasherer
 */

#ifndef SRC_FUNC_H_
#define SRC_FUNC_H_

typedef struct p3_ppm{
	char type; // Holds a 'P'(standing for PPM)
	int magic_num; //Holds a 3 for P3 PPM
	int width; // image width
	int height; //image height
	int color_max; //maximum value for the colors of the pixels; usually 255
	int *pixel_pointer; //points to a block of memory, containing the pixel rgb values
	int num_el; //holds the number of pixel rgb values
}p3_ppm;

typedef struct p6_ppm{
	char type;// Holds a 'P'(standing for PPM)
	int magic_num;//Holds a 6 for P6 ppm
	int width;// image width
	int height;//image height
	int color_max; //maximum value for the colors of the pixels; usually 255
	char *pixel_pointer; //points to a block of memory, containing the pixel rgb values
	int num_el;//holds the number of pixel rgb values
}p6_ppm;
/// printhelp() Prints the program usage
void printHelp();
/// checks the type of the file(p6/p3 ppm or other)
/// \param filename is the name of the file to be checked
/// \return returns the magic number value if it is a ppm
int type_check(char filename[]);
/// stores a given ppm file in a p6_ppm struct
/// \param filename is the name of the file to be stored
/// \param p6_ppm structure for storage
/// \param extra buffer for the pixels
void store_p6(char filename[],p6_ppm*img, char *buffer);
/// stores a given ppm file in a p3_ppm struct
/// \param filename is the name of the file to be stored
/// \param img structure for storage
/// \param extra buffer for the pixels
void store_p3(char filename[] ,p3_ppm *img, int *buffer);
/// prints a character to a p6 ppm output file
/// \param img structure to be printed
/// \param ch is the character
/// \param extra buffer for the pixels
/// \param offset position of the char
/// \param string_num takes 1 or 2 to determine the position of the char(top or bottom)
/// \param font_selection selects font
/// \param new_row_flag determines the position of the char relative to the last iteration
/// \param starting_height the height of the ppm
/// \return returns the new value for the new_row_flag
int printchar_p6(p6_ppm *img, unsigned char ch, int offset,char char_color, int string_num, char font_selection, int new_row_flag, int starting_height);
/// prints a character to a p3 ppm output file
/// \param img structure to be printed
/// \param ch is the character
/// \param extra buffer for the pixels
/// \param offset position of the char
/// \param string_num takes 1 or 2 to determine the position of the char(top or bottom)
/// \param font_selection selects font
/// \param new_row_flag determines the position of the char relative to the last iteration
/// \param starting_height the height of the ppm
/// \return returns the new value for the new_row_flag
int printchar_p3(p3_ppm *img, unsigned char ch, int offset, char char_color, int string_num, char font_selection, int new_row_flag, int starting_height);



#endif /* SRC_FUNC_H_ */
