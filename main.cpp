/*main.cpp*/

//
// <Kirun Haque>
// U. of Illinois, Chicago
// Fall 2019
// Description: GradeUtil UI (Console Application)
// This program is C++11 dependent
//

#include <iostream>
#include "gradeutil.h"
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdlib>


using namespace std;

//
// input(filename):
//
// Inputs student data from the file and returns a map of Student objects.
//
map<string,Dept> input(string filename, string &college,string &season, int &semester ) {
    ifstream  file(filename);
    string    line, year;//, college, season, year;
    
    map<string ,Dept> depts;
    
    if (!file.good())
    {
        cout << "**Error: cannot open input file!" << endl;
        return depts;
    }
    getline(file, college, ',');
    getline(file, season, ',');
    getline(file, year, ',');
    semester = stoi(year);

    getline(file, line);  // skip header row:
    //
    // for each line of data, create a student and push into the vector:
    //
    while (getline(file, line))
    {
        Course c = ParseCourse(line);
        //add department if it does not already exist
        if(depts.find(c.Dept) == depts.end()) {
            Dept d(c.Dept);
            depts[c.Dept] = d;
            depts[c.Dept].addCourse(c);
         }
        else{
            //add course to the department
            depts[c.Dept].addCourse(c);
        }
    }
    return depts;
}


//a function that will insert departments into a college
College getCollege(map<string, Dept> depts){
    College college;
    for(auto &keyvaluepair: depts){
        college.addDepartment(keyvaluepair.second);
    }
    return college;
}


//function to set the college break down in the beginning of the program
void getCollegeInfo(College college){
    cout << "# of courses taught: " << college.NumCourses() << endl;
    cout << "# of students taught: " << college.NumStudents() << endl;
    GradeStats collegeGradeStats = GetGradeDistribution(college);
    cout << "grade distribution (A-F): " << collegeGradeStats.PercentA << "%, " << collegeGradeStats.PercentB << "%, "<< collegeGradeStats.PercentC << "%, ";
    cout << collegeGradeStats.PercentD <<"%, " << collegeGradeStats.PercentF <<"%" << endl;
    int DFW, N;
    cout <<"DFW rate: " << GetDFWRate(college, DFW, N)<< "%" << endl << endl;
}


//A function to display department information
void getDeptInfo(Dept d){
    cout << d.Name << ":" << endl;
    cout << " # courses taught: " << d.NumCourses() << endl;
    cout << " # students taught: " << d.NumStudents() << endl;
    GradeStats collegeGradeStats = GetGradeDistribution(d);
    cout << " grade distribution (A-F): " << collegeGradeStats.PercentA << "%, " << collegeGradeStats.PercentB << "%, "<< collegeGradeStats.PercentC << "%, ";
    cout << collegeGradeStats.PercentD <<"%, " << collegeGradeStats.PercentF <<"%" << endl;
    int DFW, N;
    cout <<" DFW rate: " << GetDFWRate(d, DFW, N)<< "%" << endl;
}


//get summary for each department
void getSummary(College college, map<string, Dept> deptMap){
    string input;
    cout << "dept name, or all? ";
    cin >> input;
    vector<string> depts;
    if(input == "all"){
        for(auto &keyvaluepair: deptMap){
            depts.push_back(keyvaluepair.first);  //insert each department in a vector
        }
        //sort the vector by department
        std::sort(depts.begin(),depts.end(), [](const string dept1, const string dept2) {
            if(dept1 < dept2){
                return true;
            }
            else{
                return false;
            }
        });
        //traverse the sorted depts vector
        for(const auto &d: depts){
            getDeptInfo(deptMap[d]); //display summary of each department
        }
    }
    else if(deptMap.find(input) != deptMap.end()){
        getDeptInfo(deptMap[input]); //if input is in deptMap display value
    }
    else{
        cout << "**dept not found" <<endl;
    }
}


//display course information for whatever FindCourse function
void getCourseInfo(vector<Course> courses){
    if (courses.empty()){
        cout << "**none found" << endl;
        return;
    }
    string gradeType[] ={"letter","satisfactory","unknown"};
    //traverse courses vector to display the information for each course
    for(const Course &c: courses){
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: " << gradeType[c.getGradingType()] << endl;
        GradeStats collegeGradeStats = GetGradeDistribution(c);
        cout << " grade distribution (A-F): " << collegeGradeStats.PercentA << "%, " << collegeGradeStats.PercentB << "%, "<< collegeGradeStats.PercentC << "%, ";
        cout << collegeGradeStats.PercentD <<"%, " << collegeGradeStats.PercentF <<"%" << endl;
        int DFW, N;
        cout <<" DFW rate: " << GetDFWRate(c, DFW, N)<< "%" << endl;
    }
}


//a function that will display search information for either college or department
void getSearch(College college, map<string, Dept> deptMap){
    string input;
    cout << "dept name, or all? ";
    cin >> input;
    
    string instructorPrefix;
    int courseNum;
    cout << "course # or instructor prefix? ";
    cin >> instructorPrefix;
    
    if(input == "all"){ //if all find courses for the whole college
        stringstream ss(instructorPrefix);
        ss >> courseNum;  //convert to integer
    
        if(ss.fail()){ //if conversion fails then call FindCoursed for instructor prefix
            vector<Course> courses = FindCourses(college, instructorPrefix);
            getCourseInfo(courses);
        }
        else{ //Get find courses for course number
            vector<Course> courses = FindCourses(college, courseNum);
            getCourseInfo(courses);
        }
    }
    //check if the department in the map
    else if(deptMap.find(input) != deptMap.end()){
        stringstream ss(instructorPrefix);
        ss >> courseNum;
        
        if(ss.fail()){
            //get courses with the same prefix
            vector<Course> courses = FindCourses(deptMap[input], instructorPrefix);
            if (courses.empty()){
                cout << "**none found" << endl;
                return;
            }
            getCourseInfo(courses);
        }
        else{
            vector<Course> courses = FindCourses(deptMap[input], courseNum);//get courses with the same #
            if (courses.empty()){
                cout << "**none found" << endl;
                return;
            }
            getCourseInfo(courses);
        }
    }
    //if input is not "all" or found in the map, the department was not found
    else{
        cout << "**dept not found" << endl;
    }
}


void displaySatisfactory(vector<Course> courses){
    //Sort satisfactory grades
    if(courses.empty()){
        cout << "**none found" <<endl;
        return;
    }
    std::sort(courses.begin(), courses.end(), [](const Course c1, const Course c2 ){
        if(c1.Dept < c2.Dept){
            return true;
        }
        else if(c1.Dept > c2.Dept){
            return false;
        }
        else{ //if same department then sort by course number
            if(c1.Number < c2.Number){
                return true;
            }
            else if (c1.Number > c2.Number){
                return false;
            }
            else{ //if same course number then sort by section
                if( c1.Section < c2.Section){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
    });
    
    string gradeType[] ={"letter","satisfactory","unknown"};
    for(const Course &c: courses){
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: " << gradeType[c.getGradingType()] << endl;
    }
}


void getSatisfactory(College college, map<string, Dept> deptMap){
    string input;
    cout << "dept name, or all? ";
    cin >> input;
    
    if(input == "all"){ //if all find courses for the whole college
        vector<Course> courses;
        for(const Dept &d: college.Depts){
            for(const Course &c: d.Courses){
                //if grade type is satisfactory, push into the vevtor
                if(c.getGradingType() == Course::Satisfactory){
                    courses.push_back(c);
                }
            }
        }
        displaySatisfactory(courses);
    }
    //check if department exists
    else if(deptMap.find(input) != deptMap.end()){
        vector<Course> courses;
        for(const Course &c: deptMap[input].Courses){
            //if grade type is satisfactory, push into the vevtor
            if(c.getGradingType() == 1){
                courses.push_back(c);
            }
        }
        displaySatisfactory(courses);
    }
    else{
        cout << "**dept not found" << endl;
    }
}


//function that will DFWRate for each course
void displayDFW(vector<Course> courses){
    if(courses.empty()){
        cout << "**none found" << endl;
        return;
    }
    //Sort satisfactory grades
    std::sort(courses.begin(), courses.end(), [](const Course c1, const Course c2 ){
        int DFW,N;
        double c1DFW = GetDFWRate(c1, DFW, N);
        double c2DFW = GetDFWRate(c2, DFW, N);
        
        if(c1DFW > c2DFW){
            return true;
        }
        else if(c1DFW < c2DFW){
            return false;
        }
        else if (fabs(c1DFW-c2DFW) < .001){ //if same rate then sort by dept
            if(c1.Dept < c2.Dept){
                return true;
            }
            else if (c1.Dept > c2.Dept){
                return false;
            }
            else{
                if(c1.Number < c2.Number){ //if same dept then sort by number
                    return true;
                }
                else if(c1.Number > c2.Number){
                    return false;
                }
                else{
                    if(c1.Section < c2.Section){ //if same number then sort by section
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        else{
            return false;
        }
    });
    
    string gradeType[] ={"letter","satisfactory","unknown"}; //use array to display grading type
    int DFW, N;
    
    //display each course DFWRate information
    for(const Course &c: courses){
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: " << gradeType[c.getGradingType()] << endl;
        GradeStats courseStats = GetGradeDistribution(c);
        cout << " grade distribution (A-F): " << courseStats.PercentA << "%, " << courseStats.PercentB << "%, "<< courseStats.PercentC << "%, ";
        cout << courseStats.PercentD <<"%, " << courseStats.PercentF <<"%" << endl;
        cout <<" DFW rate: " << GetDFWRate(c, DFW, N)<< "%" << endl;
    }
}


//a function that takes in College and map<string, Dept> parameter and will display DFWRate information
//for college for department
void getDFW(College college, map<string, Dept> deptMap){
    string input;
    string thresholdInput;
    cout << "dept name, or all? ";
    cin >> input;
    cout<< "dfw threshold? ";
    cin>>thresholdInput;
    
    int DFW, N;
    double threshold;
    if(input == "all"){ //if all find courses for the whole college
    
        threshold = atof(thresholdInput.c_str());
        vector<Course> courses;
        //traverse department and go through each course to check if it exceeds the threshold
        for(const Dept &d: college.Depts){
            for(const Course &c: d.Courses){
                if(GetDFWRate(c, DFW, N) > threshold){
                    courses.push_back(c);
                }
            }
        }
        displayDFW(courses);
    }
    //check for department
    else if(deptMap.find(input) != deptMap.end()){
        threshold = atof(thresholdInput.c_str());
        vector<Course> courses;
        //traverse department's courses and go through each course to check if it exceeds the threshold
        for(const Course &c: deptMap[input].Courses){
            if(GetDFWRate(c, DFW, N) > threshold){
                courses.push_back(c);
            }
        }
        displayDFW(courses);
    }
    else{
        cout << "**dept not found" << endl;
    }
}


//A function that takes in vector<course> as a paremeter and
//will display what grade letter B data in sorted order
void displayLetterB(vector<Course> courses){
    //Sort satisfactory grades
    if(courses.empty()){
        cout << "**none found" << endl;
        return;
    }
    std::sort(courses.begin(), courses.end(), [](const Course c1, const Course c2 ){
        GradeStats c1CourseStat = GetGradeDistribution(c1);
        GradeStats c2CourseStat = GetGradeDistribution(c2);

        if(c1CourseStat.PercentB > c2CourseStat.PercentB){
            return true;
        }
        else if(c1CourseStat.PercentB < c2CourseStat.PercentB){
            return false;
        }
        else if (fabs(c1CourseStat.PercentB-c2CourseStat.PercentB) < .001){ //if same department then sort by course number
            if(c1.Dept < c2.Dept){
                return true;
            }
            else if (c1.Dept > c2.Dept){
                return false;
            }
            else{                   //if same dept, sort by course number
                if(c1.Number < c1.Number){
                    return true;
                }
                else if(c1.Number > c2.Number){
                    return false;
                }
                else{
                    if(c1.Section < c2.Section){    //if same course number sort by section
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
        }
        else{
            return false;
        }
    });
    
    string gradeType[] ={"letter","satisfactory","unknown"};
    int DFW, N;
    for(const Course &c: courses){
        cout << c.Dept << " " << c.Number << " (section " << c.Section << "): " << c.Instructor << endl;
        cout << " # students: " << c.getNumStudents() << endl;
        cout << " course type: " << gradeType[c.getGradingType()] << endl;
        GradeStats courseStats = GetGradeDistribution(c);
        cout << " grade distribution (A-F): " << courseStats.PercentA << "%, " << courseStats.PercentB << "%, "<< courseStats.PercentC << "%, ";
        cout << courseStats.PercentD <<"%, " << courseStats.PercentF <<"%" << endl;
        cout <<" DFW rate: " << GetDFWRate(c, DFW, N)<< "%" << endl;
    }
}


void getLetterB(College college,map<string, Dept> deptMap){
    string input;
    string thresholdInput;
    cout << "dept name, or all? ";
    cin >> input;
    
    cout<< "letter B threshold? ";
    cin>>thresholdInput;
    
    double threshold;
    if(input == "all"){ //if all find courses for the whole college
        threshold = atof(thresholdInput.c_str());
        vector<Course> courses;
        //traverse the every course in each department to check which course exceeded the threshold
        for(const Dept &d: college.Depts){
            for(const Course &c: d.Courses){
                GradeStats courseStat = GetGradeDistribution(c);
                if(courseStat.PercentB > threshold){  //check if the letterB data exceeds threshold
                    courses.push_back(c);
                }
            }
        }
        displayLetterB(courses);
    }
    //check for department
    else if(deptMap.find(input) != deptMap.end()){
        threshold = atof(thresholdInput.c_str());
        vector<Course> courses;
        for(const Course &c: deptMap[input].Courses){
            GradeStats courseStat = GetGradeDistribution(c);
            if(courseStat.PercentB > threshold){
                courses.push_back(c);
            }
        }
        displayLetterB(courses);
    }
    else{
        cout <<"**dept not found" << endl;
    }
}


//A function that will return a course's GPA average
double getCourseGPA(Course c){
    int num;
    double avg = 0.0;
    int students =0;
    if(c.getGradingType() == Course::Letter){
          num = c.NumA*4 + c.NumB*3 +c.NumC*2 +c.NumD*1 + c.NumF*0;
          students = c.NumA + c.NumB +c.NumC +c.NumD + c.NumF;
          return avg += num/double(students);
    }
    return 0.0;
}


//A function that will return a the GPA average for a department
double getDeptGPA(Dept d){
    double avg = 0.0;
    int numCourses = 0;
    for(const Course &c: d.Courses){
      if(c.getGradingType() == Course::Letter){
          avg += getCourseGPA(c);
          numCourses++;
      }
    }
    if(numCourses != 0){
        return (avg/double(numCourses));
    }
    else{
        return 0.0;
    }
}


//A function with Dept paraemter that will display the it's course's GPA
void displayAverageCourses(Dept d){
    std::sort(d.Courses.begin(),d.Courses.end(), [](const Course& c1,const Course& c2){
        double c1GPA = getCourseGPA(c1);
        double c2GPA = getCourseGPA(c2);
        if(c1GPA > c2GPA){   //sort by GPA
            return true;
        }
        else if(c1GPA < c2GPA){
            return false;
        }
        else if(fabs(c1GPA-c2GPA) < .001){ //GPA is same, then sort by number, section
            if(c1.Number < c2.Number){
                return true;
            }
            else if(c1.Number > c2.Number){
                return false;
            }
            else{
                if(c1.Section < c2.Section){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
        else{
            return false;
        }
    });
    
    for(const Course &c: d.Courses){
        //display all course's GPA
        if(c.getGradingType() == Course::Letter){
            cout << "Overall GPA for " << c.Dept << " " << c.Number << "(" << c.Section << ") : "<< getCourseGPA(c) << endl;
        }
    }
}


//A function that takes vector<Dept> department which displays the the department's GPA
void displayAverageDept(vector<Dept> departments){
    std::sort(departments.begin(),departments.end(), [](const Dept& d1,const Dept& d2){
        double d1GPA = getDeptGPA(d1); //sort by GPA
        double d2GPA = getDeptGPA(d2);
        if(d1GPA > d2GPA){ //if same GPA, then sort by department name
            return true;
        }
        else{
            return false;
        }
    });
    
    for(const Dept &d: departments){
        if(!d.Courses.empty()){
            cout << "Overall GPA for " << d.Name << " : " << getDeptGPA(d) << endl;
        }

    }
}

//A function that takes College parameter and map<string, Dept> to retrieve the College's GPA average
void getAverage(College college, map<string, Dept> deptMap){
    string input;
    cout << "dept name, or all? ";
    cin >> input;
    
    if(input == "all"){ //if all find courses for the whole college
        displayAverageDept(college.Depts);
    }
    //check for department
    else if(deptMap.find(input) != deptMap.end()){
        displayAverageCourses(deptMap[input]);
    }
    else{
        cout << "**dept not found" << endl;
    }
}


int main(int argc, const char * argv[]) {
    cout <<std::fixed;
    cout <<std::setprecision(2);
    std::string filename = " ";
    cin >> filename;
   // filename = "fall-2018.csv";
    string collegeName, season;
    int year;
    auto deptMap = input(filename, collegeName, season, year);
    College college = getCollege(deptMap);
    college.Name = collegeName;
    college.Semester = season;
    college.Year = year;
    cout << "** College of " << college.Name << ", " << college.Semester << " " << college.Year << " **" << endl;
    getCollegeInfo(college);

    string command;
    cout << "Enter a command> ";
    cin >> command;
    while(true){
        if(command == "summary"){
            getSummary(college, deptMap);
        }
        else if(command == "search"){
            getSearch(college, deptMap);
        }
        else if(command == "satisfactory"){
            getSatisfactory(college, deptMap);
        }
        else if(command == "dfw"){
            getDFW(college,deptMap);
        }
        else if(command == "letterB"){
            getLetterB(college, deptMap);
        }
        else if(command == "average"){
            getAverage(college, deptMap);
        }
        else if(command == "#"){
            return 0;
        }
        else{
            cout << "**unknown command" << endl;
        }
        cout << "Enter a command> ";
        cin >> command;
    }
    return 0;
}
