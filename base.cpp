#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;



class time
{
private:
    int year;
    int month;
    int day;
    int period;
public:
    time(int y = 2021, int m = 1, int d = 1, int p = 1): year(y), month(m), day(d), period(p)
    {
    }
    ~time();
};


//----------------------------student类，记录学生信息------------------------------------------------------------------
class student
{
    private:
        char name[30];
        char sex;
        char id[11];
    public:
        student(){};//生成空的学生类
        student(const char *name, const char sex, const char id[11])
        {
            strcpy(this->name, name);
            this->sex = sex;
            strcpy(this->id, id);
        }
        void setName(const char *name){ strcpy(this->name, name); }
        void setSex(char sex){ this->sex = sex; }
        void setId(const char id[11]){ strcpy(this->id, id); }
        void print(){ cout << name << "\t" << sex << "\t" << id << endl; }
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
        s[0] = student("常清华", 'F', "2020010001");
        s[1] = student("林北大", 'M', "2020010002");
        s[2] = student("复旦大", 'M', "2020010003");
        temp.write((char*)&s[0], sizeof(s[0]));
        temp.write((char*)&s[1], sizeof(s[1]));
        temp.write((char*)&s[2], sizeof(s[2]));
        temp.close();
    }
}

int main()
{
    checkNameList();
    ifstream studentList;
    studentList.open("studentList.txt", ios::in);
    studentList.seekg(0);
    student s;
    studentList.read((char*)&s, sizeof(s));
    s.print();
    return 0;
}