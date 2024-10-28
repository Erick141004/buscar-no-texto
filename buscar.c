#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


char **get_string_between_match(char* buffer, int index, int text_length, int context_length, int size_file) {
    char **context = malloc(sizeof(char *) * 2);
    context[0] = malloc(sizeof(char) * (context_length + 4));
    context[1] = malloc(sizeof(char) * (context_length + 4));
    
    int copy_length = context_length;
    int start = index - context_length;
    
    if (start < 0) {
        start = 0;
    }

    int end = text_length + index;

    if(start != 0){
        for (int i = 0; i < context_length; i++){
            if(buffer[start + i] == '\n'){
                copy_length = 0;
                break;
            }
        }

        if(copy_length != 0){
            strcpy(context[0], "...");    
            strncpy(&context[0][3], &buffer[start], copy_length);
            context[0][context_length + 3] = '\0';
        } else {
            context[0][0] = '\0';
        }
    }

    if(size_file > end){
        copy_length = context_length;

        for (int i = 0; i < context_length; i++) {
            if(buffer[end + i] == '\n'){
                copy_length = i;
                break;
            }
        }

        strncpy(context[1], &buffer[end], copy_length);
        context[1][copy_length] = '\0';
        strcat(context[1], "...");
    }

    return context;
}

int find_occureance(char *text_search, char *buffer, int size_file, int length_text, int *count_line){
    int total_appearence = 0;

    for (int i = 0; i < size_file; i++) {
        char aux_text[length_text + 1];
        strncpy(aux_text, &buffer[i], length_text);
        aux_text[length_text] = '\0';

        if (strcmp(aux_text, text_search) == 0) {
            char **context = get_string_between_match(buffer, i, length_text, 10, size_file);
            printf("Line %d: %s\e[1m%s\e[m%s\n", *count_line, context[0], aux_text, context[1]);
            total_appearence++;
            
            free(context[0]);
            free(context[1]);
            free(context);
        } else if (strchr(aux_text, '\n')) {
            (*count_line)++;
            i += length_text - 1;
        }
    }

    return total_appearence;
}

int main(int argc, char **argv){
    char buffer[1024];

    if (argv[1] == NULL) {
        printf("Error passing argument. Pass file path to run the program\n");
        return 0;
    }

    if(argv[2] == NULL){
        printf("Error passing argument. Pass a string to be search to run the program\n");
    }

    char* file_path = argv[1];
    char* text_search = argv[2];

    printf("Text to be search: %s\n", text_search);

    int file_descriptor = open(file_path, O_RDONLY);
    printf("File Descriptor: %d\n", file_descriptor);
    printf("File Path: %s\n", file_path);

    if (file_descriptor == -1) {
        printf("Error opening file: %s\n", file_path);
        printf("Error value: %d", errno);
        return 0;
    }
    
    int total_appearence = 0;
    int size_file = 0;
    int total_size_file = 0;
    int length_text = strlen(text_search);
    int count_line = 1;

    printf("Text Length: %d\n", length_text);

    while((size_file = read(file_descriptor, buffer, sizeof(buffer))) > 0){
        total_appearence += find_occureance(text_search, buffer, size_file, length_text, &count_line);
        total_size_file += size_file;
    }
    
    printf("Size file (bytes): %d\n", total_size_file);

    printf("\nThe string \e[1m\"%s\"\e[m has %d appearence(s) in the file %s\n", text_search, total_appearence, file_path);

    int close_information = close(file_descriptor);

    if (close_information == -1) {
        printf("Error closing file: %s\n", file_path);
        printf("Error value: %d", errno);
        return 0;
    }   
}