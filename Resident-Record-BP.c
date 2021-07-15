#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 40
#define BLOCK_SIZE 10
#define INDEX_SIZE 40
#define NUM_ALPHA 25

/*
    NAME: VIKHARANKAR VISHAKHA CHANDRAKANT
    ENROL. NO.: BT19CSE118
    SUB: DSPD-2 ASSIGNMENT-2
*/

typedef enum{false, true}boolean;

struct idnum_tag
{
    char aadhaar [15];
    char passport [15];
    char empcode [15];
}idnum;

typedef struct accommodation_tag
{
    char firstname [50];
    char lastname [50];
    char accommodation_type [20];
    int idtype;
    struct idnum_tag num;
    char address [100];
}accomodation;

//As there will be only 10 blocks of each type of accomodation, hence max possible records are 40(10 * 4 types of accomodation)

//B+ tree node(40-ary tree)

typedef struct record_node_tag
{
    int size;//Size of number of records filled in array
    //Leaf Nodes of order SIZE
    accomodation records[SIZE];
    struct record_node_tag* next;//In B+ tree all leaf nodes are connected to each other like a linked list
}record_node;

//B+ tree with order 40(40-ary tree)
typedef struct BP_Node_tag
{
    int id[SIZE];//To store type of id
    boolean is_leaf;
    union
    {
        //Either node will point to another BP_Node or to leaf node, which has records
        struct BP_Node_tag* child[SIZE + 1];
        record_node* data[SIZE + 1];
    };
}BP_Node;

//B+ tree with order 10(10-ary tree)
typedef struct allocated_block_tag
{
    //Size of allocated blocks
    int size;
    //Each element will point to record that has given accomodation type
    accomodation* records[BLOCK_SIZE];
    //Pointer to next leaf node
    struct allocated_block_tag* next;
}allocated_block;

//Node of B+ 10-ary tree
typedef struct accomodation_block_node_tag
{
    int block[BLOCK_SIZE];//To store type of accomodation
    boolean is_leaf;
    union
    {
        //Either Node will point to another accomodation block node or to leaf node, which has records
        struct accomodation_block_node_tag* child[BLOCK_SIZE + 1];
        allocated_block* data[BLOCK_SIZE + 1];
    };

}accomodation_block_node;

//Index B+ tree- 40 ary tree
typedef struct Index_data_tag
{
    int size;
    accomodation* records[INDEX_SIZE + 1];
    struct Index_data_tag* next;
}Index_data;

typedef struct Index_node_tag
{
    char key[INDEX_SIZE][50];//To store key values
    boolean is_leaf;
    union
    {
        struct Index_node_tag* child[INDEX_SIZE + 1];
        Index_data* data[INDEX_SIZE + 1];
    };
}Index_node;

BP_Node* root = NULL;
accomodation_block_node* Block_root = NULL;
Index_node* Index_root = NULL;

accomodation create_record();
void initalize_BP_tree();
void initialize_Index_tree();
void initialize_accomodation_block_tree();
void insert_record();
void insert_index_tree(accomodation*);
void delete_index_tree();
void remove_duplicates();
void print_record(accomodation);
void print_sorted_records();
void search();
void delete_record();
void update_record();
void address_search();
boolean allocate_accomodation_block(accomodation*, int);
boolean deallocate_accomodation_block(accomodation*, int);
void special_request_allocation();
void address_search();

void initialize_Index_tree()
{
    Index_root = (Index_node*)malloc(sizeof(Index_node));
    Index_root->is_leaf = false;
    char str[1] = "B";
    //Initializing all the key values of root with alphabets 'B' 'C' .... 'Z'
    for(int i = 0; i < NUM_ALPHA; ++i)
    {
        strcpy(Index_root->key[i], str);
        str[0] += 1;
        //printf("%s", Index_root->key[i]);
    }
    //Leaf node corresponding to every key and assigning this leaf node's data array size of 0
    for(int i = 0; i <= INDEX_SIZE; ++i)
    {
        Index_root->data[i] = (Index_data*)malloc(sizeof(Index_data));
        Index_root->data[i]->size = 0;
    }
    //Linking all leaf nodes
    for(int i = 0; i < INDEX_SIZE; ++i)
    {
        Index_root->data[i]->next = Index_root->data[i + 1];
    }
    Index_root->data[INDEX_SIZE]->next = NULL;
}

void insert_index_tree(accomodation* record_ptr)
{
    int i = 0, j = 0, k;
    //Traversing through keys of root and getting to correct position
    while(i < NUM_ALPHA && strcmp(record_ptr->firstname,Index_root->key[i]) > 0)
    {
        i++;
    }
    //Then traversing through leaf node till we come across position which is suitable to insert
    while(j < Index_root->data[i]->size && strcmp(record_ptr->firstname, Index_root->data[i]->records[j]->firstname) > 0)
    {
        j++;
    }
    //Now, if first name matches
    if(j < Index_root->data[i]->size && strcmp(record_ptr->firstname, Index_root->data[i]->records[j]->firstname) == 0)
    {
        //Comparing lastnames
        while(j < Index_root->data[i]->size && strcmp(record_ptr->lastname, Index_root->data[i]->records[j]->lastname) > 0)
        {
            j++;
        }
    }
    //Now shifting all values below index j and creating free space for our current record
    for(k = Index_root->data[i]->size - 1;k >= j; k--)
    {
        Index_root->data[i]->records[k + 1] = Index_root->data[i]->records[k];
    }
    //Assigning value at correct place
    Index_root->data[i]->records[j] = record_ptr;
    //Incrementing size
    Index_root->data[i]->size++;
    /*for(j = 0; j < Index_root->data[i]->size; ++j)
    {
        printf("Firstname: %s\n", Index_root->data[i]->records[j]->firstname);
        printf("Lastname: %s\n", Index_root->data[i]->records[j]->lastname);
        printf("Accomodation-type: %s\n", Index_root->data[i]->records[j]->accommodation_type);
        printf("Id-type: %d\n", Index_root->data[i]->records[j]->idtype);
        if(Index_root->data[i]->records[j]->idtype == 0)
            printf("Aadhaar No: %s\n", Index_root->data[i]->records[j]->num.aadhaar);
        else if(Index_root->data[i]->records[j]->idtype == 1)
            printf("Passport No: %s\n", Index_root->data[i]->records[j]->num.passport);
        else if(Index_root->data[i]->records[j]->idtype == 2)
            printf("Employee No: %s\n", Index_root->data[i]->records[j]->num.empcode);
        printf("Address: %s\n", Index_root->data[i]->records[j]->address);
    }*/
}

void delete_index_tree(accomodation* record_ptr)
{
    int i = 0, j = 0, k;
    while(i < NUM_ALPHA && strcmp(record_ptr->firstname,Index_root->key[i]) > 0)
    {
        i++;
        //printf("%d Done1\n", i);
    }
    while(j < Index_root->data[i]->size && strcmp(record_ptr->firstname, Index_root->data[i]->records[j]->firstname) > 0)
    {
        j++;
        //printf("%d Done2\n", j);
    }
    if(j < Index_root->data[i]->size && strcmp(record_ptr->firstname, Index_root->data[i]->records[j]->firstname) == 0)
    {
        while(j < Index_root->data[i]->size && strcmp(record_ptr->lastname, Index_root->data[i]->records[j]->lastname) > 0)
        {
            j++;
        }
    }
    if(strcmp(record_ptr->firstname, Index_root->data[i]->records[j]->firstname) == 0 && strcmp(record_ptr->lastname, Index_root->data[i]->records[j]->lastname) == 0)
    {
        for(k = j; k < Index_root->data[i]->size; ++k)
        {
            Index_root->data[i]->records[j] = Index_root->data[i]->records[j + 1];
        }
        Index_root->data[i]->size--;
        printf("Record deleted\n");
    }
    else
    {
        printf("Record not found\n");
    }
}

//To search and print records corresponding to entered first-name and lastname
void search()
{
    int i = 0, j = 0;
    char firstname[15];
    char lastname[15];
    printf("Enter firstname\n");
    scanf("%s", firstname);
    printf("Enter lastname\n");
    scanf("%s", lastname);
    //Traversing through keys of root, unless we encounter one which is greater than our string
    while(i < NUM_ALPHA && strcmp(firstname,Index_root->key[i]) > 0)
    {
        i++;
    }
    //Traversing through leaf node, unless we get correct place
    while(j < Index_root->data[i]->size && strcmp(firstname, Index_root->data[i]->records[j]->firstname) > 0)
    {
        j++;
    }
    //Now, once we get correct firstname, traverse till our lastname matches
    if(j < Index_root->data[i]->size && strcmp(firstname, Index_root->data[i]->records[j]->firstname) == 0)
    {
        while(j < Index_root->data[i]->size && strcmp(lastname, Index_root->data[i]->records[j]->lastname) > 0)
        {
            j++;
        }
    }
    //Then print all records with matching first-name and lastname
    while(strcmp(firstname, Index_root->data[i]->records[j]->firstname) == 0 && strcmp(lastname, Index_root->data[i]->records[j]->lastname) == 0)
    {
        printf("Firstname: %s\n", Index_root->data[i]->records[j]->firstname);
        printf("Lastname: %s\n", Index_root->data[i]->records[j]->lastname);
        printf("Accomodation-type: %s\n", Index_root->data[i]->records[j]->accommodation_type);
        printf("Id-type: %d\n", Index_root->data[i]->records[j]->idtype);
        if(Index_root->data[i]->records[j]->idtype == 0)
            printf("Aadhaar No: %s\n", Index_root->data[i]->records[j]->num.aadhaar);
        else if(Index_root->data[i]->records[j]->idtype == 1)
            printf("Passport No: %s\n", Index_root->data[i]->records[j]->num.passport);
        else if(Index_root->data[i]->records[j]->idtype == 2)
            printf("Employee No: %s\n", Index_root->data[i]->records[j]->num.empcode);
        printf("Address: %s\n", Index_root->data[i]->records[j]->address);
        j++;
    }
}

//Function to print records in sorted manner
void print_sorted_records()
{
    int i = 0, j;
    //Traversing through all keys of root one-by-one
    for(i = 0; i < NUM_ALPHA; ++i)
    {
        //Traversing through data array for each root and then printing records
        for(j = 0; j < Index_root->data[i]->size; ++j)
        {
            printf("Firstname: %s\n", Index_root->data[i]->records[j]->firstname);
            printf("Lastname: %s\n", Index_root->data[i]->records[j]->lastname);
            printf("Accomodation-type: %s\n", Index_root->data[i]->records[j]->accommodation_type);
            printf("Id-type: %d\n", Index_root->data[i]->records[j]->idtype);
            if(Index_root->data[i]->records[j]->idtype == 0)
                printf("Aadhaar No: %s\n", Index_root->data[i]->records[j]->num.aadhaar);
            else if(Index_root->data[i]->records[j]->idtype == 1)
                printf("Passport No: %s\n", Index_root->data[i]->records[j]->num.passport);
            else if(Index_root->data[i]->records[j]->idtype == 2)
                printf("Employee No: %s\n", Index_root->data[i]->records[j]->num.empcode);
            printf("Address: %s\n", Index_root->data[i]->records[j]->address);
        }
    }
    /*Index_data* dptr = Index_root->data[i];
    while(dptr)
    {
        for(j = 0; j < dptr->size; ++j)
        {
            print_record(*(dptr->records[j]));
        }
        dptr = dptr->next;
    }*/
}

//Function to create record
accomodation create_record()
{
    accomodation record;
    printf("Enter firstname\n");
    scanf("%s", record.firstname);
    printf("Enter lastname\n");
    scanf("%s", record.lastname);
    printf("Enter accomodation type\n");
    scanf("%s", record.accommodation_type);
    printf("Enter id type\n");
    scanf("%d", &record.idtype);
    if(record.idtype == 0)
    {
        printf("Enter aadhaar number\n");
        scanf("%s", record.num.aadhaar);
        record.num.empcode[0] = record.num.passport[0] = '\0';
    }
    else if(record.idtype == 1)
    {
        printf("Enter passport number\n");
        scanf("%s", record.num.passport);
        record.num.aadhaar[0] = record.num.empcode[0] = '\0';
    }
    else if(record.idtype == 2)
    {
        printf("Enter employee code\n");
        scanf("%s", record.num.empcode);
        record.num.aadhaar[0] = record.num.passport[0] = '\0';
    }
    printf("Enter address\n");
    fflush(stdin);
    fgets(record.address, 100, stdin);
    //printf("Record created\n");
    return record;
}

void initalize_BP_tree()
{
    root = (BP_Node*)malloc(sizeof(BP_Node));
    //Root node with 2 keys with value 1 and 2 respectively
    root->id[0] = 1;
    root->id[1] = 2;
    root->is_leaf = false;
    //As leaf nodes of B+ trees are linked, hence linking all three children
    root->data[0] = (record_node*)malloc(sizeof(record_node));
    root->data[0]->next = root->data[1];
    root->data[1] = (record_node*)malloc(sizeof(record_node));
    root->data[1]->next = root->data[2];
    root->data[2] = (record_node*)malloc(sizeof(record_node));
    root->data[2]->next = root->data[2];
    root->data[0]->size = 0;
    root->data[1]->size = 0;
    root->data[2]->size = 0;
}

void insert_record()
{
    //Calling create_record() function to create record
    accomodation record = create_record();
    int i = 0, j, type = 0;
    //Once we get record, checking whether accomodation block entered by user is correct or not.
    if(strcmp(record.accommodation_type, "Type-A") == 0)
        type = 1;
    else if(strcmp(record.accommodation_type, "Type-B") == 0)
        type = 2;
    else if(strcmp(record.accommodation_type, "Type-C") == 0)
        type = 3;
    else if(strcmp(record.accommodation_type, "Type-D") == 0)
        type = 4;
    else
        printf("Wrong accomodation block entered\n");
    //Checking if accomodation type entered is correct, and if yes- entering information in data array of node of corresp id type
    if (type && record.idtype == 0)
    {
        //Now, find position such that id-num are in sorted order
        while (i < root->data[0]->size && strcmp(root->data[0]->records[i].num.aadhaar, record.num.aadhaar) < 0)
        {
            i++;
        }
        //Once we reach correct position, checking whether duplicate record exists or not
        if(i == root->data[0]->size || (i < root->data[0]->size && strcmp(root->data[0]->records[i].num.aadhaar, record.num.aadhaar) > 0))
        {
            //Shifting all values below position we want to create free space
            for (j = root->data[0]->size; j > i; --j)
            {
                root->data[0]->records[j] = root->data[0]->records[j - 1];
            }
            //Then inserting record in correct position
            root->data[0]->records[i] = record;
            //Allocating accomodation block by passing address of record and type of accomodation
            allocate_accomodation_block(&(root->data[record.idtype]->records[i]),type);
            printf("Record inserted\n");
            //Inserting in index tree as well, by passing address
            insert_index_tree(&record);
            //Incrementing size of data node
            root->data[record.idtype]->size++;
        }
        else
        {
            printf("Duplicate found!! Hence, can't insert");
        }
    }
    //Checking if accomodation type entered is correct, and if yes- entering information in data array of node of corresp id type
    else if (type && record.idtype == 1)
    {
        //Now, find position such that id-num are in sorted order
        while (i < root->data[1]->size && strcmp(root->data[1]->records[i].num.passport, record.num.passport) < 0)
        {
            i++;
        }
        //Once we reach correct position, checking whether duplicate record exists or not
        if(i == root->data[1]->size || (i < root->data[1]->size && strcmp(root->data[1]->records[i].num.passport, record.num.passport) > 0))
        {
            //Shifting all values below position we want to create free space
            for (j = root->data[1]->size; j > i; --j)
            {
                root->data[1]->records[j] = root->data[1]->records[j - 1];
            }
            //Then inserting record in correct position
            root->data[1]->records[i] = record;
            //Allocating accomodation block by passing address of record and type of accomodation
            allocate_accomodation_block(&(root->data[record.idtype]->records[i]),type);
            printf("Record inserted\n");
            //Inserting in index tree as well, by passing address
            insert_index_tree(&record);
            //Incrementing size of data node
            root->data[record.idtype]->size++;
        }
        else
        {
            printf("Duplicate found!! Hence, can't insert");
        }
    }
    //Checking if accomodation type entered is correct, and if yes- entering information in data array of node of corresp id type
    else if (type && record.idtype == 2)
    {
        //Now, find position such that id-num are in sorted order
        while (i < root->data[2]->size && strcmp(root->data[2]->records[i].num.empcode, record.num.empcode) < 0)
        {
            i++;
        }
        //Once we reach correct position, checking whether duplicate record exists or not
        if(i == root->data[2]->size || (i < root->data[2]->size && strcmp(root->data[0]->records[i].num.empcode, record.num.empcode) > 0))
        {
            //Shifting all values below position we want to create free space
            for (j = root->data[2]->size; j > i; --j)
            {
                root->data[2]->records[j] = root->data[2]->records[j - 1];
            }
            //Then inserting record in correct position
            root->data[2]->records[i] = record;
            //Allocating accomodation block by passing address of record and type of accomodation
            allocate_accomodation_block(&(root->data[record.idtype]->records[i]),type);
            printf("Record inserted\n");
            //Inserting in index tree as well, by passing address
            insert_index_tree(&record);
            //Incrementing size of data node
            root->data[record.idtype]->size++;
        }
        else
        {
            printf("Duplicate found!! Hence, can't insert");
        }
    }
    else
    {
        printf("Not possible\n");
    }
}

//No need of duplicate function as while inserting itself, care of entry not being duplicate is taken
void remove_duplicates()
{
    int i, j;
    //Traversing through leaf of parent as idtype 0
    for(i = 1; i < root->data[0]->size; ++i)
    {
        //As records are inserted in sorted manner according to idnum, hence if we find any 2 consecutive records with same id num
        if(strcmp(root->data[0]->records[i].num.aadhaar, root->data[0]->records[i - 1].num.aadhaar) == 0)
        {
            //Shifting all remaining records above
            for(j = i + 1; j < root->data[0]->size; ++j)
            {
                root->data[0]->records[j - 1] = root->data[0]->records[j];
            }
            //And then decrementing size, as well as i, as it will be incremented in for loop again
            i--;
            root->data[0]->size--;
        }
    }
    //Traversing through leaf of parent as idtype 1
    for(i = 1; i < root->data[1]->size; ++i)
    {
        //As records are inserted in sorted manner according to idnum, hence if we find any 2 consecutive records with same id num
        if(strcmp(root->data[1]->records[i].num.passport, root->data[1]->records[i - 1].num.passport) == 0)
        {
            //Shifting all remaining records above
            for(j = i + 1; j < root->data[1]->size; ++j)
            {
                root->data[1]->records[j - 1] = root->data[1]->records[j];
            }
            //And then decrementing size, as well as i, as it will be incremented in for loop again
            i--;
            root->data[1]->size--;
        }
    }
    //Traversing through leaf of parent as idtype 0
    for(i = 1; i < root->data[2]->size; ++i)
    {
        //As records are inserted in sorted manner according to idnum, hence if we find any 2 consecutive records with same id num
        if(strcmp(root->data[2]->records[i].num.empcode, root->data[2]->records[i - 1].num.empcode) == 0)
        {
            //Shifting all remaining records above
            for(j = i + 1; j < root->data[2]->size; ++j)
            {
                root->data[2]->records[j - 1] = root->data[2]->records[j];
            }
            //And then decrementing size, as well as i, as it will be incremented in for loop again
            i--;
            root->data[2]->size--;
        }
    }
    printf("Duplicates removed\n");
}

void print_record(accomodation record)
{
    printf("Firstname: %s\n", record.firstname);
    printf("Lastname: %s\n", record.lastname);
    printf("Accomodation-type: %s\n", record.accommodation_type);
    printf("Id-type: %d\n", record.idtype);
    if(record.idtype == 0)
        printf("Aadhaar No: %s\n", record.num.aadhaar);
    else if(record.idtype == 1)
        printf("Passport No: %s\n", record.num.passport);
    else if(record.idtype == 2)
        printf("Employee No: %s\n", record.num.empcode);
    printf("Address: %s\n", record.address);
}

void print_records_in_012_order()
{
    int i;
    //Initially we will traverse through child corresp to id_type 0 and print all records in it
    for(i = 0; i < root->data[0]->size; ++i)
    {
        print_record(root->data[0]->records[i]);
        printf("\n");
    }
    //Then we will traverse through child corresp to id_type 1 and print all records in it
    for(i = 0; i < root->data[1]->size; ++i)
    {
        print_record(root->data[1]->records[i]);
        printf("\n");
    }
    //Then we will traverse through child corresp to id_type 2 and print all records in it
    for(i = 0; i < root->data[2]->size; ++i)
    {
        print_record(root->data[2]->records[i]);
        printf("\n");
    }
}

void delete_record()
{
    int id_type, i = 0, j;
    char id_num[15];
    printf("Enter id type\n");
    scanf("%d", &id_type);
    printf("Enter id number\n");
    scanf("%s", id_num);
    //Case when id type is 0
    if(id_type == 0)
    {
        //Traversing through data[0] leaf node until we reach required position
        while(i < root->data[0]->size && strcmp(root->data[0]->records[i].num.aadhaar, id_num) < 0)
        {
            i++;
        }
        //Checking whether record to delete exists
        if(strcmp(root->data[0]->records[i].num.aadhaar, id_num) == 0)
        {
            //deallocating accomodation block corresp to that record
            deallocate_accomodation_block(&(root->data[0]->records[i]), (root->data[0]->records[i].accommodation_type[5]) - 'A' + 1);
            //As well deleting entry from index tree too
            delete_index_tree(&(root->data[0]->records[i]));
            j = i + 1;
            //Then to fill the empty space, shifting all record below given record above
            while(j < root->data[0]->size)
            {
                root->data[0]->records[j - 1] = root->data[0]->records[j];
                j++;
            }
            //Decrementing size
            root->data[0]->size--;
            printf("Record deleted\n");
        }
        else
        {
            printf("Record not found!!\n");
        }
    }
    //Case when id type is 1
    else if(id_type == 1)
    {
        //Traversing through data[1] leaf node until we reach required position
        while(i < root->data[1]->size && strcmp(root->data[1]->records[i].num.passport, id_num) < 0)
        {
            i++;
        }
        //Checking whether record to delete exists
        if(strcmp(root->data[1]->records[i].num.passport, id_num) == 0)
        {
            //deallocating accomodation block corresp to that record
            deallocate_accomodation_block(&(root->data[1]->records[i]), (root->data[1]->records[i].accommodation_type[5]) - 'A' + 1);
            //As well deleting entry from index tree too
            delete_index_tree(&(root->data[1]->records[i]));
            j = i + 1;
            //Then to fill the empty space, shifting all record below given record above
            while(j < root->data[1]->size)
            {
                root->data[1]->records[j - 1] = root->data[1]->records[j];
                j++;
            }
            //Decrementing size
            root->data[1]->size--;
            printf("Record deleted\n");
        }
        else
        {
            printf("Record not found!!\n");
        }
    }
    //Case when id type is 2
    else if(id_type == 2)
    {
        //Traversing through data[2] leaf node until we reach required position
        while(i < root->data[2]->size && strcmp(root->data[2]->records[i].num.empcode, id_num) < 0)
        {
            i++;
        }
        //Checking whether record to delete exists
        if(strcmp(root->data[2]->records[i].num.empcode, id_num) == 0)
        {
            //deallocating accomodation block corresp to that record
            deallocate_accomodation_block(&(root->data[2]->records[i]), (root->data[2]->records[i].accommodation_type[5]) - 'A' + 1);
            //As well deleting entry from index tree too
            delete_index_tree(&(root->data[2]->records[i]));
            j = i + 1;
            //Then to fill the empty space, shifting all record below given record above
            while(j < root->data[2]->size)
            {
                root->data[2]->records[j - 1] = root->data[2]->records[j];
                j++;
            }
            //Decrementing size
            root->data[2]->size--;
            printf("Record deleted\n");
        }
        else
        {
            printf("Record not found!!\n");
        }
    }
}

void update_record()
{
    int id_type, flag = 1, i = 0;
    int option;
    char id_num[15];
    char updated_value[100];
    printf("Enter id type\n");
    scanf("%d", &id_type);
    printf("Enter id number\n");
    scanf("%s", id_num);
    //Searching through records and checking if record with given idtype and id-num exists
    if(id_type == 0)
    {
        while(i < root->data[0]->size && strcmp(root->data[0]->records[i].num.aadhaar, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[0]->records[i].num.aadhaar, id_num) != 0)
        {
            flag = 0;
        }
    }
    else if(id_type == 1)
    {
        while(i < root->data[1]->size && strcmp(root->data[1]->records[i].num.passport, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[1]->records[i].num.passport, id_num) != 0)
        {
            flag = 0;
        }
    }
    else if(id_type == 2)
    {
        while(i < root->data[2]->size && strcmp(root->data[2]->records[i].num.empcode, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[2]->records[i].num.empcode, id_num) != 0)
        {
            flag = 0;
        }
    }
    //If record exists
    if(flag == 1)
    {
        printf("If you want to update \n1.Firstname: Enter 1\n2.Lastname: Enter 2\n3.Address: Enter 3\n");
        scanf("%d", &option);
        printf("Enter data:\n");
        fflush(stdin);
        gets(updated_value);
        switch(option)
        {
        //If we have to update firstname, then deleting existing record from index tree, and then inserting another record with changed name in index tree
        case 1:delete_index_tree(&(root->data[id_type]->records[i]));
            strcpy(root->data[id_type]->records[i].firstname, updated_value);
            insert_index_tree(&(root->data[id_type]->records[i]));
            break;
        //Similiarly making changes to update lastname too
        case 2:delete_index_tree((&root->data[id_type]->records[i]));
            strcpy(root->data[id_type]->records[i].lastname, updated_value);
            insert_index_tree(&(root->data[id_type]->records[i]));
            break;
        //To update address, changing address without any remaining considerations
        case 3:strcpy(root->data[id_type]->records[i].address, updated_value);
            break;
        default:printf("Wrong option entered\n");
        }
    }
    else
    {
        printf("Record not found\n");
    }
}

void initialize_accomodation_block_tree()
{
    int i;
    Block_root = (accomodation_block_node*)malloc(sizeof(accomodation_block_node));
    Block_root->block[0] = 1;
    Block_root->block[1] = 2;
    Block_root->block[2] = 3;

    Block_root->data[0] = (allocated_block*)malloc(sizeof(allocated_block));
    Block_root->data[0]->size = 0;
    Block_root->data[0]->next = Block_root->data[1];
    Block_root->data[1] = (allocated_block*)malloc(sizeof(allocated_block));
    Block_root->data[1]->size = 0;
    Block_root->data[1]->next = Block_root->data[2];
    Block_root->data[2] = (allocated_block*)malloc(sizeof(allocated_block));
    Block_root->data[2]->size = 0;
    Block_root->data[2]->next = Block_root->data[3];
    Block_root->data[3] = (allocated_block*)malloc(sizeof(allocated_block));
    Block_root->data[3]->size = 0;
    Block_root->data[3]->next = NULL;
    for (i = 0; i < 10; ++i)
    {
        Block_root->data[0]->records[i] = NULL;
        Block_root->data[1]->records[i] = NULL;
        Block_root->data[2]->records[i] = NULL;
        Block_root->data[3]->records[i] = NULL;
    }
}

boolean allocate_accomodation_block(accomodation* aptr, int type)
{
    int i = 0;
    boolean ret_val = true;
    //Checking if block is free
    if(Block_root->data[type - 1]->size >= 10)
    {
        printf("No free block\n");
        ret_val = false;
    }
    else
    {
        //Finding first deallocated block
        while(Block_root->data[type - 1]->records[i] != NULL)
        {
            i++;
        }
        //Then assigning record to first vacant block
        Block_root->data[type - 1]->records[i] = aptr;
        //Incrementing value of allocated blocks
        Block_root->data[type - 1]->size++;
        printf("Block allocated\n");
    }
    return ret_val;
}

boolean deallocate_accomodation_block(accomodation* aptr, int type)
{
    int i = 0;
    boolean ret_val = true;
    //Traversing through block array until we come across value equal to pointer to given record
    while(i < Block_root->data[type - 1]->size && Block_root->data[type - 1]->records[i] != aptr)
    {
        i++;
    }
    //Deallocating block
    if(Block_root->data[type - 1]->records[i] == aptr)
    {
        Block_root->data[type - 1]->records[i] = NULL;
        //Decrementing size
        Block_root->data[type - 1]->size--;
    }
    else
    {
        printf("Record not found");
        ret_val = false;
    }
    return ret_val;
}

void address_search()
{
    char start[5], end[5];
    int type = -1, i;
    printf("Enter range of address:\n");
    printf("Start from:(ex. A1, B4):\n");
    scanf("%s", start);
    printf("End at:(ex. A9, B8:)\n");
    scanf("%s", end);
    //Checking whether given start address and end address are correct
    if(start[0] == end[0] && start[1] <= end[1])
    {
        //Corresponding to type of accomodation block given, assigning value to type.
        if(start[0] == 'A')
            type = 0;
        else if(start[0] == 'B')
            type = 1;
        else if(start[0] == 'C')
            type = 2;
        else if(start[0] == 'D')
            type = 3;

        i = start[1] - '0' - 1;

        //Traversing leaf node of given type, from start index to end index and printing records.
        for(;i <= end[1] - '0' - 1; ++i)
        {
            if(Block_root->data[type]->records[i])
            {
                print_record(*(Block_root->data[type]->records[i]));
                printf("\n");
            }
            else
                printf("Vacant block\n\n");
        }
    }
    else
    {
        printf("Wrong range given!\n");
    }
}

void special_request_allocation()
{
    int id_type, i = 0, flag = 1;
    char id_num[15];
    char new_block[15];
    printf("Enter id type\n");
    scanf("%d", &id_type);
    printf("Enter id number\n");
    scanf("%s", id_num);
    //Traversing through tree and searching if record with given id type and id-num exists
    if(id_type == 0)
    {
        while(i < root->data[0]->size && strcmp(root->data[0]->records[i].num.aadhaar, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[0]->records[i].num.aadhaar, id_num) != 0)
        {
            flag = 0;
        }
    }
    else if(id_type == 1)
    {
        while(i < root->data[1]->size && strcmp(root->data[1]->records[i].num.passport, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[1]->records[i].num.passport, id_num) != 0)
        {
            flag = 0;
        }
    }
    else if(id_type == 2)
    {
        while(i < root->data[2]->size && strcmp(root->data[2]->records[i].num.empcode, id_num) < 0)
        {
            i++;
        }
        if(strcmp(root->data[2]->records[i].num.empcode, id_num) != 0)
        {
            flag = 0;
        }
    }
    //If given record exists
    if(flag)
    {
        printf("Enter block you want\n");
        scanf("%s", new_block);
        //Storing value of current allocated block
        int prev = (root->data[id_type]->records[i].accommodation_type[5]) - 'A' + 1;
        //Checking whether asked block exists or not and if yes
        if(allocate_accomodation_block(&(root->data[id_type]->records[i]), new_block[5] - 'A' + 1))
        {
            //Deallocating previous block
            deallocate_accomodation_block(&(root->data[id_type]->records[i]), prev);
            //Changing value of accomodation block
            strcpy(root->data[id_type]->records[i].accommodation_type, new_block);
        }

    }
    else
    {
        printf("Record not found!!\n");
    }
}

int main()
{
    int n, i, option, flag = 1;;
    initalize_BP_tree();
    initialize_accomodation_block_tree();
    initialize_Index_tree();
    printf("Enter number of residents\n");
    scanf("%d", &n);
    for(i = 0; i < n; ++i)
    {
        insert_record();
        printf("\n");
    }
    //remove_duplicates();
    while(flag)
    {
        printf("If you want to\n1.Insert record: Enter 1\n2.Print record in 0-1-2 order: Enter 2\n3.Delete record: Enter 3\n4.Update record: Enter 4\n");
        printf("5.Address search: Enter 5\n6.Special Request Allocation: Enter 6\n7.Remove Duplicates: Enter 7\n8.Print sorted records: Enter 8\n");
        printf("9.Search: Enter 9\n");
        scanf("%d", &option);
        switch(option)
        {
        case 1:insert_record();
            break;
        case 2:print_records_in_012_order();
            break;
        case 3:delete_record();
            break;
        case 4:update_record();
            break;
        case 5:address_search();
            break;
        case 6:special_request_allocation();
            break;
        case 7:remove_duplicates();
            break;
        case 8:print_sorted_records();
            break;
        case 9:search();
            break;
        default:flag = 0;
        }
    }
    return 0;
}

