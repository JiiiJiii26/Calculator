#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , undoHead(nullptr)
    , undoCurrent(nullptr)
    , redoHead(nullptr)
    , firstNum(0)
    , userTypeSecNum(false)
{
    ui->setupUi(this);

    connect(ui->one,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->two,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->three,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->four,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->five,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->six,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->Seven,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->eight,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->nine,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->Zero,SIGNAL(released()),this,SLOT(press_num()));
    connect(ui->nega,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
    connect(ui->percent,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
    connect(ui->plus,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->minus,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->mult,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->divide,SIGNAL(released()),this,SLOT(binary_operation_pressed()));

    // Connect undo and redo buttons
    connect(ui->undo, SIGNAL(released()), this, SLOT(on_undo_released()));
    connect(ui->redo, SIGNAL(released()), this, SLOT(on_redo_released()));

    ui->plus->setCheckable(true);
    ui->minus->setCheckable(true);
    ui->mult->setCheckable(true);
    ui->divide->setCheckable(true);

    // Save initial state to linked list
    saveState();
}

MainWindow::~MainWindow()
{
    // Clean up linked lists
    clearUndoList();
    clearRedoList();
    delete ui;
}

void MainWindow::clearUndoList()
{
    HistoryNode* current = undoHead;
    while (current != nullptr) {
        HistoryNode* next = current->next;
        delete current;
        current = next;
    }
    undoHead = undoCurrent = nullptr;
}

void MainWindow::clearRedoList()
{
    HistoryNode* current = redoHead;
    while (current != nullptr) {
        HistoryNode* next = current->next;
        delete current;
        current = next;
    }
    redoHead = nullptr;
}

void MainWindow::addToUndoList(const QString& state)
{
    HistoryNode* newNode = new HistoryNode(state);
    
    if (undoHead == nullptr) {
        // First node in the list
        undoHead = undoCurrent = newNode;
    } else {
        // Add to the end of the list (most recent)
        HistoryNode* current = undoHead;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
        undoCurrent = newNode;
    }
}

void MainWindow::saveState()
{
    QString currentState = ui->label->text();
    
    // Add to undo linked list
    addToUndoList(currentState);
    
    // Clear redo list when new operation is performed
    clearRedoList();
    
    // Update button states
    ui->undo->setEnabled(undoHead != nullptr && undoCurrent != undoHead);
    ui->redo->setEnabled(redoHead != nullptr);
}

void MainWindow::press_num(){
    // Save state before number input
    if (!userTypeSecNum) {
        saveState();
    }

    QPushButton * button = (QPushButton*)sender();
    double labelNum;
    QString newLabel;

    if((ui->plus->isChecked() || ui->minus->isChecked() ||
         ui->mult->isChecked() || ui->divide->isChecked()) && (!userTypeSecNum)){
        labelNum = button->text().toDouble();
        userTypeSecNum = true;
        newLabel = QString::number(labelNum,'g',15);
    } else {
        if(ui->label->text().contains('.') && button->text() == "0"){
            newLabel = ui->label->text() + button->text();
        } else {
            labelNum = (ui->label->text() + button->text()).toDouble();
            newLabel = QString::number(labelNum,'g',15);
        }
    }
    ui->label->setText(newLabel);
}

void MainWindow::on_point_released()
{
    saveState();
    ui->label->setText(ui->label->text() + ".");
}

void MainWindow::unary_operation_pressed(){
    saveState();

    double labelNum;
    QString newLabel;
    QPushButton * button = (QPushButton*) sender();
    if(button->text() == "+/-"){
        labelNum = ui->label->text().toDouble();
        labelNum = labelNum * -1;
        newLabel = QString::number(labelNum,'g', 15);
        ui->label->setText(newLabel);
    }
    if(button->text() == "%"){
        labelNum = ui->label->text().toDouble();
        labelNum = labelNum * 0.01;
        newLabel = QString::number(labelNum,'g', 15);
        ui->label->setText(newLabel);
    }
}

void MainWindow::on_clr_released()
{
    saveState();

    ui->plus->setChecked(false);
    ui->minus->setChecked(false);
    ui->mult->setChecked(false);
    ui->divide->setChecked(false);

    userTypeSecNum = false;
    ui->label->setText("0");
}

void MainWindow::on_equals_released()
{
    saveState();

    double labelNum, secondNum;
    QString newLabel;
    secondNum = ui->label->text().toDouble();

    if(ui->plus->isChecked()){
        labelNum = firstNum + secondNum;
        newLabel = QString::number(labelNum,'g', 15);
        ui->label->setText(newLabel);
        ui->plus->setChecked(false);
    }
    else if(ui->minus->isChecked()){
        labelNum = firstNum - secondNum;
        newLabel = QString::number(labelNum,'g', 15);
        ui->label->setText(newLabel);
        ui->minus->setChecked(false);
    }
    else if(ui->divide->isChecked()){
        if (secondNum == 0) {
            ui->label->setText("Error");
        } else {
            labelNum = firstNum / secondNum;
            newLabel = QString::number(labelNum,'g', 15);
            ui->label->setText(newLabel);
        }
        ui->divide->setChecked(false);
    }
    else if(ui->mult->isChecked()){
        labelNum = firstNum * secondNum;
        newLabel = QString::number(labelNum,'g', 15);
        ui->label->setText(newLabel);
        ui->mult->setChecked(false);
    }
    userTypeSecNum = false;
}

void MainWindow::binary_operation_pressed(){
    saveState();

    QPushButton * button = (QPushButton*)sender();
    firstNum = ui->label->text().toDouble();
    button->setChecked(true);
}

// Undo functionality using linked list
void MainWindow::on_undo_released()
{
    if (undoCurrent != nullptr && undoCurrent->prev != nullptr) {
        // Move current state to redo list
        HistoryNode* redoNode = new HistoryNode(undoCurrent->data);
        if (redoHead == nullptr) {
            redoHead = redoNode;
        } else {
            redoNode->next = redoHead;
            redoHead->prev = redoNode;
            redoHead = redoNode;
        }
        
        // Move to previous state in undo list
        undoCurrent = undoCurrent->prev;
        ui->label->setText(undoCurrent->data);
        
        // Update button states
        ui->undo->setEnabled(undoCurrent->prev != nullptr);
        ui->redo->setEnabled(true);
    }
}

// Redo functionality using linked list
void MainWindow::on_redo_released()
{
    if (redoHead != nullptr) {
        // Get the state from redo list
        QString state = redoHead->data;
        
        // Add to undo list
        addToUndoList(state);
        
        // Remove from redo list
        HistoryNode* temp = redoHead;
        redoHead = redoHead->next;
        if (redoHead != nullptr) {
            redoHead->prev = nullptr;
        }
        delete temp;
        
        // Update display
        ui->label->setText(state);
        
        // Update button states
        ui->undo->setEnabled(true);
        ui->redo->setEnabled(redoHead != nullptr);
    }
}