#include "comboboxdelegate.h"

#include <QComboBox>
#include <QSettings>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem & option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *editor = new QComboBox(parent);
    QSettings opciones;
    QStringList contrataciones = opciones.value("contrataciones").toString().split('\n');
    for (QString& contratacion : contrataciones)
    {
        editor->addItem(contratacion);
    }
    editor->setEditable(true);
    editor->setFrame(false);

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.data(Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(value);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
