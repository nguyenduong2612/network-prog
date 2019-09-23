#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//func prototype
void readfile();
void writefile();

typedef struct node {   // implement linked list
  char username[30], password[30];
  int status;
  struct node *next;
} node;

node *head = NULL;

int session = 0;      //not signin state
char current_username[30] = "";

// Utility
void print_menu() {
  printf("\nUSER MANAGEMENT PROGRAM\n");
  printf("-------------------------------------------\n");
  printf("\t1. Register\n");
  printf("\t2. Sign in\n");
  printf("\t3. Search\n");
  printf("\t4. Sign out\n");
  printf("Your choice (1-4, other to quit): ");
}

void print_info() {
  if (session == 1) printf("\nLogged in\n");
  else printf("\nNot logged in\n");
  node *temp;

  if (head == NULL) return;
  temp = head;

  do {
    printf("%s %s %d\n", temp->username, temp->password, temp->status);
    if (temp->next == NULL) return;
    temp = temp->next;
  } while (1);
}

void add_new_account(char username[], char password[], int status) {
  node *account, *temp;     //create new node
  account = (node*)malloc(sizeof(node));

  if (head == NULL) {       //if linked list doesn't exist then create new one
    head = account;
    strcpy(head->username, username);
    strcpy(head->password, password);
    head->status = status;
    head->next = NULL;
  } else {
    temp = head;
    while (temp->next != NULL) temp = temp->next; //travel to the end of linked list

    temp->next = account;     //add new node to the end of linked list
    strcpy(account->username, username);
    strcpy(account->password, password);
    account->status = status;
    account->next = NULL;
  }
}

int check_account_existed(char username[]) {
  node *temp;

  if (head == NULL) return 0;     //if linked list doesn't exist then exit
  temp = head;

  do {
    if (strcmp(username, temp->username) == 0) return 1;  // if account existed then return 1
    if (temp->next == NULL) return 0;      // else return 0
    temp = temp->next;
  } while (1);
}

int check_login(char username[], char password[]) {
  node *temp;

  if (head == NULL) return 0;
  temp = head;

  do {
    if (strcmp(username, temp->username) == 0) {           // if username and password is correct
      if (strcmp(password, temp->password) == 0) return 1; // then return 1
      else return 0;      //else return 0
    }
    temp = temp->next;
  } while (1);
}

int check_account_blocked(char username[]) {
  node *temp;

  if (head == NULL) return 0;
  temp = head;

  do {
    if (strcmp(username, temp->username) == 0) {      // check username
      if (temp->status == 0) return 1;        // return 1 if account is blocked
      else return 0;    // else return 0
    }
    temp = temp->next;
  } while (1);
}

void block_account(char username[]) {
  node *temp;

  if (head == NULL) return;
  temp = head;

  do {
    if (strcmp(username, temp->username) == 0) {    // check username
      temp->status = 0;       // change account status to 0
      writefile();      //rewrite file
      return;
    }
    temp = temp->next;
  } while (1);
}
// End of Utility

// Main function
void signup() {
  char username[30], password[30];
  int status = 1;
  printf("New username: ");     // input username
  scanf("%s", username);
  if (check_account_existed(username) == 1) {   // check account existed
    printf("\nACCOUNT EXISTED\n");
    return;
  }
  printf("New password: ");   // input password
  scanf("%s", password);

  add_new_account(username, password, status);    // add new account to linked list
  printf("\nSUCCESSFUL REGISTRATION\n");

  writefile();    // rewrite file
  return;
}

void login() {
  char username[30], password[30];
  printf("Username: ");     // input username
  scanf("%s", username);
  if (check_account_existed(username) == 0) {     // check account existed
    printf("\nCANNOT FIND ACCOUNT\n");
    return;
  }
  if (check_account_blocked(username) == 1) {     // check account blocked
    printf("\nACCOUNT IS BLOCKED\n");
    return;
  }

  int count = 0;      // count incorrect input
  while (1) {
    printf("Password: ");
    scanf("%s", password);

    if (check_login(username, password) == 1) {     // check username and password
      printf("\nHELLO %s\n", username);     // signed in
      strcpy(current_username, username);   // set current username
      session = 1;      // start session
      break;
    }
    else {
      if (count < 3) {
        printf("\nPASSWORD IS INCORRECT\n");
        count++;
      }
      else {
        printf("\nPASSWORD IS IN CORRECT. ACCOUNT IS BLOCKED\n");
        block_account(username);      // block account
        break;
      }
    }
  }
  return;
}

void search() {
  if (session == 0) {     // if not sign in
    printf("\nACCOUNT IS NOT SIGN IN\n");
    return;
  }
  char username[30];
  printf("Search username: ");    // input username
  scanf("%s", username);
  if (check_account_existed(username) == 0) {   //check account existed
    printf("\nCANNOT FIND ACCOUNT\n");
    return;
  }

  // search username
  node *temp;

  if (head == NULL) return;
  temp = head;

  do {
    if (strcmp(username, temp->username) == 0) {    // if username existed then check status
      if (temp->status == 0) printf("\nACCOUNT IS BLOCKED\n");
      else printf("\nACCOUNT IS ACTIVE\n");
      return;
    }
    temp = temp->next;
  } while (1);
}

void signout() {
  if (session == 0) printf("\nACCOUNT IS NOT SIGN IN\n");
  else {
    printf("\nGOODBYE %s\n", current_username);
    session = 0;      //destroy session
    strcpy(current_username, "");   // reset current username
  }
}
// End of Main function


// File handle
void readfile() {
  FILE *file;
  file = fopen("account.txt", "r");   // open read mode
  if (file) {
    if (head != NULL) free(head);     // reset linked list if existed
    while (1) {
      char username[30], password[30];
      int status;

      if (fscanf(file,"%s %s %d", username, password, &status) == EOF) break;   // read line by line to the end

      add_new_account(username, password, status);    // add new account to linked list
    }

    fclose(file);       // close file
  } else {
    printf("\nCAN'T OPEN FILE\n");
  }
}

void writefile() {
  FILE *file;
  file = fopen("account.txt", "w");    // open write mode

  node *temp;
  temp = head;

  do {
    fprintf(file, "%s %s %d\n", temp->username, temp->password, temp->status);    // write line by line
    if (temp->next == NULL) break;
    temp = temp->next;
  } while (1);

  fclose(file);     // close file
}
// End of File handle

// Main
int main() {
  while(1) {
    int c;
    readfile();
    //print_info();
    print_menu();
    scanf("%d", &c);
    switch(c) {
      case 1: signup(); break;
      case 2: login(); break;
      case 3: search(); break;
      case 4: signout(); break;
      default: return 0;
    }
  }
  return 0;
}
// End of Main
