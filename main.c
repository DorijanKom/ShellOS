#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

                            // Color values using the ANSI color method from the  <stdlib.h>
#define BRED "\e[1;31m"     // Red with bold characters
#define BYEL "\e[1;33m"     // Yellow with bold characters
#define BCYN "\e[1;36m"     // Cyan with bold characters
#define reset "\e[0m"       // Value for reseting the characters to the default format

void prompt(){
    
    char hostname[HOST_NAME_MAX]; // We pass the hostname to this variable
    
    /* We call the getenv function which is located in the <stdlib.h> library
    and we pass the argument USER to get the username, after which we save it
    into the username pointer (Works only on Linux!). We can achieve the same with getlogin() */
    char *username = getenv("USER");
    gethostname(hostname,HOST_NAME_MAX); // We get the hostname through the gethostname function
    
    printf(BRED"[");            // We implement the colors from this line and reset them to the default on line 30
    printf(BYEL"%s",hostname);
    printf(BRED"]");
    printf(BCYN"@");
    printf(BRED"[");
    printf(BYEL"%s",username);
    printf(BRED"]");
    printf(BCYN":~$ ");
    printf(reset);
}

void cat(int argc, char *argv[]){ // We pass the filename as paramater

    FILE *fpReadFile; 

    unsigned int counter;
    char c; // This is used for reading for reading characters in a file

    fpReadFile = fopen(argv[1],"r"); // Opens the file

    if(fpReadFile==NULL){
        
        printf("\nError: Unable to open file.");
        exit(0);
    }
    while ((c = getc(fpReadFile)) != EOF) // Reads file character by character
    {
        printf("%c",c); // Prints it out
    }
    fclose(fpReadFile);     // Closes the file
    printf("\n"); 
}

void rm(int argc, char *filename[]){

    int ret = -1;

    ret = remove(filename[1]);

    if(ret == 0){
        printf("File deleted succesfully.\n");
    }
    else{
        printf("Error: Unable to delete file.\n");
    }
}

void cowsay(int argc,char *input[]){
    

    unsigned int counter;
    unsigned int argscharcount=0;;
    unsigned short skiparg;
    unsigned short thought=0;
    char upperbubble = '\\';
    char lowerbubble = '\\';
   

    /*In case a user only types in cowsay this is printed out*/
    if( argc == 1 ) {
        printf("This program displays all of it arguments in a speech bubble.\n"
        "After typing in the command, type in what you want Tux to say.");
        exit(EXIT_FAILURE);
    }
    for(counter=1;counter<argc; counter++){
        if(!strcmp(input[counter],"-t") || (!strcmp(input[counter],"--thought"))){
            thought=1;
            strcpy(&upperbubble,"O");
            strcpy(&lowerbubble,"o");
        }
        else if(!strcmp(input[counter],"-v") || !strcmp(input[counter],"--version")){
            printf("Version 0.7\n");
        }
        else if(!strcmp(input[counter],"-h") || !strcmp(input[counter],"--help")){
            printf("Usage:\n"
            "The flags should be the first set of arguments.\n"
            "cowsay <flag(s)> Sentence to say\n"
            "\n"
            "Flags:\n"
            "-h or --help    - Displays this help text\n"
            "-v or --version - Displays the package version\n"
            "-t or --thought - Thought bubble\n");
            exit(EXIT_SUCCESS);
        }
    }
    
    for(counter=1;counter<argc;counter++){
        skiparg=0;
        if(!strcmp(input[counter],"-t") || !strcmp(input[counter],"--thought")){
            skiparg=1;
        }
        else if(counter<argc && skiparg==0){
        argscharcount=(argscharcount+1+strlen(input[counter])); //Counts the characters in the given argument
        }
    }
    if(argscharcount==0){
        printf("This program displays all of it arguments in a speech bubble.\n"
        "After typing in the command, type in what you want Tux to say."); //If no arguments are given this message is printed out.
        exit(EXIT_FAILURE);
    }

    argscharcount=argscharcount+1;

    //Displays the speech bubble
    printf(" ");
    for(counter=1;counter<=argscharcount;counter++){
        printf("_");
        if(counter==50) break;
    }

    if ( thought == 0 ) {
        printf("\n< ");
    }
    else if ( thought == 1 ) {
        printf("\n( ");
    }

    int letterCounter=0;
    for(counter=1;counter<argc;counter++){
        // For loop which is going through the letters of each word, one by one.
        for(int j=0;j<strlen(input[counter]);j++){

            if(skiparg==0){
                printf("%c",input[counter][j]);

                // Counting how many letters have been displayed, to format them properly.
                letterCounter++;
                if (letterCounter % 39 == 0)
                {
                    printf("\n<");
                }
            }
            else{
                printf("%c",input[counter][j+1]);
                letterCounter++;
                if (letterCounter % 39 == 0){
                    printf("\n(");
                }
            }
        }
        printf(" ");
    }
    if ( thought == 0 ) {
        printf(">\n");
    }
    else if ( thought == 1 ) {
        printf(")\n");
    }

    printf(" ");
    for(counter=1;counter<=argscharcount;counter++){
        printf("-");
        if(counter==50) break;
    }
    printf("\n");


    printf("   %c \n"          // Character that displays when you type in cowsay
    "      %c \n"
    "        .--. \n"
    "       |o_o | \n"
    "       |:_/ | \n"
    "      //   \\ \\ \n"
    "     (|     | ) \n"
    "    /'\\_   _/`\\ \n"
    "    \\___)=(___/ \n",upperbubble,lowerbubble);
}

void clear(){
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J"; // ANSI code for clearing screen (Works only on Linux because of the file descriptor we are using)
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);    // Write method takes in the CLEAR_SCREEN_ANSI variable and
}                                                   // outputs it, which clears the screen

void cfork(){

    int rc = fork();

    if (rc < 0){

        printf("Fork failed \n");

    }else if (rc == 0){
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        execv("./child", NULL);
    } else { 
    int rc_wait = wait(NULL);
    printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
     }

}

int main(void){

    // We use the input variable for user input
    char input[MAX_INPUT];
    system("clear"); //When we first initiate our shell the screen is cleared(This is Linux specific, on Windows it is system("cls"))
    while (1)
    {
        prompt();
        /* fgets is more optimal than scanf for user input because it reads a line of text and is
        better at handling overflow of arrays */
        fgets(input,MAX_INPUT,stdin);
    }
        
    return 0;
}