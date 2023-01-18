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
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    QTime time = QTime::fromString(value, "h:mm");
    qDebug() << time;
    timeEdit->setTime(time);
}

void TimeEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    QString value = timeEdit->time().toString("h:mm");
    qDebug() << value;
    model->setData(index, value, Qt::EditRole);
}

void TimeEditDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
