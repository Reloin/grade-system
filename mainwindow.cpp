#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
    ui->studentTable->insertRow(0);
    ui->studentTable->setItem(0, 0, new QTableWidgetItem("Names"));
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDataStream &operator<<(QDataStream &out, student &s)
{
    out << s.getName() << s.getSex() << s.getID() << s.dob();
    return out;
}

void MainWindow::checkStudentList()
{

}
//-------------------------类的方法-----------------------

//info类的函数
info::info(QString const &name, QString const &id)
{
    this->name = name;
    this->id = id;
}

QString info::getID(){ return id; } //返回学生的学号
QString info::getName(){ return name;} //返回学生姓名

//date类的函数
date::date(int y, int m, int d): year(y), month(m), day(d){};

//返回生日日期
QString date::dob()
{
    QString temp;
    temp.asprintf("%d-%d-%d", year, month, day);
    return temp;
}

//----------------student类的函数
student::student(QString const &name, const char sex, QString const &id, int y, int m, int d): date(y, m, d), info(name, id)
{
    this->sex = sex;
}

void student::setName(const QString &name){ this->name = name; }
void student::setSex(char sex){ this->sex = sex; }
void student::setId(const QString &id){ this->id = id;}
char student::getSex(){ return sex;}

//course类的函数
course::course(QString const &name, QString const &id, float c): info(name, id), credit(c)
{}

float course::getCredit(){ return credit;}

//compulsory类的函数
compulsory::compulsory(QString const &name, QString const id, float c): course(name, id, c){};
void compulsory::insertGradeByID(QString const &id, float g)
{
    grade[id] = g;
}

float compulsory::getGrade(const QString &id)
{
    return grade[id];
}

//compulsory类的函数
elective::elective(QString const &name, QString const &id, float c): course(name, id, c){};
void elective::insertGradeByID(const QString &id, char g)
{
    grade[id] = g;
}
char elective::getGrade(const QString &id)
{
    return grade[id];
}
