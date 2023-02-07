/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3	// A small graph
#endif

typedef struct user_struct {
    char name[MAX_STR_LEN];
    struct friend_node_struct* friends;
    struct brand_node_struct* brands;
    bool visited;
} User;

typedef struct friend_node_struct {
    User* user;
    struct friend_node_struct* next;
} FriendNode;

typedef struct brand_node_struct {
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct* next;
} BrandNode;

/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
  * This is usually bad coding practice but it will allow us to reuse the
  * helper functions.
  **/
FriendNode* allUsers; 

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node) {
  for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->user->name, node->name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name) {
  for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->brand_name, name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (in_friend_list(head, node)) {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->user->name, node->name) > 0) {
    fn->next = head;
    return fn;
  } 

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (in_brand_list(head, node)) {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->brand_name, node) > 0) {
    fn->next = head;
    return fn;
  } 

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (!in_friend_list(head, node)) {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0) {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (!in_brand_list(head, node)) {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0) {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user) {
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next) {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next) {
    printf("   %s\n", b->brand_name);
  }
}

void print_all_user(FriendNode *alluser){

  FriendNode *p = alluser;
  
  while (p != NULL) {
      printf("%s\n",p -> user -> name);
      p = p -> next;
  
  }
  return;

}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand_names[i], name) == 0) {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name) {
  int idx = get_brand_index(brand_name);
  if (idx < 0) {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++) {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0) {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char* file_name) {
    // Read the file
    char buff[MAX_STR_LEN];
    FILE* f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char* line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int) buff[y*2];
            if (value == 48) { value = 0; }
            else {value = 1;}
            brand_adjacency_matrix[x][y] = value;
        }
    }
}


// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/
User *create_user(char* name){
  User *new_user = (User *)calloc(1,sizeof(User));
  FriendNode *u = allUsers;
  while(u != NULL){
    if(strcmp(u -> user -> name, name) == 0){
      return NULL;
    }
    u = u -> next; 
  }
  strcpy(new_user -> name, name);
  new_user -> friends = NULL;
  new_user -> brands = NULL;
  new_user -> visited = false;
  allUsers = insert_into_friend_list(allUsers, new_user);
  return new_user;
}

/**TODO: Comhjuplete this function
 * Deletes a given user. 
 * Returns 0 on success, -1 on failure.
 **/

int delete_user(User* user){

  if(in_friend_list(allUsers, user) == false){
    return -1;
  }
  allUsers = delete_from_friend_list(allUsers, user);
  FriendNode* tmp = allUsers;
  while(tmp!= NULL){
    User* p = tmp -> user;
    while(p -> friends != NULL){
      if(in_friend_list(p -> friends, user)){
        delete_from_friend_list(p -> friends, user);
      }
      p -> friends = p -> friends -> next; 
    }
    tmp = tmp -> next;
  }
  free(user);
  user = NULL;
  return 0;

}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User* user, User* friend){
  if(in_friend_list(user -> friends, friend) == true){
    return -1;
  }
  if(strcmp(user -> name, friend -> name)==0 ){
    return -1;
  }
  user -> friends = insert_into_friend_list(user -> friends, friend);
  friend -> friends = insert_into_friend_list(friend -> friends, user);
  return 0;
}


/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User* user, User* friend){
  if(in_friend_list(user -> friends, friend) == false ){
    return -1;
  }else{
    user -> friends = delete_from_friend_list(user -> friends, friend);
    friend -> friends = delete_from_friend_list(friend -> friends, user);
    return 0;
  }
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User* user, char* brand_name){
  if(in_brand_list(user -> brands, brand_name) == true){
    return -1;
  }else{
    user -> brands = insert_into_brand_list(user -> brands, brand_name);
    return 0;
  }
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User* user, char* brand_name){
  if(in_brand_list(user -> brands, brand_name) == false){
    return -1;
  }else{
    user -> brands =delete_from_brand_list(user -> brands, brand_name);
    return 0;
  }
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User* a, User* b){
  int mutual = 0;
  FriendNode *f = a -> friends;
  if(strcmp(a -> name, b -> name)==0){
    return -1;
  }
  while(f != NULL){
    if(in_friend_list(b -> friends, f -> user)){
      mutual ++;
    }
    f = f -> next;
    }
  return mutual;
}

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 * 
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 * (X,Y) (Y,Z) (Z,X),return the min degree 1.
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/
int get_user_num(FriendNode *allUsers){
  FriendNode *p = allUsers;
  int num = 0;
  while(p != NULL){
    p = p -> next;
    num ++ ; 
  }
  return num;
}

void free_friend_list(FriendNode *f){
  FriendNode *cur = f;
  FriendNode *t = NULL;
  while (cur != NULL){
    t = cur -> next;
    free(cur);
    cur = t;
  }
}

int get_friend_degree(FriendNode *f, User* b, int degree, int max_degree) {
  int result;
  FriendNode *t1 = f;
  FriendNode *t2 = NULL;
  FriendNode *next_level = NULL;
  if (f == NULL){
    return -1;
  }

  if(in_friend_list(f,b) == true){
    return degree;
  }
  if(degree > max_degree){
    return -1;
  }
  //next_level = (FriendNode *)calloc(1,sizeof(FriendNode));

  while(t1 != NULL){
    t2 = t1 -> user -> friends;
    while (t2 != NULL){
      next_level = insert_into_friend_list(next_level, t2 -> user);
      t2 = t2 -> next;
    }
    t1 = t1 -> next;
  }

  result = get_friend_degree(next_level, b, degree+1, max_degree);
  if (next_level){
    free_friend_list(next_level);
  }
  return result;
}

int get_degrees_of_connection(User* a, User* b){
  if(strcmp(a->name, b->name) == 0){
    return 0;
  }
  int total = get_user_num(allUsers);
  return get_friend_degree(a -> friends, b ,1, total);
}


// Brands
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char* brandNameA, char* brandNameB){
  int index_a = get_brand_index(brandNameA);
  int index_b = get_brand_index(brandNameB);
  brand_adjacency_matrix[index_a][index_b] = 1;
  brand_adjacency_matrix[index_b][index_a] = 1;

}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char* brandNameA, char* brandNameB){
  int index_a = get_brand_index(brandNameA);
  int index_b = get_brand_index(brandNameB);
  brand_adjacency_matrix[index_a][index_b] = 0;
  brand_adjacency_matrix[index_b][index_a] = 0;

}
int get_mutual_brands(User* a, User* b){
  int mutual = 0;
  BrandNode *f = a -> brands;
  while(f != NULL){
    if(in_brand_list(b -> brands, f -> brand_name)){
      mutual ++;
    }
    f = f -> next;
    }
  return mutual;
}

 
// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * Return the not-already-friended user with the most brands they follow in common. 
 * If there’s a tie, return reverse-alphabetically (z to a).
 **/ 
User* get_suggested_friend(User* user){
  
  User *result = NULL;
  int max = 0;
  FriendNode *f = allUsers;
  while(f != NULL){
    if(strcmp(f -> user -> name, user -> name) != 0 ){
      if(in_friend_list(f -> user -> friends, user) == false){
        int mutual = get_mutual_brands(user, f -> user);
        if(mutual > max){
          max = mutual;
          result = f -> user;
        }else if(mutual == max){
          if(result != NULL){
            if(strcmp(f -> user -> name, result -> name) > 0){
              result = f -> user;
            }
          }
          if(result == NULL){
            result = f -> user;
          }
        }
    }
  }
  f = f -> next;
}

return result;
}

/**TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User* user, int n){
  User *suggested_friend = NULL;
  for(int i = 0; i < n; i++){
    suggested_friend = get_suggested_friend(user);
    if (suggested_friend != NULL){
      add_friend(user, suggested_friend);
    }else{
      return -1;
    }
  }
  return 0;
}

//*head 是该好友的friend list， node是循环到的brand名字。
int samilar_brands_num(BrandNode *head, char* node){
  int result = 0;
  BrandNode *f = head;
  while(f != NULL){
    int index1 = get_brand_index(node);
    int index2 = get_brand_index(f -> brand_name);
    if(brand_adjacency_matrix[index1][index2] == 1){
      result ++ ;
    }
    f = f -> next;
  }
return result; 
}

char* get_suggested_brands(User* user, char result[MAX_STR_LEN]){
  char cur[MAX_STR_LEN];
  strcpy(cur, "1");
  char largest[MAX_STR_LEN];
  int max_same_brand = 0;
  int count = 0;
  for(int i=0; i<MAT_SIZE; i++){
    if(in_brand_list(user -> brands, brand_names[i]) == false){
      if(strcmp(brand_names[i], cur) > 0){
        strcpy(cur, brand_names[i]);
      }
    }else{
      count++;
      }
    }
  if(count == MAT_SIZE){
    return NULL;
  }
  strcpy(result, cur);
  ////result先存最大的brand/不在列表里的
  for(int i = 0; i < MAT_SIZE; i++){
    char* name;
    name = brand_names[i];
    if(in_brand_list(user -> brands, brand_names[i]) == false){
      if(user -> brands == NULL){
        return result;
      }
      int samilar = samilar_brands_num(user -> brands, name);
      if(samilar > max_same_brand){
        max_same_brand = samilar;
        strcpy(result, name);
      }
      if(samilar == max_same_brand){
        if(strcmp(name, result) > 0){
          strcpy(result, name);
        }
      }
    }
  }
  return result;
}

/**TODO: Complete this function
 * Follows n suggested brands for the given user.
 * See the handout for how we define a suggested brand.     
 * Returns how many brands were successfully followed. 	  	
 **/

int follow_suggested_brands(User* user, int n){
  char* suggested_brand;
  suggested_brand = NULL;
  for(int i=0; i< n; i++){
    char result[MAX_STR_LEN];
    suggested_brand = get_suggested_brands(user, result);
    if (suggested_brand != NULL){
      follow_brand(user, suggested_brand);
    }else{
      return -1;
    }
  }
  return 0;
}

