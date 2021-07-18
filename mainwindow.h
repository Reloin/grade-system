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
        QString sex;
    public:
        student();
        student(QString const &name,QString const &sex, QString const &id);
        void setName(QString const &name);
        void setSex(QString const &sex);
        void setId(QString const &id);
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
    QMap<QString, float> grade;
    compulsory(QString const &name, QString const id, float c);
    compulsory();
    //~compulsory();
    void insertGradeByID(QString const &id, float g);
    QStringList getGrades();
    float getGrade(QString const &id);
};

class elective : public course
{
private:
    QMap<QString, QString> grade;//p or f
public:
    elective(QString const &name, QString const &id, float c);
    void insertGradeByID(QString const &id, QString const &g);
    QString EgetGrade(QString const &id);
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

private:
    Ui::MainWindow *ui;
    QString studentFileName = "studentList.csv";
    QString compulsoryFileName = "compulsoryList.csv";
    QString electiveFileName = "electiveList.csv";
    bool init = true;
};

#endif // MAINWINDOW_H
