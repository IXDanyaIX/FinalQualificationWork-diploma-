#include "searhform.h"
#include "ui_searhform.h"

SearhForm::SearhForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearhForm)
{
    ui->setupUi(this);
}

SearhForm::~SearhForm()
{
    delete ui;
}

void SearhForm::on_searchButton_clicked()
{
    emit searchTextEntered(ui->searchLineEdit->text());

}

