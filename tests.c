#include "parson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void get_json();
int parsing();

int main(){
	int n;
	int counter;
	for(n=1; n<=135000; n++){
		printf("%i    ",n);
		counter += parsing(n);
		printf("       %i\n", counter);
	}
	return 0;
}

int parsing(int id){
	JSON_Value *root_value;
    JSON_Array *commits;
    JSON_Object *commit;
    size_t i;
    FILE *IF;
    FILE *OF;
    int c;


    char curl_command[512];
    char cleanup_command1[256];
    char cleanup_command2[256];
    char output_filename[]="id.json";
    char working_file[]="idwork.json";
    char number[16];
    int output;


    sprintf(curl_command, 
        "curl -s \"https://alpha-api.app.net/stream/0/users/%i\" > %s",
        id, output_filename);
    sprintf(cleanup_command1, "rm -f %s", output_filename);
    sprintf(cleanup_command2, "rm -f %s", working_file);

    system(curl_command);

    
    IF = fopen(output_filename, "r");
    OF = fopen(working_file, "w+");
    fprintf(OF, "[{\"counts");
    while((c = fgetc(IF)) != EOF){
        if(c != 91 && c != 93){
        	if(c == 99){
        		c = fgetc(IF);
        		if(c == 111){
        			c = fgetc(IF);
        			if(c == 117){
        				c = fgetc(IF);
        				if(c == 110){
        					c = fgetc(IF);
        					if(c == 116){
        						c = fgetc(IF);
        						if(c == 115){
        							while((c = fgetc(IF)) != 125){
        								fputc(c, OF);
        							}
        							fputc(125, OF);
        							while((c = fgetc(IF)) != 125){
        								fputc(c, OF);
        							}
        						}
        					}
        				}
        			}
        		}
        	}
        }
    }
    fputc(125,OF);
    fputc(93,OF);
    fclose(IF);
    fclose(OF);

 	root_value = json_parse_file(working_file);
    if (json_value_get_type(root_value) != JSONArray) {
        system(cleanup_command1);
        system(cleanup_command2);
        printf("fail");
        return 0;
    }
    commits = json_value_get_array(root_value);
    commit = json_array_get_object(commits, 0);

    sprintf(number, "%f",
    	json_object_dotget_number(commit, "counts.following"));
    /*printf("%s", number);*/

    json_value_free(root_value);
    system(cleanup_command1);
    system(cleanup_command2);

    output = atoi(number);
    printf("%i", output);

    if(output > 40){
    	return 1;
    }
    else{

    	return 0;
    }
}