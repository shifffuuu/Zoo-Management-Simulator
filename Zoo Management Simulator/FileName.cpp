#include <iostream>
#include <list>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <random>

using namespace std;

// Класс Animal — описание животного
class Animal {
public:
    string name;
    int age;
    int weight;
    enum Climate { Desert, Forest, Artic, Ocean } climate;
    bool isCarnivore;
    int happiness;
    enum Gender { Male, Female } gender;
    string species;
    string parent1Name;
    string parent2Name;
    bool isSick;

    // Конструктор с минимумом параметров
    Animal(string n, int a, int w, Climate c, bool carn, Gender g, string s)
        : name(n), age(a), weight(w), climate(c), isCarnivore(carn), happiness(70), gender(g), species(s), parent1Name("Неизвестно"), parent2Name("Неизвестно"), isSick(false) {
    }
    // Конструктор с указанием родителей
    Animal(string n, int a, int w, Climate c, bool carn, Gender g, string s, string p1, string p2)
        : name(n), age(a), weight(w), climate(c), isCarnivore(carn), happiness(70), gender(g), species(s), parent1Name(p1), parent2Name(p2), isSick(false) {
    }

    // Расчет стоимости животного
    int PriceAnimal() const {
        int basePrice = 100;
        int price = basePrice;
        price += weight * 2;
        price -= age * 5;
        price += isCarnivore ? 100 : 0;
        price += static_cast<int>(climate) * 50;
        return max(price, 10);
    }

    // Обновление уровня счастья в зависимости от кормления, количества вольера и болезни
    void updateHappiness(bool isFed, int enclosurePopulation) {
        if (isFed) {
            happiness += 10;
        }
        else {
            happiness -= 20;
        }

        if (enclosurePopulation > 1) {
            happiness += 15;
        }
        else {
            happiness -= 40;
        }

        if (isSick) {
            happiness -= 30;
        }

        happiness = max(0, min(100, happiness));
    }

    // Проверка, сбежит ли животное из-за низкого счастья
    bool checkEscape() const {
        if (happiness <= 20) {
            return (rand() % 100) < 50; // 50% шанс сбежать при низком счастье
        }
        return false;
    }
    // Переименование животного
    void rename(const string& newName) {
        name = newName;
    }
    // Проверка, умерет ли животное из-за старости
    bool checkOldAgeDeath() const {
        if (age > 50) {
            return (rand() % 100) < (age - 50); // Вероятность смерти увеличивается с возрастом
        }
        return false;
    }
    // Оператор для размножения животных — создает детеныша
    Animal operator+(const Animal& other) const {
        // Проверки на возраст, пол, климат и тип питания
        if (age < 5 || other.age < 5) {
            throw runtime_error("Животные слишком молоды для размножения.");
        }
        if (gender == other.gender) {
            throw runtime_error("Нельзя размножать однополых животных.");
        }
        if (climate != other.climate) {
            throw runtime_error("Нельзя размножать животных из разных климатических зон.");
        }
        if (isCarnivore != other.isCarnivore) {
            throw runtime_error("Нельзя размножать хищников с травоядными.");
        }
        // Название малыша и параметры
        string babyName = "Детеныш " + species + "-" + other.species;
        int babyAge = 0;
        int babyWeight = (weight + other.weight) / 4;
        Gender babyGender = (rand() % 2 == 0) ? Male : Female;
        string parent1 = name;
        string parent2 = other.name;

        // Создаем гибридное название вида
        string hybridSpecies;
        if (rand() % 2 == 0) {
            hybridSpecies = species.substr(0, species.length() / 2) + other.species.substr(other.species.length() / 2);
        }
        else {
            hybridSpecies = other.species.substr(0, other.species.length() / 2) + species.substr(species.length() / 2);
        }

        return Animal(babyName, babyAge, babyWeight, climate, isCarnivore, babyGender, hybridSpecies, parent1, parent2);
    }
};

// Класс Aviary — описание вольера
class Aviary {
public:
    Animal::Climate climat;
    int vmestimost;
    list<Animal> animals;
    int daytrati;
    int level;
    int sickAnimalsCount;
    bool isClean;
    bool isFed;

    // Конструктор
    Aviary(Animal::Climate c, int cap)
        : climat(c), vmestimost(cap), level(1), sickAnimalsCount(0), isClean(true), isFed(false) {
        daytrati = calculateDailyCost(); // расходы в день 
    }

    // Проверка, можно ли добавить животное в вольер (по климату и типу питания)
    bool volerskan(const Animal& animal) {
        if (animals.size() >= vmestimost) return false;
        if (animal.climate != climat) return false;
        if (!animals.empty()) {
            if (animals.front().isCarnivore != animal.isCarnivore) return false;
        }
        return true;
    }

    // Добавление животного в вольер
    void volerskanback(const Animal& animal) {
        if (volerskan(animal)) {
            animals.push_back(animal);
            if (animal.isSick) {
                sickAnimalsCount++;
                cout << "Уведомление: Животное " << animal.name << " добавлено в вольер больным!\n";
            }
        }
        else {
            cout << "Невозможно добавить животное в вольер (неподходящие условия или нет места).\n";
        }
    }
    // Удаление животного по имени
    void removeAnimal(const string& name) {
        for (auto it = animals.begin(); it != animals.end(); ++it) {
            if (it->name == name) {
                if (it->isSick) {
                    sickAnimalsCount--;
                }
                animals.erase(it);
                return;
            }
        }
    }

    // Расчет стоимости вольера
    int calculateCost() const {
        int baseCost = 100;
        int cost = baseCost;
        cost += vmestimost * 10;
        cost += static_cast<int>(climat) * 50;
        return max(cost, 150);
    }
    // Расчет ежедневных расходов на вольер
    int calculateDailyCost() const {
        int baseDailyCost = 10;
        int dailyCost = baseDailyCost;
        dailyCost += vmestimost / 10;
        dailyCost += static_cast<int>(climat) * 5;
        return max(dailyCost, 10);
    }

    // Обновление состояния вольера и животных (инфекции, смерть, счастье)
    vector<string> updateAviaryState() {
        vector<string> notifications;
        int initialSickCount = sickAnimalsCount;
        int totalAnimals = animals.size();

        // Распространение инфекции
        if (initialSickCount > 0 && totalAnimals > 0) {
            vector<int> susceptibleIndices;
            for (int i = 0; i < animals.size(); ++i) {
                auto it = animals.begin();
                advance(it, i);
                if (!it->isSick) {
                    susceptibleIndices.push_back(i);
                }
            }

            // Перемешивание и заражение части животных
            random_device rd;
            mt19937 g(rd());
            shuffle(susceptibleIndices.begin(), susceptibleIndices.end(), g);

            int infectionsToCause = initialSickCount * 2;
            int actuallyInfected = 0;

            for (int i = 0; i < min((int)susceptibleIndices.size(), infectionsToCause); ++i) {
                auto it = animals.begin();
                advance(it, susceptibleIndices[i]);
                it->isSick = true;
                sickAnimalsCount++;
                actuallyInfected++;
                notifications.push_back("Животное \"" + it->name + "\" заразилось тиаравирусом!");
            }
        }

        // Умершие от болезни
        vector<string> diedFromSickness;
        if (totalAnimals > 0 && (double)sickAnimalsCount / totalAnimals > 0.5) {
            int deathsToCause = sickAnimalsCount / 2;
            for (int i = 0; i < deathsToCause && sickAnimalsCount > 0; ++i) {
                vector<int> sickIndices;
                for (int j = 0; j < animals.size(); ++j) {
                    auto it = animals.begin();
                    advance(it, j);
                    if (it->isSick) {
                        sickIndices.push_back(j);
                    }
                }

                if (!sickIndices.empty()) {
                    int randomIndex = sickIndices[rand() % sickIndices.size()];
                    auto it = animals.begin();
                    advance(it, randomIndex);
                    diedFromSickness.push_back(it->name + " (от тиаравируса)");
                    it = animals.erase(it);
                    sickAnimalsCount--;
                }
            }
            if (!diedFromSickness.empty()) {
                notifications.push_back("В вольере климата \"" + getClimateName() + "\" умерло от болезни: " + to_string(diedFromSickness.size()) + " животных.");
            }
        }

        isFed = false;
        isClean = false;

        return notifications;
    }

    // Вылечить до maxToHeal животных
    int healAnimals(int maxToHeal) {
        int healedCount = 0;
        for (auto& animal : animals) {
            if (animal.isSick && healedCount < maxToHeal) {
                animal.isSick = false;
                sickAnimalsCount--;
                healedCount++;
                cout << "Животное \"" << animal.name << "\" вылечено!\n";
            }
        }
        return healedCount;
    }

    // Уборка вольера
    void cleanAviary() {
        isClean = true;
        for (auto& animal : animals) {
            animal.happiness = min(100, animal.happiness + 10);
        }
        cout << "Вольер с климатом \"" << getClimateName() << "\" очищен.\n";
    }

    // Покормить вольер, если есть еды
    bool feedAviary(int& availableFood) {
        int requiredFood = animals.size();
        if (availableFood >= requiredFood) {
            availableFood -= requiredFood;
            isFed = true;
            cout << "Вольер с климатом \"" << getClimateName() << "\" накормлен.\n";
            return true;
        }
        else {
            cout << "Недостаточно еды для кормления вольера с климатом \"" << getClimateName() << "\".\n";
            return false;
        }
    }

    // Улучшение вольера — увеличение вместимости и снижение ежедневных расходов
    bool upgrade(int baseUpgradeCost) {
        if (level >= 3) {
            cout << "Достигнут максимальный уровень улучшения!\n";
            return false;
        }
        vmestimost *= 2;
        daytrati += calculateDailyCost() / 2;
        level++;
        return true;
    }

    // Получение названия климатической зоны
    string getClimateName() const {
        switch (climat) {
        case Animal::Desert: return "Пустыня";
        case Animal::Forest: return "Лес";
        case Animal::Artic: return "Арктика";
        case Animal::Ocean: return "Океан";
        default: return "Неизвестный";
        }
    }
};

// описание работника
class Employee {
public:
    string name;
    enum Position { Director, Cleaner, Veterinarian, Feeder } position;
    int salary;
    int maxCapacity;
    vector<Aviary*> assignedAviaries;
    vector<Animal*> assignedAnimals;

    Employee(string n, Position pos, int sal, int max)
        : name(n), position(pos), salary(sal), maxCapacity(max) {
    }

    string getPositionName() const {
        switch (position) {
        case Director: return "Директор";
        case Cleaner: return "Уборщик";
        case Veterinarian: return "Ветеринар";
        case Feeder: return "Кормилец";
        default: return "Неизвестно";
        }
    }
};
//Основной класс зоопарка
class Zoo {
public:
    string name;
    int money, food, popularity;
    int day;
    list<Aviary> enclosures;
    list<Employee> employees;

    Zoo(string n, int initialMoney)
        : name(n), money(initialMoney), food(0), popularity(50), day(1) {
    }

    // Переход к следующему дню
    void nextDay() {
        cout << "\n--- День " << day << " ---\n";
        cout << "Бюджет прошлого дня: " << money << " монет\n";

        // Расчет дохода
        int visitors = max(0, 2 * popularity);
        int totalAnimals = getTotalAnimals();
        int income = visitors * totalAnimals;

        cout << "Посетители сегодня: " << visitors << "\n";
        cout << "Доход за день: +" << income << " монет\n";

        // Обновление бюджета
        money += income;

        // Выплаты зарплаты
        for (auto& emp : employees) {
            money -= emp.salary;
        }

        // Расходы на содержание вольеров
        for (auto& enc : enclosures) {
            money -= enc.daytrati;
        }

        // Обработка смертей из-за старости
        vector<string> deadAnimals;
        for (auto& enc : enclosures) {
            for (auto it = enc.animals.begin(); it != enc.animals.end(); ) {
                it->age++;
                if (it->checkOldAgeDeath()) {
                    deadAnimals.push_back(it->name + " (от старости)");
                    if (it->isSick) {
                        enc.sickAnimalsCount--;
                    }
                    it = enc.animals.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        // Обработка голода и болезней
        vector<string> aviaryNotifications;
        for (auto& enc : enclosures) {
            if (!enc.isFed && enc.animals.size() > 0) {
                int deficit = enc.animals.size();
                for (auto it = enc.animals.begin(); it != enc.animals.end() && deficit > 0;) {
                    if (rand() % 2 == 0) {
                        deadAnimals.push_back(it->name + " (от голода)");
                        if (it->isSick) {
                            enc.sickAnimalsCount--;
                        }
                        it = enc.animals.erase(it);
                        deficit--;
                    }
                    else {
                        ++it;
                    }
                }
                if (enc.animals.empty()) {
                    aviaryNotifications.push_back("В вольере климата \"" + enc.getClimateName() + "\" все животные умерли от голода.");
                }
                else if (deficit > 0) {
                    aviaryNotifications.push_back("В вольере климата \"" + enc.getClimateName() + "\" умерло " + to_string(deficit) + " животных от голода.");
                }
            }

            // Обновление счастья животных
            int population = enc.animals.size();
            for (auto& animal : enc.animals) {
                animal.updateHappiness(enc.isFed, population);
            }

            // Обработка инфекции и сбежавших
            vector<string> currentAviaryNotifications = enc.updateAviaryState();
            aviaryNotifications.insert(aviaryNotifications.end(), currentAviaryNotifications.begin(), currentAviaryNotifications.end());

            // Животные сбегают при низком счастье
            vector<string> escapedAnimalsInAviary;
            for (auto it = enc.animals.begin(); it != enc.animals.end(); ) {
                if (it->checkEscape()) {
                    escapedAnimalsInAviary.push_back(it->name);
                    if (it->isSick) {
                        enc.sickAnimalsCount--;
                    }
                    it = enc.animals.erase(it);
                }
                else {
                    ++it;
                }
            }
            // Уведомления о побегах
            if (!escapedAnimalsInAviary.empty()) {
                for (const string& name : escapedAnimalsInAviary) {
                    aviaryNotifications.push_back("Животное \"" + name + "\" сбежало из вольера климата \"" + enc.getClimateName() + "\" из-за низкого уровня счастья!");
                }
            }
        }

        // Популярность падает при большом числе больных
        int totalSickAnimals = 0;
        for (auto& enc : enclosures) {
            totalSickAnimals += enc.sickAnimalsCount;
        }
        popularity = max(0, popularity - totalSickAnimals);

        // Варьирование популярности
        int fluctuation = popularity * 0.1;
        int change = (rand() % (2 * fluctuation + 1)) - fluctuation;
        popularity += change;
        popularity = max(popularity, 0);

        // Выполнение ежедневных задач
        performDailyWork();

        // Инфекция случайного животного
        infectRandomAnimal();

        cout << "Бюджет текущего дня: " << money << " монет\n";

        if (!deadAnimals.empty()) {
            cout << "\n--- Уведомления о смерти ---\n";
            for (const string& name : deadAnimals) {
                cout << "Животное \"" << name << "\" умерло.\n";
            }
        }
        if (!aviaryNotifications.empty()) {
            cout << "\n--- Уведомления о вольерах ---\n";
            for (const string& note : aviaryNotifications) {
                cout << note << "\n";
            }
        }

        day++;
    }

    // Возвращает общее число животных
    int getTotalAnimals() {
        int total = 0;
        for (auto& enc : enclosures) total += enc.animals.size();
        return total;
    }

    // Основные действия за день — лечение, кормление, уборка
    void performDailyWork() {
        // Ветеринары лечат животных
        for (auto& emp : employees) {
            if (emp.position == Employee::Veterinarian && !emp.assignedAnimals.empty()) {
                int healedCount = 0;
                for (Animal* animal : emp.assignedAnimals) {
                    if (animal->isSick && healedCount < emp.maxCapacity) {
                        animal->isSick = false;
                        for (auto& enc : enclosures) {
                            for (auto& a : enc.animals) {
                                if (&a == animal) {
                                    enc.sickAnimalsCount--;
                                    break;
                                }
                            }
                        }
                        healedCount++;
                    }
                }
            }
        }

        // Кормильцы кормят вольеры
        for (auto& emp : employees) {
            if (emp.position == Employee::Feeder) {
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->feedAviary(food);
                }
            }
        }

        // Уборщики чистят вольеры
        for (auto& emp : employees) {
            if (emp.position == Employee::Cleaner) {
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->cleanAviary();
                }
            }
        }
    }

    // Инфицировать случайное животное
    void infectRandomAnimal() {
        vector<Animal*> allAnimals;
        for (auto& enc : enclosures) {
            for (auto& animal : enc.animals) {
                if (!animal.isSick) {
                    allAnimals.push_back(&animal);
                }
            }
        }
        if (!allAnimals.empty()) {
            int index = rand() % allAnimals.size();
            allAnimals[index]->isSick = true;
            for (auto& enc : enclosures) {
                for (auto& animal : enc.animals) {
                    if (&animal == allAnimals[index]) {
                        enc.sickAnimalsCount++;
                        cout << "Уведомление: Животное \"" << animal.name << "\" заболело тиаравирусом!\n";
                        break;
                    }
                }
            }
        }
    }
};

// Массивы видов для каждого климата (чтобы рандомно выбирать виды)
const string ARCTIC_SPECIES[] = { "Снеговик", "Ледяной дракон", "Морж бронинсец", "Летающий пингвин" };
const string DESERT_SPECIES[] = { "Пустынный червь", "Монстр кактус", "Мумия", "Пустынная змея", "Ящер" };
const string OCEAN_SPECIES[] = { "Кракен", "Мегалодон", "Годзилла", "Морской демон", "Мега кит" };
const string FOREST_SPECIES[] = { "Оборотень", "Слоновый медведь", "Заяц прыгун", "Чужой", "Клыкастый тигр" };

// Функция для получения рандомного вида животного по климату
string getRandomSpecies(Animal::Climate climate) {
    switch (climate) {
    case Animal::Desert:
        return DESERT_SPECIES[rand() % (sizeof(DESERT_SPECIES) / sizeof(DESERT_SPECIES[0]))];
    case Animal::Forest:
        return FOREST_SPECIES[rand() % (sizeof(FOREST_SPECIES) / sizeof(FOREST_SPECIES[0]))];
    case Animal::Artic:
        return ARCTIC_SPECIES[rand() % (sizeof(ARCTIC_SPECIES) / sizeof(ARCTIC_SPECIES[0]))];
    case Animal::Ocean:
        return OCEAN_SPECIES[rand() % (sizeof(OCEAN_SPECIES) / sizeof(OCEAN_SPECIES[0]))];
    default:
        return "Неизвестный вид";
    }
}

// Функция для получения массива видов по климату
vector<string> getSpeciesByClimate(Animal::Climate climate) {
    switch (climate) {
    case Animal::Desert:
        return vector<string>(begin(DESERT_SPECIES), end(DESERT_SPECIES));
    case Animal::Forest:
        return vector<string>(begin(FOREST_SPECIES), end(FOREST_SPECIES));
    case Animal::Artic:
        return vector<string>(begin(ARCTIC_SPECIES), end(ARCTIC_SPECIES));
    case Animal::Ocean:
        return vector<string>(begin(OCEAN_SPECIES), end(OCEAN_SPECIES));
    default:
        return {};
    }
}

// Генерация случайного животного
Animal generateRandomAnimal() {
    Animal::Climate climates[] = { Animal::Desert, Animal::Forest, Animal::Artic, Animal::Ocean };

    int randomAge = rand() % 40 + 1;
    int randomWeight = rand() % 3000 + 10;
    Animal::Climate randomClimate = climates[rand() % 4];
    bool isCarnivore = rand() % 2 == 0;
    Animal::Gender randomGender = (rand() % 2 == 0) ? Animal::Male : Animal::Female;
    string randomSpecies = getRandomSpecies(randomClimate);

    return Animal(randomSpecies, randomAge, randomWeight, randomClimate, isCarnivore, randomGender, randomSpecies);
}

// Вспомогательная функция для получения целого числа с проверкой
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Попробуйте снова.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Управление работниками — нанять, уволить, просмотреть, назначить
void manageEmployees(Zoo& zoo) {
    cout << "\n--- Управление работниками ---\n";
    cout << "1. Нанять сотрудника\n";
    cout << "2. Уволить сотрудника\n";
    cout << "3. Просмотреть список\n";
    cout << "4. Назначить работников\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nНаем сотрудника:\n";
        string name;
        cout << "Введите имя: ";
        getline(cin, name);

        cout << "1. Уборщик\n2. Ветеринар\n3. Кормилец\n";
        int posChoice = getIntegerInput("Выберите должность: ");
        Employee::Position position;
        int salary, maxCapacity;

        switch (posChoice) {
        case 1:
            position = Employee::Cleaner;
            salary = 100; // Уборщик обслуживает 1 вольер
            maxCapacity = 1;
            break;
        case 2:
            position = Employee::Veterinarian;
            salary = 300; // Ветеринар может обслуживать 20 животных
            maxCapacity = 20;
            break;
        case 3:
            position = Employee::Feeder;
            salary = 150; // Кормилец может обслуживать 2 вольера
            maxCapacity = 2;
            break;
        default:
            cout << "Неверный выбор!\n";
            return;
        }

        if (zoo.money >= salary) {
            zoo.employees.emplace_back(name, position, salary, maxCapacity);
            zoo.money -= salary;
            cout << "Сотрудник нанят!\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        break;
    }
    case 2: {
        cout << "\nУвольнение сотрудника:\n";
        int index = 1;
        vector<Employee*> fireableEmployees;
        for (auto& emp : zoo.employees) {
            if (emp.position != Employee::Director) {
                fireableEmployees.push_back(&emp);
                cout << index << ". " << emp.name << " (" << emp.getPositionName() << ")\n";
                index++;
            }
        }

        if (fireableEmployees.empty()) {
            cout << "Нет сотрудников для увольнения (кроме директора).\n";
            break;
        }

        int choice = getIntegerInput("Введите номер сотрудника: ");
        if (choice <= 0 || choice > fireableEmployees.size()) {
            cout << "Неверный номер!\n";
            break;
        }

        auto it = zoo.employees.begin();
        int current_index = 1;
        while (it != zoo.employees.end()) {
            if (it->position != Employee::Director) {
                if (current_index == choice) {
                    it = zoo.employees.erase(it);
                    cout << "Сотрудник уволен!\n";
                    break;
                }
                current_index++;
            }
            ++it;
        }
        break;
    }
    case 3: {
        cout << "\nСписок сотрудников:\n";
        for (auto& emp : zoo.employees) {
            cout << "- " << emp.name << " (" << emp.getPositionName() << ") Зарплата: "
                << emp.salary << ", Обслуживает: ";
            if (emp.position == Employee::Veterinarian) {
                cout << emp.assignedAnimals.size() << "/" << emp.maxCapacity << " животных";
            }
            else if (emp.position == Employee::Cleaner || emp.position == Employee::Feeder) {
                cout << emp.assignedAviaries.size() << "/" << emp.maxCapacity << " вольеров";
            }
            else {
                cout << "Неприменимо";
            }
            cout << "\n";
        }
        break;
    }
    case 4: {
        cout << "\n--- Назначение работников ---\n";
        if (zoo.employees.empty() || (zoo.employees.size() == 1 && zoo.employees.front().position == Employee::Director)) {
            cout << "У вас нет работников для назначения (кроме директора).\n";
            break;
        }
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров для назначения работников.\n";
            break;
        }

        cout << "Выберите работника для назначения:\n";
        int empIndex = 1;
        vector<Employee*> assignableEmployees;
        for (auto& emp : zoo.employees) {
            if (emp.position != Employee::Director) {
                assignableEmployees.push_back(&emp);
                cout << empIndex << ". " << emp.name << " (" << emp.getPositionName() << ") Назначено: ";
                if (emp.position == Employee::Veterinarian) {
                    cout << emp.assignedAnimals.size() << "/" << emp.maxCapacity << " животных";
                }
                else if (emp.position == Employee::Cleaner || emp.position == Employee::Feeder) {
                    cout << emp.assignedAviaries.size() << "/" << emp.maxCapacity << " вольеров";
                }
                cout << "\n";
                empIndex++;
            }
        }

        int selectedEmpChoice = getIntegerInput("Введите номер работника: ");
        if (selectedEmpChoice <= 0 || selectedEmpChoice > assignableEmployees.size()) {
            cout << "Неверный номер работника!\n";
            break;
        }
        Employee* selectedEmployee = assignableEmployees[selectedEmpChoice - 1];

        if (selectedEmployee->position == Employee::Veterinarian) {
            cout << "\nНазначение ветеринара \"" << selectedEmployee->name << "\" к животным:\n";
            if (selectedEmployee->assignedAnimals.size() >= selectedEmployee->maxCapacity) {
                cout << "Ветеринар уже обслуживает максимальное количество животных.\n";
                break;
            }

            cout << "Выберите вольер, из которого взять животных:\n";
            int aviaryIndex = 1;
            vector<Aviary*> aviariesWithAnimals;
            for (auto& enc : zoo.enclosures) {
                if (!enc.animals.empty()) {
                    aviariesWithAnimals.push_back(&enc);
                    cout << aviaryIndex << ". Климат: " << enc.getClimateName() << ", Животных: " << enc.animals.size() << "\n";
                    aviaryIndex++;
                }
            }
            if (aviariesWithAnimals.empty()) {
                cout << "Нет вольеров с животными для назначения.\n";
                break;
            }

            int selectedAviaryChoice = getIntegerInput("Введите номер вольера: ");
            if (selectedAviaryChoice <= 0 || selectedAviaryChoice > aviariesWithAnimals.size()) {
                cout << "Неверный номер вольера!\n";
                break;
            }
            Aviary* selectedAviary = aviariesWithAnimals[selectedAviaryChoice - 1];

            cout << "\nВыберите животных для назначения ветеринару (до " << selectedEmployee->maxCapacity - selectedEmployee->assignedAnimals.size() << "):\n";
            int animalIndex = 1;
            vector<Animal*> assignableAnimals;
            for (auto& animal : selectedAviary->animals) {
                bool alreadyAssigned = false;
                for (Animal* assigned : selectedEmployee->assignedAnimals) {
                    if (assigned == &animal) {
                        alreadyAssigned = true;
                        break;
                    }
                }
                if (!alreadyAssigned) {
                    assignableAnimals.push_back(&animal);
                    cout << animalIndex << ". " << animal.name << " (" << animal.species << ") - " << (animal.isSick ? "Болен" : "Здоров") << "\n";
                    animalIndex++;
                }
            }
            if (assignableAnimals.empty()) {
                cout << "Нет доступных животных в этом вольере для назначения.\n";
                break;
            }

            int numToAssign = getIntegerInput("Сколько животных назначить? ");
            if (numToAssign <= 0 || numToAssign > assignableAnimals.size() || numToAssign > selectedEmployee->maxCapacity - selectedEmployee->assignedAnimals.size()) {
                cout << "Неверное количество или превышен лимит ветеринара.\n";
                break;
            }

            for (int i = 0; i < numToAssign; ++i) {
                int animalChoice = getIntegerInput("Введите номер животного " + to_string(i + 1) + ": ");
                if (animalChoice <= 0 || animalChoice > assignableAnimals.size()) {
                    cout << "Неверный номер животного! Назначение прервано.\n";
                    break;
                }
                selectedEmployee->assignedAnimals.push_back(assignableAnimals[animalChoice - 1]);
                cout << "Животное \"" << assignableAnimals[animalChoice - 1]->name << "\" назначено ветеринару.\n";
            }

        }
        else if (selectedEmployee->position == Employee::Cleaner || selectedEmployee->position == Employee::Feeder) {
            cout << "\nНазначение " << selectedEmployee->getPositionName() << "а \"" << selectedEmployee->name << "\" к вольерам:\n";
            if (selectedEmployee->assignedAviaries.size() >= selectedEmployee->maxCapacity) {
                cout << selectedEmployee->getPositionName() << " уже обслуживает максимальное количество вольеров.\n";
                break;
            }

            cout << "Выберите вольер для назначения (до " << selectedEmployee->maxCapacity - selectedEmployee->assignedAviaries.size() << "):\n";
            int aviaryIndex = 1;
            vector<Aviary*> assignableAviaries;
            for (auto& enc : zoo.enclosures) {
                bool alreadyAssigned = false;
                for (Aviary* assigned : selectedEmployee->assignedAviaries) {
                    if (assigned == &enc) {
                        alreadyAssigned = true;
                        break;
                    }
                }
                if (!alreadyAssigned) {
                    assignableAviaries.push_back(&enc);
                    cout << aviaryIndex << ". Климат: " << enc.getClimateName() << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
                    aviaryIndex++;
                }
            }
            if (assignableAviaries.empty()) {
                cout << "Нет доступных вольеров для назначения.\n";
                break;
            }

            int numToAssign = getIntegerInput("Сколько вольеров назначить? ");
            if (numToAssign <= 0 || numToAssign > assignableAviaries.size() || numToAssign > selectedEmployee->maxCapacity - selectedEmployee->assignedAviaries.size()) {
                cout << "Неверное количество или превышен лимит работника.\n";
                break;
            }

            for (int i = 0; i < numToAssign; ++i) {
                int aviaryChoice = getIntegerInput("Введите номер вольера " + to_string(i + 1) + ": ");
                if (aviaryChoice <= 0 || aviaryChoice > assignableAviaries.size()) {
                    cout << "Неверный номер вольера! Назначение прервано.\n";
                    break;
                }
                selectedEmployee->assignedAviaries.push_back(assignableAviaries[aviaryChoice - 1]);
                cout << "Вольер с климатом \"" << assignableAviaries[aviaryChoice - 1]->getClimateName() << "\" назначен " << selectedEmployee->getPositionName() << "у.\n";
            }
        }

        break;
    }
    case 0:
        return;
    }
}
// управлению вольерами
void manageAviary(Zoo& zoo) {
    cout << "\n--- Управление вольерами ---\n";
    cout << "1. Построить вольер\n";
    cout << "2. Просмотреть вольеры\n";
    cout << "3. Улучшить вольер\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\n--- Создание нового вольера ---\n";
        cout << "Выберите климат для вольера:\n";
        cout << "0. Пустыня\n";
        cout << "1. Лес\n";
        cout << "2. Арктика\n";
        cout << "3. Океан\n";
        Animal::Climate climate = static_cast<Animal::Climate>(getIntegerInput("Ваш выбор: "));

        int capacity = getIntegerInput("Вместимость (Одно место = 50 монет): ");

        Aviary newEnclosure(climate, capacity);
        int cost = newEnclosure.calculateCost();

        cout << "Стоимость вольера: " << cost << " монет\n";
        cout << "Хотите построить этот вольер?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Строительство отменено.\n";
            break;
        }

        if (zoo.money < cost) {
            cout << "Недостаточно средств для строительства!\n";
            break;
        }

        zoo.enclosures.emplace_back(climate, capacity);
        zoo.money -= cost;
        cout << "Вольер успешно построен!\n";
        break;
    }

    case 2: {
        cout << "\nСписок вольеров:\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров.\n";
        }
        else {
            int index = 1;
            for (auto& enc : zoo.enclosures) {
                string climate;
                switch (enc.climat) {
                case Animal::Desert: climate = "Пустыня"; break;
                case Animal::Forest: climate = "Лес"; break;
                case Animal::Artic: climate = "Арктика"; break;
                case Animal::Ocean: climate = "Океан"; break;
                }
                cout << index << ". Климат: " << climate
                    << ", Уровень: " << enc.level
                    << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost
                    << ", Больных: " << enc.sickAnimalsCount
                    << ", Расходы в день: " << enc.daytrati << "\n";
                index++;
            }
        }
        break;
    }
    case 3: {
        cout << "\nУлучшение вольера:\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров для улучшения.\n";
            break;
        }
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climat) {
            case Animal::Desert: climate = "Пустыня"; break;
            case Animal::Forest: climate = "Лес"; break;
            case Animal::Artic: climate = "Арктика"; break;
            case Animal::Ocean: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Уровень: " << enc.level
                << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost
                << ", Расходы в день: " << enc.daytrati << "\n";
            index++;
        }

        int choice = getIntegerInput("Введите номер вольера для улучшения: ");
        if (choice <= 0 || choice > zoo.enclosures.size()) {
            cout << "Неверный номер!\n";
            break;
        }

        auto it = zoo.enclosures.begin();
        advance(it, choice - 1);

        int upgradeCost = it->vmestimost * 5 * (it->level + 1);
        cout << "Стоимость улучшения: " << upgradeCost << " монет\n";
        cout << "Хотите улучшить этот вольер?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Улучшение отменено.\n";
            break;
        }

        if (zoo.money < upgradeCost) {
            cout << "Недостаточно средств для улучшения!\n";
            break;
        }

        if (it->upgrade(upgradeCost)) {
            zoo.money -= upgradeCost;
            cout << "Вольер успешно улучшен до уровня " << it->level << "!\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// Управление животными
void manageAnimals(Zoo& zoo) {
    cout << "\n--- Управление животными ---\n";
    cout << "1. Купить готовое животное\n";
    cout << "2. Продать животное\n";
    cout << "3. Просмотреть животных\n";
    cout << "4. Переименовать животное\n";
    cout << "5. Размножить животных\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\n--- Покупка готового животного ---\n";
        if (zoo.day > 10 && zoo.getTotalAnimals() >= 1) {
            cout << "После 10-го дня вы можете купить только одного животного!\n";
            break;
        }

        const int NUM_RANDOM_ANIMALS = 5;
        vector<Animal> randomAnimals;
        for (int i = 0; i < NUM_RANDOM_ANIMALS; ++i) {
            randomAnimals.push_back(generateRandomAnimal());
        }

        cout << "Доступные животные:\n";
        for (int i = 0; i < randomAnimals.size(); ++i) {
            Animal& animal = randomAnimals[i];
            string climateName;
            switch (animal.climate) {
            case Animal::Desert: climateName = "Пустыня"; break;
            case Animal::Forest: climateName = "Лес"; break;
            case Animal::Artic: climateName = "Арктика"; break;
            case Animal::Ocean: climateName = "Океан"; break;
            }

            string genderName = (animal.gender == Animal::Male) ? "Самец" : "Самка";

            cout << i + 1 << ". Вид: " << animal.species
                << ", Возраст: " << animal.age << " дней"
                << ", Вес: " << animal.weight << " кг"
                << ", Климат: " << climateName
                << ", Тип: " << (animal.isCarnivore ? "Хищник" : "Травоядное")
                << ", Пол: " << genderName
                << ", Цена: " << animal.PriceAnimal() << " монет\n";
        }

        int choice = getIntegerInput("Введите номер животного для покупки: ");
        if (choice <= 0 || choice > randomAnimals.size()) {
            cout << "Неверный номер!\n";
            break;
        }

        Animal selectedAnimal = randomAnimals[choice - 1];
        int price = selectedAnimal.PriceAnimal();

        cout << "Итоговая цена животного: " << price << " монет\n";
        cout << "Хотите купить это животное?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Покупка отменена.\n";
            break;
        }

        if (zoo.money < price) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }

        string name;
        cout << "Введите имя для животного: ";
        getline(cin, name);
        selectedAnimal.name = name;

        vector<Aviary*> suitableEnclosures;
        for (auto& enc : zoo.enclosures) {
            if (enc.climat == selectedAnimal.climate && enc.volerskan(selectedAnimal)) {
                suitableEnclosures.push_back(&enc);
            }
        }

        if (suitableEnclosures.empty()) {
            cout << "Ошибка: Нет подходящего вольера!\n";
            break;
        }

        cout << "\nВыберите вольер для размещения животного:\n";
        for (int i = 0; i < suitableEnclosures.size(); ++i) {
            Aviary* enc = suitableEnclosures[i];
            cout << i + 1 << ". Климат: " << (enc->climat == Animal::Desert ? "Пустыня" :
                enc->climat == Animal::Forest ? "Лес" :
                enc->climat == Animal::Artic ? "Арктика" : "Океан")
                << ", Животных: " << enc->animals.size() << "/" << enc->vmestimost << "\n";
        }

        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > suitableEnclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        Aviary* selectedEnclosure = suitableEnclosures[enclosureChoice - 1];
        selectedEnclosure->volerskanback(selectedAnimal);
        zoo.money -= price;

        cout << "Животное \"" << selectedAnimal.name << "\" успешно добавлено в вольер!\n";
        break;
    }
    case 2: {
        cout << "\n--- Продажа животных ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "Пустыня"; break;
            case Animal::Forest: climateName = "Лес"; break;
            case Animal::Artic: climateName = "Арктика"; break;
            case Animal::Ocean: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.empty()) {
            cout << "В этом вольере нет животных!\n";
            break;
        }

        cout << "\nЖивотные в вольере:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "Самец" : "Самка";
            cout << index << ". " << animal.name << ", Возраст: " << animal.age << " дней"
                << ", Вес: " << animal.weight << " кг, Счастье: " << animal.happiness << "/100"
                << ", Пол: " << genderName
                << ", Цена: " << animal.PriceAnimal() << "\n";
            index++;
        }

        int animalChoice = getIntegerInput("Введите номер животного для продажи: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }

        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);

        int price = animalIt->PriceAnimal();
        int sellPrice = price * 0.8;

        cout << "Животное \"" << animalIt->name << "\" можно продать за " << sellPrice << " монет.\n";
        cout << "Вы уверены, что хотите продать это животное?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Продажа отменена.\n";
            break;
        }

        string animalName = animalIt->name;
        zoo.money += sellPrice;
        encIt->removeAnimal(animalName);

        cout << "Животное \"" << animalName << "\" продано за " << sellPrice << " монет.\n";
        break;
    }
    case 3: {
        cout << "\nСписок животных:\n";
        if (zoo.getTotalAnimals() == 0) {
            cout << "В зоопарке нет животных.\n";
            break;
        }
        for (auto& enc : zoo.enclosures) {
            for (auto& animal : enc.animals) {
                string genderName = (animal.gender == Animal::Male) ? "Самец" : "Самка";
                cout << "- " << animal.name << " (" << animal.species << "), " << animal.age << " дней, "
                    << animal.weight << " кг, Счастье: " << animal.happiness << "/100, "
                    << (animal.isCarnivore ? "Хищник" : "Травоядное") << ", Пол: " << genderName
                    << ", Состояние: " << (animal.isSick ? "Болен" : "Здоров")
                    << ", Родители: " << animal.parent1Name << " и " << animal.parent2Name << "\n";
            }
        }
        break;
    }
    case 4: {
        cout << "\n--- Переименование животного ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "Пустыня"; break;
            case Animal::Forest: climateName = "Лес"; break;
            case Animal::Artic: climateName = "Арктика"; break;
            case Animal::Ocean: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.empty()) {
            cout << "В этом вольере нет животных!\n";
            break;
        }

        cout << "\nЖивотные в вольере:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            cout << index << ". " << animal.name << " (" << animal.species << ")\n";
            index++;
        }

        int animalChoice = getIntegerInput("Введите номер животного для переименования: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }

        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);

        string newName;
        cout << "Введите новое имя для животного: ";
        getline(cin, newName);

        animalIt->rename(newName);
        cout << "Животное успешно переименовано в \"" << newName << "\"\n";
        break;
    }
    case 5: {
        cout << "\n--- Размножение животных ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "Пустыня"; break;
            case Animal::Forest: climateName = "Лес"; break;
            case Animal::Artic: climateName = "Арктика"; break;
            case Animal::Ocean: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("Выберите номер вольера для размножения: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.size() < 2) {
            cout << "В этом вольере недостаточно животных для размножения.\n";
            break;
        }

        cout << "\nВыберите первое животное для размножения:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "Самец" : "Самка";
            cout << index << ". " << animal.name << " (" << animal.species << "), Возраст: " << animal.age << " дней, Пол: " << genderName << "\n";
            index++;
        }

        int animal1Choice = getIntegerInput("Введите номер первого животного: ");
        if (animal1Choice <= 0 || animal1Choice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }

        auto animal1It = encIt->animals.begin();
        advance(animal1It, animal1Choice - 1);

        cout << "\nВыберите второе животное для размножения:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "Самец" : "Самка";
            cout << index << ". " << animal.name << " (" << animal.species << "), Возраст: " << animal.age << " дней, Пол: " << genderName << "\n";
            index++;
        }

        int animal2Choice = getIntegerInput("Введите номер второго животного: ");
        if (animal2Choice <= 0 || animal2Choice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }

        auto animal2It = encIt->animals.begin();
        advance(animal2It, animal2Choice - 1);

        if (animal1It == animal2It) {
            cout << "Нельзя размножать животное само с собой!\n";
            break;
        }

        try {
            Animal baby = (*animal1It) + (*animal2It);

            if (encIt->animals.size() >= encIt->vmestimost) {
                cout << "Ошибка: В вольере нет свободного места для детеныша.\n";
                break;
            }

            encIt->animals.push_back(baby);
            cout << "Успех! Родилось новое животное \"" << baby.name << "\" (" << baby.species << ")!\n";

        }
        catch (const runtime_error& e) {
            cout << "Ошибка размножения: " << e.what() << endl;
        }
        break;
    }

    case 0:
        return;
    }
}

// Управление ресурсами (еда, реклама)
void manageResources(Zoo& zoo) {
    cout << "\n--- Управление ресурсами ---\n";
    cout << "1. Купить еду\n";
    cout << "2. Заказать рекламу\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nПокупка еды:\n";
        int amount = getIntegerInput("Сколько единиц еды хотите купить? ");
        if (amount <= 0) {
            cout << "Неверное количество!\n";
            break;
        }

        int cost = amount * 2;
        if (zoo.money < cost) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }

        zoo.food += amount;
        zoo.money -= cost;
        cout << "Куплено " << amount << " единиц еды за " << cost << " монет.\n";
        break;
    }
    case 2: {
        const int COST_PER_POPULARITY = 30;
        cout << "Стоимость одной единицы популярности: " << COST_PER_POPULARITY << " монет\n";

        int cost = getIntegerInput("Введите сумму для рекламной кампании: ");
        if (cost <= 0) {
            cout << "Неверная сумма!\n";
            break;
        }

        if (zoo.money >= cost) {
            int popularityIncrease = cost / COST_PER_POPULARITY;
            zoo.money -= cost;
            zoo.popularity += popularityIncrease;

            cout << "Популярность увеличена на " << popularityIncrease << "!\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// Действия работников (лечить, кормить, убирать)
void performEmployeeActions(Zoo& zoo) {
    cout << "\n--- Действия работников ---\n";
    cout << "1. Ветеринары лечат животных\n";
    cout << "2. Кормильцы кормят вольеры\n";
    cout << "3. Уборщики чистят вольеры\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\n--- Лечение животных ветеринарами ---\n";
        bool foundVet = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Veterinarian && !emp.assignedAnimals.empty()) {
                foundVet = true;
                cout << "Ветеринар \"" << emp.name << "\" лечит животных:\n";
                int healedCount = 0;
                for (Animal* animal : emp.assignedAnimals) {
                    if (animal->isSick && healedCount < emp.maxCapacity) {
                        animal->isSick = false;
                        for (auto& enc : zoo.enclosures) {
                            for (auto& a : enc.animals) {
                                if (&a == animal) {
                                    enc.sickAnimalsCount--;
                                    break;
                                }
                            }
                        }
                        healedCount++;
                        cout << "  - Вылечено животное \"" << animal->name << "\" (" << animal->species << ")\n";
                    }
                }
                if (healedCount == 0) {
                    cout << "  - Нет больных животных для лечения в этом назначении.\n";
                }
            }
        }
        if (!foundVet) {
            cout << "Нет ветеринаров с назначенными животными.\n";
        }
        break;
    }
    case 2: {
        cout << "\n--- Кормление вольеров кормильцами ---\n";
        bool foundFeeder = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Feeder && !emp.assignedAviaries.empty()) {
                foundFeeder = true;
                cout << "Кормилец \"" << emp.name << "\" кормит вольеры:\n";
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->feedAviary(zoo.food);
                }
            }
        }
        if (!foundFeeder) {
            cout << "Нет кормильцев с назначенными вольерами.\n";
        }
        break;
    }
    case 3: {
        cout << "\n--- Уборка вольеров уборщиками ---\n";
        bool foundCleaner = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Cleaner && !emp.assignedAviaries.empty()) {
                foundCleaner = true;
                cout << "Уборщик \"" << emp.name << "\" чистит вольеры:\n";
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->cleanAviary();
                }
            }
        }
        if (!foundCleaner) {
            cout << "Нет уборщиков с назначенными вольерами.\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// основная функция — запуск симуляции
int main() {
    srand(time(0));
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    string zooName;
    cout << "------------------------------------------------------------------------------------------------------------------------ \n";
    cout << "                                                               Памятка\n";
    cout << "         1) У вас есть 10 дней чтобы поуправлять зоопарко, но если за это время вы потеряете все деньги игра окончится(. \n";
    cout << "         2) животных надо кормить или они умрут (( также нужно следить за их уровнем счастья или они могут разозлиться и сбежать. \n ";
    cout << "         3) Хищники могут жить только с хищниками, а травоядные - только с травоядными (разные виды в одном вольере разрешены).\n";
    cout << "         4) Остерегайтесь тиаравируса! Больные животные заражают других, а если их станет слишком много - начнут умирать. Болезнь также уменьшает популярность.\n";
    cout << "         5) Назначайте работников для лечения больных животных, кормления и уборки вольеров.\n";
    cout << "------------------------------------------------------------------------------------------------------------------------ \n";
    cout << "Введите название зоопарка: ";
    cin.clear();
    cin.sync();
    getline(cin, zooName);

    int initialMoney = getIntegerInput("Введите начальный капитал: ");
    while (initialMoney < 0) {
        cout << "Недопустимое значение. ";
        initialMoney = getIntegerInput("Введите начальный капитал: ");
    }

    Zoo zoo(zooName, initialMoney);
    zoo.employees.emplace_back("Ярополк", Employee::Director, 500, 0);

    while (true) {
        cout << "\n\n=== " << zoo.name << " ===\n";
        cout << "День: " << zoo.day << "\n";
        cout << "Деньги: " << zoo.money << " монет\n";
        cout << "Еда: " << zoo.food << " кг\n";
        cout << "Популярность: " << zoo.popularity << "\n";
        cout << "Животных: " << zoo.getTotalAnimals() << "\n";
        cout << "Вольеров: " << zoo.enclosures.size() << "\n";
        cout << "Работников: " << zoo.employees.size() << "\n";
        cout << "Посетители сегодня: " << max(0, 2 * zoo.popularity) << "\n";

        cout << "\n[1] Животные\n";
        cout << "[2] Работники\n";
        cout << "[3] Вольеры\n";
        cout << "[4] Ресурсы\n";
        cout << "[5] Действия работников\n";
        cout << "[0] Следующий день\n";

        int choice = getIntegerInput("Ваш выбор: ");
        if (choice == 0) {
            zoo.nextDay();
            if (zoo.money < 0) {
                cout << "\nБАНКРОТСТВО! Вы проиграли.\n";
                break;
            }
            if (zoo.day > 12) {
                cout << "\nПоздравляем! Вы успешно управляли зоопарком 10 дней!\n";
                break;
            }
        }
        else if (choice == 1) {
            manageAnimals(zoo);
        }
        else if (choice == 2) {
            manageEmployees(zoo);
        }
        else if (choice == 3) {
            manageAviary(zoo);
        }
        else if (choice == 4) {
            manageResources(zoo);
        }
        else if (choice == 5) {
            performEmployeeActions(zoo);
        }
    }

    return 0;
}
