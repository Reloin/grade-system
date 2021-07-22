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
    QMap<QString, student>::const_iterator s = studentList.constBegin();
    while (s != studentList.constEnd())
    {
        int row = ui->studentTable->rowCount();
        ui->studentTable->insertRow(row);
        student temp = s.value();
        ui->studentTable->setItem(row, 0, new QTableWidgetItem(temp.getName()));
        ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(temp.getSex())));
        ui->studentTable->setItem(row, 2, new QTableWidgetItem(yearConverter(temp.getYear())));
        //让id不可更改
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(temp.getID());
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->studentTable->setItem(row, 3, item);
        s++;
    }

    //将必选课的分数展示出来
    QMap<QString, compulsory>::const_iterator c = compulsoryList.constBegin();
    while(c != compulsoryList.constEnd())
    {
        int col = ui->studentTable->columnCount();
        compulsory temp = c.value();
        ui->studentTable->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(temp.getName());
        item->setData(Qt::WhatsThisRole, temp.getID());
        ui->studentTable->setHorizontalHeaderItem(col, item);
        for(int row = 0; row < ui->studentTable->rowCount(); row++)
        {
            QString id = ui->studentTable->item(row, 3)->text();
            ui->studentTable->setItem(row, col, new QTableWidgetItem(QString::number(temp.getGrade(id))));
        }
        c++;
    }

    //将选修课的分数展示出来
    QMap<QString, elective>::const_iterator e = electiveList.constBegin();
    while(e != electiveList.constEnd())
    {
        int col = ui->studentTable->columnCount();
        elective temp = e.value();
        ui->studentTable->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(temp.getName());
        item->setData(Qt::WhatsThisRole, temp.getID());
        ui->studentTable->setHorizontalHeaderItem(col, item);
        for(int row = 0; row < ui->studentTable->rowCount(); row++)
        {
            QString id = ui->studentTable->item(row, 3)->text();
            ui->studentTable->setItem(row, col, new QTableWidgetItem(temp.getGrade(id)));
        }
        e++;
    }
    init = !init;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString yearConverter(int num)
{
    switch (num) {
    case 1:
        return "大一";
    case 2:
        return "大二";
    case 3:
        return "大三";
    case 4:
        return "大四";
    default:
        return "";
    }
}
//-------------------------info类的方法-----------------------

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
student::student(QString const &name,const QString &sex, int y, QString const &id): info(name, id), year(y)
{
    this->sex = sex;
}
student::student(){}

//返回一些private里的数据
void student::setName(const QString &name){ this->name = name; }
void student::setSex(const QString &sex){ this->sex = sex; }
void student::setYear(int y){ year = y; }
void student::setId(const QString &id){ this->id = id;}
int student::getYear(){ return year; }
QString student::getSex(){ return sex; }


//----------------course类的函数----------------
course::course(QString const &name, QString const &id, float c): info(name, id), credit(c){}
course::course(){}
float course::getCredit(){ return credit;}

//----------------compulsory类的函数----------------
compulsory::compulsory(QString const &name, QString const id, float c): course(name, id, c){};
compulsory::compulsory(){}
void compulsory::insertGradeByID(QString const &id, float g)
{
    grade[id] = g;
}

void compulsory::removeGradeByID(const QString &id)
{
    grade.remove(id);
}

float compulsory::getGrade(const QString &id)
{
    return grade[id];
}

QStringList compulsory::getGrades()
{
    QStringList temp;
    QMap<QString, float>::const_iterator i = grade.constBegin();
    while(i != grade.constEnd())
    {
        temp.append(i.key() + ":" + QString::number(i.value()));
        ++i;
    }
    return temp;
}

//----------------elective类的函数----------------
elective::elective(QString const &name, QString const &id, float c): course(name, id, c){};
elective::elective(){}
void elective::insertGradeByID(const QString &id, QString const &g)
{
    grade[id] = g;
}
void elective::removeGradeByID(QString const &id)
{
    grade.remove(id);
}
QStringList elective::getGrades()
{
    QStringList temp;
    QMap<QString, QString>::const_iterator i = grade.constBegin();
    while(i != grade.constEnd())
    {
        temp.append(i.key() + ":" + i.value());
        ++i;
    }
    return temp;
}

QString elective::getGrade(QString const &id)
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
    auto result = dialog.exec();

    init = true;

    if(result == QDialog::Accepted)
    {
        //获取数据并显示与存进List里
        student temp = student(dialog.getName(), dialog.getSex(), dialog.getYear(), dialog.getID());
        int row = ui->studentTable->rowCount();
        ui->studentTable->insertRow(row);
        ui->studentTable->setItem(row, 0, new QTableWidgetItem(temp.getName()));
        ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString(temp.getSex())));
        ui->studentTable->setItem(row, 2, new QTableWidgetItem(yearConverter(temp.getYear())));
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(temp.getID());
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->studentTable->setItem(row, 3, item);

        studentList[temp.getID()] = temp;
        saveStudent();
    }
    init = false;
}

void MainWindow::addCourse()
{
    //弹出框框输入课程数据
    courseDialog dialog;
    dialog.setModal(true);
    auto result = dialog.exec();

    init = true;
    if(result == QDialog::Accepted)
    {
        //获取并存储课程数据
        int col = ui->studentTable->columnCount();
        ui->studentTable->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem();
        QString id = dialog.getID();
        item->setText(dialog.getName());

        //在id前加c或e，用于识别必选课或选修课
        if(dialog.getType() == 0)
        {
            id = "c" + id;
            compulsoryList[id] = compulsory(dialog.getName(), id, dialog.getCredit());
        }
        else if(dialog.getType() == 1)
        {
            id = "e" + id;
            electiveList[id] = elective(dialog.getName(), id, dialog.getCredit());
        }

        item->setData(Qt::WhatsThisRole, id);
        ui->studentTable->setHorizontalHeaderItem(col, item);
        saveCourse();
    }
    init = false;
}

void MainWindow::saveStudent()
{
    //在程序的文档里创建csv文件
    QFile file(studentFileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QTextStream out(&file);

    //循环list里的学生逐个存入文件中
    QMap<QString, student>::const_iterator s = studentList.constBegin();
    while (s != studentList.constEnd())
    {
        student temp = s.value();
        out << temp.getName() << ";" << temp.getSex() << ";"  << temp.getYear() << ";" << temp.getID() << "\n";
        s++;
    }
    file.flush();
    file.close();
}

void MainWindow::loadStudent()
{
    //在程序的文档中寻找list文件
    QFile file(studentFileName);
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
        //0:name, 1:sex, 3:year, 3:id
        studentList[data.at(3)] = student(data.at(0), data.at(1), data.at(2).toInt(),data.at(3));
    }

}

void MainWindow::saveCourse()
{
    //在程序的文档里创建csv文件
    QFile file(courseFileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("文件出现错误"), file.errorString());
    }

    QTextStream out(&file);

    //循环list里的课程逐个存入文件中
    QMap<QString, compulsory>::const_iterator c = compulsoryList.constBegin();
    while(c != compulsoryList.constEnd())
    {
        compulsory temp = c.value();
        out << temp.getName() << ";" << temp.getID() << ";" << temp.getCredit();

        QStringList grades = temp.getGrades();
        for (int j = 0; j < grades.count(); j++)
        {
            QStringList data = grades.at(j).split(":");
            out << ";" << data.at(0) << ":" << data.at(1);
        }
        out << "\n";
        c++;
    }

    QMap<QString, elective>::const_iterator e = electiveList.constBegin();
    while(e != electiveList.constEnd())
    {
        elective temp = e.value();
        out << temp.getName() << ";" << temp.getID() << ";" << temp.getCredit();

        QStringList grades = temp.getGrades();
        for (int j = 0; j < grades.count(); j++)
        {
            QStringList data = grades.at(j).split(":");
            out << ";" << data.at(0) << ":" << data.at(1);
        }
        out << "\n";
        e++;
    }
    file.flush();
    file.close();
}

void MainWindow::loadCourse()
{
    //在程序的文档中寻找list文件
    QFile file(courseFileName);
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
    //保险起见将两个QMap清空
    compulsoryList.empty();
    electiveList.empty();

    QStringList cList = in->readAll().split("\n");
    //loop文件读取信息

    for (int i = 0; i < cList.count() - 1; i++)
    {

        QStringList entry = cList.at(i).split(";");
        QString id = entry.at(1);

        //如果是必选课就加入必选课的QMap
        if(id.at(0) == 'c')
        {
            compulsory c = compulsory(entry.at(0), id, entry.at(2).toFloat());
            //循环之后的学生成绩数据
            for (int j = 3; j < entry.count(); j++)
            {
                QStringList data = entry.at(j).split(":");
                c.insertGradeByID(data.at(0), data.at(1).toFloat());
            }
            compulsoryList[c.getID()] = c;
        }
        //如果是选修课就加入选修课的QMap
        else if(entry.at(1).at(0) == 'e')
        {
            elective e = elective(entry.at(0), id, entry.at(2).toFloat());
            //循环之后的学生成绩数据
            for (int j = 3; j < entry.count(); j++)
            {
                QStringList data = entry.at(j).split(":");
                e.insertGradeByID(data.at(0), data.at(1));
            }
            electiveList[e.getID()] = e;
            }
      }
}

void MainWindow::removeGrade(QString const &courseID, QString const &studentID)
{
    compulsory c = compulsoryList[courseID];
    c.removeGradeByID(studentID);
    compulsoryList[courseID] = c;
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
            QString id = ui->studentTable->item(index.row(), 3)->text();
            for (int j = 4; j < ui->studentTable->columnCount(); j++) {
                QTableWidgetItem *item = ui->studentTable->horizontalHeaderItem(j);
                QString courseID = item->data(Qt::WhatsThisRole).toString();
                if(courseID.at(0) == 'c')
                {
                    compulsory c = compulsoryList[courseID];
                    c.removeGradeByID(id);
                    compulsoryList[courseID] = c;
                }
                else if(courseID.at(0) == 'e')
                {
                    elective e = electiveList[courseID];
                    e.removeGradeByID(id);
                    electiveList[courseID] = e;
                }
            }
            //在list中移除学生
            studentList.remove(id);

            //在表中移除学生
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
            QString courseID = ui->studentTable->horizontalHeaderItem(index.column())->data(Qt::WhatsThisRole).toString();
            if(courseID.at(0) == 'c')
            {
                compulsoryList.remove(courseID);
            }
            else if(courseID.at(0) == 'e')
            {
                electiveList.remove(courseID);
            }
            ui->studentTable->removeColumn(index.column());

        }
        saveCourse();
    }
}


void MainWindow::on_studentTable_cellChanged(int row, int column)
{
    if(init){ return;}
    //当用户更改学生姓名也更改QMap中的姓名
    else if(column == 0)
    {
        student temp = studentList[ui->studentTable->item(row, 3)->text()];
        temp.setName(ui->studentTable->item(row, column)->text());
        studentList[temp.getID()] = temp;
        saveStudent();
    }
    //更改用户性别也更改QMap中的性别
    else if(column == 1)
    {
        student temp = studentList[ui->studentTable->item(row, 3)->text()];
        temp.setSex(ui->studentTable->item(row, column)->text());
        studentList[temp.getID()] = temp;
        saveStudent();
    }
    //输入及更改分数
    else if(column > 3)
    {
        QTableWidgetItem *item = ui->studentTable->horizontalHeaderItem(column);
        QString id = item->data(Qt::WhatsThisRole).toString();

        if(id.at(0) == 'c')
        {
            compulsory temp = compulsoryList[id];
            temp.insertGradeByID(ui->studentTable->item(row, 3)->text(), ui->studentTable->item(row, column)->text().toFloat());
            compulsoryList[temp.getID()] = temp;
        }
        else if(id.at(0) == 'e')
        {
            elective temp = electiveList[id];
            temp.insertGradeByID(ui->studentTable->item(row, 3)->text(), ui->studentTable->item(row, column)->text());
            electiveList[id] = temp;
        }
        saveCourse();
    }
}


void MainWindow::on_pushButton_clicked()
{

    QModelIndexList select = ui->studentTable->selectionModel()->selectedColumns();
    int col = select.at(0).column();
    if(col != 1 && select.count() == 1)
    {
        //让排序顺着后逆着
        ui->studentTable->sortItems(col, ascend?Qt::AscendingOrder:Qt::DescendingOrder);
        ascend = !ascend;
    }
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QTableWidget *table = ui->studentTable;
    switch (index) {
    case 0:
        for (int i = 0; i < table->rowCount(); i++) {
            table->showRow(i);
        }
        break;
    case 1:
        for (int i = 0; i < table->rowCount(); i++) {
            if(QString::compare(table->item(i, 2)->text(), "大一", Qt::CaseInsensitive)) table->hideRow(i);
            else if(QString::compare(table->item(i, 2)->text(), "大一", Qt::CaseInsensitive) == 0) table->showRow(i);
        }
        break;
    case 2:
        for (int i = 0; i < table->rowCount(); i++) {
            if(QString::compare(table->item(i, 2)->text(), "大二", Qt::CaseInsensitive)) table->hideRow(i);
            else if(QString::compare(table->item(i, 2)->text(), "大二", Qt::CaseInsensitive) == 0) table->showRow(i);
        }
        break;
    case 3:
        for (int i = 0; i < table->rowCount(); i++) {
            if(QString::compare(table->item(i, 2)->text(), "大三", Qt::CaseInsensitive)) table->hideRow(i);
            else if(QString::compare(table->item(i, 2)->text(), "大三", Qt::CaseInsensitive) == 0) table->showRow(i);
        }
        break;
    case 4:
        for (int i = 0; i < table->rowCount(); i++) {
            if(QString::compare(table->item(i, 2)->text(), "大四", Qt::CaseInsensitive)) table->hideRow(i);
            else if(QString::compare(table->item(i, 2)->text(), "大四", Qt::CaseInsensitive) == 0) table->showRow(i);
        }
        break;
    }
}

