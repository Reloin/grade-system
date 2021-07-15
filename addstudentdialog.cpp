#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

#include <QDebug>

addStudentDialog::addStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addStudentDialog)
{
    ui->setupUi(this);
}

addStudentDialog::~addStudentDialog()
{
    delete ui;
}

QString addStudentDialog::getName()
{
    return ui->studentNameBox->text();
}

QString addStudentDialog::getSex()
{
    switch (ui->genderBox->currentIndex()) {
    case 1: return "M";
    case 2: return "F";
    case 3: return "O";
    default: return "O";
    }
}

QString addStudentDialog::getID()
{
    return ui->idTextBox->text();
}
