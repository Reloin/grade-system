#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QMap>
#include <QDir>
#include <QString>
#include <QMapIterator>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QString num2year(int num);
int year2num(QString const &year);
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

//----------------------------student类，记录学生信息------------------------------------------------------------------
class student : public info
{
    private:
        int year;
        QString sex;
    public:
        student();
        student(QString const &name, QString const &sex, int y, QString const &id);
        void setName(QString const &name);
        void setYear(int y);
        void setSex(QString const &sex);
        void setId(QString const &id);
        int getYear();
        QString getSex();
};

//-------------------Course类，用于记录课程及学生分数----------------------------
class course: public info
{
private:
    float credit;
public:
    course(QString const &name, QString const &id, float c);
    course();
    float getCredit();
};

//----------------compulsory和elective类，用于记gpa和pf课程------------------
class compulsory : public course
{
public:
    compulsory(QString const &name, QString const id, float c);
    compulsory();
    //~compulsory();
    void insertGradeByID(QString const &id, float g);
    void removeGradeByID(QString const &id);
    QStringList getGrades();
    float getGrade(QString const &id);
private:
    QMap<QString, float> grade;
};

class elective : public course
{
public:
    elective(QString const &name, QString const &id, float c);
    elective();
    void insertGradeByID(QString const &id, QString const &g);
    void removeGradeByID(QString const &id);
    QStringList getGrades();
    QString getGrade(QString const &id);
private:
    QMap<QString, QString> grade;//p or f
};

//--------------------------------MainWindow类-----------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMap<QString, student> studentList;
    QMap<QString, compulsory> compulsoryList;
    QMap<QString, elective> electiveList;
    void addStudent();
    void addCourse();
    void saveStudent();
    void loadStudent();
    void saveCourse();
    void loadCourse();
    void removeGrade(QString const &courseID,QString const &studentID);

private slots:
    void on_addStudentBtn_clicked();

    void on_actionxin_triggered();

    void on_delStudentBtn_clicked();

    void on_AddCourseBtn_clicked();

    void on_action_triggered();

    void on_delCourseBtn_clicked();

    void on_studentTable_cellChanged(int row, int column);

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QString studentFileName = "studentList.csv";
    QString courseFileName = "courseList.csv";
    bool init = true;
    bool ascend = true;
};

#endif // MAINWINDOW_H
