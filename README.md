# Construction Finance

Приложение для управления финансами и статистикой строительных компаний на Qt/QML/C++ с 3D визуализацией.

## Возможности

### 📊 Финансовый учет
- Учет доходов и расходов по проектам
- Управление банковскими счетами
- Категоризация операций
- Планирование бюджетов

### 🏗️ Управление проектами
- Создание и отслеживание строительных проектов
- Этапы строительства
- Контроль бюджета и сроков
- Привязка контрагентов

### 📈 Аналитика и отчеты
- Дашборд с ключевыми показателями
- Отчеты P&L (Прибыли и убытки)
- ДДС (Движение денежных средств)
- Бюджет vs Факт
- Экспорт в PDF и Excel

### 🎨 3D Визуализация
- Просмотр 3D моделей зданий
- Тепловые карты финансовых показателей
- Навигация по этажам
- Интеграция с BIM/IFC

### 👥 Управление пользователями
- Ролевая модель доступа
- Аутентификация
- Настройки компании

## Технологический стек

- **Язык**: C++17
- **UI Framework**: Qt6 QML + Qt Quick Controls (Material Design)
- **3D Графика**: Qt3D + OpenGL
- **База данных**: SQLite (локальная) / PostgreSQL (сетевая)
- **Графики**: Qt Charts
- **Сборка**: qmake + GCC/MinGW/Clang

## Системные требования

- Qt 6.2 или выше
- Компилятор с поддержкой C++17
- OpenGL 3.3+ (для 3D визуализации)

## Установка

### Linux (Ubuntu/Debian)

```bash
# Установка зависимостей
sudo apt-get update
sudo apt-get install build-essential qt6-base-dev qt6-declarative-dev \
    qt6-charts-dev qt6-3d-dev libqt6sql6-sqlite

# Клонирование репозитория
git clone https://github.com/yourcompany/construction-finance.git
cd construction-finance

# Сборка
qmake ConstructionFinance.pro
make -j$(nproc)

# Запуск
./ConstructionFinance
```

### Windows (MinGW)

```cmd
:: Установите Qt6 с компонентами: Qt Quick, Qt Charts, Qt 3D, MinGW

:: Откройте Qt Command Prompt
cd construction-finance
qmake ConstructionFinance.pro
mingw32-make -j4

:: Деплой
windeployqt --qmldir qml release/ConstructionFinance.exe
```

### macOS

```bash
# Установка Qt6 через Homebrew
brew install qt@6

# Сборка
cd construction-finance
qmake ConstructionFinance.pro
make -j$(sysctl -n hw.ncpu)

# Создание .app bundle
macdeployqt ConstructionFinance.app -qmldir=qml
```

## Структура проекта

```
ConstructionFinance/
├── ConstructionFinance.pro      # Главный .pro файл
├── src/
│   ├── main.cpp                 # Точка входа
│   ├── app/                     # Приложение и настройки
│   ├── models/                  # C++ модели данных
│   ├── database/                # Работа с БД
│   ├── managers/                # Бизнес-логика
│   ├── utils/                   # Утилиты
│   └── threed/                  # 3D модуль
├── qml/
│   ├── main.qml                 # Главное окно
│   ├── components/              # UI компоненты
│   ├── views/                   # Экраны приложения
│   ├── dialogs/                 # Диалоговые окна
│   └── styles/                  # Стили
├── resources/                   # Ресурсы (иконки, переводы)
└── tests/                       # Тесты
```

## Архитектура

### Модели данных
- `Project` - Проект/объект строительства
- `Transaction` - Финансовая операция
- `Contractor` - Контрагент (заказчик, поставщик, субподрядчик)
- `User` - Пользователь системы
- `BankAccount` - Банковский счет
- `Category` - Категория доходов/расходов
- `Material` - Материалы и закупки

### Менеджеры
- `ProjectManager` - Управление проектами
- `FinanceManager` - Финансовые операции
- `ReportManager` - Генерация отчетов
- `UserManager` - Управление пользователями

### 3D Модуль
- `Scene3D` - 3D сцена
- `BuildingModel` - Модель здания
- `CameraController` - Управление камерой

## Использование

### Первый запуск

1. При первом запуске создается пользователь `admin@construction.finance` с паролем `admin`
2. Войдите в систему и смените пароль
3. Создайте компанию и настройте параметры

### Создание проекта

1. Перейдите в раздел "Проекты"
2. Нажмите "Новый проект"
3. Заполните информацию о проекте
4. Установите бюджет и сроки

### Добавление операции

1. Перейдите в раздел "Операции" или "Финансы"
2. Нажмите "Новая операция"
3. Выберите тип (доход/расход)
4. Укажите сумму и категорию
5. Привяжите к проекту (опционально)

### Просмотр 3D модели

1. Откройте проект
2. Нажмите на иконку 3D
3. Используйте мышь для навигации:
   - Левая кнопка - вращение
   - Правая кнопка - панорамирование
   - Колесико - масштабирование

## Лицензия

MIT License

## Контакты

Для вопросов и предложений: support@construction.finance
