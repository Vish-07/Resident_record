#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{FAILURE, SUCCESS}status_code;

typedef struct idnum_tag
{
    char aadhaar[15];
    char passport[15];
    char empcode[15];
}Idnum;

typedef struct accommodation_tag
{
    char firstname[50];
    char lastname[50];
    char accommodation_type[20];
    int idtype;
    Idnum idnum;
    char address[100];
}accomodation;

typedef struct node_tag
{
    accomodation data;
    struct node_tag* next;
    struct node_tag* prev;
}Node;

//Index struct that stores indices in alphabetical order as well as pointer to respective nodes from accomodation list
typedef struct index_tag
{
    int index;
    Node* ptr_to_list;
    struct index_tag* next;
}Index;

//Node pointer, to keep track of accomodated places.
//Status = 0 means not accomodated
//status = 1 means accomodated
typedef struct track_node_tag
{
    int status;
    struct track_node_tag* next;
}track_node;

//Struct that stores pointer to all 4 lists of accomodation type
struct accomodation_record_tag
{
    track_node* Type1;
    track_node* Type2;
    track_node* Type3;
    track_node* Type4;
}Record;

//Traversal pointer for those with id type 0
extern Node* nptr0;
//Traversal pointer for those with id type 1
extern Node* nptr1;
//Traversal pointer for those with id type 2
extern Node* nptr2;
//Head pointer of record list
extern Node* head;
//Head pointer of index list
extern Index* index_head;

//Function declarations

//Function that creates node for record list
Node* create_node(char*, char*, char*, int, char*, char*);
//Print records such that aadhar no is listed first, then passport, then empcode
void print_records();
//Inserting a new node in current list at proper position
Node* insert();

//Sorting list(Merge sort)
//Splitting list into two parts
void split(Index*, Index**, Index**);
//Merging two sorted parts
Index* merge(Index*, Index*);
//Merge sort to call, splitting, sorting and merging
void merge_sort(Index**);
//Sort the list
void sort_list();

//FUnction to remove duplicates and make changes in index as well as accomodation type list
void remove_duplicates();
//To print sorted records via index list
void print_sorted_records();
//Searching for a given node, based on id num and id type
void search();
//To delete a given record, and then make changes in index list and accomodation list accordingly
void delete_record();
//After deletion- updating index list
void update_index_deletion(Node*);
//After insertion updating index list
void update_index_insertion(Node*);
//Update required member in data, as asked by user
void update_record();

//Accomodation record functions
//Function to initialize accomodation record, with all 10 nodes of every list as status = 0
void initialize_accom_record();
//To insert accomodation if there's vacancy
status_code insert_accomodation(char*);
//To remove accomodation
void remove_accomodation(char*);
//Allocating given residence, if its vacant according to users request
void special_request_allocation();

#endif // RECORD_H_INCLUDED
