#include "timeeditdelegate.h"

#include <QTimeEdit>

TimeEditDelegate::TimeEditDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *TimeEditDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    QTimeEdit *editor = new QTimeEdit(parent);
    editor->setFrame(false);
    return editor;
}

void TimeEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    QString value = index.data(Qt::EditRole).toString();
    // qDebug() << value;
    if (value.isEmpty())
    {
        timeEdit->setEnabled(false);
    }
    else
    {
        QTime time = QTime::fromString(value, "hh:mm");
        // qDebug() << time;
        timeEdit->setEnabled(true);
        timeEdit->setTime(time);
    }
}

void TimeEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    if (timeEdit->isEnabled())
    {
        QString value = timeEdit->time().toString("hh:mm");
        // qDebug() << value;
        model->setData(index, value, Qt::EditRole);
    }
    else
    {
        model->setData(index, "", Qt::EditRole);
    }
}

void TimeEditDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
