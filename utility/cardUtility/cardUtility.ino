/* cardUtility */

#include <SD.h>
#include <SPI.h>

#define LINE_BUF_SIZE 128   //Maximum input string length
#define ARG_BUF_SIZE 64     //Maximum argument string length
#define MAX_NUM_ARGS 8      //Maximum number of arguments

bool error_flag = false;
 
char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

File root;
 
//Function declarations
int cmd_help();
int cmd_list();
int cmd_exit();
 
//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_list,
    &cmd_exit
};
 
//List of command names
const char *commands_str[] = {
    "help",
    "list",
    "exit"
};
 
int num_commands = sizeof(commands_str) / sizeof(char *);
 
void setup() {
    Serial.begin(115200);
    if (!SD.begin(10)) {
      Serial.println("initialization failed!");
      return;
    }
    cli_init();
}
 
void loop() {
    my_cli();
}

void cli_init(){
    Serial.println("Welcome to the cardUtility command line interface (CLI).");
    Serial.println("Type \"help\" to see a list of commands.");
}
 
void my_cli(){
    Serial.print("> ");
 
    read_line();
    if(!error_flag){
        parse_line();
    }
    if(!error_flag){
        execute();
    }
 
    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
 
    error_flag = false;
}

int cmd_help(){
    if(args[1] == NULL){
        help_help();
    }
    else if(strcmp(args[1], commands_str[0]) == 0){
        help_help();
    }
    else if(strcmp(args[1], commands_str[1]) == 0){
        help_list();
    }
    else if(strcmp(args[1], commands_str[2]) == 0){
        help_exit();
    }
    else{
        help_help();
    }
}

int cmd_list() {
  root = SD.open("/");
  printDirectory(root, 0);
}
 
void help_help(){
    Serial.println("The following commands are available:");
 
    for(int i=0; i<num_commands; i++){
        Serial.print("  ");
        Serial.println(commands_str[i]);
    }
    Serial.println("");
    Serial.println("You can for instance type \"help list\" for more info on the LIST command.");
}

void help_list() {
  Serial.println("This will list all files present on the SD card.");
}
 
void help_exit(){
    Serial.println("This will exit the CLI. To restart the CLI, restart the program.");
}

void read_line(){
    String line_string;
 
    while(!Serial.available());
 
    if(Serial.available()){
        line_string = Serial.readStringUntil('\n');
        if(line_string.length() < LINE_BUF_SIZE){
          line_string.toCharArray(line, LINE_BUF_SIZE);
          Serial.println(line_string);
        }
        else{
          Serial.println("Input string too long.");
          error_flag = true;
        }
    }
}
 
void parse_line(){
    char *argument;
    int counter = 0;
 
    argument = strtok(line, " ");
 
    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                argument = strtok(NULL, " ");
                counter++;
            }
            else{
                Serial.println("Input string too long.");
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}
 
int execute(){  
    for(int i=0; i<num_commands; i++){
        if(strcmp(args[0], commands_str[i]) == 0){
            return(*commands_func[i])();
        }
    }
 
    Serial.println("Invalid command. Type \"help\" for more.");
    return 0;
}
 
int cmd_exit(){
    Serial.println("Exiting CLI.");
 
    while(1);
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
