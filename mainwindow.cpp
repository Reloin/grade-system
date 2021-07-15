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
    loadStudent();

    QList<student>::iterator i;
    for (i = studentList.begin(); i != studentList.end() ; ++i)
    {
        int row = ui->studentTable->rowCount();
        ui->studentTable->insertRow(row);
        ui->studentTable->setItem(row, 0, new QTableWidgetItem(i->getName()));
        ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(i->getSex())));
        ui->studentTable->setItem(row, 2, new QTableWidgetItem(i->getID()));
    }
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
student::student(QString const &name,const QString &sex, QString const &id): info(name, id)
{
    this->sex = sex;
}
student::student(){}

void student::setName(const QString &name){ this->name = name; }
void student::setSex(const QString &sex){ this->sex = sex; }
void student::setId(const QString &id){ this->id = id;}
QString student::getSex(){ return sex; }


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

//--------------------------------主界面函数-----------------------------------------
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
    QString path = QDir::currentPath() + "/studentlist.csv";
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QTextStream out(&file);

    QList<student>::iterator i;
    for (i = studentList.begin(); i != studentList.end() ; ++i)
    {
        out << i->getName() << ";" << i->getSex() << ";" << i->getID() << "\n";
    }
    file.flush();
    file.close();
}

void MainWindow::loadStudent()
{
    QString path = QDir::currentPath() + "/studentlist.csv";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QFile file("studentList.csv");
        file.open(QIODevice::WriteOnly);
        file.flush();
        file.close();
        return;
    }

    QTextStream *in = new QTextStream(&file);
    studentList.empty();

    QStringList sList = in->readAll().split("\n");

    for (int i = 0; i < sList.count() - 1; i++)
    {
        QStringList data = sList.at(i).split(";");
        studentList.append(student(data.at(0), data.at(1), data.at(2)));
    }

}

//-----------------------------------界面的按键-----------------------------------
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
            int index = select.at(i).row();
            QString id = ui->studentTable->itemAt(index, 2)->text();

            for (int t = 0; t != studentList.count() ; ++t)
            {
                student s = studentList.at(t);
                if(QString::compare(s.getName(), id, Qt::CaseInsensitive))
                {
                    studentList.removeAt(t);
                    break;
                }
            }
            ui->studentTable->removeRow(index);

        }
    }

    saveStudent();
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

