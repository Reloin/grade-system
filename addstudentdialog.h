#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class addStudentDialog;
}

class addStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addStudentDialog(QWidget *parent = nullptr);
    ~addStudentDialog();
    QString getName();
    char getSex();
    QString getID();

private:
    Ui::addStudentDialog *ui;
};


#endif // ADDSTUDENTDIALOG_H
