//  Books.cpp


#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Books.h"
using namespace std;

/* CLASS DECLARATION -------------------------------------------------------------*/
class database;

class Books{
private:
    
public:
    
    long long int isbn;
    string title;
    string author;
    int edition;
    string datePub;
    double cost;
    vector<pair<double, char>> bookOptions;
    
    Books(long long int bookIsbn, string bookTitle, database &storage, int edition = 0);
    ~Books();
    
};

class classSection{
private:
    
    
public:
    
    string departmentCode;
    int courseNumber;
    string courseName;
    int sectionNumber;
    
    vector<pair<bool, Books*>> courseBookList;
    
    
    classSection(string department, int courseNum, string name, database &storage, bool real);
    ~classSection();
    
};

class database {
public:
    vector<Books> bookList;
    vector<classSection> courseList;
    
    bool printCourseBooks(string department, int courseNum);
    bool assignBook(long long int bookIsbn, string department, int courseNum, int sectionNum, char bookRequired, bool real);
    bool printSectionBooks(string department, int courseNum, int sectionNum);
    bool printBookInfo(long long int bookIsbn);
    void printBookList();
    void printCourseList();
    void printPubDate(string datePub);
    void printDeptBooks(string department);
    void printDeptAverageMin(string department);
    void printDeptAverageMax(string department);
    database(){
        bookList = vector<Books>();
        courseList = vector<classSection>();
    }
    
    
};

/* BOOK CLASS DEFINITION -------------------------------------------------------------*/
Books::Books(long long int bookIsbn, string bookTitle, database &storage, int bookEdition){
    //VALIDATE - isbn is 13 digits, title is a string
    isbn = bookIsbn;
    title = bookTitle;
    edition = bookEdition;
    storage.bookList.push_back(*this);
    
}

Books::~Books(){
    //destructor
}

/* CLASS SECTION DEFINITIONS -------------------------------------------------------------*/
classSection::classSection(string department, int courseNum, string name, database &storage, bool real){
    departmentCode = department;
    courseNumber = courseNum;
    courseName = name;
    sectionNumber = 0;
    if(real){
        storage.courseList.push_back(*this);
    }
}

classSection::~classSection(){
    //destructor
}

/* DATABASE CLASS DEFINITION -------------------------------------------------------------*/
bool database::assignBook(long long int bookIsbn, string department, int courseNum, int sectionNum, char bookRequired, bool real){
    //check that book exist
    int i;
    bool found = true;
    for(i =0; i < bookList.size(); i++){
        if(bookList[i].isbn == bookIsbn){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    //check that course, section, and department exist
    int j;
    for(j =0; j < courseList.size(); j++){
        if(courseList[j].departmentCode == department && courseList[j].courseNumber == courseNum && courseList[j].sectionNumber == sectionNum ){
            found = true;
            break;
        } else if(courseList[j].departmentCode == department && courseList[j].courseNumber == courseNum && courseList[j].sectionNumber == 0 ){
            courseList[j].sectionNumber = sectionNum;
            found = true;
            break;
            
        } else if(courseList[j].departmentCode == department && courseList[j].courseNumber == courseNum ){
            classSection newClass(courseList[j].departmentCode, courseList[j].courseNumber, courseList[j].courseName, *this, false);
            newClass.sectionNumber = sectionNum;
            
            courseList.push_back(newClass);
            j = courseList.size()-1;
            found = true;
            break;
            
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    
    
    //check if book is optional, otherwise default 0
    Books* bookFound = &bookList[i];
    int requiredValue = bookRequired == 'R' ? 1 : 0;
    
    //add to courseBookList
    pair<bool, Books*> newBook(requiredValue,bookFound);
    courseList[j].courseBookList.push_back(newBook);
    
    return true;
}

bool database::printCourseBooks(string department, int courseNum){
    
    //verify that department and course number exist
    int k;
    bool found = true;
    for(k =0; k < courseList.size(); k++){
        if(courseList[k].departmentCode == department && courseList[k].courseNumber == courseNum){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    //Print Book info for books that match the department and course number
    for(int i = 0; i < courseList.size(); i++){
        if(courseList[i].departmentCode == department && courseList[i].courseNumber == courseNum){
            for(int j = 0; j < courseList[i].courseBookList.size(); j++){
                printBookInfo(courseList[i].courseBookList[j].second->isbn);
            }
        }
    }
    return found;
    
}

bool database::printSectionBooks(string department, int courseNum, int sectionNum){
    //check that department, course number, and section number
    int k;
    bool found = true;
    for(k =0; k < courseList.size(); k++){
        if(courseList[k].departmentCode == department && courseList[k].courseNumber == courseNum && courseList[k].sectionNumber == sectionNum){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    //Print all books that match the department, course number, and section number
    for(int i = 0; i < courseList.size(); i++){
        if(courseList[i].departmentCode == department && courseList[i].courseNumber == courseNum && courseList[i].sectionNumber == sectionNum){
            for(int j = 0; j < courseList[i].courseBookList.size(); j++){
                printBookInfo(courseList[i].courseBookList[j].second->isbn);
            }
        }
    }
    
    return found;
    
}

bool database::printBookInfo(long long int bookIsbn){
    //verify that book exist by iterating through Booklist
    int i;
    bool found = true;
    for(i =0; i < bookList.size(); i++){
        if(bookList[i].isbn == bookIsbn){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    //print book attributes
    cout << "TITLE: " << bookList[i].title << endl;
    
    if(bookList[i].author.size() > 0){
        cout << "AUTHOR: " << bookList[i].author << endl;
    }
    
    if(bookList[i].edition > 0){
        cout << "EDITION: " << bookList[i].edition << endl;
    }
    
    if(bookList[i].datePub.size() > 0){
        cout << "DATE PUBLISHED: " << bookList[i].datePub << endl;
    }
    
    cout << "ISBN: " << bookList[i].isbn << endl;
    
    if(bookList[i].bookOptions.size() > 0){
        cout << "PURCHASING OPTIONS: " << endl;
        for(int j = 0; j < bookList[i].bookOptions.size(); j++ ){
            cout << bookList[i].bookOptions[j].first << " " << bookList[i].bookOptions[j].second << endl;
        }
    }
    
    return found;
}

void database::printBookList(){
    //Iterate entire book list and print each title
    for(int i = 0; i < bookList.size(); i++){
        cout << bookList[i].title << endl;
    }
}

void database::printCourseList(){
    //Iterate entire course list and print each course name
    for(int i = 0; i < courseList.size(); i++){
        cout << courseList[i].departmentCode << " " << courseList[i].courseNumber << "-" << courseList[i].sectionNumber << " " << courseList[i].courseName << endl;
    }
}

void database::printPubDate(string datePub){
    //Iterate entire book list and print only titles that match datePub
    for(int i = 0; i < bookList.size(); i++){
        if(bookList[i].datePub == datePub){
            cout << bookList[i].title << endl;
        }
    }
}

void database::printDeptBooks(string department){
    //Iterate course list and print each course's book-list (within given department)
    for(int i = 0; i < courseList.size(); i++){
        if(courseList[i].departmentCode == department){
            for(int j = 0; j < courseList[i].courseBookList.size(); j++){
                cout <<courseList[i].courseBookList[j].second->title << endl;
            }
        }
    }
}

void database::printDeptAverageMin(string department){
    //average min - only consider the required books in calculation
    int minTotal = 0;
    double minCost = 0;
    
    for(int i = 0; i < courseList.size(); i++){
        if(courseList[i].departmentCode == department){
            for(int j = 0; j < courseList[i].courseBookList.size(); j++){
                double minimumCostVersion = 100000;
                if(courseList[i].courseBookList[j].first){
                    minTotal++;
                    for(int k = 0; k < (courseList[i].courseBookList[j].second)->bookOptions.size(); k++){
                        if((courseList[i].courseBookList[j].second)->bookOptions[k].first < minimumCostVersion){
                            minimumCostVersion = (courseList[i].courseBookList[j].second)->bookOptions[k].first;
                        }
                    }
                    minCost+=minimumCostVersion;
                }
            }
        }
    }
    cout << "AVERAGE MINIMUM FOR "<< department << " DEPARTMENT: " << minCost/minTotal << endl;
    
}

void database::printDeptAverageMax(string department){
    //average max - consider all books (required and optional) within calculations
    int maxTotal = 0;
    double maxCost = 0;
    
    for(int i = 0; i < courseList.size(); i++){
        if(courseList[i].departmentCode == department){
            for(int j = 0; j < courseList[i].courseBookList.size(); j++){
                double maximumCostVersion = 0;
                maxTotal++;
                for(int k = 0; k < (courseList[i].courseBookList[j].second)->bookOptions.size(); k++){
                    if((courseList[i].courseBookList[j].second)->bookOptions[k].first > maximumCostVersion){
                        maximumCostVersion = (courseList[i].courseBookList[j].second)->bookOptions[k].first;
                    }
                }
                maxCost+=maximumCostVersion;
            }
        }
    }
    cout << "AVERAGE MAXIMUM FOR "<< department << " DEPARTMENT: " << maxCost/maxTotal << endl;
    
}
/* EXTRA FUNCTIONS -------------------------------------------------------------*/
bool defineCost(long long int bookIsbn, double bookCost, char bookCondition, database &storage){
    //verify that book exist
    int i;
    bool found = true;
    for(i =0; i < storage.bookList.size(); i++){
        if(storage.bookList[i].isbn == bookIsbn){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    //validate book condition flag
    if(bookCondition != 'N' && bookCondition != 'U' && bookCondition != 'R' && bookCondition != 'E' ){
        return false;
    }
    //Add new book option (cost mapped with book Condition)
    pair<double, char> newOption;
    newOption.first = bookCost;
    newOption.second = bookCondition;
    storage.bookList[i].bookOptions.push_back(newOption);
    
    return true;
}

bool defineCharacteristics(long long int bookIsbn, char flag, void* value, database &storage){
    //Iterate through booklist and verify that book exist
    int i;
    bool found = true;
    for(i =0; i < storage.bookList.size(); i++){
        if(storage.bookList[i].isbn == bookIsbn){
            found = true;
            break;
        } else {
            found =  false;
        }
    }
    
    if(found == false){
        return found;
    }
    
    //Check which flag is passed in
    if(flag == 'A'){
        storage.bookList[i].author  = *((string*) value);
        
    } else if(flag == 'E'){
        if(isdigit(*((char*) value)) || (*(int*) value) <= 0){
            cout << "Edition is incorrect." << endl;
            return false;
        }
        storage.bookList[i].edition = *((int*) value);
        
    } else {
        //validate the date by iterating through
        int dateValidator = 1;
        for(int j = 0; j < (*(string*) value).size(); j++){
            if(!(isdigit((int)(*(string*) value)[j])) && !(dateValidator == 3 && (*(string*) value)[j] == '/')){
                break;
            }
            dateValidator++;
        }
        //validate the date by string size
        if((*(string*) value).size() != 7){
            cout << "Date not formatted corrected." << endl;
            return false;
        }
        
        storage.bookList[i].datePub = (*(string*) value);
    }
    
    return true;
}

/* MAIN -------------------------------------------------------------*/
int main(int argc, char * argv[]){
    
    database storage;
    
    while(true){
        string str;
        getline(cin, str);
        
        switch(str[0]){
            case 'B':
            {
                //parse and validate ISBN and title
                long long int isbn;
                string tempIsbn = "";
                string title = "";
                int index = 0;
                
                for(index = 2; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempIsbn+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempIsbn.size() != 13){
                    cout << "ISBN is too short." << endl;
                    break;
                }
                isbn = stoll(tempIsbn);
                for(index; index < str.size(); index++){
                    title+=str[index];
                }
                Books newBook(isbn, title, storage);
            }
                break;
            case 'D':
            {
                //parse and validate ISBN, characteristic flag, and values to be passed in
                long long int isbn;
                string tempIsbn = "";
                char characteristic = ' ';
                string value = "";
                int index = 0;
                
                for(index = 2; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempIsbn+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempIsbn.size() != 13){
                    cout << "ISBN is too short/long." << endl;
                    break;
                }
                isbn = stoll(tempIsbn);
                characteristic = str[index];
                index = index + 2;
                for(index; index < str.size(); index++){
                    value+=str[index];
                }
                //check if function has success, if not print error
                bool valid = defineCharacteristics(isbn, characteristic, &value, storage);
                if(!valid){
                    cout << "Defining error. "<< endl;
                }
            }
                break;
            case 'M':
            {
                //parse and validate ISBN, book cost, and book condition to be passed in
                long long int isbn;
                string tempIsbn = "";
                string tempCost;
                char condition = ' ';
                double bookCost;
                int decimalCounter = 0;
                bool afterDecimal = false;
                int index = 0;
                
                for(index = 2; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempIsbn+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempIsbn.size() != 13){
                    cout << "ISBN is too short." << endl;
                    break;
                }
                isbn = stoll(tempIsbn);
                for(index; index < str.size(); index++){
                    if(decimalCounter == 2) {
                        break;
                    }
                    if(afterDecimal){
                        decimalCounter++;
                    }
                    if(str[index] == '.'){
                        afterDecimal = true;
                    }
                    tempCost+=str[index];
                }
                bookCost = stod(tempCost);
                for(index; index < str.size(); index++){
                    condition = str[index];
                }
                //check if function has success, if not print error
                bool valid = defineCost(isbn,bookCost,condition, storage);
                if(!valid){
                    cout << "Defining cost error. "<< endl;
                }
            }
                break;
            case 'C':
            {
                //parse and validate department code, course number, and course name to be passed in
                string departmentCode = "";
                int courseNum;
                string tempCourseNum = "";
                string courseName = "";
                int index = 0;
                
                for(index = 2; index < str.size(); index++){
                    if(str[index] != ' '){
                        departmentCode+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(departmentCode.size() != 4){
                    cout << "Department code is too short/long." << endl;
                    break;
                }
                for(index; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempCourseNum+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempCourseNum.size() != 3){
                    cout << "Course number error" << endl;
                    break;
                }
                courseNum = stoi(tempCourseNum);
                for(index; index < str.size(); index++){
                    courseName+=str[index];
                }
                
                classSection newClass(departmentCode, courseNum, courseName, storage, true);
            }
                break;
            case 'A':
            {
                //parse and validate ISBN, department code, course number, and section number to be passed in
                long long int isbn;
                string tempIsbn = "";
                string departmentCode = "";
                int courseNum;
                string tempCourseNum = "";
                int sectionNum;
                string tempSectionNum = "";
                int index = 0;
                char required = ' ';
                
                for(index = 2; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempIsbn+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempIsbn.size() != 13){
                    cout << "ISBN is too short." << endl;
                    break;
                }
                isbn = stoll(tempIsbn);
                for(index; index < str.size(); index++){
                    if(str[index] != ' '){
                        departmentCode+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(departmentCode.size() != 4){
                    cout << "Department code is too short/long." << endl;
                    break;
                }
                
                for(index; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempCourseNum+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempCourseNum.size() != 3){
                    cout << "Course number error" << endl;
                    break;
                }
                courseNum = stoi(tempCourseNum);
                
                for(index; index < str.size(); index++){
                    if(str[index] != ' '){
                        tempSectionNum+=str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                if(tempSectionNum.size() != 3){
                    cout << "Section number error" << endl;
                    break;
                }
                sectionNum = stoi(tempSectionNum);
                
                for(index; index < str.size(); index++){
                    if(str[index] != ' '){
                        required = str[index];
                    } else{
                        index++;
                        break;
                    }
                }
                //check if function has success, if not print error
                bool valid = storage.assignBook(isbn, departmentCode, courseNum, sectionNum, required, true);
                if(!valid){
                    cout << "Assignment Error " << endl;
                }
                
                
            }
                break;
            case 'G':
                switch (str[1]) {
                    case 'C':
                    {
                        //parse and validate department code and course number to be passed in
                        string departmentCode = "";
                        int courseNum;
                        string tempCourseNum = "";
                        
                        int index = 0;
                        
                        for(index = 3; index < str.size(); index++){
                            if(str[index] != ' '){
                                departmentCode+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(departmentCode.size() != 4){
                            cout << "Department code is too short/long." << endl;
                            break;
                        }
                        for(index; index < str.size(); index++){
                            if(str[index] != ' '){
                                tempCourseNum+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(tempCourseNum.size() != 3){
                            cout << "Course number error" << endl;
                            break;
                        }
                        courseNum = stoi(tempCourseNum);
                        
                        cout << "PRINTING BOOKS FOR " << departmentCode << " " << courseNum << endl;
                        storage.printCourseBooks(departmentCode, courseNum);
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'S':
                    {
                        //parse and validate department code, course number, and section number to be passed in
                        string departmentCode = "";
                        int courseNum;
                        string tempCourseNum = "";
                        string tempSectionNum = "";
                        int sectionNum;
                        
                        int index = 0;
                        
                        for(index = 3; index < str.size(); index++){
                            if(str[index] != ' '){
                                departmentCode+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(departmentCode.size() != 4){
                            cout << "Department code is too short/long." << endl;
                            break;
                        }
                        for(index; index < str.size(); index++){
                            if(str[index] != ' '){
                                tempCourseNum+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(tempCourseNum.size() != 3){
                            cout << "Course number error" << endl;
                            break;
                        }
                        courseNum = stoi(tempCourseNum);
                        
                        for(index; index < str.size(); index++){
                            if(str[index] != ' '){
                                tempSectionNum+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(tempSectionNum.size() != 3){
                            cout << "Section number error" << endl;
                            break;
                        }
                        sectionNum = stoi(tempSectionNum);
                        cout << "PRINTING BOOKS FOR " << departmentCode << " " << courseNum << " " << sectionNum << endl;
                        storage.printSectionBooks(departmentCode, courseNum, sectionNum);
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'B':
                    {
                        //parse and validate isbn to be passed in
                        long long int isbn;
                        string tempIsbn = "";
                        int index = 0;
                        
                        for(index = 3; index < str.size(); index++){
                            if(str[index] != ' '){
                                tempIsbn+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(tempIsbn.size() != 13){
                            cout << "ISBN is too short." << endl;
                            break;
                        }
                        isbn = stoll(tempIsbn);
                        cout << "PRINTING INFO FOR: " << isbn << endl;
                        storage.printBookInfo(isbn);
                        cout << "----------------------------------------------------------------------" << endl;
                        
                    }
                        break;
                    default:
                        cout << "Command does not exist." << endl;
                        break;
                }
                break;
            case 'P':
                switch (str[1]) {
                    case 'B':
                    {
                        cout << "PRINTING ALL BOOKS: " << endl;
                        storage.printBookList();
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'C':
                    {
                        cout << "PRINTING ALL COURSES: " << endl;
                        storage.printCourseList();
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'Y':
                    {
                        //parse and validate date to be passed in
                        int index = 0;
                        string date = "";
                        for(index = 3; index < str.size(); index++){
                            date+=str[index];
                        }
                        
                        int dateValidator = 1;
                        for(int j = 0; j < date.size(); j++){
                            if(!(isdigit((int)(date)[j])) && !(dateValidator == 3 && (date)[j] == '/')){
                                break;
                            }
                            dateValidator++;
                        }
                        
                        if(date.size() != 7 && dateValidator != 3){
                            cout << "Date not formatted corrected." << endl;
                            break;
                        }
                        cout << "PRINTING BOOKS PUBLISHED ON: " << date << endl;
                        storage.printPubDate(date);
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'D':
                    {
                        //parse and validate department code to be passed in
                        string departmentCode = "";
                        int index = 0;
                        
                        for(index = 3; index < str.size(); index++){
                            if(str[index] != ' '){
                                departmentCode+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(departmentCode.size() != 4){
                            cout << "Department code is too short/long." << endl;
                            break;
                        }
                        cout << "PRINTING ALL BOOKS IN THE " << departmentCode << " DEPARTMENT:" <<  endl;
                        storage.printDeptBooks(departmentCode);
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    case 'M':
                    {
                        //parse and validate department code to be passed in
                        string departmentCode = "";
                        int index = 0;
                        
                        for(index = 3; index < str.size(); index++){
                            if(str[index] != ' '){
                                departmentCode+=str[index];
                            } else{
                                index++;
                                break;
                            }
                        }
                        if(departmentCode.size() != 4){
                            cout << "Department code is too short/long." << endl;
                            break;
                        }
                        
                        storage.printDeptAverageMin(departmentCode);
                        storage.printDeptAverageMax(departmentCode);
                        cout << "----------------------------------------------------------------------" << endl;
                    }
                        break;
                    default:
                        cout << "Command does not exist." << endl;
                        break;
                }
                break;
                
            default:
                cout << "Command does not exist." << endl;
                break;
        }
    }
    return 0;
}
