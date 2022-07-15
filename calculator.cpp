#include "calculator.h"
#include "./ui_calculator.h"
#include "button.h"
#include "iterstack.h"
#include "bigint.h"
#include <string>
#include <iostream>
#include <QDebug>
#include <QtMath>
#include <QLineEdit>
#include <QGridLayout>
using namespace std;

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculator)
{
    display = new QLineEdit("");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignLeft);
    display->setMaxLength(20);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);


    for(int i = 0; i < NumDigitButtons;++i){
        digitButtons[i] = createButton(QString::number(i),SLOT(digitClicked()));
    }

    auto addButton = createButton("+",SLOT(operClicked()));

    //Layout-------------------------
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display,0,0,1,6);
    for(int i = 1; i < NumDigitButtons;++i){
        int row = ((9-i)/3)+2;
        int col = ((i-1)%3)+1;
        mainLayout->addWidget(digitButtons[i],row,col);
    }
    mainLayout->addWidget(addButton,5,1);
    setLayout(mainLayout);
    setWindowTitle(tr("Calculator"));
    //ui->setupUi(this);
}

void Calculator::digitClicked(){
    //Take the object that emits the signal and get the button Object
    myButton *clickedButton = qobject_cast<myButton *>(sender());
    //Get the text from the button
    int digitValue = clickedButton->text().toInt();
    //We create a new bigint
    bigint newbig = bigint(static_cast<long>(digitValue));
    cout << newbig; //Debugging output
    cout.flush();
    //Now we want to add the bigint to the stack
    //(Add bigint to stack)
    operand_stack.push(newbig);
    //And we add the digit to the display GUI
    display->setText(display->text().append(QString::number(digitValue)));
}

void Calculator::do_arith(const char oper){
    if(operand_stack.size() < 2) throw invalid_argument("stack empty");
    //We pop both numbers off the stack
    bigint right = operand_stack.top();
    operand_stack.pop();
    bigint left = operand_stack.top();
    operand_stack.pop();
    bigint res;
    //Now we call the operator that we defined in each class
    switch(oper){
        case '+': res = left+right; break;
        case '-': res = left-right; break;
        default : throw invalid_argument(""s+oper);
    }
    operand_stack.push(res);
    cout << "res = " << res;
}

void Calculator::operClicked(){
    auto clickedButton = qobject_cast<myButton *>(sender());
    char oper = (clickedButton->text().toStdString())[0];

    switch(oper){
        case '+': cout << "plus clicked\n"; do_arith(oper); break;
        case '-': cout << "sub clicked\n"; break;
        default : cout << "No Operation matches\n" << (oper);
    }
    cout.flush();
}

Calculator::~Calculator()
{
    delete ui;
}


myButton *Calculator::createButton(const QString &text, const char *member){
    myButton *button = new myButton(text);
    connect(button,SIGNAL(clicked()),
            this,member);
    return button;
}

