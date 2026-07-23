#ifndef CUSTONPLAINTEXTEDIT_H
#define CUSTONPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class CustomPlainTextEdit: public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit CustomPlainTextEdit(QWidget *parent = nullptr);

signals:
  void EnterPresssedTextEdit();

protected:
  void keyPressEvent(QKeyEvent *event) override;
};
#endif // CUSTONPLAINTEXTEDIT_H
