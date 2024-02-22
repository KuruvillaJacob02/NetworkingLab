#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    DIR *dir;		// Directory pointer
    struct dirent *entry;
    dir = opendir(".");	// Open the current directory
    
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    printf("Files in the current directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);

    return 0;
}
