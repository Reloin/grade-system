#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addstudentdialog.h"
#include "coursedialog.h"

#include <QDebug>

QDataStream &operator<<(QDataStream &out, const student &s)
{
    out << s.name << s.sex << s.id;
    return out;
}

QDataStream &operator>>(QDataStream &in, student &s)
{
    QString name, id;
    char sex;
    in >> name >> sex >> id;
    s.setName(name);
    s.setSex(sex);
    s.setId(id);
    return in;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadStudent();

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
info::info(){}

QString info::getID(){ return id; } //返回学生的学号
QString info::getName(){ return name;} //返回学生姓名

//----------------student类的函数----------------
student::student(QString const &name, char sex, QString const &id): info(name, id)
{
    this->sex = sex;
}
student::student(){}

void student::setName(const QString &name){ this->name = name; }
void student::setSex(char sex){ this->sex = sex; }
void student::setId(const QString &id){ this->id = id;}
char student::getSex(){ return sex; }


//----------------course类的函数----------------
course::course(QString const &name, QString const &id, float c): info(name, id), credit(c){}

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
//增加学生资料的函数
void MainWindow::addStudent()
{
    addStudentDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    student temp = student(dialog.getName(), dialog.getSex(), dialog.getID());
    int row = ui->studentTable->rowCount();
    ui->studentTable->insertRow(row);
    ui->studentTable->setItem(row, 0, new QTableWidgetItem(temp.getName()));
    ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(temp.getSex())));
    ui->studentTable->setItem(row, 2, new QTableWidgetItem(temp.getID()));

    studentList.append(temp);
    saveStudent();
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

void MainWindow::saveStudent()
{
    QString path = QDir::currentPath() + "/studentlist.txt";
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_5);

    QList<student>::iterator i;
    for (i = studentList.begin(); i != studentList.end() ; ++i)
    {
        out << i;
    }
    file.flush();
    file.close();
}

void MainWindow::loadStudent()
{
    QString path = QDir::currentPath() + "/studentlist.txt";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    studentList.empty();
    while (!in.atEnd()) {
        student temp;
        in >> temp;
        studentList.append(temp);
    }

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

