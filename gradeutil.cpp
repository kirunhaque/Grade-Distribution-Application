/*gradeutil.cpp*/

//
// <<Kirun Haque>>
// U. of Illinois, Chicago
// Fall 2019
// Description: GradeUtil API
// 

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "gradeutil.h"
#include "catch.hpp"

#include <iostream>

using namespace std;
void Dept::addCourse(Course course)
{
	// Add course to Courses
	this->Courses.push_back(course);

}

void College::addDepartment(Dept department)
{
	// Add department to Depts.
    this->Depts.push_back(department);

}




//
// API:
//

// 
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior 
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{

  //
  // TODO:
  //
    if(csvline == ""){
        return Course();
    }
    string Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor;
    //Parse through cvsline string
    std::string courseline = csvline;
    Course c;
    int n = std::count(csvline.begin(),csvline.end(), ',');
    if(n == 14){
        stringstream ss(csvline);
        getline(ss, Dept, ',');
        getline(ss, Number, ',');
        getline(ss, Section, ',');
        getline(ss, Title, ',');
        getline(ss, A, ',');
        getline(ss, B, ',');
        getline(ss, C, ',');
        getline(ss, D, ',');
        getline(ss, F, ',');
        getline(ss, I, ',');
        getline(ss, NR, ',');
        getline(ss, S, ',');
        getline(ss, U, ',');
        getline(ss, W, ',');
        getline(ss, Instructor, ',');
    }
    c.Dept = Dept;
    c.Title = Title;
    c.Number = stoi(Number);
    c.Section = stoi(Section);
    c.NumA = stoi(A);
    c.NumB = stoi(B);
    c.NumC = stoi(C);
    c.NumD = stoi(D);
    c.NumF = stoi(F);
    c.NumI = stoi(I);
    c.NumS = stoi(S);
    c.NumU = stoi(U);
    c.NumW = stoi(W);
    c.NumNR = stoi(NR);
    c.Instructor = Instructor;
    return c;
}


//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course, 
// department, or college.  For a course whose grading type 
// is defined as Course::Letter, the DFW rate is defined as
// 
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.  
//
// When computed for a dept or college, all courses of type 
// Course::Letter are considered in computing an overall DFW 
// rate for the dept or college.  The reference parameters 
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
  //
  // TODO:
    if(c.getGradingType() == 0){
        DFW = c.NumD+c.NumF+c.NumW;
        N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW; 
        if(N != 0){ 
            return (DFW/double(N)*100);
        }
    }
    else{
        return 0.0;
    }
    return 0.0;
}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
    int cDFW = 0;
    int cN = 0;
  //
  // TODO:
    if(dept.Courses.empty()){
        return 0.0;
    }
    else{
        for(const Course& c: dept.Courses){ //for each course in the department get the DFWRate, DFW, N
            GetDFWRate(c, cDFW, cN);
            DFW += cDFW;
            N += cN;
        }
        if(N != 0){ //check if not 0 because departments can possibly be empty.
            return (DFW/double(N)*100);
        }
    }
    return 0.0;
}

double GetDFWRate(const College& college, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  
   //
  // TODO:
    int dDFW = 0;
    int dN = 0;
    if(college.Depts.empty()){ //check if college is empty
        return 0.0;
    }
    else{
        for(const Dept& d: college.Depts){ //traverse through each department and retrieve DFW and N for each department
            GetDFWRate(d, dDFW, dN);
            DFW += dDFW;
            N += dN;
        }
        if(N != 0){ //check if not 0 because departments can possibly be empty.
            return (DFW/double(N)*100);
        }
    }
    return 0.0;
}  


//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B, 
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall 
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c)
{
  
   //
  // TODO:
    if(c.getGradingType() == 0){
        int N = c.NumA+c.NumB+c.NumC+c.NumD+c.NumF; //get number of grades assigned
        double pA = (c.NumA/double(N))*100;
        double pB = (c.NumB/double(N))*100;
        double pC = (c.NumC/double(N))*100;
        double pD = (c.NumD/double(N))*100;
        double pF = (c.NumF/double(N))*100;

        return GradeStats(N, c.NumA, c.NumB,c.NumC,c.NumD,c.NumF,pA,pB,pC,pD,pF);
    }
    else{
        return GradeStats();
    }  
}

GradeStats GetGradeDistribution(const Dept& dept)
{
  
  int NumA = 0, NumB = 0, NumC = 0,NumD = 0,NumF = 0;
    int N = 0;
    
  //
  // TODO:
    
    if(dept.Courses.empty()){
        return GradeStats(); 
    }
    else{
        for(Course c: dept.Courses){
            GradeStats gs = GetGradeDistribution(c); //Get grade distribution from each course
            NumA += gs.NumA;
            NumB += gs.NumB;
            NumC += gs.NumC;
            NumD += gs.NumD;
            NumF += gs.NumF;
            N += gs.N;
        }
        if(N != 0){ //Check if N is non zero for empty courses in the department
            double pA = (NumA/double(N))*100;
            double pB = (NumB/double(N))*100;
            double pC = (NumC/double(N))*100;
            double pD = (NumD/double(N))*100;
            double pF = (NumF/double(N))*100;
            return GradeStats(N, NumA, NumB,NumC,NumD,NumF,pA,pB,pC,pD,pF);
        }
    }
  return GradeStats();
}

GradeStats GetGradeDistribution(const College& college)
{
    int NumA = 0, NumB = 0, NumC = 0,NumD = 0,NumF = 0;
    int N = 0;
  //
  // TODO:
    if(college.Depts.empty()){
        return GradeStats();
    }
    else{
        for(const Dept& d: college.Depts){
            GradeStats gs = GetGradeDistribution(d); //Get grade distribution from each department
            NumA += gs.NumA;
            NumB += gs.NumB;
            NumC += gs.NumC;
            NumD += gs.NumD;
            NumF += gs.NumF;
            N += gs.N;
        }
        if(N != 0){ //Check if N is non zero for empty courses in the department
            double pA = (NumA/double(N))*100;
            double pB = (NumB/double(N))*100;
            double pC = (NumC/double(N))*100;
            double pD = (NumD/double(N))*100;
            double pF = (NumF/double(N))*100;
            return GradeStats(N, NumA, NumB,NumC,NumD,NumF,pA,pB,pC,pD,pF);
        }
    }
  //
  
  return GradeStats();
}

//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
  vector<Course>  courses;
  
  //
  // TODO:
    for(const Course& c : dept.Courses){ //push back all of courses with same courseNumber into the vector
        if(c.Number == courseNumber){
            courses.push_back(c);
        }
    }
    if(!courses.empty()){
        //Make an anonymous class that will compare the 2 course's sections
        auto sortSection = [](const Course& c1,const Course& c2){
            if(c1.Section < c2.Section){
                return true;
            }
            else{
                return false;
            }
        };
        sort(courses.begin(), courses.end(),sortSection); //sort by section number
    }
  return courses;
}


//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If 
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in 
// ascending order by course number.  If two courses have the 
// same course number, they are given in ascending order by 
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
  vector<Course>  courses;
    
  
    for(const Course& c : dept.Courses){
        if(c.Instructor != "?"){
            if(!(c.Instructor.size() < instructorPrefix.size())){
                auto isPrefix = std::mismatch(instructorPrefix.begin(), instructorPrefix.end(), c.Instructor.begin()); //
                if (isPrefix.first ==  instructorPrefix.end()){
                    courses.push_back(c);
                };
            }
        }
    }
    
    if(!courses.empty()){
        //Make an anonymous class that will compare the 2 course's sections
        auto sortSection = [](const Course& c1,const Course& c2){
            if(c1.Number < c2.Number){ //sort by course number
                return true;
            }
            else if ( c1.Number > c2.Number){
                return false;
            }
            else{
                if(c1.Section < c2.Section){ //if course number is the same, then sort by section number
                    return true;
                }
                else{
                    return false;
                }
            }
        };
        sort(courses.begin(), courses.end(),sortSection); //sort by course number, section number
    }
   
  return courses;
}


//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber)
{
  vector<Course>  courses;
  
  //
  // TODO:
    for(const Dept& d : college.Depts){ //go through every course in each deparment
        if(!d.Courses.empty()){
            for(const Course& c : d.Courses){
                if(c.Number == courseNumber){
                    courses.push_back(c); //push course with same course number into the courses vector
                }
            }
        }
    }
    if(!courses.empty()){
        //Make an anonymous class that will compare the 2 course's sections
        auto sortSection = [](const Course& c1,const Course& c2){
            if(c1.Dept < c2.Dept){  //sort by departmen
                return true;
            }
            else if (c1.Dept > c2.Dept){
                return false;
            }
            else{  //if department is same, then sort by section number
                if(c1.Section < c2.Section){
                    return true;
                }
                else{
                    return false;
                }
            }
        };
        sort(courses.begin(), courses.end(),sortSection); //sort by department, section number
    }
  //
  return courses;
}


//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department,
// then course number, and then section.
//
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
  vector<Course>  courses;
  
  //
  // TODO:
    for (const Dept& d : college.Depts){
        if(!d.Courses.empty()){
            for(const Course& c : d.Courses){
               if(c.Instructor != "?"){
                   if(!(c.Instructor.size() < instructorPrefix.size())){
                       auto isPrefix = std::mismatch(instructorPrefix.begin(), instructorPrefix.end(), c.Instructor.begin()); //
                       if (isPrefix.first ==  instructorPrefix.end()){                         
                           courses.push_back(c);
                        };
                    }
                }
            }
        }
    }
    
    if(!courses.empty()){
        //Make an anonymous class that will compare the 2 course's sections
        auto sortSection = [](const Course& c1,const Course& c2){
            if(c1.Dept < c2.Dept){  //sort by departmen
                return true;
            }
            else if (c1.Dept > c2.Dept){
                return false;
            }
            else{  //if department is same, then sort by section number
                if(c1.Number < c2.Number){
                    return true;
                }
                else if (c1.Number > c2.Number){ //sort by course number if same department
                    return false;
                }
                else{
                    if(c1.Section < c2.Section){ //sort by section if same course number
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
        };
        sort(courses.begin(), courses.end(),sortSection); //sort by department, course number, section number
    }
  //
  return courses;
    
    
}

