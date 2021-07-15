#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QMessageBox>
#include <QMap>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//----------------------info类，用于记录名字与ID----------------------------------------------
class info
{
protected:
    QString name;
    QString id;
public:
    info(QString const &name, QString const &id);
    info();
    virtual QString getID();
    virtual QString getName();
};

//-----------------------time类，用于记录生日----------------------------------------------
class date
{
    protected:
        int year, month, day;
    public:
        date(int y = 2000, int m = 1, int d = 1);
        date(const date &d);
        virtual QString dob();//以yyyy-mm-dd形式显示生日
};

//----------------------------student类，记录学生信息------------------------------------------------------------------
class student : public info
{
    private:
        char sex;
    public:
        student();
        student(QString const &name, char sex, QString const &id);
        void setName(QString const &name);
        void setSex(char sex);
        void setId(QString const &id);
        char getSex();
};

//-------------------Course类，用于记录课程及学生分数----------------------------
class course: public info
{
private:
    float credit;
public:
    course(QString const &name, QString const &id, float c);
    virtual float getCredit();
};

//----------------compulsory和elective类，用于记gpa和pf课程------------------
class compulsory : public course
{
private:
    QMap<QString, float> grade;
public:
    compulsory(QString const &name, QString const id, float c);
    //~compulsory();
    void insertGradeByID(QString const &id, float g);
    float getGrade(QString const &id);
};

class elective : public course
{
private:
    QMap<QString, char> grade;//p or f
public:
    elective(QString const &name, QString const &id, float c);
    void insertGradeByID(QString const &id, char g);
    char getGrade(QString const &id);
};

//--------------------------------MainWindow类-----------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<student*> studentList;
    void addStudent();
    void addCourse();
    void saveStudent();

private slots:
    void on_addStudentBtn_clicked();

    void on_actionxin_triggered();

    void on_delStudentBtn_clicked();

    void on_AddCourseBtn_clicked();

    void on_action_triggered();

    void on_delCourseBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
