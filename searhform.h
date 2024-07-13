#ifndef SEARHFORM_H
#define SEARHFORM_H

#include <QMainWindow>

namespace Ui {
class SearhForm;
}

class SearhForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearhForm(QWidget *parent = nullptr);
    ~SearhForm();

signals:
    void searchTextEntered(const QString &text);

private slots:
    void on_searchButton_clicked();

private:
    Ui::SearhForm *ui;
};

#endif // SEARHFORM_H
