#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStack<QString> undoStack;
    QStack<QString> redoStack;
    void saveState();

private slots:
    void press_num();
    void on_point_released();
    void unary_operation_pressed();
    void on_clr_released();
    void on_equals_released();
    void binary_operation_pressed();
    void on_undo_released();
    void on_redo_released();
};
#endif
