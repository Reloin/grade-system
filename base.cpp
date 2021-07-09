#include <iostream>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sys/stat.h>

using namespace std;


//-----------------------time类，用于记录出勤的日期----------------------------------------------
class date
{
    protected:
        int year, month, day;
    public:
        date(int y = 2000, int m = 1, int d = 1): year(y), month(m), day(d){}
        date(const date &d):year(d.year), month(d.month), day(d.day){}
        string dob()//以yyyy-mm-dd形式显示生日
        {
            char temp[11];
            sprintf(temp, "%d-%d-%d", year, month, day);
            return string(temp);
        }
};


//----------------------------student类，记录学生信息------------------------------------------------------------------
class student : public date
{
    private:
        char name[30];
        char sex;
        char id[11];
    public:
        student(){}
        student(const char *name, const char sex, const char id[11], date bd): date(bd)
        {
            strcpy(this->name, name);
            this->sex = sex;
            strcpy(this->id, id);
        }
        student(const char *name, const char sex, const char id[11], int y, int m, int d): date(y, m, d)
        {
            strcpy(this->name, name);
            this->sex = sex;
            strcpy(this->id, id);
        }
        void setName(const char *name){ strcpy(this->name, name); }
        void setSex(char sex){ this->sex = sex; }
        void setId(const char id[11]){ strcpy(this->id, id); }
        void print(){ cout << name << "\t" << sex << "\t" << id << "\t" << dob() << endl; }
};

//用于确认文件是否存在
inline bool existTest (const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

//如果没有指定文件，则生成指定的文件
void checkNameList()
{
    try
    {
        if(!existTest("studentList.txt")) {throw(404); }
    }
    catch(int e)
    {
        //生成有三个学生的名单
        ofstream temp;
        temp.open("studentList.txt", ios_base::trunc);
        student s[3];
        s[0] = student("常清华", 'F', "2020010001", 2000, 1, 1);
        s[1] = student("林北大", 'M', "2020010002", 2001, 2, 2);
        s[2] = student("复旦大", 'M', "2020010003", 2010, 3, 3);
        temp.write((char*)&s[0], sizeof(s[0]));
        temp.write((char*)&s[1], sizeof(s[1]));
        temp.write((char*)&s[2], sizeof(s[2]));
        temp.close();
    }
}

int main()
{
    //检查文件是否存在，然后读取学生文件
    checkNameList();
    ifstream studentList;
    studentList.open("studentList.txt", ios::in);
    studentList.seekg(0);
    student s;
    studentList.read((char*)&s, sizeof(s));
    s.print();
    return 0;
}