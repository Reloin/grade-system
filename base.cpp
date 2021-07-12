#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <map>
#include <limits>

using namespace std;

//----------------------info类，用于记录名字与ID----------------------------------------------
class info
{
protected:
    char name[30];
    char id[11];
public:
    info(const char *name, const char *id)
    {
        strcpy(this->name, name);
        strcpy(this->id, id);
    }
    info(){}
    virtual char *getID()
    {
        char *t = id;
        return t;
    }
    virtual char *getName()
    {
        char *t = name;
        return t;
    }
};



//-----------------------time类，用于记录生日----------------------------------------------
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
class student : public date, public info
{
    private:
        char sex;
    public:
        student(){}
        student(const char *name, const char sex, const char *id, int y, int m, int d): info(name, id), date(y, m, d)
        {
            this->sex = sex;
        }
        void setName(const char *name){ strcpy(this->name, name); }
        void setSex(char sex){ this->sex = sex; }
        void setId(const char id[11]){ strcpy(this->id, id); }
        void print(){ cout << name << "\t" << sex << "\t" << id << "\t" << dob() << endl; }
};

//-------------------Course类，用于记录课程及学生分数----------------------------
class course: public info
{
private:
    float credit;
public:
    course(const char *name, const char *id, float c): info(name, id), credit(c){}
    virtual float getCredit(){ return credit;}
};

//----------------compulsory和elective类，用于记gpa和pf课程
class compulsory : public course
{
private:
    map<string, float> grade;
public:
    compulsory(const char *name, const char *id, float c):course(name, id, c){};
    //~compulsory();
    void insertGradeByID(const char *id, float g)
    {
        grade[id] = g;
    };
    void list()
    {
        for (std::map<string,float>::iterator it=grade.begin(); it!=grade.end(); ++it)
            cout << it->first << " | " << it->second << '\n';
    }
    float getGrade(const char *id){ return grade[id]; }
};

class elective : public course
{
private:
    map<string, char> grade;//p or f
public:
    elective(const char *name, const char *id, float c):course(name, id, c){};
    //~elective();
    void insertGradeByID(char *id, char g)
    {
        grade[id] = g;
    };
    void list()
    {
        for (std::map<string, char>::iterator it=grade.begin(); it!=grade.end(); ++it)
            cout << it->first << " | " << it->second << '\n';
    }
    char getGrade(const char *id){ return grade[id]; }
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
        //生成有三个student类的名单
        ofstream temp("studentList.txt", ios_base::trunc);
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

void checkCourseList()
{
    try
    {
        if(!existTest("courseList.txt")) throw(404);
    }
    catch(int e)
    {
        //生成有一个比选课的compulsory类的文件
        ofstream temp("courseList", ios::trunc);
        compulsory c = compulsory("计算机程序设计基础(2)", "100001", 3);
        temp.write((char*)&c, sizeof(c));
    }
    
}

int main()
{
    //检查文件是否存在，然后读取学生文件
    checkNameList();
    fstream studentList("studentList.txt", ios::in | ios::out | ios::app);
    studentList.seekg(0);
    //student s = studentlist.read((char*)&s, sizeof(s));
    while (true)
    {
        cout << endl << "--------------------程设大学学生成绩记录系统--------------------" << endl;
        cout << "-----------------------------选项-----------------------------" << endl;
        cout << "1. 显示成绩" << endl;
        cout << "______________________________________________________________" << endl;
        cout << "请输入1至5的号码：";

        int input;

        cin >> input;
        while (true)
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << "输入错误，请输入1至5的号码：";
                cin >> input;
            }
            if(!cin.fail()) break;
        }
        

    }
    

    return 0;
}