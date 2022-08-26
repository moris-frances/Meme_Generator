
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "func.h"


int main(int argc, char *argv[]){

	char *file_choice = "";
	char *str_up = "";
	char *str_down = "";
	char color= 'b';
	char font_choice= 'b';

	int option;

		while((option=getopt(argc, argv, "f:t:b:c:hl:"))!=-1){

			switch(option){
			case 'f':
				file_choice = optarg;
				break;
			case 't':
				str_up = optarg;
				break;
			case 'b':
				str_down = optarg;
				break;
			case 'c':
				color = optarg[0];
				break;
			case 'l':
				font_choice = optarg[0];
				break;
			case '?':
				printf("\nType -h for help\n\n");
				return 0;
			case 'h':
				printHelp();
				return 0;
			default:
				printf("\nType -h for help\n\n");
				return 0;
			}
		}
		if (argc==1){
			printf("\nType -h for help\n\n");
			return 0;
		}
	int type_choice=type_check(file_choice);

	if(type_choice==6){
		printf("%d",type_choice);
			p6_ppm *p6_struct;
			char *pixel_buffer=NULL;
			int new_line_flag=0;
			int original_height;
			//prints a string to an output file
			for (long unsigned i=0; i<strlen(str_up); i++){
				if(i==0){
					p6_struct = malloc(sizeof(p6_ppm));
					store_p6(file_choice, p6_struct, pixel_buffer);
					original_height = p6_struct->height;
					new_line_flag = printchar_p6(p6_struct, *(str_up+i), (i), color, 1, font_choice, new_line_flag, p6_struct->height);
					free(pixel_buffer);
					free(p6_struct);
				}else{
					p6_struct = malloc(sizeof(p6_ppm));
					store_p6("out.ppm", p6_struct, pixel_buffer);
					new_line_flag = printchar_p6(p6_struct, *(str_up+i), (i), color, 1, font_choice, new_line_flag, p6_struct->height);
					free(pixel_buffer);
					free(p6_struct);
				}
			}
			new_line_flag=0;

			for (long unsigned i=0; i<strlen(str_down); i++){
				p6_struct = malloc(sizeof(p6_ppm));
				store_p6("out.ppm", p6_struct, pixel_buffer);
				new_line_flag = printchar_p6(p6_struct, *(str_down+i), (i), color, 2, font_choice, new_line_flag, original_height);
				free(pixel_buffer);
				free(p6_struct);
				}
	}else if(type_choice==3){
		p3_ppm *p3_struct;
		int *pixel_buffer=NULL;
		int original_height;
		int new_line_flag = 0;
		//prints a string to an output file
		for (long unsigned i=0; i<strlen(str_up); i++){
			if(i==0){
				p3_struct = malloc(sizeof(p3_ppm));
				store_p3(file_choice, p3_struct, pixel_buffer);
				original_height = p3_struct->height;
				new_line_flag = printchar_p3(p3_struct, *(str_up+i), (i), color,1, font_choice,new_line_flag, p3_struct->height);
				free(pixel_buffer);
				free(p3_struct);
			}else{
				p3_struct = malloc(sizeof(p3_ppm));
				store_p3("out.ppm", p3_struct, pixel_buffer);
				new_line_flag = printchar_p3(p3_struct, *(str_up+i), (i), color,1, font_choice,new_line_flag, p3_struct->height);
				free(pixel_buffer);
				free(p3_struct);
			}
		}
		new_line_flag=0;
		for (long unsigned i=0; i<strlen(str_down); i++){
			p3_struct = malloc(sizeof(p3_ppm));
			store_p3("out.ppm", p3_struct, pixel_buffer);
			new_line_flag = printchar_p3(p3_struct, *(str_down+i), (i), color,2, font_choice,new_line_flag, original_height);
			free(pixel_buffer);
			free(p3_struct);
		}
	}else{
		printf("Wrong file Format");
	}

	return 0;
}
