#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addstudentdialog.h"
#include "coursedialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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

//----------------date类的函数----------------
date::date(int y, int m, int d): year(y), month(m), day(d){};

//返回生日日期
QString date::dob()
{
    return QString("%1-%2-%3").arg(year).arg(month).arg(day);
}

//----------------student类的函数----------------
student::student(QString const &name, char sex, QString const &id): info(name, id)
{
    this->sex = sex;
}

void student::setName(const QString &name){ this->name = name; }
void student::setSex(char sex){ this->sex = sex; }
void student::setId(const QString &id){ this->id = id;}
char student::getSex(){ return sex; }


//----------------course类的函数----------------
course::course(QString const &name, QString const &id, float c): info(name, id), credit(c)
{}

float course::getCredit(){ return credit;}

//----------------compulsory类的函数----------------
compulsory::compulsory(QString const &name, QString const id, float c): course(name, id, c){};
void compulsory::insertGradeByID(QString const &id, float g)
{
    grade[id] = g;
}

float compulsory::getGrade(const QString &id)
{
    return grade[id];
}

//----------------elective类的函数----------------
elective::elective(QString const &name, QString const &id, float c): course(name, id, c){};
void elective::insertGradeByID(const QString &id, char g)
{
    grade[id] = g;
}
char elective::getGrade(const QString &id)
{
    return grade[id];
}

//--------------------------------界面操作-----------------------------------------

void MainWindow::addStudent()
{
    addStudentDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    int row = ui->studentTable->rowCount();
    ui->studentTable->insertRow(row);
    ui->studentTable->setItem(row, 0, new QTableWidgetItem(dialog.getName()));
    ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(dialog.getSex())));
    ui->studentTable->setItem(row, 2, new QTableWidgetItem(dialog.getID()));
}

void MainWindow::addCourse()
{
    courseDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    int col = ui->studentTable->columnCount();
    ui->studentTable->insertColumn(col);
    ui->studentTable->setHorizontalHeaderItem(col, new QTableWidgetItem(dialog.getName()));
}

void MainWindow::on_addStudentBtn_clicked()
{
    addStudent();
}


void MainWindow::on_actionxin_triggered()
{
    addStudent();
}


void MainWindow::on_delStudentBtn_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "删除学生",
                                                              "此操作将不可逆，是否确定删除？",
                                                              QMessageBox::Yes| QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QModelIndexList select = ui->studentTable->selectionModel()->selectedRows();
        for (int i = 0; i < select.count(); i++) {
            QModelIndex index = select.at(i);
            ui->studentTable->removeRow(index.row());
        }
    }
}


void MainWindow::on_AddCourseBtn_clicked()
{
    addCourse();
}


void MainWindow::on_action_triggered()
{
    addCourse();
}


void MainWindow::on_delCourseBtn_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "删除课程",
                                                              "此操作将不可逆，是否确定删除？",
                                                              QMessageBox::Yes| QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QModelIndexList select = ui->studentTable->selectionModel()->selectedColumns();
        for (int i = 0; i < select.count(); i++) {
            QModelIndex index = select.at(i);
            ui->studentTable->removeColumn(index.column());
        }
    }
}

