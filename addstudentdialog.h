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
    QString getSex();
    QString getID();

private slots:
    void on_buttonBox_rejected();

private:
    Ui::addStudentDialog *ui;
};


#endif // ADDSTUDENTDIALOG_H
