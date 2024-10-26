#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv){
    char buffer[1024];
    char* text_search = argv[2];

    printf("Text to be search: %s\n", text_search);

    if (argv[1] == NULL) {
        printf("Error passing path argument. Pass file path to run the program\n");
        return 0;
    }

    int file_descriptor = open(argv[1], O_RDONLY);
    printf("File Descriptor: %d\n", file_descriptor);
    printf("File Path: %s\n", argv[1]);

    if (file_descriptor == -1) {
        printf("Error opening file: %s\n", argv[1]);
        printf("Error value: %d", errno);
        return 0;
    }
    
    int text_file = read(file_descriptor, buffer, sizeof(buffer));

    int close_information = close(file_descriptor);

    if (close_information == -1) {
        printf("Error closing file: %s\n", argv[1]);
        printf("Error value: %d", errno);
        return 0;
    }

    int length_text = strlen(argv[2]);
    printf("Text Length: %d\n", length_text);
    int count_line = 0;

    for (int i = 0; i < text_file; i++) {
        
    }

}