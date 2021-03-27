# Resident_record

The field idtype takes a value 0, 1, or 2, indicating whether the ID is Aadhaar Card number,Passport number, or Employee code respectively.

(a) InsertRecord(), reads a record from user and insert into a linkedlist of structures. The records with idtype 0 must be stored first, followed by records with
idtype 1 and 2. This order is preserved throughout.

(b) removeDuplicates() removes the duplicate entries (those with same idtype and idnum) from the records.

(c) printRecords(),prints the records in linked list so that all those who have provided Aadhar number are listed first, all those who have provided Passport number are listed next, and all those who have provided Employee code are listed at the end.

(d) To print the records in LL in alphabetic order of names (sorted by firstname and ties resolved by lastname). Instead of sorting the records in LL, I have created a thing called index. For this purpose, created a LL, index, of N nodes. printSortedRecords() uses the index LL, to print the records in alphabetic order of names.

(e) search() function prints the whole information for a given Employee name. If there are multiple records with same name then printed in alphabetic order.

(f) deleteRecord() deletes the record of a given employee, the input to this function is idtype and idnum.

(g) updateRecord() function updates the record of an employee. The field to update is the user input.

(h) There's an additional structure having 10 blocks of each type of accommodation. The types of accommodation are type-I, type-II, type-III, and type-IV. specialRequestAllocation() changes a type of block requested by an employee depending on the availability of that type of block.
