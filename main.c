#include "Record.h"

//All these pointers are extern in Record.h
//Traversal pointer for those with id type 0
Node* nptr0 = NULL;
//Traversal pointer for those with id type 1
Node* nptr1 = NULL;
//Traversal pointer for those with id type 2
Node* nptr2 = NULL;
//Head pointer of record list
Node* head = NULL;
//Head pointer of index list
Index* index_head = NULL;

int main()
{
    printf("Some points to consider while entering information\n1.While entering accomodation block either enter\n Type-I or\n Type-II or\n Type-III or\n Type-IV\n");
    printf(" It is case sensitive.\n2.Possible id types are 0, 1 and 2 only. Don't enter vague numbers, else compiler will creep");
    printf("\n3.Before updating Records, deleting records, requesting allocation, duplicates will be removed to avoid discrepancy.\n");
    printf("4.Make sure you enter correct id type and id number. Once entered can't be changed, and your identity in system will be maintained via it only.\n\n");
    printf("That's it!! Good to go!!\n\n");
    printf("\n**************************************************************************************\n\n");
    int n, i, choice = 0;
    int flag = 1;
    Node* temp = NULL;
    initialize_accom_record();
    printf("Enter number of students\n");
    scanf("%d", &n);
    for(i = 0; i < n; ++i)
    {
        insert();
        printf("\n**************************************************************************************\n\n");
    }

    sort_list(n);

    while(flag)
    {
        printf("Enter\n1.Insert record\n2.Remove duplicates\n3.Print record\n4.Print sorted records\n5.Search via Firstname and lastname\n");
        printf("6.Delete Record\n7.Update Record\n8.Special Request Allocation\n9.Exit\n\n");
        scanf("%d", &choice);
        printf("\n");
        switch(choice)
        {
        case 1:temp = insert();
            update_index_insertion(temp);
            break;
        case 2:remove_duplicates();
            break;
        case 3:print_records();
            break;
        case 4:print_sorted_records();
            break;
        case 5:search();
            break;
        case 6:remove_duplicates();
            delete_record();
            break;
        case 7:remove_duplicates();
            update_record();
            break;
        case 8:remove_duplicates();
            special_request_allocation();
            break;
        default:flag = 0;

        }
        printf("\n**************************************************************************************\n\n");
    }
    printf("THANKYOU!!\n");
    return 0;
}
