
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "func.h"
#include "6x8_horizontal_LSB_1.h"
#include "7x12_horizontal_LSB_1.h"
#include "font8x8_basic.h"

void printHelp() {
    printf("\n");
    printf("  USAGE: ./<programm_name> -flag argument(s) -flag argument(s) -flag argument(s) \n");
    printf("  Options needed for the programm to start:\n");
    printf("  -f <filepath>   //(from root)\n");
    printf("  Optional, but strongly recomended options\n");
    printf("  -t <top text>   //use " " if the text has more than one word\n");
    printf("  -b <bottom text>   //use " " if the text has more than one word\n");
    printf("  -c <color_choice> \n"
    	   "        Possible colors: black (argument: 'b'), yellow (argument: 'y'), green (argument: 'g'), red (argument: 'r'),white (argument: 'w')\n");
    printf("  -l <font_choice> \n"
    	   "        Possible fonts: basic (argument: 'b'), monochrome (argument: 'm'), hunter (argument: 'h')\n");
    printf("  -h help\n");
    printf("  Example : ./Meme_Generator -f image.ppm -c y -l m -t Sample_top_text -b Sample_bottom_text\n\n");
    exit(0);
}

int type_check(char filename[]){

	int magic_number=0;
	FILE *input_file = fopen(filename, "rb");
	if(input_file==NULL){
		printf("\n The file could not be opened\n");
	    exit(1);
	}
	fgetc(input_file);
	fscanf(input_file, "%d", &magic_number);
	fclose(input_file);
	return magic_number;
}

void store_p6(char filename[],p6_ppm*img, char *buffer){
	char commentcheck;
	FILE *input_file = fopen(filename, "rb");
	if(input_file==NULL){
		printf("\n The file could not be opened\n");
	    exit(1);
	}
	fscanf(input_file, "%c", &img->type);
	fscanf(input_file, "%d\n", &img->magic_num);
	//checks for comments and ignores saving them
	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}
	fscanf(input_file, "%d %d\n", &img->width, &img->height);
	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}
	fscanf(input_file, "%d\n", &img->color_max);
	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}

	buffer = malloc((img->width)*(img->height)*sizeof(char)*3);
	img->pixel_pointer = buffer;

	int i;
	for(i=0; (i<(3*(img->width)*(img->height)))&&(!feof(input_file)); i++){
		fscanf(input_file, "%c", (img->pixel_pointer+i));

	}
	img->num_el = i;
	fclose(input_file);
}

void store_p3(char filename[] ,p3_ppm *img, int *buffer){
	FILE *input_file = fopen(filename, "rb");
	if(input_file==NULL){
	        printf("\n The file could not be opened");
	        exit(1);
	}
	img->type = fgetc(input_file);


	fscanf(input_file, "%d\n", &img->magic_num);
	//checks for comments and ignores saving them
	char commentcheck;
	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}
	fscanf(input_file, "%d %d\n", &img->width, &img->height);

	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}
	fscanf(input_file, "%d\n", &img->color_max);
	commentcheck=getc(input_file);
	if(commentcheck=='#'){
		fscanf(input_file, "%*[^\n]\n");
	}
	else{
		fseek( input_file, -1, SEEK_CUR);
	}

	buffer = malloc((img->width)*(img->height)*sizeof(unsigned int)*3);
	img->pixel_pointer = buffer;
	int i;
	for(i=0; (i<(3*(img->width)*(img->height)))&&(!feof(input_file)); i++){
		fscanf(input_file, "%d ", (img->pixel_pointer+i));
	}
	img->num_el = i;
	fclose(input_file);
}

int printchar_p6(p6_ppm *img, unsigned char ch, int offset,char char_color, int string_num, char font_selection, int new_row_flag, int starting_height){
	FILE *output_file = fopen("out.ppm", "w");

	int bitmap_flag;
	//scanline counter for the character printing
	int bitmap_line_cnt=0;
	int font_default_size;
	//reference size for the chacacters
	int char_size =ceil(sqrt((img->width)*(starting_height)/(300*224)));
	int new_scanline_offset;
	int scanline_cnt = 1;
	//setting the minimum size
	if(char_size<1){
		char_size=1;
	}
		if((font_selection == 'b')||(font_selection == 'B')){

			font_default_size = 8;
		}
		else if((font_selection == 'm')||(font_selection == 'M')){


			font_default_size = 12;
		}
		else if((font_selection == 'h')||(font_selection == 'H')){
			font_default_size = 8;
		}else{
			font_default_size = 8;
		}
		//Coefficient for a new row in case of a long string
		int new_row_offset = (img->width)*new_row_flag*font_default_size*char_size + new_row_flag*font_default_size*char_size;
		//New height in case of a long bottom string
		if((new_row_flag>0)&&(string_num ==2)){

			img->height = starting_height + new_row_flag*font_default_size*char_size;

		}
		//printing header
		fprintf(output_file,"%c%d\n%d %d\n%d\n", img->type,6, img->width, img->height, img->color_max);
	//printing pixel loop
	for(int i=1; i<((img->num_el/3)+1+font_default_size*char_size + new_row_flag*(img->width)*font_default_size*char_size); i++){
		//offset value for a new row in case of a long string
		int temp = bitmap_line_cnt*((img->width))+(img->width)/16+2+offset*font_default_size*char_size+new_row_offset;

		if(string_num ==1){
			new_scanline_offset = temp;
		}else{
			//adding the offset in case of a bottom string
			new_scanline_offset = temp+(img->width)*(starting_height-font_default_size*char_size);
		}
			//bitmap character printing loop
			//the set variable is "set" when a value of the bitmap char is 1
			//and a color pixel is printed instead of the pixel stored in the array
			 if((bitmap_line_cnt<font_default_size*(char_size))&&((i==new_scanline_offset+bitmap_line_cnt))){
				 //font selection
					for (int y=0; y < font_default_size; y++) {
						if((font_selection == 'b')||(font_selection == 'B')){
							bitmap_flag = font8x8_basic[ch][bitmap_line_cnt/char_size] & 1 << (y);
						}
						else if((font_selection == 'm')||(font_selection == 'M')){

							bitmap_flag = seven_font[ch][bitmap_line_cnt/char_size] & 1 << (y);
						}
						else if((font_selection == 'h')||(font_selection == 'H')){
							bitmap_flag = six_font[ch][(bitmap_line_cnt/char_size)] & 1 << (y);
						}else{
							bitmap_flag = font8x8_basic[ch][bitmap_line_cnt/char_size] & 1 << (y);
						}
						for (int i1=0; i1<char_size; i1++){
							 if(bitmap_flag){
								 //color selection
									switch(char_color){
										case('B'):
										case ('b'):
											fprintf(output_file, "%c%c%c",0,0,0);
											break;
										case('G'):
										case ('g'):
											fprintf(output_file, "%c%c%c",0,128,0);
											break;
										case('R'):
										case ('r'):
											fprintf(output_file, "%c%c%c",255,0,0);
											break;
										case('Y'):
										case ('y'):
											fprintf(output_file, "%c%c%c",255,255,0);
											break;
										case('W'):
										case ('w'):
											fprintf(output_file, "%c%c%c",255,255,255);
											break;
										default:
											fprintf(output_file, "%c%c%c",0,0,0);
											break;
									}
								img->pixel_pointer = img->pixel_pointer+3;
								}
							else{
								//in case of a bottom overflow; prints a gray string row
								if(i>=(img->num_el/3)+1+font_default_size*char_size){
									fprintf(output_file, "%c%c%c",100,100,100);
								}else{
									fprintf(output_file, "%c%c%c",*img->pixel_pointer,*(img->pixel_pointer+1),*(img->pixel_pointer+2));
										img->pixel_pointer = img->pixel_pointer+3;
								}

								}
							i++;
						}
						if((i/((img->width)*3)))
						{
							scanline_cnt++ ;

						}

					} //setting a new string row (top)
					if(((font_default_size*char_size)>(((scanline_cnt)*(img->width)*3)-i))&&(string_num==1)){
						new_row_flag++;

					}
					if((new_scanline_offset+font_default_size*char_size+bitmap_line_cnt>=(img->num_el/3)+new_row_flag*(img->width)*font_default_size*char_size)){
						new_row_flag ++;
					}
					bitmap_line_cnt++;
				 }
			else{
				if(i>=(img->num_el/3)+1+font_default_size*char_size){
					fprintf(output_file, "%c%c%c",100,100,100);
					}else{
						fprintf(output_file, "%c%c%c",*img->pixel_pointer,*(img->pixel_pointer+1),*(img->pixel_pointer+2));
						img->pixel_pointer = img->pixel_pointer+3;
					}
			}

	}
fclose(output_file);
//tells the function that a string row is to be created in the comming iterations
return new_row_flag;
}

int printchar_p3(p3_ppm *img, unsigned char ch, int offset, char char_color, int string_num, char font_selection, int new_row_flag, int starting_height){
	FILE *output_file = fopen("out.ppm", "w");

		int bitmap_flag;
		//scanline counter for the character printing
		int bitmap_line_cnt=0;
		int font_default_size;
		//reference size for the chacacters
		int char_size =ceil(sqrt((img->width)*(starting_height)/(300*224)));
		int new_scanline_offset;
		int scanline_cnt = 1;

		//setting the minimum size
		if(char_size<1){
			char_size=1;
		}
			if((font_selection == 'b')||(font_selection == 'B')){

				font_default_size = 8;
			}
			else if((font_selection == 'm')||(font_selection == 'M')){

				//font_size = 13;
				font_default_size = 12;
			}
			else if((font_selection == 'h')||(font_selection == 'H')){
				font_default_size = 8;
			}else{
				font_default_size = 8;
			}
			//Coefficient for a new row in case of a long string
			int new_row_offset = (img->width)*new_row_flag*font_default_size*char_size + new_row_flag*font_default_size*char_size;

			//New height new row in case of a long bottom string
			if((new_row_flag>0)&&(string_num ==2)){

				img->height = starting_height + new_row_flag*font_default_size*char_size;

			}
			//printing header
			fprintf(output_file,"%c%d\n%d %d\n%d\n", img->type,3, img->width, img->height, img->color_max);

		//printing pixel loop
		for(int i=1; i<((img->num_el/3)+1+font_default_size*char_size + new_row_flag*(img->width)*font_default_size*char_size); i++){

			//offset value for a new row in case of a long string
			int temp = bitmap_line_cnt*((img->width))+(img->width)/16+2+offset*font_default_size*char_size+new_row_offset;

			if(string_num ==1){
				new_scanline_offset = temp;
			}else{
				//adding the offset in case of a bottom string
				new_scanline_offset = temp+(img->width)*(starting_height-font_default_size*char_size);
			}
				//bitmap character printing loop
				//the set variable is "set" when a value of the bitmap char is 1
				//and a color pixel is printed instead of the pixel stored in the array
				 if((bitmap_line_cnt<font_default_size*(char_size))&&((i==new_scanline_offset+bitmap_line_cnt))){
					 //font selection
						for (int y=0; y < font_default_size; y++) {
							if((font_selection == 'b')||(font_selection == 'B')){
								bitmap_flag = font8x8_basic[ch][bitmap_line_cnt/char_size] & 1 << (y);
							}
							else if((font_selection == 'm')||(font_selection == 'M')){
								//set = bitshift(mono_font[ch-31][((-2-x))/size]) & 1 << (y);
								bitmap_flag = seven_font[ch][bitmap_line_cnt/char_size] & 1 << (y);
							}
							else if((font_selection == 'h')||(font_selection == 'H')){
								bitmap_flag = six_font[ch][(bitmap_line_cnt/char_size)] & 1 << (y);
							}else{
								bitmap_flag = font8x8_basic[ch][bitmap_line_cnt/char_size] & 1 << (y);
							}
							for (int i1=0; i1<char_size; i1++){

								if(bitmap_flag){
									//color selection
									switch(char_color){
										case('B'):
										case ('b'):
											fprintf(output_file, "%d %d %d ",0,0,0);
											break;
										case('G'):
										case ('g'):
											fprintf(output_file, "%d %d %d ",0,128,0);
											break;
										case('R'):
										case ('r'):
											fprintf(output_file, "%d %d %d ",255,0,0);
											break;
										case('Y'):
										case ('y'):
											fprintf(output_file, "%d %d %d ",255,255,0);
											break;
										case('W'):
										case ('w'):
											fprintf(output_file, "%d %d %d ",255,255,255);
											break;
										default:
											fprintf(output_file, "%d %d %d ",0,0,0);
											break;
									}

					img->pixel_pointer = img->pixel_pointer+3;
					}
				else{
					//in case of a bottom overflow; prints a gray string row
					if(i>=(img->num_el/3)+1+font_default_size*char_size){
						fprintf(output_file, "%d %d %d ",100,100,100);
					//img->pixel_pointer = img->pixel_pointer+3;
					}else{
						fprintf(output_file, "%d %d %d ",*img->pixel_pointer,*(img->pixel_pointer+1),*(img->pixel_pointer+2));
							img->pixel_pointer = img->pixel_pointer+3;
					}

					}
				i++;
			}if((i/((img->width)*3))) {
				scanline_cnt++ ;
			}

		}//setting a new string row (top)
		if(((font_default_size*char_size)>=(((scanline_cnt)*(img->width)*3)-i))&&(string_num==1)){
			new_row_flag++;

		}//setting a new string row (bottom)
		if((new_scanline_offset+font_default_size*char_size+bitmap_line_cnt>=(img->num_el/3)+new_row_flag*(img->width)*font_default_size*char_size)){
			new_row_flag ++;
		}
		bitmap_line_cnt++;
	 }
else{
	if(i>=(img->num_el/3)+1+font_default_size*char_size){
		fprintf(output_file, "%d %d %d ",100,100,100);;
		}else{
			fprintf(output_file, "%d %d %d ",*img->pixel_pointer,*(img->pixel_pointer+1),*(img->pixel_pointer+2));
			img->pixel_pointer = img->pixel_pointer+3;
		}
}

}
fclose(output_file);
//tells the function that a string row is to be created in the comming iterations
return new_row_flag;
}
