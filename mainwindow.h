#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


struct HistoryNode {
    QString data;
    HistoryNode* next;
    HistoryNode* prev;
    
    HistoryNode(const QString& value) : data(value), next(nullptr), prev(nullptr) {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    
    HistoryNode* undoHead;
    HistoryNode* undoCurrent;
    HistoryNode* redoHead;
    
    double firstNum;
    bool userTypeSecNum;

    void saveState();
    void addToUndoList(const QString& state);
    void clearRedoList();
    void clearUndoList();

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