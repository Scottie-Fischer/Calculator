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
#include <QSizePolicy>
using namespace std;

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculator)
{
    //------------Display Lines-----------------
    stackDisplay = new QLineEdit("stack->");
    stackDisplay->setReadOnly(true);
    stackDisplay->setAlignment(Qt::AlignLeft);
    stackDisplay->setMaxLength(20);

    display = new QLineEdit("");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignLeft);
    display->setMaxLength(20);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);
    //--------------------------------------------

    //---------------Buttons----------------------
    for(int i = 0; i < NumDigitButtons;++i){
        digitButtons[i] = createButton(QString::number(i),SLOT(digitClicked()));
    }

    //The Operation Buttons
    auto addButton = createButton("+",SLOT(operClicked()));
    auto subButton = createButton("-",SLOT(operClicked()));
    auto divButton = createButton("/",SLOT(operClicked()));
    auto multButton = createButton("*",SLOT(operClicked()));

    //Helper Buttons
    auto clearButton = createButton("clear",SLOT(clearClicked()));
    auto pushButton = createButton("push",SLOT(pushClicked()));
    auto signButton = createButton("+/-",SLOT(signClicked()));
    //--------------------------------------------
    //Layout-------------------------
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display,1,0,1,6);
    mainLayout->addWidget(stackDisplay,0,0,1,6);
    for(int i = 1; i < NumDigitButtons;++i){
        int row = ((9-i)/3)+3;
        int col = ((i-1)%3)+1;
        mainLayout->addWidget(digitButtons[i],row,col);
    }

    mainLayout->addWidget(addButton,3,4);
    mainLayout->addWidget(subButton,4,4);
    mainLayout->addWidget(multButton,5,4);
    mainLayout->addWidget(divButton,6,4);

    mainLayout->addWidget(clearButton,6,2);
    mainLayout->addWidget(pushButton,6,3);
    mainLayout->addWidget(signButton,6,1);


    setLayout(mainLayout);
    setWindowTitle(tr("Calculator"));
    //ui->setupUi(this);
}

string Calculator::print_stack(){
    string stack = "stack->";
    const bigint beg = *operand_stack.begin();
    for(auto& c :operand_stack){
        stack.append(c.toString());
        if(!(c == beg)){stack.append("->");}
    }
    return stack;
}

void Calculator::signClicked(){
    QString curr;
    if(is_neg){
        is_neg = !is_neg;
        curr = display->text();
        curr.remove("-");
    }else{
        is_neg = !is_neg;
        curr = display->text();
        curr.insert(0,"-");
    }
    display->setText(curr);
}

void Calculator::clearClicked(){
    display->clear();
    is_neg = false;
}

void Calculator::pushClicked(){
    if(not display->text().isEmpty()){
        QString txt = display->text();
        QString stack_txt = stackDisplay->text();
        stack_txt.insert(7,txt);
        stack_txt.insert(7+txt.length(),"->");
        stackDisplay->setText(stack_txt);

        int int_txt = txt.toInt();
        bigint newbigint = bigint(int_txt);
        operand_stack.push(newbigint);
    }
    display->clear();
    is_neg = false;
}

void Calculator::digitClicked(){
    //Take the object that emits the signal and get the button Object
    myButton *clickedButton = qobject_cast<myButton *>(sender());
    //Get the text from the button
    int digitValue = clickedButton->text().toInt();
    //Add the digit to the display GUI
    display->setText(display->text().append(QString::number(digitValue)));
}

void Calculator::do_arith(const char oper){
    if(operand_stack.size() >= 2){
        //We pop both numbers off the stack

        bigint right = operand_stack.top();
        operand_stack.pop();

        bigint left = operand_stack.top();
        operand_stack.pop();

        bigint res;

        //Now we call the operator that we defined in each class
        switch(oper){
            case '+': res = left + right; break;
            case '-': res = left - right; break;
            case '*': res = left * right; break;
            case '/': res = left / right; break;
            default : throw invalid_argument(""s+oper);
        }
        //Push Result to Stack
        operand_stack.push(res);

        //Update Display
        QString new_stack = QString::fromStdString(print_stack());
        stackDisplay->setText(new_stack);

    }else{
        cout << "too few arguments on stack:"
             << operand_stack.size();
    }
}

void Calculator::operClicked(){
    auto clickedButton = qobject_cast<myButton *>(sender());
    char oper = (clickedButton->text().toStdString())[0];

    switch(oper){
        case '+': do_arith(oper); break;
        case '-': do_arith(oper); break;
        case '*': do_arith(oper); break;
        case '/': do_arith(oper); break;
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

