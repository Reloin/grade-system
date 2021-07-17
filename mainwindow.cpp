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

    //读取csv文件中的学生数据然后存在studentList里
    loadStudent();
    loadCourse();

    //将存储的学生数据展示出来
    QList<student>::iterator i;
    for (i = studentList.begin(); i != studentList.end() ; i++)
    {
        int row = ui->studentTable->rowCount();
        ui->studentTable->insertRow(row);
        ui->studentTable->setItem(row, 0, new QTableWidgetItem(i->getName()));
        ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(i->getSex())));
        ui->studentTable->setItem(row, 2, new QTableWidgetItem(i->getID()));
    }

    QList<compulsory>::iterator c;
    for(c = courseList.begin(); c != courseList.end(); ++c)
    {
        int col = ui->studentTable->columnCount();
        ui->studentTable->insertColumn(col);
        ui->studentTable->setHorizontalHeaderItem(col, new QTableWidgetItem(c->getName()));
        for(int row = 0; row < ui->studentTable->rowCount(); row++)
        {
            QString id = ui->studentTable->item(row, 2)->text();
            ui->studentTable->setItem(row, col, new QTableWidgetItem(QString::number(c->grade[id])));
        }
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

//返回一些private里的数据
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

QStringList compulsory::getGrades()
{
    QStringList temp;
    QMap<QString, float>::const_iterator i = grade.begin();

    while(i != grade.constEnd())
    {
        temp.append(i.key() + ":" + QString::number(i.value()));
        ++i;
    }
    return temp;
}

//----------------elective类的函数----------------
elective::elective(QString const &name, QString const &id, float c): course(name, id, c){};
void elective::insertGradeByID(const QString &id, QString const &g)
{
    grade[id] = g;
}
QString elective::EgetGrade(const QString &id)
{
    return grade[id];
}

//--------------------------------主界面函数-----------------------------------------
//增加学生资料的函数
void MainWindow::addStudent()
{
    //弹出框框输入学生数据
    addStudentDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    //获取数据并显示与存进List里
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
    //弹出框框输入课程数据
    courseDialog dialog;
    dialog.setModal(true);
    dialog.exec();

    //获取并存储课程数据
    int col = ui->studentTable->columnCount();
    ui->studentTable->insertColumn(col);
    ui->studentTable->setHorizontalHeaderItem(col, new QTableWidgetItem(dialog.getName()));
    if(dialog.getType() == 0)
    {
        courseList.append(compulsory(dialog.getName(), dialog.getID(), dialog.getCredit()));
    }/*
    else if(dialog.getType() == 1)
    {
        courseList.append(elective(dialog.getName(), dialog.getID(), dialog.getCredit()));
    }*/
    saveCourse();
}

void MainWindow::saveStudent()
{
    //在程序的文档里创建csv文件
    QFile file("studentlist.csv");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QTextStream out(&file);

    //循环list里的学生逐个存入文件中
    QList<student>::iterator i;
    for (i = studentList.begin(); i != studentList.end() ; i++)
    {
        out << i->getName() << ";" << i->getSex() << ";" << i->getID() << "\n";
    }
    file.flush();
    file.close();
}

void MainWindow::loadStudent()
{
    //在程序的文档中寻找list文件
    QFile file("studentlist.csv");
    if(!file.open(QIODevice::ReadOnly))
    {
        //如果没找到就创建一个空的
        file.open(QIODevice::WriteOnly);
        file.flush();
        file.close();
        return;
    }

    //每一行都是一个学生数据，循环每一行读取并存入list
    QTextStream *in = new QTextStream(&file);
    studentList.empty();

    QStringList sList = in->readAll().split("\n");

    for (int i = 0; i < sList.count() - 1; i++)
    {
        QStringList data = sList.at(i).split(";");
        studentList.append(student(data.at(0), data.at(1), data.at(2)));
    }

}

void MainWindow::saveCourse()
{
    //在程序的文档里创建csv文件
    QFile file("courselist.csv");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QTextStream out(&file);

    //循环list里的课程逐个存入文件中
    QList<compulsory>::iterator i;
    for (i = courseList.begin(); i != courseList.end() ; i++)
    {
        out << i->getName() << ";" << i->getID() << ";" << i->getCredit();

        QStringList grades = i->getGrades();
        for (int j = 0; j < grades.count(); j++)
        {
            QStringList data = grades.at(j).split(":");
            out << ";" << data.at(0) << ":" << data.at(1);
        }
        out << "\n";

    }
    file.flush();
    file.close();
}

void MainWindow::loadCourse()
{
    //在程序的文档中寻找list文件
    QFile file("courselist.csv");
    if(!file.open(QIODevice::ReadOnly))
    {
        //如果没找到就创建一个空的
        file.open(QIODevice::WriteOnly);
        file.flush();
        file.close();
        return;
    }

    //每一行都是一个学生数据，循环每一行读取并存入list
    QTextStream *in = new QTextStream(&file);
    courseList.empty();

    QStringList cList = in->readAll().split("\n");

    for (int i = 0; i < cList.count() - 1; i++)
    {
        QStringList entry = cList.at(i).split(";");

        compulsory c = compulsory(entry.at(0), entry.at(1), entry.at(2).toFloat());
        for (int j = 3; j < entry.count(); j++)
        {
            QStringList data = entry.at(j).split(":");
            c.insertGradeByID(data.at(0), data.at(1).toFloat());
        }
        courseList.append(c);

    }
}

void MainWindow::removeGrade(int i, QString const &id)
{
    compulsory c = courseList.at(i);
    c.grade.remove(id);
    courseList.replace(i, c);
}

//搜索学生条目
int MainWindow::searchByName(QString const &name)
{
    for (int i = 0; i < studentList.count(); i++)
    {
        student temp = studentList.at(i);
        if(QString::compare(temp.getName(), name, Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return -1;
}

int MainWindow::searchByID(QString const &id)
{
    for (int i = 0; i < studentList.count(); i++)
    {
        student temp = studentList.at(i);
        if(QString::compare(temp.getID(), id, Qt::CaseInsensitive) == 0)
        {
            qDebug() << "Name in list:" << temp.getID();
            qDebug() << "Id given:" << id;
            return i;
        }
    }
    return -1;
}


//----------------------------------------------界面的按键----------------------------------------------
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
            QString id = ui->studentTable->item(index.row(), 2)->text();
            for (int j = 0; j + 3 < ui->studentTable->columnCount(); j++) {
                removeGrade(j, id);
            }
            studentList.removeAt(searchByID(id));

            ui->studentTable->removeRow(index.row());
        }
    }

    saveStudent();
    saveCourse();
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
        saveCourse();
    }
}


void MainWindow::on_studentTable_cellChanged(int row, int column)
{
    if(column > 2)
    {
        compulsory temp = courseList.at(column - 3);
        temp.insertGradeByID(ui->studentTable->item(row, 2)->text(), ui->studentTable->item(row, column)->text().toFloat());
        courseList.replace(column - 3, temp);
        saveCourse();
    }
}

