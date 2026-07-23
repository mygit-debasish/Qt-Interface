#include "customplaintextedit.h"
#include <QDebug>

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent)
  :QPlainTextEdit(parent)
{
  qDebug()<<"Constructor: "<<__PRETTY_FUNCTION__;
}

void CustomPlainTextEdit::keyPressEvent(QKeyEvent *event)
{

  if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
    {
      qDebug()<<"ENTER pressed ..";
      emit EnterPresssedTextEdit();
    }
  QPlainTextEdit::keyPressEvent(event);
}
