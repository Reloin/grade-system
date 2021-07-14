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

char addStudentDialog::getSex()
{
    switch (ui->genderBox->currentIndex()) {
    case 0: return 'M';
    case 1: return 'F';
    case 2: return 'O';
    default: return 'O';
    }
}

QString addStudentDialog::getID()
{
    return ui->idTextBox->text();
}
