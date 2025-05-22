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

// ����� Animal � �������� ���������
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

    // ����������� � ��������� ����������
    Animal(string n, int a, int w, Climate c, bool carn, Gender g, string s)
        : name(n), age(a), weight(w), climate(c), isCarnivore(carn), happiness(70), gender(g), species(s), parent1Name("����������"), parent2Name("����������"), isSick(false) {
    }
    // ����������� � ��������� ���������
    Animal(string n, int a, int w, Climate c, bool carn, Gender g, string s, string p1, string p2)
        : name(n), age(a), weight(w), climate(c), isCarnivore(carn), happiness(70), gender(g), species(s), parent1Name(p1), parent2Name(p2), isSick(false) {
    }

    // ������ ��������� ���������
    int PriceAnimal() const {
        int basePrice = 100;
        int price = basePrice;
        price += weight * 2;
        price -= age * 5;
        price += isCarnivore ? 100 : 0;
        price += static_cast<int>(climate) * 50;
        return max(price, 10);
    }

    // ���������� ������ ������� � ����������� �� ���������, ���������� ������� � �������
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

    // ��������, ������ �� �������� ��-�� ������� �������
    bool checkEscape() const {
        if (happiness <= 20) {
            return (rand() % 100) < 50; // 50% ���� ������� ��� ������ �������
        }
        return false;
    }
    // �������������� ���������
    void rename(const string& newName) {
        name = newName;
    }
    // ��������, ������ �� �������� ��-�� ��������
    bool checkOldAgeDeath() const {
        if (age > 50) {
            return (rand() % 100) < (age - 50); // ����������� ������ ������������� � ���������
        }
        return false;
    }
    // �������� ��� ����������� �������� � ������� ��������
    Animal operator+(const Animal& other) const {
        // �������� �� �������, ���, ������ � ��� �������
        if (age < 5 || other.age < 5) {
            throw runtime_error("�������� ������� ������ ��� �����������.");
        }
        if (gender == other.gender) {
            throw runtime_error("������ ���������� ��������� ��������.");
        }
        if (climate != other.climate) {
            throw runtime_error("������ ���������� �������� �� ������ ������������� ���.");
        }
        if (isCarnivore != other.isCarnivore) {
            throw runtime_error("������ ���������� �������� � �����������.");
        }
        // �������� ������ � ���������
        string babyName = "������� " + species + "-" + other.species;
        int babyAge = 0;
        int babyWeight = (weight + other.weight) / 4;
        Gender babyGender = (rand() % 2 == 0) ? Male : Female;
        string parent1 = name;
        string parent2 = other.name;

        // ������� ��������� �������� ����
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

// ����� Aviary � �������� �������
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

    // �����������
    Aviary(Animal::Climate c, int cap)
        : climat(c), vmestimost(cap), level(1), sickAnimalsCount(0), isClean(true), isFed(false) {
        daytrati = calculateDailyCost(); // ������� � ���� 
    }

    // ��������, ����� �� �������� �������� � ������ (�� ������� � ���� �������)
    bool volerskan(const Animal& animal) {
        if (animals.size() >= vmestimost) return false;
        if (animal.climate != climat) return false;
        if (!animals.empty()) {
            if (animals.front().isCarnivore != animal.isCarnivore) return false;
        }
        return true;
    }

    // ���������� ��������� � ������
    void volerskanback(const Animal& animal) {
        if (volerskan(animal)) {
            animals.push_back(animal);
            if (animal.isSick) {
                sickAnimalsCount++;
                cout << "�����������: �������� " << animal.name << " ��������� � ������ �������!\n";
            }
        }
        else {
            cout << "���������� �������� �������� � ������ (������������ ������� ��� ��� �����).\n";
        }
    }
    // �������� ��������� �� �����
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

    // ������ ��������� �������
    int calculateCost() const {
        int baseCost = 100;
        int cost = baseCost;
        cost += vmestimost * 10;
        cost += static_cast<int>(climat) * 50;
        return max(cost, 150);
    }
    // ������ ���������� �������� �� ������
    int calculateDailyCost() const {
        int baseDailyCost = 10;
        int dailyCost = baseDailyCost;
        dailyCost += vmestimost / 10;
        dailyCost += static_cast<int>(climat) * 5;
        return max(dailyCost, 10);
    }

    // ���������� ��������� ������� � �������� (��������, ������, �������)
    vector<string> updateAviaryState() {
        vector<string> notifications;
        int initialSickCount = sickAnimalsCount;
        int totalAnimals = animals.size();

        // ��������������� ��������
        if (initialSickCount > 0 && totalAnimals > 0) {
            vector<int> susceptibleIndices;
            for (int i = 0; i < animals.size(); ++i) {
                auto it = animals.begin();
                advance(it, i);
                if (!it->isSick) {
                    susceptibleIndices.push_back(i);
                }
            }

            // ������������� � ��������� ����� ��������
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
                notifications.push_back("�������� \"" + it->name + "\" ���������� ������������!");
            }
        }

        // ������� �� �������
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
                    diedFromSickness.push_back(it->name + " (�� �����������)");
                    it = animals.erase(it);
                    sickAnimalsCount--;
                }
            }
            if (!diedFromSickness.empty()) {
                notifications.push_back("� ������� ������� \"" + getClimateName() + "\" ������ �� �������: " + to_string(diedFromSickness.size()) + " ��������.");
            }
        }

        isFed = false;
        isClean = false;

        return notifications;
    }

    // �������� �� maxToHeal ��������
    int healAnimals(int maxToHeal) {
        int healedCount = 0;
        for (auto& animal : animals) {
            if (animal.isSick && healedCount < maxToHeal) {
                animal.isSick = false;
                sickAnimalsCount--;
                healedCount++;
                cout << "�������� \"" << animal.name << "\" ��������!\n";
            }
        }
        return healedCount;
    }

    // ������ �������
    void cleanAviary() {
        isClean = true;
        for (auto& animal : animals) {
            animal.happiness = min(100, animal.happiness + 10);
        }
        cout << "������ � �������� \"" << getClimateName() << "\" ������.\n";
    }

    // ��������� ������, ���� ���� ���
    bool feedAviary(int& availableFood) {
        int requiredFood = animals.size();
        if (availableFood >= requiredFood) {
            availableFood -= requiredFood;
            isFed = true;
            cout << "������ � �������� \"" << getClimateName() << "\" ���������.\n";
            return true;
        }
        else {
            cout << "������������ ��� ��� ��������� ������� � �������� \"" << getClimateName() << "\".\n";
            return false;
        }
    }

    // ��������� ������� � ���������� ����������� � �������� ���������� ��������
    bool upgrade(int baseUpgradeCost) {
        if (level >= 3) {
            cout << "��������� ������������ ������� ���������!\n";
            return false;
        }
        vmestimost *= 2;
        daytrati += calculateDailyCost() / 2;
        level++;
        return true;
    }

    // ��������� �������� ������������� ����
    string getClimateName() const {
        switch (climat) {
        case Animal::Desert: return "�������";
        case Animal::Forest: return "���";
        case Animal::Artic: return "�������";
        case Animal::Ocean: return "�����";
        default: return "�����������";
        }
    }
};

// �������� ���������
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
        case Director: return "��������";
        case Cleaner: return "�������";
        case Veterinarian: return "���������";
        case Feeder: return "��������";
        default: return "����������";
        }
    }
};
//�������� ����� ��������
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

    // ������� � ���������� ���
    void nextDay() {
        cout << "\n--- ���� " << day << " ---\n";
        cout << "������ �������� ���: " << money << " �����\n";

        // ������ ������
        int visitors = max(0, 2 * popularity);
        int totalAnimals = getTotalAnimals();
        int income = visitors * totalAnimals;

        cout << "���������� �������: " << visitors << "\n";
        cout << "����� �� ����: +" << income << " �����\n";

        // ���������� �������
        money += income;

        // ������� ��������
        for (auto& emp : employees) {
            money -= emp.salary;
        }

        // ������� �� ���������� ��������
        for (auto& enc : enclosures) {
            money -= enc.daytrati;
        }

        // ��������� ������� ��-�� ��������
        vector<string> deadAnimals;
        for (auto& enc : enclosures) {
            for (auto it = enc.animals.begin(); it != enc.animals.end(); ) {
                it->age++;
                if (it->checkOldAgeDeath()) {
                    deadAnimals.push_back(it->name + " (�� ��������)");
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

        // ��������� ������ � ��������
        vector<string> aviaryNotifications;
        for (auto& enc : enclosures) {
            if (!enc.isFed && enc.animals.size() > 0) {
                int deficit = enc.animals.size();
                for (auto it = enc.animals.begin(); it != enc.animals.end() && deficit > 0;) {
                    if (rand() % 2 == 0) {
                        deadAnimals.push_back(it->name + " (�� ������)");
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
                    aviaryNotifications.push_back("� ������� ������� \"" + enc.getClimateName() + "\" ��� �������� ������ �� ������.");
                }
                else if (deficit > 0) {
                    aviaryNotifications.push_back("� ������� ������� \"" + enc.getClimateName() + "\" ������ " + to_string(deficit) + " �������� �� ������.");
                }
            }

            // ���������� ������� ��������
            int population = enc.animals.size();
            for (auto& animal : enc.animals) {
                animal.updateHappiness(enc.isFed, population);
            }

            // ��������� �������� � ���������
            vector<string> currentAviaryNotifications = enc.updateAviaryState();
            aviaryNotifications.insert(aviaryNotifications.end(), currentAviaryNotifications.begin(), currentAviaryNotifications.end());

            // �������� ������� ��� ������ �������
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
            // ����������� � �������
            if (!escapedAnimalsInAviary.empty()) {
                for (const string& name : escapedAnimalsInAviary) {
                    aviaryNotifications.push_back("�������� \"" + name + "\" ������� �� ������� ������� \"" + enc.getClimateName() + "\" ��-�� ������� ������ �������!");
                }
            }
        }

        // ������������ ������ ��� ������� ����� �������
        int totalSickAnimals = 0;
        for (auto& enc : enclosures) {
            totalSickAnimals += enc.sickAnimalsCount;
        }
        popularity = max(0, popularity - totalSickAnimals);

        // ������������ ������������
        int fluctuation = popularity * 0.1;
        int change = (rand() % (2 * fluctuation + 1)) - fluctuation;
        popularity += change;
        popularity = max(popularity, 0);

        // ���������� ���������� �����
        performDailyWork();

        // �������� ���������� ���������
        infectRandomAnimal();

        cout << "������ �������� ���: " << money << " �����\n";

        if (!deadAnimals.empty()) {
            cout << "\n--- ����������� � ������ ---\n";
            for (const string& name : deadAnimals) {
                cout << "�������� \"" << name << "\" ������.\n";
            }
        }
        if (!aviaryNotifications.empty()) {
            cout << "\n--- ����������� � �������� ---\n";
            for (const string& note : aviaryNotifications) {
                cout << note << "\n";
            }
        }

        day++;
    }

    // ���������� ����� ����� ��������
    int getTotalAnimals() {
        int total = 0;
        for (auto& enc : enclosures) total += enc.animals.size();
        return total;
    }

    // �������� �������� �� ���� � �������, ���������, ������
    void performDailyWork() {
        // ���������� ����� ��������
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

        // ��������� ������ �������
        for (auto& emp : employees) {
            if (emp.position == Employee::Feeder) {
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->feedAviary(food);
                }
            }
        }

        // �������� ������ �������
        for (auto& emp : employees) {
            if (emp.position == Employee::Cleaner) {
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->cleanAviary();
                }
            }
        }
    }

    // ������������ ��������� ��������
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
                        cout << "�����������: �������� \"" << animal.name << "\" �������� ������������!\n";
                        break;
                    }
                }
            }
        }
    }
};

// ������� ����� ��� ������� ������� (����� �������� �������� ����)
const string ARCTIC_SPECIES[] = { "��������", "������� ������", "���� ���������", "�������� �������" };
const string DESERT_SPECIES[] = { "��������� �����", "������ ������", "�����", "��������� ����", "����" };
const string OCEAN_SPECIES[] = { "������", "���������", "��������", "������� �����", "���� ���" };
const string FOREST_SPECIES[] = { "���������", "�������� �������", "���� ������", "�����", "��������� ����" };

// ������� ��� ��������� ���������� ���� ��������� �� �������
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
        return "����������� ���";
    }
}

// ������� ��� ��������� ������� ����� �� �������
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

// ��������� ���������� ���������
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

// ��������������� ������� ��� ��������� ������ ����� � ���������
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������������ ����. ���������� �����.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// ���������� ����������� � ������, �������, �����������, ���������
void manageEmployees(Zoo& zoo) {
    cout << "\n--- ���������� ����������� ---\n";
    cout << "1. ������ ����������\n";
    cout << "2. ������� ����������\n";
    cout << "3. ����������� ������\n";
    cout << "4. ��������� ����������\n";
    cout << "0. �����\n";

    int choice = getIntegerInput("�������� ��������: ");
    switch (choice) {
    case 1: {
        cout << "\n���� ����������:\n";
        string name;
        cout << "������� ���: ";
        getline(cin, name);

        cout << "1. �������\n2. ���������\n3. ��������\n";
        int posChoice = getIntegerInput("�������� ���������: ");
        Employee::Position position;
        int salary, maxCapacity;

        switch (posChoice) {
        case 1:
            position = Employee::Cleaner;
            salary = 100; // ������� ����������� 1 ������
            maxCapacity = 1;
            break;
        case 2:
            position = Employee::Veterinarian;
            salary = 300; // ��������� ����� ����������� 20 ��������
            maxCapacity = 20;
            break;
        case 3:
            position = Employee::Feeder;
            salary = 150; // �������� ����� ����������� 2 �������
            maxCapacity = 2;
            break;
        default:
            cout << "�������� �����!\n";
            return;
        }

        if (zoo.money >= salary) {
            zoo.employees.emplace_back(name, position, salary, maxCapacity);
            zoo.money -= salary;
            cout << "��������� �����!\n";
        }
        else {
            cout << "������������ �������!\n";
        }
        break;
    }
    case 2: {
        cout << "\n���������� ����������:\n";
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
            cout << "��� ����������� ��� ���������� (����� ���������).\n";
            break;
        }

        int choice = getIntegerInput("������� ����� ����������: ");
        if (choice <= 0 || choice > fireableEmployees.size()) {
            cout << "�������� �����!\n";
            break;
        }

        auto it = zoo.employees.begin();
        int current_index = 1;
        while (it != zoo.employees.end()) {
            if (it->position != Employee::Director) {
                if (current_index == choice) {
                    it = zoo.employees.erase(it);
                    cout << "��������� ������!\n";
                    break;
                }
                current_index++;
            }
            ++it;
        }
        break;
    }
    case 3: {
        cout << "\n������ �����������:\n";
        for (auto& emp : zoo.employees) {
            cout << "- " << emp.name << " (" << emp.getPositionName() << ") ��������: "
                << emp.salary << ", �����������: ";
            if (emp.position == Employee::Veterinarian) {
                cout << emp.assignedAnimals.size() << "/" << emp.maxCapacity << " ��������";
            }
            else if (emp.position == Employee::Cleaner || emp.position == Employee::Feeder) {
                cout << emp.assignedAviaries.size() << "/" << emp.maxCapacity << " ��������";
            }
            else {
                cout << "�����������";
            }
            cout << "\n";
        }
        break;
    }
    case 4: {
        cout << "\n--- ���������� ���������� ---\n";
        if (zoo.employees.empty() || (zoo.employees.size() == 1 && zoo.employees.front().position == Employee::Director)) {
            cout << "� ��� ��� ���������� ��� ���������� (����� ���������).\n";
            break;
        }
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� �������� ��� ���������� ����������.\n";
            break;
        }

        cout << "�������� ��������� ��� ����������:\n";
        int empIndex = 1;
        vector<Employee*> assignableEmployees;
        for (auto& emp : zoo.employees) {
            if (emp.position != Employee::Director) {
                assignableEmployees.push_back(&emp);
                cout << empIndex << ". " << emp.name << " (" << emp.getPositionName() << ") ���������: ";
                if (emp.position == Employee::Veterinarian) {
                    cout << emp.assignedAnimals.size() << "/" << emp.maxCapacity << " ��������";
                }
                else if (emp.position == Employee::Cleaner || emp.position == Employee::Feeder) {
                    cout << emp.assignedAviaries.size() << "/" << emp.maxCapacity << " ��������";
                }
                cout << "\n";
                empIndex++;
            }
        }

        int selectedEmpChoice = getIntegerInput("������� ����� ���������: ");
        if (selectedEmpChoice <= 0 || selectedEmpChoice > assignableEmployees.size()) {
            cout << "�������� ����� ���������!\n";
            break;
        }
        Employee* selectedEmployee = assignableEmployees[selectedEmpChoice - 1];

        if (selectedEmployee->position == Employee::Veterinarian) {
            cout << "\n���������� ���������� \"" << selectedEmployee->name << "\" � ��������:\n";
            if (selectedEmployee->assignedAnimals.size() >= selectedEmployee->maxCapacity) {
                cout << "��������� ��� ����������� ������������ ���������� ��������.\n";
                break;
            }

            cout << "�������� ������, �� �������� ����� ��������:\n";
            int aviaryIndex = 1;
            vector<Aviary*> aviariesWithAnimals;
            for (auto& enc : zoo.enclosures) {
                if (!enc.animals.empty()) {
                    aviariesWithAnimals.push_back(&enc);
                    cout << aviaryIndex << ". ������: " << enc.getClimateName() << ", ��������: " << enc.animals.size() << "\n";
                    aviaryIndex++;
                }
            }
            if (aviariesWithAnimals.empty()) {
                cout << "��� �������� � ��������� ��� ����������.\n";
                break;
            }

            int selectedAviaryChoice = getIntegerInput("������� ����� �������: ");
            if (selectedAviaryChoice <= 0 || selectedAviaryChoice > aviariesWithAnimals.size()) {
                cout << "�������� ����� �������!\n";
                break;
            }
            Aviary* selectedAviary = aviariesWithAnimals[selectedAviaryChoice - 1];

            cout << "\n�������� �������� ��� ���������� ���������� (�� " << selectedEmployee->maxCapacity - selectedEmployee->assignedAnimals.size() << "):\n";
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
                    cout << animalIndex << ". " << animal.name << " (" << animal.species << ") - " << (animal.isSick ? "�����" : "������") << "\n";
                    animalIndex++;
                }
            }
            if (assignableAnimals.empty()) {
                cout << "��� ��������� �������� � ���� ������� ��� ����������.\n";
                break;
            }

            int numToAssign = getIntegerInput("������� �������� ���������? ");
            if (numToAssign <= 0 || numToAssign > assignableAnimals.size() || numToAssign > selectedEmployee->maxCapacity - selectedEmployee->assignedAnimals.size()) {
                cout << "�������� ���������� ��� �������� ����� ����������.\n";
                break;
            }

            for (int i = 0; i < numToAssign; ++i) {
                int animalChoice = getIntegerInput("������� ����� ��������� " + to_string(i + 1) + ": ");
                if (animalChoice <= 0 || animalChoice > assignableAnimals.size()) {
                    cout << "�������� ����� ���������! ���������� ��������.\n";
                    break;
                }
                selectedEmployee->assignedAnimals.push_back(assignableAnimals[animalChoice - 1]);
                cout << "�������� \"" << assignableAnimals[animalChoice - 1]->name << "\" ��������� ����������.\n";
            }

        }
        else if (selectedEmployee->position == Employee::Cleaner || selectedEmployee->position == Employee::Feeder) {
            cout << "\n���������� " << selectedEmployee->getPositionName() << "� \"" << selectedEmployee->name << "\" � ��������:\n";
            if (selectedEmployee->assignedAviaries.size() >= selectedEmployee->maxCapacity) {
                cout << selectedEmployee->getPositionName() << " ��� ����������� ������������ ���������� ��������.\n";
                break;
            }

            cout << "�������� ������ ��� ���������� (�� " << selectedEmployee->maxCapacity - selectedEmployee->assignedAviaries.size() << "):\n";
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
                    cout << aviaryIndex << ". ������: " << enc.getClimateName() << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
                    aviaryIndex++;
                }
            }
            if (assignableAviaries.empty()) {
                cout << "��� ��������� �������� ��� ����������.\n";
                break;
            }

            int numToAssign = getIntegerInput("������� �������� ���������? ");
            if (numToAssign <= 0 || numToAssign > assignableAviaries.size() || numToAssign > selectedEmployee->maxCapacity - selectedEmployee->assignedAviaries.size()) {
                cout << "�������� ���������� ��� �������� ����� ���������.\n";
                break;
            }

            for (int i = 0; i < numToAssign; ++i) {
                int aviaryChoice = getIntegerInput("������� ����� ������� " + to_string(i + 1) + ": ");
                if (aviaryChoice <= 0 || aviaryChoice > assignableAviaries.size()) {
                    cout << "�������� ����� �������! ���������� ��������.\n";
                    break;
                }
                selectedEmployee->assignedAviaries.push_back(assignableAviaries[aviaryChoice - 1]);
                cout << "������ � �������� \"" << assignableAviaries[aviaryChoice - 1]->getClimateName() << "\" �������� " << selectedEmployee->getPositionName() << "�.\n";
            }
        }

        break;
    }
    case 0:
        return;
    }
}
// ���������� ���������
void manageAviary(Zoo& zoo) {
    cout << "\n--- ���������� ��������� ---\n";
    cout << "1. ��������� ������\n";
    cout << "2. ����������� �������\n";
    cout << "3. �������� ������\n";
    cout << "0. �����\n";

    int choice = getIntegerInput("�������� ��������: ");
    switch (choice) {
    case 1: {
        cout << "\n--- �������� ������ ������� ---\n";
        cout << "�������� ������ ��� �������:\n";
        cout << "0. �������\n";
        cout << "1. ���\n";
        cout << "2. �������\n";
        cout << "3. �����\n";
        Animal::Climate climate = static_cast<Animal::Climate>(getIntegerInput("��� �����: "));

        int capacity = getIntegerInput("����������� (���� ����� = 50 �����): ");

        Aviary newEnclosure(climate, capacity);
        int cost = newEnclosure.calculateCost();

        cout << "��������� �������: " << cost << " �����\n";
        cout << "������ ��������� ���� ������?\n";
        cout << "1. ��\n2. ���\n";
        int confirm = getIntegerInput("��� �����: ");

        if (confirm != 1) {
            cout << "������������� ��������.\n";
            break;
        }

        if (zoo.money < cost) {
            cout << "������������ ������� ��� �������������!\n";
            break;
        }

        zoo.enclosures.emplace_back(climate, capacity);
        zoo.money -= cost;
        cout << "������ ������� ��������!\n";
        break;
    }

    case 2: {
        cout << "\n������ ��������:\n";
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� ��������.\n";
        }
        else {
            int index = 1;
            for (auto& enc : zoo.enclosures) {
                string climate;
                switch (enc.climat) {
                case Animal::Desert: climate = "�������"; break;
                case Animal::Forest: climate = "���"; break;
                case Animal::Artic: climate = "�������"; break;
                case Animal::Ocean: climate = "�����"; break;
                }
                cout << index << ". ������: " << climate
                    << ", �������: " << enc.level
                    << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost
                    << ", �������: " << enc.sickAnimalsCount
                    << ", ������� � ����: " << enc.daytrati << "\n";
                index++;
            }
        }
        break;
    }
    case 3: {
        cout << "\n��������� �������:\n";
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� �������� ��� ���������.\n";
            break;
        }
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climat) {
            case Animal::Desert: climate = "�������"; break;
            case Animal::Forest: climate = "���"; break;
            case Animal::Artic: climate = "�������"; break;
            case Animal::Ocean: climate = "�����"; break;
            }
            cout << index << ". ������: " << climate
                << ", �������: " << enc.level
                << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost
                << ", ������� � ����: " << enc.daytrati << "\n";
            index++;
        }

        int choice = getIntegerInput("������� ����� ������� ��� ���������: ");
        if (choice <= 0 || choice > zoo.enclosures.size()) {
            cout << "�������� �����!\n";
            break;
        }

        auto it = zoo.enclosures.begin();
        advance(it, choice - 1);

        int upgradeCost = it->vmestimost * 5 * (it->level + 1);
        cout << "��������� ���������: " << upgradeCost << " �����\n";
        cout << "������ �������� ���� ������?\n";
        cout << "1. ��\n2. ���\n";
        int confirm = getIntegerInput("��� �����: ");

        if (confirm != 1) {
            cout << "��������� ��������.\n";
            break;
        }

        if (zoo.money < upgradeCost) {
            cout << "������������ ������� ��� ���������!\n";
            break;
        }

        if (it->upgrade(upgradeCost)) {
            zoo.money -= upgradeCost;
            cout << "������ ������� ������� �� ������ " << it->level << "!\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// ���������� ���������
void manageAnimals(Zoo& zoo) {
    cout << "\n--- ���������� ��������� ---\n";
    cout << "1. ������ ������� ��������\n";
    cout << "2. ������� ��������\n";
    cout << "3. ����������� ��������\n";
    cout << "4. ������������� ��������\n";
    cout << "5. ���������� ��������\n";
    cout << "0. �����\n";

    int choice = getIntegerInput("�������� ��������: ");
    switch (choice) {
    case 1: {
        cout << "\n--- ������� �������� ��������� ---\n";
        if (zoo.day > 10 && zoo.getTotalAnimals() >= 1) {
            cout << "����� 10-�� ��� �� ������ ������ ������ ������ ���������!\n";
            break;
        }

        const int NUM_RANDOM_ANIMALS = 5;
        vector<Animal> randomAnimals;
        for (int i = 0; i < NUM_RANDOM_ANIMALS; ++i) {
            randomAnimals.push_back(generateRandomAnimal());
        }

        cout << "��������� ��������:\n";
        for (int i = 0; i < randomAnimals.size(); ++i) {
            Animal& animal = randomAnimals[i];
            string climateName;
            switch (animal.climate) {
            case Animal::Desert: climateName = "�������"; break;
            case Animal::Forest: climateName = "���"; break;
            case Animal::Artic: climateName = "�������"; break;
            case Animal::Ocean: climateName = "�����"; break;
            }

            string genderName = (animal.gender == Animal::Male) ? "�����" : "�����";

            cout << i + 1 << ". ���: " << animal.species
                << ", �������: " << animal.age << " ����"
                << ", ���: " << animal.weight << " ��"
                << ", ������: " << climateName
                << ", ���: " << (animal.isCarnivore ? "������" : "����������")
                << ", ���: " << genderName
                << ", ����: " << animal.PriceAnimal() << " �����\n";
        }

        int choice = getIntegerInput("������� ����� ��������� ��� �������: ");
        if (choice <= 0 || choice > randomAnimals.size()) {
            cout << "�������� �����!\n";
            break;
        }

        Animal selectedAnimal = randomAnimals[choice - 1];
        int price = selectedAnimal.PriceAnimal();

        cout << "�������� ���� ���������: " << price << " �����\n";
        cout << "������ ������ ��� ��������?\n";
        cout << "1. ��\n2. ���\n";
        int confirm = getIntegerInput("��� �����: ");

        if (confirm != 1) {
            cout << "������� ��������.\n";
            break;
        }

        if (zoo.money < price) {
            cout << "������������ ������� ��� �������!\n";
            break;
        }

        string name;
        cout << "������� ��� ��� ���������: ";
        getline(cin, name);
        selectedAnimal.name = name;

        vector<Aviary*> suitableEnclosures;
        for (auto& enc : zoo.enclosures) {
            if (enc.climat == selectedAnimal.climate && enc.volerskan(selectedAnimal)) {
                suitableEnclosures.push_back(&enc);
            }
        }

        if (suitableEnclosures.empty()) {
            cout << "������: ��� ����������� �������!\n";
            break;
        }

        cout << "\n�������� ������ ��� ���������� ���������:\n";
        for (int i = 0; i < suitableEnclosures.size(); ++i) {
            Aviary* enc = suitableEnclosures[i];
            cout << i + 1 << ". ������: " << (enc->climat == Animal::Desert ? "�������" :
                enc->climat == Animal::Forest ? "���" :
                enc->climat == Animal::Artic ? "�������" : "�����")
                << ", ��������: " << enc->animals.size() << "/" << enc->vmestimost << "\n";
        }

        int enclosureChoice = getIntegerInput("������� ����� �������: ");
        if (enclosureChoice <= 0 || enclosureChoice > suitableEnclosures.size()) {
            cout << "�������� ����� �������!\n";
            break;
        }

        Aviary* selectedEnclosure = suitableEnclosures[enclosureChoice - 1];
        selectedEnclosure->volerskanback(selectedAnimal);
        zoo.money -= price;

        cout << "�������� \"" << selectedAnimal.name << "\" ������� ��������� � ������!\n";
        break;
    }
    case 2: {
        cout << "\n--- ������� �������� ---\n";
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� ��������!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "�������"; break;
            case Animal::Forest: climateName = "���"; break;
            case Animal::Artic: climateName = "�������"; break;
            case Animal::Ocean: climateName = "�����"; break;
            }
            cout << index << ". ������: " << climateName
                << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("������� ����� �������: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "�������� ����� �������!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.empty()) {
            cout << "� ���� ������� ��� ��������!\n";
            break;
        }

        cout << "\n�������� � �������:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "�����" : "�����";
            cout << index << ". " << animal.name << ", �������: " << animal.age << " ����"
                << ", ���: " << animal.weight << " ��, �������: " << animal.happiness << "/100"
                << ", ���: " << genderName
                << ", ����: " << animal.PriceAnimal() << "\n";
            index++;
        }

        int animalChoice = getIntegerInput("������� ����� ��������� ��� �������: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "�������� ����� ���������!\n";
            break;
        }

        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);

        int price = animalIt->PriceAnimal();
        int sellPrice = price * 0.8;

        cout << "�������� \"" << animalIt->name << "\" ����� ������� �� " << sellPrice << " �����.\n";
        cout << "�� �������, ��� ������ ������� ��� ��������?\n";
        cout << "1. ��\n2. ���\n";
        int confirm = getIntegerInput("��� �����: ");

        if (confirm != 1) {
            cout << "������� ��������.\n";
            break;
        }

        string animalName = animalIt->name;
        zoo.money += sellPrice;
        encIt->removeAnimal(animalName);

        cout << "�������� \"" << animalName << "\" ������� �� " << sellPrice << " �����.\n";
        break;
    }
    case 3: {
        cout << "\n������ ��������:\n";
        if (zoo.getTotalAnimals() == 0) {
            cout << "� �������� ��� ��������.\n";
            break;
        }
        for (auto& enc : zoo.enclosures) {
            for (auto& animal : enc.animals) {
                string genderName = (animal.gender == Animal::Male) ? "�����" : "�����";
                cout << "- " << animal.name << " (" << animal.species << "), " << animal.age << " ����, "
                    << animal.weight << " ��, �������: " << animal.happiness << "/100, "
                    << (animal.isCarnivore ? "������" : "����������") << ", ���: " << genderName
                    << ", ���������: " << (animal.isSick ? "�����" : "������")
                    << ", ��������: " << animal.parent1Name << " � " << animal.parent2Name << "\n";
            }
        }
        break;
    }
    case 4: {
        cout << "\n--- �������������� ��������� ---\n";
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� ��������!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "�������"; break;
            case Animal::Forest: climateName = "���"; break;
            case Animal::Artic: climateName = "�������"; break;
            case Animal::Ocean: climateName = "�����"; break;
            }
            cout << index << ". ������: " << climateName
                << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("������� ����� �������: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "�������� ����� �������!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.empty()) {
            cout << "� ���� ������� ��� ��������!\n";
            break;
        }

        cout << "\n�������� � �������:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            cout << index << ". " << animal.name << " (" << animal.species << ")\n";
            index++;
        }

        int animalChoice = getIntegerInput("������� ����� ��������� ��� ��������������: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "�������� ����� ���������!\n";
            break;
        }

        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);

        string newName;
        cout << "������� ����� ��� ��� ���������: ";
        getline(cin, newName);

        animalIt->rename(newName);
        cout << "�������� ������� ������������� � \"" << newName << "\"\n";
        break;
    }
    case 5: {
        cout << "\n--- ����������� �������� ---\n";
        if (zoo.enclosures.empty()) {
            cout << "� ��� ��� ��������!\n";
            break;
        }

        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climat) {
            case Animal::Desert: climateName = "�������"; break;
            case Animal::Forest: climateName = "���"; break;
            case Animal::Artic: climateName = "�������"; break;
            case Animal::Ocean: climateName = "�����"; break;
            }
            cout << index << ". ������: " << climateName
                << ", ��������: " << enc.animals.size() << "/" << enc.vmestimost << "\n";
            index++;
        }

        int enclosureChoice = getIntegerInput("�������� ����� ������� ��� �����������: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "�������� ����� �������!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        if (encIt->animals.size() < 2) {
            cout << "� ���� ������� ������������ �������� ��� �����������.\n";
            break;
        }

        cout << "\n�������� ������ �������� ��� �����������:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "�����" : "�����";
            cout << index << ". " << animal.name << " (" << animal.species << "), �������: " << animal.age << " ����, ���: " << genderName << "\n";
            index++;
        }

        int animal1Choice = getIntegerInput("������� ����� ������� ���������: ");
        if (animal1Choice <= 0 || animal1Choice > encIt->animals.size()) {
            cout << "�������� ����� ���������!\n";
            break;
        }

        auto animal1It = encIt->animals.begin();
        advance(animal1It, animal1Choice - 1);

        cout << "\n�������� ������ �������� ��� �����������:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            string genderName = (animal.gender == Animal::Male) ? "�����" : "�����";
            cout << index << ". " << animal.name << " (" << animal.species << "), �������: " << animal.age << " ����, ���: " << genderName << "\n";
            index++;
        }

        int animal2Choice = getIntegerInput("������� ����� ������� ���������: ");
        if (animal2Choice <= 0 || animal2Choice > encIt->animals.size()) {
            cout << "�������� ����� ���������!\n";
            break;
        }

        auto animal2It = encIt->animals.begin();
        advance(animal2It, animal2Choice - 1);

        if (animal1It == animal2It) {
            cout << "������ ���������� �������� ���� � �����!\n";
            break;
        }

        try {
            Animal baby = (*animal1It) + (*animal2It);

            if (encIt->animals.size() >= encIt->vmestimost) {
                cout << "������: � ������� ��� ���������� ����� ��� ��������.\n";
                break;
            }

            encIt->animals.push_back(baby);
            cout << "�����! �������� ����� �������� \"" << baby.name << "\" (" << baby.species << ")!\n";

        }
        catch (const runtime_error& e) {
            cout << "������ �����������: " << e.what() << endl;
        }
        break;
    }

    case 0:
        return;
    }
}

// ���������� ��������� (���, �������)
void manageResources(Zoo& zoo) {
    cout << "\n--- ���������� ��������� ---\n";
    cout << "1. ������ ���\n";
    cout << "2. �������� �������\n";
    cout << "0. �����\n";

    int choice = getIntegerInput("�������� ��������: ");
    switch (choice) {
    case 1: {
        cout << "\n������� ���:\n";
        int amount = getIntegerInput("������� ������ ��� ������ ������? ");
        if (amount <= 0) {
            cout << "�������� ����������!\n";
            break;
        }

        int cost = amount * 2;
        if (zoo.money < cost) {
            cout << "������������ ������� ��� �������!\n";
            break;
        }

        zoo.food += amount;
        zoo.money -= cost;
        cout << "������� " << amount << " ������ ��� �� " << cost << " �����.\n";
        break;
    }
    case 2: {
        const int COST_PER_POPULARITY = 30;
        cout << "��������� ����� ������� ������������: " << COST_PER_POPULARITY << " �����\n";

        int cost = getIntegerInput("������� ����� ��� ��������� ��������: ");
        if (cost <= 0) {
            cout << "�������� �����!\n";
            break;
        }

        if (zoo.money >= cost) {
            int popularityIncrease = cost / COST_PER_POPULARITY;
            zoo.money -= cost;
            zoo.popularity += popularityIncrease;

            cout << "������������ ��������� �� " << popularityIncrease << "!\n";
        }
        else {
            cout << "������������ �������!\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// �������� ���������� (������, �������, �������)
void performEmployeeActions(Zoo& zoo) {
    cout << "\n--- �������� ���������� ---\n";
    cout << "1. ���������� ����� ��������\n";
    cout << "2. ��������� ������ �������\n";
    cout << "3. �������� ������ �������\n";
    cout << "0. �����\n";

    int choice = getIntegerInput("�������� ��������: ");
    switch (choice) {
    case 1: {
        cout << "\n--- ������� �������� ������������ ---\n";
        bool foundVet = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Veterinarian && !emp.assignedAnimals.empty()) {
                foundVet = true;
                cout << "��������� \"" << emp.name << "\" ����� ��������:\n";
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
                        cout << "  - �������� �������� \"" << animal->name << "\" (" << animal->species << ")\n";
                    }
                }
                if (healedCount == 0) {
                    cout << "  - ��� ������� �������� ��� ������� � ���� ����������.\n";
                }
            }
        }
        if (!foundVet) {
            cout << "��� ����������� � ������������ ���������.\n";
        }
        break;
    }
    case 2: {
        cout << "\n--- ��������� �������� ����������� ---\n";
        bool foundFeeder = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Feeder && !emp.assignedAviaries.empty()) {
                foundFeeder = true;
                cout << "�������� \"" << emp.name << "\" ������ �������:\n";
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->feedAviary(zoo.food);
                }
            }
        }
        if (!foundFeeder) {
            cout << "��� ���������� � ������������ ���������.\n";
        }
        break;
    }
    case 3: {
        cout << "\n--- ������ �������� ���������� ---\n";
        bool foundCleaner = false;
        for (auto& emp : zoo.employees) {
            if (emp.position == Employee::Cleaner && !emp.assignedAviaries.empty()) {
                foundCleaner = true;
                cout << "������� \"" << emp.name << "\" ������ �������:\n";
                for (Aviary* enc : emp.assignedAviaries) {
                    enc->cleanAviary();
                }
            }
        }
        if (!foundCleaner) {
            cout << "��� ��������� � ������������ ���������.\n";
        }
        break;
    }
    case 0:
        return;
    }
}

// �������� ������� � ������ ���������
int main() {
    srand(time(0));
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    string zooName;
    cout << "------------------------------------------------------------------------------------------------------------------------ \n";
    cout << "                                                               �������\n";
    cout << "         1) � ��� ���� 10 ���� ����� ����������� ��������, �� ���� �� ��� ����� �� ��������� ��� ������ ���� ���������(. \n";
    cout << "         2) �������� ���� ������� ��� ��� ����� (( ����� ����� ������� �� �� ������� ������� ��� ��� ����� ����������� � �������. \n ";
    cout << "         3) ������� ����� ���� ������ � ���������, � ���������� - ������ � ����������� (������ ���� � ����� ������� ���������).\n";
    cout << "         4) ������������� �����������! ������� �������� �������� ������, � ���� �� ������ ������� ����� - ������ �������. ������� ����� ��������� ������������.\n";
    cout << "         5) ���������� ���������� ��� ������� ������� ��������, ��������� � ������ ��������.\n";
    cout << "------------------------------------------------------------------------------------------------------------------------ \n";
    cout << "������� �������� ��������: ";
    cin.clear();
    cin.sync();
    getline(cin, zooName);

    int initialMoney = getIntegerInput("������� ��������� �������: ");
    while (initialMoney < 0) {
        cout << "������������ ��������. ";
        initialMoney = getIntegerInput("������� ��������� �������: ");
    }

    Zoo zoo(zooName, initialMoney);
    zoo.employees.emplace_back("�������", Employee::Director, 500, 0);

    while (true) {
        cout << "\n\n=== " << zoo.name << " ===\n";
        cout << "����: " << zoo.day << "\n";
        cout << "������: " << zoo.money << " �����\n";
        cout << "���: " << zoo.food << " ��\n";
        cout << "������������: " << zoo.popularity << "\n";
        cout << "��������: " << zoo.getTotalAnimals() << "\n";
        cout << "��������: " << zoo.enclosures.size() << "\n";
        cout << "����������: " << zoo.employees.size() << "\n";
        cout << "���������� �������: " << max(0, 2 * zoo.popularity) << "\n";

        cout << "\n[1] ��������\n";
        cout << "[2] ���������\n";
        cout << "[3] �������\n";
        cout << "[4] �������\n";
        cout << "[5] �������� ����������\n";
        cout << "[0] ��������� ����\n";

        int choice = getIntegerInput("��� �����: ");
        if (choice == 0) {
            zoo.nextDay();
            if (zoo.money < 0) {
                cout << "\n�����������! �� ���������.\n";
                break;
            }
            if (zoo.day > 12) {
                cout << "\n�����������! �� ������� ��������� ��������� 10 ����!\n";
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