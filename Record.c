#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Record.h"

Node* create_node(char first_name[], char last_name[], char accom_type[], int id_type, char id_no[], char address[])
{
    //Allocating space in heap.
    Node* acc = (Node*)malloc(sizeof(Node));
    acc->next = acc->prev = NULL;
    //Assigning values to data members of node.
    strcpy(acc->data.accommodation_type, accom_type);
    strcpy(acc->data.firstname, first_name);
    strcpy(acc->data.lastname, last_name);
    strcpy(acc->data.address, address);
    acc->data.idtype = id_type;
    acc->data.idnum.aadhaar[0] = acc->data.idnum.empcode[0] = acc->data.idnum.passport[0] = '\0';
    if(id_type == 0)
    {
        strcpy(acc->data.idnum.aadhaar, id_no);
    }
    else if(id_type == 1)
    {
        strcpy(acc->data.idnum.passport, id_no);
    }
    else if(id_type == 2)
    {
        strcpy(acc->data.idnum.empcode, id_no);
    }
    //Returning the node
    return acc;
}

void print_records()
{
    Node* temp = head;
    //Condition of 0 nodes
    if(head == NULL)
    {
        printf("OOPS!! No records found.\n");
    }
    //Else traversing the list and printing info
    while(temp)
    {
        int val = -1;
        printf("First name: %s\n", temp->data.firstname);
        printf("Last name: %s\n", temp->data.lastname);
        printf("Accomodation type: %s\n", temp->data.accommodation_type);
        printf("Id type: %d\n", temp->data.idtype);
        val = temp->data.idtype;
        if(val == 0)
            printf("Id no: %s\n", temp->data.idnum.aadhaar);
        else if(val == 1)
            printf("Id no: %s\n", temp->data.idnum.passport);
        else if(val == 2)
            printf("Id no: %s\n", temp->data.idnum.empcode);
        printf("Address: %s\n", temp->data.address);
        temp = temp->next;
        printf("\n-------------------------------------------------------------------------------------\n\n");
    }
}

Node* insert()
{
    char firstname[50];
    char lastname[50];
    char accomodation_type[20];
    char id_no[15];
    char address[100];
    int id_type;
    Node* temp = NULL;
    //Inputting data from user, regarding node to be inserted
    printf("Enter first name\n");
    scanf("%s", firstname);
    printf("Enter last name\n");
    scanf("%s", lastname);
    printf("Enter accomodation type\n");
    scanf("%s", accomodation_type);
    printf("Enter id type\n");
    insert_accomodation(accomodation_type);
    scanf("%d", &id_type);
    //Checking if inputted id_type is within range
    if(id_type < 0 || id_type > 2)
    {
        printf("Wrong information given\n");
    }
    //If inputted information is correct
    else
    {
        printf("Enter id no\n");
        scanf("%s", id_no);
        printf("Enter address\n");
        fflush(stdin);
        gets(address);
        temp = create_node(firstname, lastname, accomodation_type, id_type, id_no, address);
        //Case when node to be inserted is first node of the list
        if(head == NULL)
        {
            if(id_type == 0)
                head = nptr0 = temp;
            else if(id_type == 1)
                head = nptr1 = temp;
            else if(id_type == 2)
                head = nptr2 = temp;
        }
        else
        {
            //For id type = 0
            if(id_type == 0)
            {
                //When node to be inserted is first one of id type 0
                if(nptr0 == NULL)
                {
                    nptr0 = temp;
                    nptr0->next = head;
                    head->prev = nptr0;
                    head = nptr0;
                }
                //When it isn't 1st node of type 0
                else
                {
                    //Changing links by checking all conditions
                    temp->next = nptr0->next;
                    if(nptr0->next)
                        nptr0->next->prev = temp;
                    temp->prev = nptr0;
                    nptr0->next = temp;
                    nptr0 = temp;
                }
            }
            //For id type 1
            else if(id_type == 1)
            {
                //Condition when its first node of type 1
                if(nptr1 == NULL)
                {
                    //When there is no node of Type 0 too
                    if(nptr0 == NULL)
                    {
                        //Appropriate linking
                        temp->next = head;
                        head->prev = temp;
                        nptr1 = temp;
                        head = temp;
                    }
                    //When there exists at least 1 node of type 0
                    else
                    {
                        temp->next = nptr0->next;
                        if(nptr0->next)
                            nptr0->next->prev = temp;
                        nptr0->next = temp;
                        temp->prev = nptr0;
                        nptr1 = temp;
                    }
                }
                //Condition when there are previously nodes of type 1
                else
                {
                    temp->next = nptr1->next;
                    if(nptr1->next)
                        nptr1->next->prev = temp;
                    temp->prev = nptr1;
                    nptr1->next = temp;
                    nptr1 = temp;
                }
            }
            //Id type 2
            else if(id_type == 2)
            {
                //Condition when its first node of type 2
                if(nptr2 == NULL)
                {
                    //Condition when theres no node of type 1
                    if(nptr1 == NULL)
                    {
                        nptr0->next = temp;
                        temp->prev = nptr0;
                        nptr2 = temp;
                    }
                    //Condition when node of type 1 exist
                    else
                    {
                        nptr1->next = temp;
                        temp->prev = nptr1;
                        nptr2 = temp;
                    }
                }
                //Condition when node of type 2 exists
                else
                {
                    temp->next = NULL;
                    nptr2->next = temp;
                    temp->prev = nptr2;
                    nptr2 = temp;
                }
            }
        }
    }
    printf("DONE!!\n");
    return temp;
}

//Applying merge sort on Index list

void split(Index* start, Index** first, Index** second)
{
    //Using fast and slow pointer approach
    Index* fast;
    Index* slow;
    slow = start;
    fast = start->next;

    while(fast)
    {
        fast = fast->next;
        if(fast)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    //Getting the midpoint of list
    *first = start;
    *second = slow->next;
    slow->next = NULL;
}

Index* merge(Index* a, Index* b)
{
    Index* result = NULL;
    //If size of 1 of the 2 lists is 0
    if(a == NULL)
        result = b;
    else if(b == NULL)
        result = a;
    else
    {
        //Merging two lists by comparing data and then chosing proper position
        if(strcmp(a->ptr_to_list->data.firstname, b->ptr_to_list->data.firstname) < 0)
        {
            result = a;
            result->next = merge(a->next, b);
        }
        else if(strcmp(a->ptr_to_list->data.firstname, b->ptr_to_list->data.firstname) > 0)
        {
            result = b;
            result->next = merge(a, b->next);
        }
        else
        {
            if(strcmp(a->ptr_to_list->data.lastname, b->ptr_to_list->data.lastname) <= 0)
            {
                result = a;
                result->next = merge(a->next, b);
            }
            else
            {
                result = b;
                result->next = merge(a, b->next);
            }
        }
    }
    return result;
}

void merge_sort(Index** head_ref)
{
    //Recursive merge sort
    Index* start = *head_ref;
    Index* a;
    Index* b;

    if(start == NULL || (start->next) == NULL)
    {
        return;
    }
    else
    {
        split(start, &a, &b);
        merge_sort(&a);
        merge_sort(&b);
        *head_ref = merge(a, b);
    }

}

void sort_list()
{
    int i = 1;
    Node* lptr = head;
    Index* iptr = NULL;
    //Creating Index list and then sorting it via merge sort
    while(lptr)
    {
        //Creating list, initializing index and ptr_to_list of Index node
        Index* temp = (Index*)malloc(sizeof(Index));
        temp->index = i;
        temp->ptr_to_list = lptr;
        temp->next = NULL;
        if(iptr == NULL)
        {
            index_head = iptr = temp;
        }
        else
        {
            iptr->next = temp;
            iptr = temp;
        }
        i++;
        lptr = lptr->next;
    }
    //Calling merge sort on list
    merge_sort(&index_head);
}

void remove_duplicates()
{
    char accom_type[20];
    Node* start = head;
    Node* temp = NULL;
    while(start)
    {
        //Checking for all possible pairs, and if they are same, then deleting the another one
        temp = start->next;
        while(temp)
        {
            if(start->data.idtype == temp->data.idtype && strcmp(start->data.idnum.aadhaar, temp->data.idnum.aadhaar) == 0
               && strcmp(start->data.idnum.passport, temp->data.idnum.passport) == 0 && strcmp(start->data.idnum.empcode, temp->data.idnum.empcode) == 0)
            {
                strcpy(accom_type, temp->data.accommodation_type);
                remove_accomodation(accom_type);
                temp->prev->next = temp->next;
                if(temp->next)
                    temp->next->prev = temp->prev;
                Node* del = temp;
                temp = temp->next;
                update_index_deletion(del);
                free(del);
            }
            else
                temp = temp->next;
        }
        start = start->next;
    }
    printf("Duplicates removed\n\n");
}

void print_sorted_records()
{
    Index* iptr = index_head;
    //Condition when no of nodes is 0
    if(iptr == NULL)
    {
        printf("OOPS!! No entries found\n");
    }
    while(iptr)
    {
        Node* temp = iptr->ptr_to_list;
        int val = -1;
        printf("First name: %s\n", temp->data.firstname);
        printf("Last name: %s\n", temp->data.lastname);
        printf("Accomodation type: %s\n", temp->data.accommodation_type);
        printf("Id type: %d\n", temp->data.idtype);
        val = temp->data.idtype;
        if(val == 0)
            printf("Id no: %s\n", temp->data.idnum.aadhaar);
        else if(val == 1)
            printf("Id no: %s\n", temp->data.idnum.passport);
        else if(val == 2)
            printf("Id no: %s\n", temp->data.idnum.empcode);
        printf("Address: %s\n\n", temp->data.address);
        iptr = iptr->next;
        printf("\n--------------------------------------------------------------------------------------\n\n");
    }
}

void search()
{
    char first_name[50];
    char last_name[50];
    int flag = 0;
    printf("Enter firstname\n");
    scanf("%s", first_name);
    printf("Enter lastname\n");
    scanf("%s", last_name);
    Index* iptr = index_head;
    //As we cant use binary search on linked list, hence, using linear search
    while(iptr && strcmp(iptr->ptr_to_list->data.firstname, first_name) < 0)
    {
        iptr = iptr->next;
    }
    while(iptr && strcmp(iptr->ptr_to_list->data.firstname, first_name) == 0 && strcmp(iptr->ptr_to_list->data.lastname, last_name) < 0)
    {
        iptr = iptr->next;
    }
    //Traversing through all the records with matching first name and last name
    while(iptr && strcmp(iptr->ptr_to_list->data.firstname, first_name) == 0 && strcmp(iptr->ptr_to_list->data.lastname, last_name) == 0)
    {
        Node* temp = iptr->ptr_to_list;
        int val = -1;
        printf("First name: %s\n", temp->data.firstname);
        printf("Last name: %s\n", temp->data.lastname);
        printf("Accomodation type: %s\n", temp->data.accommodation_type);
        printf("Id type: %d\n", temp->data.idtype);
        val = temp->data.idtype;
        if(val == 0)
            printf("Id no: %s\n", temp->data.idnum.aadhaar);
        else if(val == 1)
            printf("Id no: %s\n", temp->data.idnum.passport);
        else if(val == 2)
            printf("Id no: %s\n", temp->data.idnum.empcode);
        printf("Address: %s\n\n", temp->data.address);
        iptr = iptr->next;
        flag = 1;
    }
    //condition when record not found
    if(flag == 0)
    {
        printf("OOPS!! Record not found\n");
    }
    printf("\n-------------------------------------------------------------------------------\n\n");
}

void update_index_deletion(Node* nptr)
{
    Index* iptr = index_head;
    Index* ptr = NULL;
    Index* del = NULL;
    int flag = 1;
    int val = -1;
    while(iptr && flag)
    {
        //When required pointer is found
        if(iptr->ptr_to_list == nptr)
        {
            val = iptr->index;
            //When ptr to be deleted is first one itself
            if(ptr == NULL)
            {
                index_head = iptr->next;
                del = iptr;
                free(del);
                iptr = index_head;
            }
            else
            {
                ptr->next = iptr->next;
                del = iptr;
                iptr = iptr->next;
                free(del);
            }
        }
        else
        {
            ptr = iptr;
            iptr = iptr->next;
        }
    }
    iptr = index_head;
    //Decrementing index value of all the pointer which come after deleted pointer
    while(iptr && val != -1)
    {
        if(iptr->index > val)
        {
            iptr->index--;
        }
        iptr = iptr->next;
    }
}

void delete_record()
{
    int id_type, flag = 1;
    char id_num[15];
    char accom_type[10];
    Node* lptr = head;
    printf("Enter id type of record you want to delete\n");
    scanf("%d", &id_type);
    printf("Enter id number\n");
    scanf("%s", id_num);
    //Traversing through list to get required position
    while(lptr && flag)
    {
        if(lptr->data.idtype == id_type)
        {
            //id type and id number matched
            if(id_type == 0 && strcmp(lptr->data.idnum.aadhaar, id_num) == 0)
            {
                //deallocating accomodation
                strcpy(accom_type, lptr->data.accommodation_type);
                remove_accomodation(accom_type);
                //Node to be deleted isn't first in list
                if(lptr->prev)
                {
                    lptr->prev->next = lptr->next;
                    printf("Done\n");
                }
                //Node to be deleted is first node in list
                else
                {
                    head = lptr->next;
                }
                //Node to be deleted isn't last node of list
                if(lptr->next)
                {
                    //Considering changes to be made in nptr0 as well

                    if(lptr->next->data.idtype == 0)
                        lptr->next->prev = lptr->prev;
                    //If its last node in list of 0 id type
                    else
                    {
                        lptr->next->prev = lptr->prev;
                        if(lptr->prev && lptr->prev->data.idtype == 0)
                            nptr0 = lptr->prev;
                        //If it is only node of type 0
                        else
                            nptr0 = NULL;
                    }
                }
                //Checking case if its last node as well as only node of type 0
                else
                {
                    if(lptr->prev->data.idtype == 0)
                    {
                        nptr0 = lptr->prev;
                    }
                    else
                    {
                        nptr0 = NULL;
                    }
                }
                flag = 0;
                update_index_deletion(lptr);
                printf("RECORD DELETED\n");
            }
            //id type and id number matched
            else if(id_type == 1 && strcmp(lptr->data.idnum.passport, id_num) == 0)
            {
                //Deallocating block
                strcpy(accom_type, lptr->data.accommodation_type);
                remove_accomodation(accom_type);
                //Checking if node to be deleted is first node
                if(lptr->prev)
                {
                    lptr->prev->next = lptr->next;
                }
                else
                {
                    head = lptr->next;
                }
                //Checking if node to be deleted is last node
                if(lptr->next)
                {
                    if(lptr->next->data.idtype == 1)
                        lptr->next->prev = lptr->prev;
                    //Checking if node to be deleted is last node of type 1
                    else
                    {
                        lptr->next->prev = lptr->prev;
                        //Checking if it is only node of type 1
                        if(lptr->prev && lptr->prev->data.idtype == 1)
                            nptr1 = lptr->prev;
                        else
                            nptr1 = NULL;
                    }
                }
                //Checking case if its last node as well as only node of type 1
                else
                {
                    if(lptr->prev->data.idtype == 1)
                    {
                        nptr1 = lptr->prev;
                    }
                    else
                    {
                        nptr1 = NULL;
                    }
                }
                flag = 0;
                update_index_deletion(lptr);
                printf("RECORD DELETED\n");
            }
            //id type and id num matched
            else if(id_type == 2 && strcmp(lptr->data.idnum.empcode, id_num) == 0)
            {
                //Deallocating accomodation
                strcpy(accom_type, lptr->data.accommodation_type);
                remove_accomodation(accom_type);
                //Considering case if node to be deleted is first node
                if(lptr->prev)
                {
                    lptr->prev->next = lptr->next;
                }
                else
                {
                    head = lptr->next;
                }
                //Considering case if node to be deleted is last node of list
                if(lptr->next)
                {
                    if(lptr->next->data.idtype == 2)
                        lptr->next->prev = lptr->prev;
                    else
                    {
                        lptr->next->prev = lptr->prev;
                        if(lptr->prev && lptr->prev->data.idtype == 2)
                            nptr2 = lptr->prev;
                        else
                            nptr2 = NULL;
                    }
                }
                //Checking case if its last node as well as only node of type 2
                else
                {
                    if(lptr->prev->data.idtype == 2)
                    {
                        nptr2 = lptr->prev;
                    }
                    else
                    {
                        nptr2 = NULL;
                    }
                }
                flag = 0;
                update_index_deletion(lptr);
                printf("RECORD DELETED\n");
            }

        }
        lptr = lptr->next;
    }
    if(flag == 1)
    {
        printf("OOPS!! Record not found\n");
    }
}

void update_index_insertion(Node* nptr)
{
    Node* start = head;
    Index* iptr = index_head;
    Index* ptr = NULL;
    Index* new_node = (Index*)malloc(sizeof(Index));
    int count = 1;
    //Finding the correct position to insert node
    while(iptr && strcmp(iptr->ptr_to_list->data.firstname, nptr->data.firstname) < 0)
    {
        ptr = iptr;
        iptr = iptr->next;
    }
    while(iptr && strcmp(iptr->ptr_to_list->data.firstname, nptr->data.firstname) == 0 && strcmp(iptr->ptr_to_list->data.lastname, nptr->data.lastname) < 0)
    {
        ptr = iptr;
        iptr = iptr->next;
    }
    //Finding position in data list of given node, so as to keep track if index
    while(start && start != nptr)
    {
        start = start->next;
        count++;
    }
    //Assigning value to new_node' ptr_to_list
    new_node->ptr_to_list = nptr;
    //Checking if index node to be inserted is first one or not and making changes accordingly
    if(ptr)
    {
        ptr->next = new_node;
        new_node->next = iptr;
    }
    else
    {
        new_node->next = index_head;
        index_head = new_node;
    }
    //Changing links
    new_node->index = -1;
    iptr = index_head;
    //Traversing through list and changing index value of nodes coming after inserted node
    while(iptr)
    {
        if(iptr->index >= count)
            iptr->index++;
        iptr = iptr->next;
    }
    new_node->index = count;
}

void update_record()
{
    int id_type;
    int option;
    char id_num[15];
    int flag = 1;
    char update_data[100];
    Node* nptr = head;
    Node* temp = NULL;

    printf("Enter id type\n");
    scanf("%d", &id_type);
    printf("Enter id num\n");
    scanf("%s", id_num);
    //Finding position of required node
    while(nptr && flag)
    {
        if(id_type == 0 && strcmp(nptr->data.idnum.aadhaar, id_num) == 0)
        {
            temp = nptr;
            flag = 0;
        }
        else if(id_type == 1 && strcmp(nptr->data.idnum.passport, id_num) == 0)
        {
            temp = nptr;
            flag = 0;
        }
        else if(id_type == 2 && strcmp(nptr->data.idnum.empcode, id_num) == 0)
        {
            temp = nptr;
            flag = 0;
        }
        nptr = nptr->next;
    }

    printf("Enter\n1.Update Firstname\n2.Update Lastname\n3.Update address\n\n");
    scanf("%d", &option);
    //In case of updating first name or last name, we will deleting corresponding node from Index list
    if(option == 1 || option == 2)
    {
        update_index_deletion(temp);
    }
    printf("Enter data\n");
    fflush(stdin);
    gets(update_data);
    //Updating data accordingly
    switch(option)
    {
    case 1:strcpy(temp->data.firstname, update_data);
        break;
    case 2:strcpy(temp->data.lastname, update_data);
        break;
    case 3:strcpy(temp->data.address, update_data);
        break;
    default:printf("Wrong info entered\n");
    }
    //And then if data changed is first name or last name, again inserting corresponding node in Index
    if(option == 1 || option == 2)
    {

        update_index_insertion(temp);
    }
    printf("Done\n");
}

void initialize_accom_record()
{
    int i;
    track_node* type1 = NULL;
    track_node* type2 = NULL;
    track_node* type3 = NULL;
    track_node* type4 = NULL;
    track_node* temp = NULL;
    //Creating list of 10 nodes of type 1, 2, 3, 4 each and initializing all with zero
    for(i = 0; i < 10; ++i)
    {
        temp = (track_node*)malloc(sizeof(track_node));
        temp->status = 0;
        temp->next = NULL;
        //case when node to be inserted is first node of list
        if(type1 == NULL)
        {
            type1 = Record.Type1 = temp;
        }
        else
        {
            type1->next = temp;
            type1 = temp;
        }
        temp = (track_node*)malloc(sizeof(track_node));
        temp->status = 0;
        temp->next = NULL;
        //case when node to be inserted is first node of list
        if(type2 == NULL)
        {
            type2 = Record.Type2 = temp;
        }
        else
        {
            type2->next = temp;
            type2 = temp;
        }
        temp = (track_node*)malloc(sizeof(track_node));
        temp->status = 0;
        temp->next = NULL;
        //case when node to be inserted is first node of list
        if(type3 == NULL)
        {
            type3 = Record.Type3 = temp;
        }
        else
        {
            type3->next = temp;
            type3 = temp;
        }
        temp = (track_node*)malloc(sizeof(track_node));
        temp->status = 0;
        temp->next = NULL;
        //case when node to be inserted is first node of list
        if(type4 == NULL)
        {
            type4 = Record.Type4 = temp;
        }
        else
        {
            type4->next = temp;
            type4 = temp;
        }

    }
}

status_code insert_accomodation(char accom_type[])
{
    status_code sc = FAILURE;
    //For every accomodation type, firstly checking if place is vacant and then assigning block
    if(strcmp(accom_type, "Type-I") == 0)
    {
        track_node* type = Record.Type1;
        //Finding first non-accomodated block
        while(type && (type->status == 1))
        {
            type = type->next;
        }
        if(type)
        {
            sc = SUCCESS;
            //Changing status of block as accomodated
            type->status = 1;
        }
        else
            printf("No vacant place\n");
    }
    else if(strcmp(accom_type, "Type-II") == 0)
    {
        track_node* type = Record.Type2;
        //Finding first non-accomodated block
        while(type && (type->status == 1))
        {
            type = type->next;
        }
        if(type)
        {
            sc = SUCCESS;
            //Changing status of block as accomodated
            type->status = 1;
        }
        else
            printf("No vacant place\n");
    }
    else if(strcmp(accom_type, "Type-III") == 0)
    {
        track_node* type = Record.Type3;
        //Finding first non-accomodated block
        while(type && (type->status == 1))
        {
            type = type->next;
        }
        if(type)
        {
            sc = SUCCESS;
            //Changing status of block as accomodated
            type->status = 1;
        }
        else
            printf("No vacant place\n");
    }
    else if(strcmp(accom_type, "Type-IV") == 0)
    {
        track_node* type = Record.Type4;
        //Finding first non-accomodated block
        while(type && (type->status == 1))
        {
            type = type->next;
        }
        if(type)
        {
            sc = SUCCESS;
            //Changing status of block as accomodated
            type->status = 1;
        }
        else
            printf("No vacant place\n");
    }
    return sc;
}

void remove_accomodation(char accom_type[])
{
    //Checking whether given accomodation type is of 1, 2, 3 or 4
    if(strcmp(accom_type, "Type-I") == 0)
    {
        track_node* type = Record.Type1;
        //Finding first accomodated node
        while(type && (type->status == 0))
        {
            type = type->next;
        }
        //Deallocating the node
        type->status = 0;
    }
    else if(strcmp(accom_type, "Type-II") == 0)
    {
        track_node* type = Record.Type2;
        //Finding first accomodated node
        while(type && (type->status == 0))
        {
            type = type->next;
        }
        //Deallocating the node
        type->status = 0;
    }
    else if(strcmp(accom_type, "Type-III") == 0)
    {
        track_node* type = Record.Type3;
        //Finding first accomodated node
        while(type && (type->status == 0))
        {
            type = type->next;
        }
        //Deallocating the node
        type->status = 0;
    }
    else if(strcmp(accom_type, "Type-IV") == 0)
    {
        track_node* type = Record.Type4;
        //Finding first accomodated node
        while(type && (type->status == 0))
        {
            type = type->next;
        }
        //Deallocating the node
        type->status = 0;
    }
}

void special_request_allocation()
{
    int id_type;
    int flag = 1;
    char id_num[20];
    char accomodation_type[20];
    printf("Enter id type\n");
    scanf("%d", &id_type);
    printf("Enter id num\n");
    scanf("%s", id_num);
    printf("Enter type of accomodation block you want\n");
    scanf("%s", accomodation_type);
    //Initially checking if block is available
    if(insert_accomodation(accomodation_type))
    {
        Node* nptr = head;
        //Traversing through list and finding correct node
        while(nptr && flag)
        {
            if(nptr->data.idtype == id_type)
            {
                if(id_type == 0 && (strcmp(nptr->data.idnum.aadhaar, id_num) == 0))
                {
                    //Deallocating block from accomodation
                    remove_accomodation(nptr->data.accommodation_type);
                    //Changing accomodation type of current node
                    strcpy(nptr->data.accommodation_type, accomodation_type);
                    flag = 0;
                }
                else if(id_type == 1 && (strcmp(nptr->data.idnum.passport, id_num) == 0))
                {
                    //Deallocating block from accomodation
                    remove_accomodation(nptr->data.accommodation_type);
                    //Changing accomodation type of current node
                    strcpy(nptr->data.accommodation_type, accomodation_type);
                    flag = 0;
                }
                else if(id_type == 2 && (strcmp(nptr->data.idnum.empcode, id_num) == 0))
                {
                    //Deallocating block from accomodation
                    remove_accomodation(nptr->data.accommodation_type);
                    //Changing accomodation type of current node
                    strcpy(nptr->data.accommodation_type, accomodation_type);
                    flag = 0;
                }
            }
            nptr = nptr->next;
        }
        printf("Done\n");
    }

}
