#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include "bigint.h"
#include "iterstack.h"

using bigint_stack = iterstack<bigint>;

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE
class myButton;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    //This is the slot that we created for myButton class
    //This handles when a digit is clicked
    void digitClicked();
    void operClicked();
    void pushClicked();
    void clearClicked();
    void signClicked();

private:

    bool is_neg = false;
    QLineEdit *display;
    QLineEdit *stackDisplay;
    Ui::Calculator *ui;

    myButton *createButton(const QString &text, const char *member);

    enum {NumDigitButtons = 10};

    myButton *digitButtons[NumDigitButtons];

    //This will store operands(digits) to be executed
    bigint_stack operand_stack;

    //Helper functions
    void do_arith(const char oper);
    string print_stack();
};
#endif // CALCULATOR_H
