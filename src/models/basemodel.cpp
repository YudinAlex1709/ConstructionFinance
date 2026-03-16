#include "basemodel.h"

BaseModel::BaseModel(QObject *parent)
    : QObject(parent)
    , m_createdAt(QDateTime::currentDateTime())
    , m_updatedAt(QDateTime::currentDateTime())
{
}
